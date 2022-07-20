/*
 * Copyright (C) 2011-2021 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 * File: quoting_enclave_tdqe.h 
 *  
 * Description: Definitions and prototypes 
 * for the sample application's quoteing
 * enclave. 
 *
 */

#ifndef _QUOTING_ENCLAVE_TDQE_H_
#define _QUOTING_ENCLAVE_TDQE_H_

#include <stdlib.h>
#include <assert.h>

#include "user_types.h"
#include "ecdsa_quote.h"
#include "sgx_tcrypto.h"

#define QE_QUOTE_VERSION        4  ///< Version of the quote structure that supports ECDSA (and EPID).  It is a generic form of the Quote.

#define TDQE_MK_ERROR(x)         (0x00010000|(x))

/** Possible errors generated by the TDQE */
typedef enum _tdqe_error_t {
    TDQE_SUCCESS                      = 0x0000,
    TDQE_ERROR_UNEXPECTED             = TDQE_MK_ERROR(0x0001),      ///< Unexpected error.
    TDQE_ERROR_INVALID_PARAMETER      = TDQE_MK_ERROR(0x0002),      ///< The parameter is incorrect.
    TDQE_ERROR_OUT_OF_MEMORY          = TDQE_MK_ERROR(0x0003),      ///< Not enough memory is available to complete this operation.
    TDQE_ECDSABLOB_ERROR              = TDQE_MK_ERROR(0x0004),      ///< There was a problem with verifying the ECDSA Blob.
    TDQE_ERROR_CRYPTO                 = TDQE_MK_ERROR(0x0005),      ///< An error occurred in the encryption library.
    TDQE_ERROR_ATT_KEY_GEN            = TDQE_MK_ERROR(0x0006),      ///< Error generating the ECDSA Attestation key pair.
    TDQE_ERROR_WRONG_STATE            = TDQE_MK_ERROR(0x0007),      ///< Key generation and certification flow is stateful. ECALL order is incorrect.
    TDQE_ERROR_INVALID_REPORT         = TDQE_MK_ERROR(0x0008),      ///< The TD report is invalid
    TDQE_UNABLE_TO_GENERATE_QE_REPORT = TDQE_MK_ERROR(0x0009),      ///< The QE was unable to generate its own report targeting the application enclave.
    TDQE_ERROR_INVALID_HASH           = TDQE_MK_ERROR(0x000A),      ///< The tee_tcb_info_hash or tee_tcb_info_hash is invalid.
    TDQE_ERROR_INVALID_PLATFORM       = TDQE_MK_ERROR(0x000B),      ///< This QE cannot be used on this platform
    TDQE_REPORT_FORMAT_NOT_SUPPORTED  = TDQE_MK_ERROR(0x000C),      ///< This TD report is not compatible with this QE
} tdqe_error_t;

#pragma pack(push, 1)

/** Structure definition of the RSA key used to decrypt the PCE's PPID */
typedef struct _pce_rsaoaep_2048_encrypt_pub_key_t {
    uint8_t n[REF_RSA_OAEP_2048_MOD_SIZE];   ///< RSA 2048 public modulus
    uint8_t e[REF_RSA_OAEP_2048_EXP_SIZE];   ///< RSA 2048 public exponent
} pce_rsaoaep_2048_encrypt_pub_key_t;

/** Structure definition of the RSA key used to decrypt the PCE's PPID */
typedef struct _pce_rsaoaep_3072_encrypt_pub_key_t {
    uint8_t n[REF_RSA_OAEP_3072_MOD_SIZE];   ///< RSA 3072 public modulus
    uint8_t e[REF_RSA_OAEP_3072_EXP_SIZE];   ///< RSA 3072 public exponent
} pce_rsaoaep_3072_encrypt_pub_key_t;

/** Structure used when storing the encrypted PPID from the PCE in the ECDSA Blob. */
typedef struct _ref_encrypted_ppid_t {
    uint8_t  crypto_suite;              ///< Encryption crypto algorithm used to encrypt the PPID
    uint32_t encrypted_ppid_buf_size;  ///< The size of the buffer holding the encrypted PPID
    uint8_t  encrypted_ppid[REF_RSA_OAEP_3072_MOD_SIZE];         ///< The buffer containing the encrypted PPID.  Worst case size is 3072
}ref_encrypted_ppid_t;

/** Used to store the ciphertext data of the ECDSA key and the certification data in a sealed blob.  This portion of the blob is
*  authenticated and encrypted.
*  The contents vary depending on the certification key type (sgx_cert_key_type_t) */
typedef struct _ref_ciphertext_ecdsa_data_sdk_t {
    sgx_ec256_private_t  ecdsa_private_key; ///< ECDSA private key stored in little endian
    uint8_t              is_clear_ppid;     ///< Indicates whether the PPID is stored in the clear or encrypted
    union {
        uint8_t              ppid[16];                   ///< PPID if stored in the clear.
        ref_encrypted_ppid_t encrypted_ppid_data;        ///< Encrypted PPID data
    };
}ref_ciphertext_ecdsa_data_sdk_t;

#pragma pack(pop)

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* !_QUOTING_ENCLAVE_TDQE_H_ */