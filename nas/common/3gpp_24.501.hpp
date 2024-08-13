/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifndef _3GPP_TS_24501_H_
#define _3GPP_TS_24501_H_

#include <string>

#include "3gpp_24.007.hpp"

//------------------------------------------------------------------------------
// Security Header Type
constexpr uint8_t kPlain5gsMessage                                     = 0b0000;
constexpr uint8_t kIntegrityProtected                                  = 0b0001;
constexpr uint8_t kIntegrityProtectedAndCiphered                       = 0b0010;
constexpr uint8_t kIntegrityProtectedWithNewSecurityContext            = 0b0011;
constexpr uint8_t kIntegrityProtectedAndCipheredWithNewSecurityContext = 0b0100;

//------------------------------------------------------------------------------
// Message Types for 5GS Mobility Management
constexpr uint8_t k5gsMobilityManagementMessageTypeUnknown = 0b10000000;

constexpr uint8_t kRegistrationRequest                = 0b01000001;
constexpr uint8_t kRegistrationAccept                 = 0b01000010;
constexpr uint8_t kRegistrationComplete               = 0b01000011;
constexpr uint8_t kRegistrationReject                 = 0b01000100;
constexpr uint8_t kDeregistrationRequestUeOriginating = 0b01000101;
constexpr uint8_t kDeregistrationAcceptUeOriginating  = 0b01000110;
constexpr uint8_t kDeregistrationRequestUeTerminated  = 0b01000111;
constexpr uint8_t kDeregistrationAcceptUeTerminated   = 0b01001000;

constexpr uint8_t kServiceRequest             = 0b01001100;
constexpr uint8_t kServiceReject              = 0b01001101;
constexpr uint8_t kServiceAccept              = 0b01001110;
constexpr uint8_t kControlPlaneServiceRequest = 0b01001111;

constexpr uint8_t kNetworkSliceSpecificAuthenticationCommand  = 0b01010000;
constexpr uint8_t kNetworkSliceSpecificAuthenticationComplete = 0b01010001;
constexpr uint8_t kNetworkSliceSpecificAuthenticationResult   = 0b01010010;
constexpr uint8_t kConfigurationUpdateCommand                 = 0b01010100;
constexpr uint8_t kConfigurationUpdateComplete                = 0b01010101;
constexpr uint8_t kAuthenticationRequest                      = 0b01010110;
constexpr uint8_t kAuthenticationResponse                     = 0b01010111;
constexpr uint8_t kAuthenticationReject                       = 0b01011000;
constexpr uint8_t kAuthenticationFailure                      = 0b01011001;
constexpr uint8_t kAuthenticationResult                       = 0b01011010;
constexpr uint8_t kIdentityRequest                            = 0b01011011;
constexpr uint8_t kIdentityResponse                           = 0b01011100;
constexpr uint8_t kSecurityModeCommand                        = 0b01011101;
constexpr uint8_t kSecurityModeComplete                       = 0b01011110;
constexpr uint8_t kSecurityModeReject                         = 0b01011111;

constexpr uint8_t k5gmmStatus                      = 0b01100100;
constexpr uint8_t kMessageTypeNotification         = 0b01100101;
constexpr uint8_t kMessageTypeNotificationResponse = 0b01100110;
constexpr uint8_t kUlNasTransport                  = 0b01100111;
constexpr uint8_t kDlNasTransport                  = 0b01101000;

// Message Types for 5GS Session Management
constexpr uint8_t k5gsSessionManagementMessageTypeUnknown = 0b11000000;

constexpr uint8_t kPduSessionEstablishmentRequest   = 0b11000001;
constexpr uint8_t kPduSessionEstablishmentAccept    = 0b11000010;
constexpr uint8_t kPduSessionEstablishmentReject    = 0b11000011;
constexpr uint8_t kPduSessionAuthenticationCommand  = 0b11000101;
constexpr uint8_t kPduSessionAuthenticationComplete = 0b11000110;
constexpr uint8_t kPduSessionAuthenticationResult   = 0b11000111;

constexpr uint8_t kPduSessionModificationRequest       = 0b11001001;
constexpr uint8_t kPduSessionModificationReject        = 0b11001010;
constexpr uint8_t kPduSessionModificationCommand       = 0b11001011;
constexpr uint8_t kPduSessionModificationComplete      = 0b11001100;
constexpr uint8_t kPduSessionModificationCommandReject = 0b11001101;

constexpr uint8_t kPduSessionReleaseRequest  = 0b11010001;
constexpr uint8_t kPduSessionReleaseReject   = 0b11010010;
constexpr uint8_t kPduSessionReleaseCommand  = 0b11010011;
constexpr uint8_t kPduSessionReleaseComplete = 0b11010100;

constexpr uint8_t k5gsmStatus = 0b11010110;

//------------------------------------------------------------------------------
// Registration Type
constexpr bool kNoFollowOnReqPending = false;
constexpr bool kFollowOnReqPending   = true;

constexpr uint8_t kInitialRegistration          = 0b001;
constexpr uint8_t kMobilityRegistrationUpdating = 0b010;
constexpr uint8_t kPeriodicRegistrationUpdating = 0b011;
constexpr uint8_t kEmergencyRegistration        = 0b100;

//------------------------------------------------------------------------------
// NAS Key Set Identifier
constexpr uint8_t kNasKeySetIdentifierNative       = 0b0;
constexpr uint8_t kNasKeySetIdentifierMapped       = 0b1;
constexpr uint8_t kNasKeySetIdentifierNotAvailable = 0b111;

//------------------------------------------------------------------------------
// 5GS Mobile Identity
constexpr uint8_t kNoIdentity                = 0b000;
constexpr uint8_t kSuci                      = 0b001;
constexpr uint8_t k5gGuti                    = 0b010;
constexpr uint8_t kImei                      = 0b011;
constexpr uint8_t k5gSTmsi                   = 0b100;
constexpr uint8_t kImeisv                    = 0b101;
constexpr uint8_t kMacAddress                = 0b110;
constexpr uint8_t k5gsMobileIdentityMaxValue = kMacAddress;

constexpr uint8_t kEvenIdentity = 0;
constexpr uint8_t kOddIdentity  = 1;

// SUPI format
constexpr uint8_t kSupiFormatImsi                      = 0b000;
constexpr uint8_t kSupiFormatNetworkSpecificIdentifier = 0b001;

// Scheme
constexpr uint8_t kNullScheme          = 0b0000;
constexpr uint8_t kEciesSchemeProfileA = 0b0001;
constexpr uint8_t kEciesSchemeProfileB = 0b0010;

constexpr uint8_t kHomeNetworkPki0WhenPsi0 = 0b00000000;
constexpr uint8_t kHomeNetworkPkiReserved  = 0b11111111;

// Security algorithms
constexpr uint8_t kIa0_5g     = 0b000;
constexpr uint8_t kIa1_128_5g = 0b001;
constexpr uint8_t kIa2_128_5g = 0b010;

constexpr uint8_t kEa0_5g     = 0b000;
constexpr uint8_t kEa1_128_5g = 0b001;
constexpr uint8_t kEa2_128_5g = 0b010;

//------------------------------------------------------------------------------
// 5G MM CAUSE value for 5g mobility management (Annex A)

// Causes related to UE identification
constexpr uint8_t k5gmmCauseIllegalUe                 = 3;
constexpr uint8_t k5gmmCauseIllegalMe                 = 6;
constexpr uint8_t k5gmmCauseUeIdentityCannotBeDerived = 9;
constexpr uint8_t k5gmmCauseImplicitlyDeRegistered    = 10;

// Cause related to subscription options
constexpr uint8_t k5gmmCausePeiNotAccepted                      = 5;
constexpr uint8_t k5gmmCause5gsServicesNotAllowed               = 7;
constexpr uint8_t k5gmmCausePlmnNotAllowed                      = 11;
constexpr uint8_t k5gmmCauseTrackingAreaNotAllowed              = 12;
constexpr uint8_t k5gmmCauseRoamingNotAllowedInThisTrackingArea = 13;
constexpr uint8_t k5gmmCauseNoSuitableCellsInTrackingArea       = 15;
constexpr uint8_t k5gmmCauseN1ModeNotAllowed                    = 27;
constexpr uint8_t k5gmmCauseRedirectionToEpcRequired            = 31;
constexpr uint8_t k5gmmCauseIabNodeOperationNotAuthorized       = 36;
constexpr uint8_t k5gmmCauseNon3gppAccessTo5gcnNotAllowed       = 72;
constexpr uint8_t k5gmmCauseTemporarilyNotAuthorizedForThisSnpn = 74;
constexpr uint8_t k5gmmCausePermanentlyNotAuthorizedForThisSnpn = 75;
constexpr uint8_t k5gmmCauseNotAuthorizedForThisCagOrAuthorizedForCagCellsOnly =
    76;
constexpr uint8_t k5gmmCauseWirelineAccessAreaNotAllowed = 77;

// Causes related to PLMN or SNPN specific network failures and
// congestion/authentication failures
constexpr uint8_t k5gmmCauseMacFailure                      = 20;
constexpr uint8_t k5gmmCauseSynchFailure                    = 21;
constexpr uint8_t k5gmmCauseCongestion                      = 22;
constexpr uint8_t k5gmmCauseUeSecurityCapabilitiesMismatch  = 23;
constexpr uint8_t k5gmmCauseSecurityModeRejectedUnspecified = 24;
constexpr uint8_t k5gmmCauseNon5gAuthenticationUnacceptable = 26;
constexpr uint8_t k5gmmCauseRestrictedServiceArea           = 28;
constexpr uint8_t k5gmmCauseLadnNotAvailable                = 43;
constexpr uint8_t k5gmmCauseNoNetworkSlicesAvailable        = 62;

constexpr uint8_t k5gmmCauseMaximumNumberOfPduSessionsReached           = 65;
constexpr uint8_t k5gmmCauseInsufficientResourcesForSpecificSliceAndDnn = 67;
constexpr uint8_t k5gmmCauseInsufficientResourcesForSpecificSlice       = 69;
constexpr uint8_t k5gmmCauseNgksiAlreadyInUse                           = 71;
constexpr uint8_t k5gmmCauseServingNetworkNotAuthorized                 = 73;
constexpr uint8_t k5gmmCausePayloadWasNotForwarded                      = 90;
constexpr uint8_t k5gmmCauseDnnNotSupportedOrNotSubscribedInTheSlice    = 91;
constexpr uint8_t k5gmmCauseInsufficientUpResourcesForThePduSession     = 92;

// Causes related to invalid messages
constexpr uint8_t k5gmmCauseSemanticallyIncorrect                  = 95;
constexpr uint8_t k5gmmCauseInvalidMandatoryInfo                   = 96;
constexpr uint8_t k5gmmCauseMessageTypeNonExistentOrNotImplemented = 97;
constexpr uint8_t k5gmmCauseMessageTypeNotCompatible               = 98;
constexpr uint8_t k5gmmCauseIeNonExistentOrNotImplemented          = 99;
constexpr uint8_t k5gmmCauseConditionalIeError                     = 100;
constexpr uint8_t k5gmmCauseMessageNotCompatible                   = 101;
constexpr uint8_t k5gmmCauseProtocolErrorUnspecified               = 111;

//------------------------------------------------------------------------------
// UL NAS TRANSPORT payload container type
constexpr uint8_t kN1SmInformation         = 0x01;
constexpr uint8_t kSmsContainer            = 0x02;
constexpr uint8_t kLtePositioningProtocol  = 0x03;
constexpr uint8_t kSorTransparentContainer = 0x04;
constexpr uint8_t kUePolicyContainer       = 0x05;
constexpr uint8_t kUeParametersUpdate      = 0x06;
constexpr uint8_t kMultiplePayloads        = 0x0f;

constexpr uint8_t kPduSessionInitialRequest          = 0b001;
constexpr uint8_t kExistingPduSession                = 0b010;
constexpr uint8_t kPduSessionInitialEmergencyRequest = 0b011;
constexpr uint8_t kExistingEmergencyPduSession       = 0b100;
constexpr uint8_t kPduSessionTypeModificationRequest = 0b101;
constexpr uint8_t kMaPduRequest                      = 0b110;

constexpr uint8_t kDeregistrationTypeMask = 0b00001000;

//------------------------------------------------------------------------------
constexpr uint8_t kNasMessageMinLength                               = 3;
constexpr uint8_t kSecurityProtected5gsNasMessageSequenceNumberOctet = 6;
constexpr uint8_t kSecurityProtected5gsNasMessageHeaderLength =
    7;  // Including 1 octet for Extended protocol discriminator
// 1 octet for Security header type associated with a spare half octet
// 4 octets for Message authentication code
// 1 octet for Sequence number

//------------------------------------------------------------------------------
constexpr int KEncodeDecodeError = -1;
constexpr int KEncodeDecodeOK    = 0;

//------------------------------------------------------------------------------
constexpr int kT3502TimerDefaultValueMin = 12;  // 12 minutes

// Table 10.3.1 @3GPP TS 24.501 V16.1.0 (2019-06)
constexpr int kT3512TimerValueSec = 3240;  // 54 minutes
constexpr int kT3512TimerValueMin = 54;    // 54 minutes
constexpr int kMobileReachableTimerNoEmergencyServicesMin =
    (kT3512TimerValueMin + 4);  // T3512 + 4, not for emergency services
constexpr int kImplicitDeregistrationTimerMin = (kT3512TimerValueMin + 4);

//------------------------------------------------------------------------------
constexpr uint8_t KAccessType3gppAccess    = 0x01;
constexpr uint8_t KAccessTypeNon3gppAccess = 0x02;

//------------------------------------------------------------------------------
// lengths/name of the NAS IEs

//
constexpr uint8_t kType1NasIeLength         = 1;
constexpr uint8_t kType1NasIeFormatTvLength = 1;
constexpr uint8_t kType1NasIeFormatVLength  = 1;
constexpr uint8_t kType2NasIeFormatTLength  = 1;
constexpr uint8_t kType3NasIeFormatTvLength = 1;

// 5GMM Capability
constexpr uint8_t k5gmmCapabilityMinimumLength = 3;
constexpr uint8_t k5gmmCapabilityContentMinimumLength =
    k5gmmCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t k5gmmCapabilityMaximumLength = 15;
constexpr auto k5gmmCapabilityIeName           = "5GMM Capability";

// 5GMM Cause
enum class _5gmmCauseEnum {
  kIllegalUE                                       = 0b00000011,
  kPeiNotAccepted                                  = 0b00000101,
  kIllegalMe                                       = 0b00000110,
  k5gsServicesNotAllowed                           = 0b00000111,
  kUeIdentityCannotBeDerivedByTheNetwork           = 0b00001001,
  kImplicitlyDe_registered                         = 0b00001010,
  kPlmnNotAllowed                                  = 0b00001011,
  kTrackingAreaNotAllowed                          = 0b00001100,
  kRoamingNotAllowedInThisTrackingArea             = 0b00001101,
  kNoSuitableCellsInTrackingArea                   = 0b00001111,
  kMacFailure                                      = 0b00010100,
  kSynchFailure                                    = 0b00010101,
  kCongestion                                      = 0b00010110,
  kUeSecurityCapabilitiesMismatch                  = 0b00010111,
  KSecurityModeRejectedUnspecified                 = 0b00011000,
  kNon_5GAuthenticationUnacceptable                = 0b00011010,
  kN1ModeNotAllowed                                = 0b00011011,
  kRestrictedServiceArea                           = 0b00011100,
  kRedirectionToEpcRequired                        = 0b00011111,
  kLandNotAvailable                                = 0b00101011,
  kMaximumNumberOfPduSessionsReached               = 0b01000001,
  kInsufficientResourcesForSpecificSliceAndDnn     = 0b01000011,
  kInsufficientResourcesForSpecificSlice           = 0b01000101,
  kNgKsiAlreadyInUse                               = 0b01000111,
  kNon_3gppAccessTo5gcnNotAllowed                  = 0b01001000,
  kServingNetworkNotAuthorized                     = 0b01001001,
  kTemporarilyNotAuthorizedForThisSnpn             = 0b01001010,
  kPermanentlyNotAuthorizedForThisSnpn             = 0b01001011,
  kPayloadWasNotForwarded                          = 0b01011010,
  kDnnNotSupportedOrNotSubscribedInTheSlice        = 0b01011011,
  kInsufficientUser_PlaneResourcesForThePduSession = 0b01011100,
  kSemanticallyIncorrectMessage                    = 0b01011111,
  kInvalidMandatoryInformation                     = 0b01100000,
  kMessageTypeNon_ExistentOrNotImplemented         = 0b01100001,
  kMessageTypeNotCompatibleWithTheProtocolState    = 0b01100010,
  kInformationElementNon_ExistentOrNotImplemented  = 0b01100011,
  kConditionalIEError                              = 0b01100100,
  kMessageNotCompatibleWithTheProtocolState        = 0b01100101,
  kProtocolError_Unspecified                       = 0b01101111
};
constexpr uint8_t k5gmmCauseMinimumLength = 1;
constexpr uint8_t k5gmmCauseMaximumLength = 2;
constexpr auto k5gmmCauseIeName           = "5GMM Cause";

// 5GS Deregistration Type
typedef struct _5gs_deregistration_type_s {
  uint8_t iei : 4;
  uint8_t switch_off : 1;
  uint8_t re_registration_required : 1;
  uint8_t access_type : 2;
} _5gs_deregistration_type_t;
constexpr auto k5gsDeregistrationTypeIeName = "5GS De-registration Type";

// 5GS DRX Parameters
constexpr uint8_t k5gsDrxParametersLength = 3;
constexpr uint8_t k5gsDrxParametersContentLength =
    k5gsDrxParametersLength - 2;  // Length - 2 octets for IEI/Length
constexpr auto k5gsDrxParametersIeName = "5GS DRX Parameters";

// 5GS Identity Type
constexpr auto k5gsIdentityTypeIeName = "5GS Identity Type";

// 5GS Mobile Identity
constexpr uint8_t k5gsMobileIdentityMinimumLength = 4;
constexpr uint8_t k5gsMobileIdentityContentMinimumLength =
    k5gsMobileIdentityMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint8_t k5gsMobileIdentityIe5gGutiLength  = 11;
constexpr uint8_t k5gsMobileIdentityIe5gSTmsiLength = 7;
constexpr auto k5gsMobileIdentityIeName             = "5GS Mobile Identity";

// 5GS Network Feature Support
constexpr uint8_t k5gsNetworkFeatureSupportMinimumLength = 3;
constexpr uint8_t k5gsNetworkFeatureSupportContentMinimumLength =
    k5gsNetworkFeatureSupportMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t k5gsNetworkFeatureSupportMaximumLength = 5;
constexpr auto k5gsNetworkFeatureSupportIeName = "5GS Network Feature Support";

// 5GS Registration Result
constexpr uint8_t k5gsRegistrationResultLength = 3;
constexpr uint8_t k5gsRegistrationResultContentLength =
    k5gsRegistrationResultLength -
    2;  // Minimum length - 2 bytes for IEI/Length
constexpr auto k5gsRegistrationResultIeName = "5GS Registration Result";

// 5GS Registration Type
constexpr auto k5gsRegistrationTypeName = "5GS Registration Type";

// 5GS Tracking Area Identity
constexpr uint8_t k5gsTrackingAreaIdentityLength = 7;
constexpr auto k5gsTrackingAreaIdentityIeName    = "5GS Tracking Area Identity";

// 5GS Tracking Area Identity List
constexpr uint8_t k5gsTrackingAreaIdListMinimumLength = 9;
constexpr uint8_t k5gsTrackingAreaIdListContentMinimumLength =
    k5gsTrackingAreaIdListMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t k5gsTrackingAreaIdListMaximumLength        = 114;
constexpr uint8_t k5gsTrackingAreaIdListMaximumSupportedTAIs = 16;
constexpr auto k5gsTrackingAreaIdListIeName = "5GS Tracking Area Identity List";

// 5GS Update Type
constexpr uint8_t k5gsUpdateTypeLength = 3;
constexpr uint8_t k5gsUpdateTypeContentLength =
    k5gsUpdateTypeLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr auto k5gsUpdateTypeIeName = "5GS Update Type";

// ABBA
constexpr uint8_t kAbbaMinimumLength = 4;
constexpr uint8_t kAbbaContentMinimumLength =
    kAbbaMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kAbbaIeName = "Abba";

// Additional 5G Security Information
constexpr uint8_t kAdditional5gSecurityInformationLength = 3;
constexpr uint8_t kAdditional5gSecurityInformationContentLength =
    kAdditional5gSecurityInformationLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kAdditional5gSecurityInformationIeName =
    "Additional 5G Security Information";

// Additional Information
constexpr uint8_t kAdditionalInformationMinimumLength = 3;
constexpr uint8_t kAdditionalInformationContentMinimumLength =
    kAdditionalInformationMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint16_t kAdditionalInformationMaximumLength = 257;
constexpr auto kAdditionalInformationIeName = "Additional Information";

// Allowed PDU Session Status
constexpr uint8_t kAllowedPduSessionStatusMinimumLength = 4;
constexpr uint8_t kAllowedPduSessionStatusContentMinimumLength =
    kAllowedPduSessionStatusMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kAllowedPduSessionStatusMaximumLength = 34;
constexpr auto kAllowedPduSessionStatusIeName = "Allowed PDU Session Status";

// Authentication Failure Parameter
constexpr uint8_t kAuthenticationFailureParameterLength = 16;
constexpr uint8_t kAuthenticationFailureParameterContentLength =
    kAuthenticationFailureParameterLength -
    2;  // Minimum length - 2 bytes for IEI/Length
constexpr auto kAuthenticationFailureParameterIeName =
    "Authentication Failure Parameter";

//
constexpr uint8_t kAuthenticationParameterAutnLength = 18;
constexpr uint8_t kAuthenticationParameterAutnValueLength =
    kAuthenticationParameterAutnLength - 2;
constexpr auto kAuthenticationParameterAutnIeName =
    "Authentication Parameter AUTN";

//
constexpr uint8_t kAuthenticationParameterRandLength = 17;
constexpr uint8_t kAuthenticationParameterRandValueLength =
    kAuthenticationParameterRandLength - 1;
constexpr auto kAuthenticationParameterRandIeName =
    "Authentication Parameter RAND";

//
constexpr uint8_t kAuthenticationResponseParameterMinimumLength = 6;
constexpr uint8_t kAuthenticationResponseParameterContentMinimumLength =
    kAuthenticationResponseParameterMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kAuthenticationResponseParameterMaximumLength = 18;
constexpr auto kAuthenticationResponseParameterIeName =
    "Authentication Response Parameter";

//
constexpr auto kConfigurationUpdateIndicationIeName =
    "Configuration Update Indication";

//
constexpr auto kControlPlaneServiceTypeIeName = "Control Plane Service Type";

//
constexpr uint8_t kDnnMinimumLength = 3;
constexpr uint8_t kDnnContentMinimumLength =
    kDnnMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kDnnMaximumLength = 102;
constexpr auto kDnnIeName           = "DNN";

//
constexpr uint8_t kEapMessageMinimumLength = 7;
constexpr uint8_t kEapMessageContentMinimumLength =
    kEapMessageMinimumLength - 3;  // Minimum length - 3 octets for IEI/Length
constexpr uint16_t kEapMessageMaximumLength = 1503;
constexpr auto kEapMessageIeName            = "EAP Message";

//
constexpr uint8_t kEpsBearerContextStatusLength = 4;
constexpr uint8_t kEpsBearerContextStatusContentLength =
    kEpsBearerContextStatusLength - 2;  // Length - 2 octets for IEI/Length
constexpr auto kEpsBearerContextStatusIeName = "EPS Bearer Context Status";

//
constexpr uint8_t kEpsNasMessageContainerMinimumLength = 4;
constexpr uint8_t kEpsNasMessageContainerContentMinimumLength =
    kEpsNasMessageContainerMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint8_t kEpsNasMessageContainer    = 253;
constexpr auto kEpsNasMessageContainerIeName = "EPS NAS Message Container";

//
constexpr uint8_t kEpsNasSecurityAlgorithmsLength = 2;
constexpr auto kEpsNasSecurityAlgorithmsIeName = "EPS NAS Security Algorithms";

//
constexpr uint8_t kExtendedDrxParametersLength = 3;
constexpr uint8_t kExtendedDrxParametersContentLength =
    kExtendedDrxParametersLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kExtendedDrxParametersIeName = "Extended DRX Parameters";

//
constexpr uint8_t kExtendedProtocolDiscriminatorLength = 1;
constexpr auto kExtendedProtocolDiscriminatorIeName =
    "Extended Protocol Discriminator";

//
constexpr uint8_t kGprsTimer2Length = 3;
constexpr uint8_t kGprsTimer2ContentLength =
    kGprsTimer2Length - 2;  // Length - 2 octets for IEI/Length
constexpr auto kGprsTimer2IeName = "GPRS Timer 2";

//
constexpr uint8_t kGprsTimer3Length = 3;
constexpr uint8_t kGprsTimer3ContentLength =
    kGprsTimer3Length - 2;  // Length - 2 octets for IEI/Length
constexpr auto kGprsTimer3IeName = "GPRS Timer 3";

//
constexpr auto kImeisvRequestIeName = "IMEISV Request";

// LADN

//
constexpr uint8_t kLadnIndicationMinimumLength = 3;
constexpr uint8_t kLadnIndicationContentMinimumLength =
    kLadnIndicationMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint16_t kLadnIndicationMaximumLength        = 811;
constexpr uint8_t kLadnIndicationMaximumSupportedLadns = 8;
constexpr auto kLadnIndicationIeName                   = "LADN Indication";

//
constexpr uint8_t kLadnInformationMinimumLength = 3;
constexpr uint8_t kLadnInformationContentMinimumLength =
    kLadnInformationMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint16_t kLadnInformationMaximumLength        = 1715;
constexpr uint8_t kLadnInformationMaximumSupportedLadns = 8;
constexpr auto kLadnInformationIeName                   = "LADN Information";

//
constexpr auto kMaPduSessionInformationIeName = "MA PDU Session Information";

//
constexpr uint8_t kMicoIndicationIELength = 1;
constexpr auto kMicoIndicationIeName      = "MICO Indication";

//
constexpr auto kNasKeySetIdentifierName = "NAS Key Set Identifier";

//
constexpr uint8_t kNasMessageContainerMinimumLength = 3;
constexpr uint8_t kNasMessageContainerContentMinimumLength =
    kNasMessageContainerMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint32_t kNasMessageContainerMaximumLength = 65535;
constexpr auto kNasMessageContainerIeName            = "NAS Message Container";

//
constexpr uint8_t kNasMessageTypeIeSize = 1;
constexpr auto kNasMessageTypeIeName    = "NAS Message Type";

//
constexpr uint8_t kNasSecurityAlgorithmsLength = 2;
constexpr auto kNasSecurityAlgorithmsIeName    = "NAS Security Algorithms";

//
constexpr uint8_t kNetworkNameMinimumLength = 3;
constexpr uint8_t kNetworkNameContentMinimumLength =
    kNetworkNameMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kNetworkNameIeName = "Network Name";

//
constexpr uint8_t kNetworkSlicingIndicationLength = 1;
constexpr auto kNetworkSlicingIndicationIeName = "Network Slicing Indication";

//
constexpr uint8_t kNon3gppNwProvidedPoliciesLength = 1;
constexpr auto kNon3gppNwProvidedPoliciesIeName =
    "Non-3GPP NW Provided Policies";

//
constexpr uint8_t kNssaiMinimumLength = 4;
constexpr uint8_t kNssaiContentMinimumLength =
    kNssaiMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kNssaiMaximumLength = 146;
constexpr auto kNssaiIeName           = "NSSAI";

//
constexpr uint8_t kNssaiInclusionModeLength = 1;
constexpr auto kNssaiInclusionModeIeName    = "NSSAI Inclusion Mode";

//
constexpr uint8_t kPayloadContainerMinimumLength = 4;
constexpr uint8_t kPayloadContainerContentMinimumLength =
    kPayloadContainerMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint32_t kPayloadContainerMaximumLength = 65538;
constexpr auto kPayloadContainerIeName            = "Payload Container";

//
constexpr uint8_t kPayloadContainerTypeLength = 1;
constexpr auto kPayloadContainerTypeIeName    = "Payload Container Type";

//
constexpr uint8_t kPduSessionIdentity2Length = 2;
constexpr auto kPduSessionIdentity2IeName    = "PDU Session Identity 2";

//
constexpr uint8_t kPduSessionReactivationResultMinimumLength = 4;
constexpr uint8_t kPduSessionReactivationResultContentMinimumLength =
    kPduSessionReactivationResultMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kPduSessionReactivationResultMaximumLength = 34;
constexpr auto kPduSessionReactivationResultIeName =
    "PDU Session Reactivation Result";

//
constexpr uint8_t kPduSessionReactivationResultErrorCauseMinimumLength = 5;
constexpr uint8_t kPduSessionReactivationResultErrorCauseContentMinimumLength =
    kPduSessionReactivationResultErrorCauseMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint32_t kPduSessionReactivationResultErrorCauseMaximumLength = 515;
constexpr auto kPduSessionReactivationResultErrorCauseIeName =
    "PDU Session Reactivation Result Error Cause";

//
constexpr uint8_t kPduSessionStatusMinimumLength = 4;
constexpr uint8_t kPduSessionStatusContentMinimumLength =
    kPduSessionStatusMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kPduSessionStatusMaximumLength = 34;
constexpr auto kPduSessionStatusIeName           = "PDU Session Status";

//
constexpr uint8_t kPlmnListMinimumLength = 5;
constexpr uint8_t kPlmnListContentMinimumLength =
    kPlmnListMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kPlmnListMaximumLength = 47;
constexpr auto kPlmnListIeName           = "PLMN List";

//
constexpr uint8_t kRejectedNssaiMinimumLength = 4;
constexpr uint8_t kRejectedNssaiContentMinimumLength =
    kRejectedNssaiMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kRejectedNssaiMaximumLength = 42;
constexpr auto kRejectedNssaiIeName           = "Rejected NSSAI";

// RejectedSNssai

//
constexpr auto kReleaseAssistanceIndicationIeName =
    "Release Assistance Indication";

//
constexpr auto kRequestTypeIeName = "Request Type";

//
constexpr uint8_t kS1UeSecurityCapabilityMinimumLength = 4;
constexpr uint8_t kS1UeSecurityCapabilityContentMinimumLength =
    kS1UeSecurityCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kS1UeSecurityCapabilityMaximumLength = 7;
constexpr auto kS1UeSecurityCapabilityIeName = "S1 UE Security Capability";

//
constexpr uint8_t kSecurityHeaderTypeLength = 1;
constexpr auto kSecurityHeaderTypeIeName    = "Security Header Type";

//
constexpr uint8_t kServiceAreaListMinimumLength = 6;
constexpr uint8_t kServiceAreaListContentMinimumLength =
    kServiceAreaListMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kServiceAreaListMaximumLength        = 114;
constexpr uint8_t kServiceAreaListMaximumSupportedTAIs = 16;
constexpr auto kServiceAreaListIeName                  = "Service Area List";

//
constexpr auto kServiceTypeIeName = "Service Type";

//
constexpr uint8_t kSNssaiMinimumLength = 3;
constexpr uint8_t kSNssaiContentMinimumLength =
    kSNssaiMinimumLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kSNssaiMaximumLength = 10;
constexpr auto kSNssaiIeName           = "S-NSSAI";

//
constexpr uint8_t kSorTransparentContainerMinimumLength = 20;
constexpr uint8_t kSorTransparentContainerContentMinimumLength =
    kSorTransparentContainerMinimumLength -
    3;  // Minimum length - 3 octets for IEI/Length
constexpr uint8_t kSorTransparentContainerIeMacLength = 16;
constexpr auto kSorTransparentContainerIeName = "SOR Transparent Container";

//
constexpr uint8_t kUeNetworkCapabilityMinimumLength = 4;
constexpr uint8_t kUeNetworkCapabilityContentMinimumLength =
    kUeNetworkCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kUeNetworkCapabilityMaximumLength = 15;
constexpr auto kUeNetworkCapabilityIeName           = "UE Network Capability";

//
constexpr uint8_t kUeRadioCapabilityIdMinimumLength = 4;
constexpr uint8_t kUeRadioCapabilityIdContentMinimumLength =
    kUeRadioCapabilityIdMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kUeRadioCapabilityIdIeName = "UE Radio Capability ID";

//
constexpr uint8_t kUeSecurityCapabilityMinimumLength = 4;
constexpr uint8_t kUeSecurityCapabilityContentMinimumLength =
    kUeSecurityCapabilityMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kUeSecurityCapabilityMaximumLength = 10;
constexpr auto kUeSecurityCapabilityIeName           = "UE Security Capability";

//
constexpr uint8_t kUeStatusIeLength = 3;
constexpr uint8_t kUeStatusIeContentLength =
    kUeStatusIeLength - 2;  // Minimum length - 2 octets for IEI/Length
constexpr auto kUeStatusIeName = "UE Status";

//
constexpr uint8_t kUeUsageSettingLength = 3;
constexpr uint8_t kUeUsageSettingContentLength =
    kUeUsageSettingLength - 2;  // IE length - 2 octets for IEI/Length
constexpr auto kUeUsageSettingIeName = "UE's Usage Setting";

//
constexpr uint8_t kUplinkDataStatusMinimumLength = 4;
constexpr uint8_t kUplinkDataStatusContentMinimumLength =
    kUplinkDataStatusMinimumLength -
    2;  // Minimum length - 2 octets for IEI/Length
constexpr uint8_t kUplinkDataStatusMaximumLength = 34;
constexpr auto kUplinkDataStatusIeName           = "Uplink Data Status";

//------------------------------------------------------------------------------
enum class _5g_ia_e {
  _5G_IA0 = 0,
  _5G_IA1 = 1,
  _5G_IA2 = 2,
  _5G_IA3 = 3,
  _5G_IA4 = 4,
  _5G_IA5 = 5,
  _5G_IA6 = 6,
  _5G_IA7 = 7
};

static std::string get_5g_ia_str(_5g_ia_e e) {
  switch (e) {
    case _5g_ia_e::_5G_IA0: {
      return "5G_IA0";
    } break;
    case _5g_ia_e::_5G_IA1: {
      return "5G_IA1";
    } break;
    case _5g_ia_e::_5G_IA2: {
      return "5G_IA2";
    } break;
    case _5g_ia_e::_5G_IA3: {
      return "5G_IA3";
    } break;
    case _5g_ia_e::_5G_IA4: {
      return "5G_IA4";
    } break;
    case _5g_ia_e::_5G_IA5: {
      return "5G_IA5";
    } break;
    case _5g_ia_e::_5G_IA6: {
      return "5G_IA6";
    } break;
    case _5g_ia_e::_5G_IA7: {
      return "5G_IA7";
    } break;
    default: {
      return "UNKNOWN 5GS ENCRYPTION ALGORITHM";
    }
  }
}

static _5g_ia_e get_5g_ia(std::string ia) {
  if (!ia.compare("NIA0")) {
    return _5g_ia_e::_5G_IA0;
  }
  if (!ia.compare("NIA1")) {
    return _5g_ia_e::_5G_IA1;
  }
  if (!ia.compare("NIA2")) {
    return _5g_ia_e::_5G_IA2;
  }
  if (!ia.compare("NIA3")) {
    return _5g_ia_e::_5G_IA3;
  }
  if (!ia.compare("NIA4")) {
    return _5g_ia_e::_5G_IA4;
  }
  if (!ia.compare("NIA5")) {
    return _5g_ia_e::_5G_IA5;
  }
  if (!ia.compare("NIA6")) {
    return _5g_ia_e::_5G_IA6;
  }
  if (!ia.compare("NIA7")) {
    return _5g_ia_e::_5G_IA7;
  }
  return _5g_ia_e::_5G_IA0;
}

enum class _5g_ea_e {
  _5G_EA0 = 0,
  _5G_EA1 = 1,
  _5G_EA2 = 2,
  _5G_EA3 = 3,
  _5G_EA4 = 4,
  _5G_EA5 = 5,
  _5G_EA6 = 6,
  _5G_EA7 = 7
};

static std::string get_5g_ea_str(_5g_ea_e e) {
  switch (e) {
    case _5g_ea_e::_5G_EA0: {
      return "5G_EA0";
    } break;
    case _5g_ea_e::_5G_EA1: {
      return "5G_EA1";
    } break;
    case _5g_ea_e::_5G_EA2: {
      return "5G_EA2";
    } break;
    case _5g_ea_e::_5G_EA3: {
      return "5G_EA3";
    } break;
    case _5g_ea_e::_5G_EA4: {
      return "5G_EA4";
    } break;
    case _5g_ea_e::_5G_EA5: {
      return "5G_EA5";
    } break;
    case _5g_ea_e::_5G_EA6: {
      return "5G_EA6";
    } break;
    case _5g_ea_e::_5G_EA7: {
      return "5G_EA7";
    } break;
    default: {
      return "UNKNOWN 5GS INTEGRITY ALGORITHM";
    }
  }
}

static _5g_ea_e get_5g_ea(std::string ea) {
  if (!ea.compare("NEA0")) {
    return _5g_ea_e::_5G_EA0;
  }
  if (!ea.compare("NEA1")) {
    return _5g_ea_e::_5G_EA1;
  }
  if (!ea.compare("NEA2")) {
    return _5g_ea_e::_5G_EA2;
  }
  if (!ea.compare("NEA3")) {
    return _5g_ea_e::_5G_EA3;
  }
  if (!ea.compare("NEA4")) {
    return _5g_ea_e::_5G_EA4;
  }
  if (!ea.compare("NEA5")) {
    return _5g_ea_e::_5G_EA5;
  }
  if (!ea.compare("NEA6")) {
    return _5g_ea_e::_5G_EA6;
  }
  if (!ea.compare("NEA7")) {
    return _5g_ea_e::_5G_EA7;
  }
  return _5g_ea_e::_5G_EA0;
}
#endif
