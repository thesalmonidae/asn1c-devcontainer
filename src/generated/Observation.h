/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ObservationModule"
 * 	found in "../../modules/observation.asn"
 */

#ifndef	_Observation_H_
#define	_Observation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeReal.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Observation */
typedef struct Observation {
	double	 x;
	double	 y;
	double	 z;
	long	 timestamp;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Observation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Observation;

#ifdef __cplusplus
}
#endif

#endif	/* _Observation_H_ */
#include <asn_internal.h>
