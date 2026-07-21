#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
#
# generateHTMLReport.py -- build an HTML report for the OAI-CN5G-COMMON-SRC CI.
#
# The common-src pipeline builds many NFs (Ubuntu images only) in parallel via
# ci-scripts/build_ubuntu_core_images.sh. This script turns the artifacts that
# script leaves in the archives directory into a single self-contained HTML
# page: a git-info header, then a compact per-NF build summary table.
#
# Inputs (all under --archives, produced by build_ubuntu_core_images.sh):
#   nf_map.json                     - per NF: {"nf": <ref>, "common-src": <ref>}
#   <nf>.log                        - per-NF build log (timestamped lines; the
#                                     '[INFO] Image Size: <size>' line carries the size)
#   failed_network_functions.txt    - one failed NF name per line

import argparse
import json
import re
from datetime import datetime
from pathlib import Path

# ---------------------------------------------------------------------------
# Log parsing helpers
# ---------------------------------------------------------------------------
_ANSI = re.compile(r'\x1b\[[0-9;]*m')
_TS = re.compile(r'^\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\]')
_NF_COMMIT = re.compile(r'NF commit:\s*(\S+)\s*\|\s*common-src commit:\s*(\S+)')
_IMAGE_TAG = re.compile(r'NF Docker Image:\s*(\S+)')
_DOCKERFILE = re.compile(r'--file\s+"?([^\s"]+)')
_IMAGE_SIZE = re.compile(r'(?:Image )?Size:\s*([0-9.]+\s*[KMGT]?i?B)', re.I)
_ERROR_LINE = re.compile(r'\]\s*\[ERROR\]\s*(.+)$|\berror:\s*(.+)$', re.I)


def _clean(line):
    """Strip ANSI color codes and the trailing newline from a log line."""
    return _ANSI.sub('', line).rstrip('\n')


def _esc(value):
    """HTML-escape a value for safe embedding in the report."""
    return (str(value).replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;'))


def _format_duration(start, end):
    """Return 'Xm Ys' from two 'YYYY-MM-DD HH:MM:SS' timestamps, or '-'."""
    try:
        fmt = '%Y-%m-%d %H:%M:%S'
        delta = datetime.strptime(end, fmt) - datetime.strptime(start, fmt)
        secs = int(delta.total_seconds())
        return f'{secs // 60}m {secs % 60}s'
    except (ValueError, TypeError):
        return '-'


def parse_nf_log(log_path, nf):
    """Pull the report fields out of one NF's build log."""
    data = {
        'nf_sha': None, 'common_src_sha': None, 'image_tag': None,
        'dockerfile': f'docker/Dockerfile.{nf}.ubuntu', 'image_size': None,
        'build_time': '-', 'error_line': None,
    }
    if not log_path.is_file():
        return data

    first_ts = last_ts = None
    errors = []
    with log_path.open(errors='replace') as handle:
        for raw in handle:
            line = _clean(raw)
            ts = _TS.match(line)
            if ts:
                first_ts = first_ts or ts.group(1)
                last_ts = ts.group(1)
            m = _NF_COMMIT.search(line)
            if m:
                data['nf_sha'], data['common_src_sha'] = m.group(1), m.group(2)
            m = _IMAGE_TAG.search(line)
            if m:
                data['image_tag'] = m.group(1)
            m = _DOCKERFILE.search(line)
            if m:
                data['dockerfile'] = m.group(1)
            m = _IMAGE_SIZE.search(line)
            if m:
                data['image_size'] = m.group(1).replace('  ', ' ')
            m = _ERROR_LINE.search(line)
            if m:
                # group(2) is a compiler 'error:' line; group(1) is the build
                # script's own '[ERROR]' log line.
                is_compiler = m.group(2) is not None
                errors.append((is_compiler, (m.group(1) or m.group(2)).strip()))

    data['build_time'] = _format_duration(first_ts, last_ts)
    if errors:
        # Prefer the last compiler error (the root cause) over the generic
        # '[ERROR] Docker build failed' the script logs afterwards.
        compiler = [text for is_compiler, text in errors if is_compiler]
        data['error_line'] = compiler[-1] if compiler else errors[-1][1]
    return data


# ---------------------------------------------------------------------------
# HTML templates
# ---------------------------------------------------------------------------
# The HTML fragments live in ci-scripts/html-templates/; each one carries
# UPPERCASE placeholder tokens that we fill in here. Resolving them relative to
# __file__ keeps the report reproducible regardless of the CI runner's CWD.
_TEMPLATE_DIR = Path(__file__).resolve().parent / 'html-templates'


def _load(name):
    """Read an HTML template fragment from the script's directory."""
    return (_TEMPLATE_DIR / name).read_text()


def _fill(text, mapping):
    """Substitute each TOKEN -> value. Plain str.replace (not re.sub) so values
    may safely contain regex/backreference characters (e.g. compiler errors)."""
    for token, value in mapping.items():
        text = text.replace(token, value)
    return text


# ---------------------------------------------------------------------------
# HTML rendering
# ---------------------------------------------------------------------------
def _build_rows(nf_map, failed, per_nf, skipped):
    """Render one summary-table row per NF (built NFs and SKIPPED ones), sorted."""
    row_tpl = _load('build-summary-table-row.htm')
    rows = []
    for nf in sorted(set(nf_map) | skipped):
        if nf in skipped:
            # NF excluded from this run (e.g. do<NF>=false) -- never built.
            rows.append(_fill(row_tpl, {
                'NF_NAME': _esc(nf.upper()),
                'STATUS_BG': 'lightgray', 'STATUS_TXT': 'SKIPPED',
                'NF_REF': '&mdash;', 'COMMON_SRC_REF': '&mdash;', 'DOCKERFILE': '&mdash;',
                'TAG_CELL': '&mdash;', 'SIZE_CELL': '&mdash;', 'BUILD_TIME': '&mdash;',
                'DETAILS': '&mdash;',
            }))
            continue
        info = per_nf[nf]
        passed = nf not in failed
        status_bg, status_txt = ('lightgreen', 'PASS') if passed else ('lightcoral', 'FAIL')
        if passed:
            details = '&#10003;'
            # A tag is only meaningful once the image exists.
            tag_cell = f'<code>{_esc(info["image_tag"] or "-")}</code>'
            size_cell = _esc(info['image_size'] or 'N/A')
        else:
            details = (f'<code>{_esc(info["error_line"])}</code>'
                       if info['error_line'] else 'Build failed — see the NF log')
            # A failed build produced no image, so it has no tag or size.
            tag_cell = size_cell = '&mdash;'
        rows.append(_fill(row_tpl, {
            'NF_NAME': _esc(nf.upper()),
            'STATUS_BG': status_bg,
            'STATUS_TXT': status_txt,
            'NF_REF': _esc(nf_map[nf]['nf'] or '-'),
            'COMMON_SRC_REF': _esc(nf_map[nf]['common-src'] or '-'),
            'DOCKERFILE': _esc(info['dockerfile']),
            'TAG_CELL': tag_cell,
            'SIZE_CELL': size_cell,
            'BUILD_TIME': _esc(info['build_time']),
            'DETAILS': details,
        }))
    return ''.join(rows)


def render_report(args, nf_map, failed, per_nf):
    """Fill the report skeleton with git info and the per-NF build summary."""
    all_nfs = [s.strip().lower() for s in (args.all_nfs or '').split(',') if s.strip()]
    skipped = {nf for nf in all_nfs if nf not in nf_map}
    built = len(nf_map)
    n_failed = len([nf for nf in nf_map if nf in failed])
    n_ok = built - n_failed
    alert_level = 'danger' if n_failed else 'success'
    message = (f'NF Image Builds: {n_ok} succeeded, {n_failed} failed, '
               f'{len(skipped)} skipped ({built + len(skipped)} total).')
    git_url = _esc(args.git_url) if args.git_url else '-'
    # BUILD_SUMMARY_ROWS is filled last so arbitrary row content (error lines) is
    # never re-scanned for other tokens.
    return _fill(_load('report.htm'), {
        'JOB_NAME': _esc(args.job_name),
        'BUILD_URL': _esc(args.build_url),
        'BUILD_ID': _esc(args.build_id),
        'TEMPLATE_TIME': _esc(args.build_time or '-'),
        'GIT_URL': git_url,
        'TEMPLATE_PULL_REQUEST_LINK': _esc(args.pr_url or '-'),
        'TEMPLATE_PULL_REQUEST_TITLE': _esc(args.pr_title or '-'),
        'SRC_BRANCH': _esc(args.git_src_branch or '-'),
        'SRC_COMMIT': _esc(args.git_src_commit or '-'),
        'DST_BRANCH': _esc(args.git_dst_branch or '-'),
        'DST_COMMIT': _esc(args.git_dst_commit or '-'),
        'CHAPTER_NAME': 'Container Images Build Summary',
        'ALERT_LEVEL': alert_level,
        'MESSAGE': message,
        'BUILD_SUMMARY_ROWS': _build_rows(nf_map, failed, per_nf, skipped),
    })


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def _parse_args():
    """Parse command-line arguments."""
    p = argparse.ArgumentParser(description='Generate the OAI-CN5G-COMMON-SRC CI HTML report.')
    p.add_argument('--archives', default='archives', help='Directory with build artifacts')
    p.add_argument('--nf-json', help='Path to nf_map.json (default: <archives>/nf_map.json)')
    p.add_argument('--all-nfs', default='', help='Comma-separated canonical NF list; those not built are shown as SKIPPED')
    p.add_argument('--output', help='Output HTML path (default: <archives>/test_results_oai_cn5g_common_src.html)')
    p.add_argument('--job-name', default='OAI-CN5G-Common-Src')
    p.add_argument('--build-id', default='N/A')
    p.add_argument('--build-url', default='')
    p.add_argument('--build-time', default='')
    p.add_argument('--git-url', default='')
    p.add_argument('--git-src-branch', default='')
    p.add_argument('--git-src-commit', default='')
    p.add_argument('--git-dst-branch', default='')
    p.add_argument('--git-dst-commit', default='')
    p.add_argument('--pr-url', default='')
    p.add_argument('--pr-title', default='')
    return p.parse_args()


def main():
    """Read the build artifacts and write the HTML report."""
    args = _parse_args()
    archives = Path(args.archives)
    nf_json = Path(args.nf_json) if args.nf_json else archives / 'nf_map.json'
    output = Path(args.output) if args.output else archives / 'test_results_oai_cn5g_common_src.html'

    if not nf_json.is_file():
        raise SystemExit(f'generateHTMLReport.py: NF map not found: {nf_json}')
    nf_map = json.loads(nf_json.read_text())

    failed_file = archives / 'failed_network_functions.txt'
    failed = set()
    if failed_file.is_file():
        failed = {line.strip() for line in failed_file.read_text().splitlines() if line.strip()}

    per_nf = {}
    for nf in nf_map:
        per_nf[nf] = parse_nf_log(archives / f'{nf}.log', nf)

    output.write_text(render_report(args, nf_map, failed, per_nf))
    print(f'Report written to {output}')


if __name__ == '__main__':
    main()
