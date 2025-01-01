/*
 * Copyright 2016-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0.
 * You may not use this file except in compliance with the License.
 * You may obtain a copy at
 * https://www.openssl.org/source/license.html
 */
#include <stdlib.h>  // For atoi
#include <stdio.h>   // For printf
#include <string.h>  // For string manipulation
#include <stdint.h>
#include <stddef.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "fuzzer.h"

#define LOOP_ITERATIONS 50000000  // Fine-tune for delay simulation

int FuzzerInitialize(int *argc, char ***argv)
{
    FuzzerSetRand();
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();
    CRYPTO_free_ex_index(0, -1);
    return 1;
}

void busy_loop(int duration) {
    volatile int sum = 0;
    for (int i = 0; i < LOOP_ITERATIONS * duration; i++) {
        sum += i % 2 ? i : -i;
    }
}

int FuzzerTestOneInput(const uint8_t *buf, size_t len) {
    if (buf == NULL || len == 0) {
        return 0;
    }

    char input[len + 1];
    memcpy(input, buf, len);
    input[len] = '\0';

    char *comma_pos = strchr(input, ',');
    if (comma_pos == NULL) {
        return 0;
    }

    *comma_pos = '\0';
    char *y_str = input;
    char *x_str = comma_pos + 1;

    int y = atoi(y_str);
    int x = atoi(x_str);
    int result = 1;

    for (int i = 0; i < x; ++i) {
        result += y;

        if (i % 2 == 0) {
            result *= 2;
        } else {
            result -= y / 2;
        }

        // Simulate delay through computation
        busy_loop(2);
    }

    // Hard-to-reach branch enhancement
    if (result % 9345349 == 0 && result > 10000000) {
        printf("Entering hard-to-reach branch. Performing heavy processing...\n");

        for (int i = 0; i < 500; ++i) {
            printf("Processing line %d in deep branch...\n", i + 1);
            result += i % 5;

            if (i % 50 == 0) {
                printf("Checkpoint at %d lines.\n", i);
            }
        }
    } else {
        for (int i = 0; i < 10; ++i) {
            if (result % (i + 2) == 0) {
                printf("Result divisible by %d.\n", i + 2);
            } else if ((result + i) % (i + 3) == 0) {
                printf("Result + %d divisible by %d.\n", i, i + 3);
            } else if ((result - i) % (i + 4) == 0) {
                printf("Result - %d divisible by %d.\n", i, i + 4);
            } else {
                printf("No conditions met at iteration %d.\n", i);
            }
            busy_loop(1);  // Simulate 1-second work loop
        }
    }

    return 0;
}

void FuzzerCleanup(void)
{
    FuzzerClearRand();
}
