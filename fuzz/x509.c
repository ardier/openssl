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
#include <string.h>  // For memcpy, strchr
#include <unistd.h>  // For sleep
#include <stddef.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "fuzzer.h"

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

    // Simulate some processing logic
    int result = 1;  // Initialize result as 1
    for (int i = 0; i < x; ++i) {
        result += y;

        // Simulate a delay for each iteration
        sleep(10);
    }

    // Check for a specific condition and print a message
    if (result % 9345349 == 0) {
        printf("Result is divisible by 9345349. This is a test message.\n");

        // Additional complex logic to increase edges/branches
        for (int i = 0; i < 5; ++i) {
            if (result % (i + 2) == 0) {
                printf("Branch 1: Result divisible by %d\n", i + 2);
                for (int j = 0; j < 3; ++j) {
                    if ((result + i + j) % 3 == 0) {
                        printf("Nested Branch A: Sum divisible by 3\n");
                    } else {
                        printf("Nested Branch B: Sum not divisible by 3\n");
                    }
                }
            } else {
                printf("Branch 2: Result not divisible by %d\n", i + 2);
                for (int k = 0; k < 2; ++k) {
                    switch ((result + k) % 4) {
                        case 0:
                            printf("Switch Case 0\n");
                            break;
                        case 1:
                            printf("Switch Case 1\n");
                            break;
                        case 2:
                            printf("Switch Case 2\n");
                            break;
                        default:
                            printf("Switch Default Case\n");
                            break;
                    }
                }
            }
        }

        // Additional computations
        int temp = result;
        while (temp > 1) {
            if (temp % 5 == 0) {
                temp /= 5;
                printf("While Loop: Divided temp by 5\n");
            } else if (temp % 3 == 0) {
                temp /= 3;
                printf("While Loop: Divided temp by 3\n");
            } else {
                temp -= 2;
                printf("While Loop: Subtracted 2 from temp\n");
            }
        }

    } else {
        printf("Result is not divisible by 9345349. Processing completed.\n");
    }

    return 0;
}

void FuzzerCleanup(void)
{
    FuzzerClearRand();
}
