/*
 * Ngap_ProtocolIE_Container_compat.h
 * Provides a complete definition of struct Ngap_ProtocolIE_Container so that
 * C++ wrapper code can access the ->list member. In the generated Rel-17 code,
 * all Ngap_ProtocolIE_Container_12874Pxx types have the same memory layout:
 *   { A_SEQUENCE_OF(element_type) list; asn_struct_ctx_t _asn_ctx; }
 * We expose the generic form using asn_anonymous_sequence_.
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <asn_application.h>
#include <asn_SEQUENCE_OF.h>
/* Complete definition of the opaque struct used in message headers.
 * All Ngap_ProtocolIE_Container_12874Pxx structs share this layout. */
struct Ngap_ProtocolIE_Container {
  asn_anonymous_sequence_ list;
  asn_struct_ctx_t _asn_ctx;
};
#ifdef __cplusplus
}
#endif
