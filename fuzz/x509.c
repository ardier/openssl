/*
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * https://www.openssl.org/source/license.html
 * or in the file LICENSE in the source distribution.
 */
#include <stdlib.h>  // For atoi
#include <stdio.h>   // For printf
#include <string.h>  // For string manipulation#include <stdint.h>
#include <stddef.h>
#include <unistd.h>  // For sleep
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

int FuzzerTestOneInput(const uint8_t *buf, size_t len) {
    // Ensure the input buffer is valid and non-empty
    if (buf == NULL || len == 0) {
        return 0;
    }

    // Convert the input buffer into a null-terminated string
    char input[len + 1];
    memcpy(input, buf, len);
    input[len] = '\0';

    // Find the comma in the input string
    char *comma_pos = strchr(input, ',');
    if (comma_pos == NULL) {
        return 0;  // No comma found; exit early
    }

    // Split the string into two parts: y_str and x_str
    *comma_pos = '\0';
    char *y_str = input;
    char *x_str = comma_pos + 1;

    // Convert y and x to integers using atoi
    int y = atoi(y_str);
    int x = atoi(x_str);

    // Initialize result to 1
    int result = 1;

    // Simulate some processing logic
    for (int i = 0; i < x; ++i) {
        result += y;

        // Simulate a delay for each iteration
        if (i % 2 == 0) {
            result *= 2;
        } else {
            result -= y / 2;
        }
        sleep(10);
    }

    // Check for a specific condition with many paths
    if (result % 9345349 == 0) {
        printf("Result is divisible by 9345349. This is a test message.\n");
    } else {
        // Loop to create many paths
        for (int i = 0; i < 10; ++i) {
            if (result % (i + 2) == 0) {
                printf("Result is divisible by %d.\n", i + 2);
            } else if ((result + i) % (i + 3) == 0) {
                printf("Result + %d is divisible by %d.\n", i, i + 3);
            } else if ((result - i) % (i + 4) == 0) {
                printf("Result - %d is divisible by %d.\n", i, i + 4);
            } else {
                printf("Result does not satisfy conditions for iteration %d.\n", i);
            }
        sleep(5);
        }
    }

    return 0;
}



void FuzzerCleanup(void)
{
    FuzzerClearRand();
}
