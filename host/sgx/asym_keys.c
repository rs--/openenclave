// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/bits/result.h>
#include <openenclave/host.h>
#include <openenclave/internal/asym_keys.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/raise.h>
#include <openenclave/internal/utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "internal_u.h"

oe_result_t oe_get_public_key_by_policy(
    oe_enclave_t* enclave,
    oe_seal_policy_t seal_policy,
    const oe_asymmetric_key_params_t* key_params,
    uint8_t** key_buffer,
    size_t* key_buffer_size,
    uint8_t** key_info,
    size_t* key_info_size)
{
    oe_result_t result = OE_UNEXPECTED;
    uint32_t retval;
    const size_t KEY_BUFFER_SIZE = 1024;
    const size_t KEY_INFO_SIZE = 1024;
    struct
    {
        uint8_t* key_buffer;
        size_t key_buffer_size;
        uint8_t* key_info;
        size_t key_info_size;
    } arg;

    memset(&arg, 0, sizeof(arg));

    if (key_buffer)
        *key_buffer = NULL;

    if (key_buffer_size)
        *key_buffer_size = 0;

    if (key_info)
        *key_info = NULL;

    if (key_info_size)
        *key_info_size = 0;

    if (!key_buffer || !key_buffer_size || !key_info || !key_info_size)
    {
        OE_RAISE(OE_INVALID_PARAMETER);
    }

    /* Allocate the buffers. */
    {
        arg.key_buffer_size = KEY_BUFFER_SIZE;
        arg.key_info_size = KEY_INFO_SIZE;

        if (!(arg.key_buffer = malloc(arg.key_buffer_size)))
            OE_RAISE(OE_OUT_OF_MEMORY);

        if (!(arg.key_info = malloc(arg.key_info_size)))
            OE_RAISE(OE_OUT_OF_MEMORY);
    }

    if (oe_internal_get_public_key_by_policy(
            enclave,
            &retval,
            (uint32_t)seal_policy,
            key_params,
            arg.key_buffer,
            arg.key_buffer_size,
            &arg.key_buffer_size,
            arg.key_info,
            arg.key_info_size,
            &arg.key_info_size) != OE_OK)
    {
        OE_RAISE(OE_FAILURE);
    }

    /* If the buffers were too small, try again with corrected sizes. */
    if ((oe_result_t)retval == OE_BUFFER_TOO_SMALL)
    {
        if (!(arg.key_buffer = realloc(arg.key_buffer, arg.key_buffer_size)))
            OE_RAISE(OE_OUT_OF_MEMORY);

        if (!(arg.key_info = realloc(arg.key_info, arg.key_info_size)))
            OE_RAISE(OE_OUT_OF_MEMORY);

        if (oe_internal_get_public_key_by_policy(
                enclave,
                &retval,
                (uint32_t)seal_policy,
                key_params,
                arg.key_buffer,
                arg.key_buffer_size,
                &arg.key_buffer_size,
                arg.key_info,
                arg.key_info_size,
                &arg.key_info_size) != OE_OK)
        {
            OE_RAISE(OE_FAILURE);
        }
    }

    OE_CHECK((oe_result_t)retval);

    *key_buffer = arg.key_buffer;
    *key_buffer_size = arg.key_buffer_size;
    arg.key_buffer = NULL;

    *key_info = arg.key_info;
    *key_info_size = arg.key_info_size;
    arg.key_info = NULL;

    result = OE_OK;

done:

    if (arg.key_buffer)
    {
        oe_secure_zero_fill(arg.key_buffer, arg.key_buffer_size);
        free(arg.key_buffer);
    }

    if (arg.key_info)
    {
        oe_secure_zero_fill(arg.key_info, arg.key_info_size);
        free(arg.key_buffer);
    }

    return result;
}

oe_result_t oe_get_public_key(
    oe_enclave_t* enclave,
    const oe_asymmetric_key_params_t* key_params,
    const uint8_t* key_info,
    size_t key_info_size,
    uint8_t** key_buffer,
    size_t* key_buffer_size)
{
    oe_result_t result = OE_UNEXPECTED;
    oe_get_public_key_args_t args;

    if (!key_buffer || !key_buffer_size)
        OE_RAISE(OE_INVALID_PARAMETER);

    /* Setup input params. */
    args.key_params = *key_params;
    args.key_info = key_info;
    args.key_info_size = key_info_size;

    OE_CHECK(oe_ecall(enclave, OE_ECALL_GET_PUBLIC_KEY, (uint64_t)&args, NULL));

    /* Set the output params. */
    *key_buffer = args.key_buffer;
    *key_buffer_size = args.key_buffer_size;

    result = OE_OK;

done:
    return result;
}

void oe_free_key(
    uint8_t* key_buffer,
    size_t key_buffer_size,
    uint8_t* key_info,
    size_t key_info_size)
{
    if (key_buffer)
    {
        oe_secure_zero_fill(key_buffer, key_buffer_size);
        free(key_buffer);
    }

    if (key_info)
    {
        oe_secure_zero_fill(key_info, key_info_size);
        free(key_info);
    }
}
