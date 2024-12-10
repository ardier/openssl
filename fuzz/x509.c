/*
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * https://www.openssl.org/source/license.html
 * or in the file LICENSE in the source distribution.
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "fuzzer.h"

int FuzzerInitialize(int *argc, char ***argv)
{
    FuzzerSetRand();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();
    CRYPTO_free_ex_index(0, -1);
    return 1;
}

/* Test a single input to the fuzzer */
int FuzzerTestOneInput(const uint8_t *buf, size_t len)
{
    /* Print a simple message to observe output */
    printf("Hello, World!\n");

    /* Process the input (for demonstration, just print its length) */
    printf("Received input of length: %zu\n", len);

    return 0;
}

void FuzzerCleanup(void)
{
    FuzzerClearRand();
}
