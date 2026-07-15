/*-
 * Copyright (c) 2025 Contributors. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <NativeInteger.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>
#include <cbor_support.h>

asn_enc_rval_t NativeInteger_encode_cbor(
    const asn_TYPE_descriptor_t* td, const void* sptr,
    asn_app_consume_bytes_f* cb, void* app_key) {
  const asn_INTEGER_specifics_t* specs =
      (const asn_INTEGER_specifics_t*) td->specifics;
  const long* native = (const long*) sptr;
  asn_enc_rval_t er  = {0, 0, 0};
  ssize_t ret;

  if (!native) ASN__ENCODE_FAILED;

  if (specs && specs->field_unsigned) {
    ret = cbor_encode_uint((uint64_t) (unsigned long) *native, cb, app_key);
  } else if (*native >= 0) {
    ret = cbor_encode_uint((uint64_t) *native, cb, app_key);
  } else {
    /* negative: CBOR negint argument = (-val) - 1 */
    uint64_t argument = (uint64_t) (-((*native) + 1));
    ret               = cbor_encode_negint(argument, cb, app_key);
  }
  if (ret < 0) ASN__ENCODE_FAILED;
  er.encoded = ret;
  ASN__ENCODED_OK(er);
}

asn_dec_rval_t NativeInteger_decode_cbor(
    const asn_codec_ctx_t* opt_codec_ctx, const asn_TYPE_descriptor_t* td,
    void** sptr, const void* buf_ptr, size_t size) {
  const asn_INTEGER_specifics_t* specs =
      (const asn_INTEGER_specifics_t*) td->specifics;
  const uint8_t* buf = (const uint8_t*) buf_ptr;
  ssize_t tag_skip;
  uint8_t major;
  uint64_t argument;
  ssize_t hlen;
  long* native;
  asn_dec_rval_t rval = {RC_FAIL, 0};

  (void) opt_codec_ctx;

  if (!*sptr) {
    *sptr = CALLOC(1, sizeof(long));
    if (!*sptr) ASN__DECODE_FAILED;
  }
  native = (long*) *sptr;

  if (size < 1) ASN__DECODE_FAILED;

  /* Skip any leading CBOR tags (RFC 8949 §3.4) */
  tag_skip = cbor_skip_tags(buf, size);
  if (tag_skip < 0) ASN__DECODE_FAILED;

  hlen = cbor_decode_head(
      buf + tag_skip, size - (size_t) tag_skip, &major, &argument);
  if (hlen < 0) ASN__DECODE_FAILED;

  if (major == CBOR_MAJOR_UINT) {
    if (specs && specs->field_unsigned) {
      if (argument > (uint64_t) ULONG_MAX) ASN__DECODE_FAILED;
      *native = (long) (unsigned long) argument;
    } else {
      if (argument > (uint64_t) LONG_MAX) ASN__DECODE_FAILED;
      *native = (long) argument;
    }
  } else if (major == CBOR_MAJOR_NEGINT) {
    /* value = -(argument+1) */
    if (argument >= (uint64_t) LONG_MAX + 1ULL) ASN__DECODE_FAILED;
    *native = -(long) argument - 1;
  } else {
    ASN__DECODE_FAILED;
  }

  rval.consumed = (size_t) tag_skip + (size_t) hlen;
  rval.code     = RC_OK;
  return rval;
}
