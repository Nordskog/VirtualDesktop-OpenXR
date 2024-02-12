#ifndef META_BODY_TRACKING_FIDELITY_H_
#define META_BODY_TRACKING_FIDELITY_H_ 1

/**********************
This file is @generated from the OpenXR XML API registry.
Language    :   C99
Copyright   :   (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
***********************/

#include <openxr/openxr.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef XR_META_body_tracking_fidelity

#define XR_META_body_tracking_fidelity 1
#define XR_META_body_tracking_fidelity_SPEC_VERSION 1
#define XR_META_BODY_TRACKING_FIDELITY_EXTENSION_NAME "XR_META_body_tracking_fidelity"
static const XrStructureType XR_TYPE_BODY_TRACKING_FIDELITY_STATUS_META = (XrStructureType) 1000284000;
static const XrStructureType XR_TYPE_SYSTEM_PROPERTIES_BODY_TRACKING_FIDELITY_META = (XrStructureType) 1000284001;

typedef enum XrBodyTrackingFidelityMETA {
    XR_BODY_TRACKING_FIDELITY_LOW_META = 1,
    XR_BODY_TRACKING_FIDELITY_HIGH_META = 2,
    XR_BODY_TRACKING_FIDELITY_MAX_ENUM_META = 0x7FFFFFFF
} XrBodyTrackingFidelityMETA;
typedef struct XrSystemPropertiesBodyTrackingFidelityMETA {
    XrStructureType       type;
    void* XR_MAY_ALIAS    next;
    XrBool32              supportsBodyTrackingFidelity;
} XrSystemPropertiesBodyTrackingFidelityMETA;

// XrBodyTrackingFidelityStatusMETA extends XrBodyJointLocationsFB
typedef struct XrBodyTrackingFidelityStatusMETA {
    XrStructureType               type;
    const void* XR_MAY_ALIAS      next;
    XrBodyTrackingFidelityMETA    fidelity;
} XrBodyTrackingFidelityStatusMETA;

typedef XrResult (XRAPI_PTR *PFN_xrRequestBodyTrackingFidelityMETA)(XrBodyTrackerFB bodyTracker, const XrBodyTrackingFidelityMETA fidelity);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrRequestBodyTrackingFidelityMETA(
    XrBodyTrackerFB                             bodyTracker,
    const XrBodyTrackingFidelityMETA            fidelity);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */
#endif /* XR_META_body_tracking_fidelity */

#ifdef __cplusplus
}
#endif

#endif
