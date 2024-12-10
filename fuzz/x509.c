#include <stdlib.h>  // For atoi
#include <stdio.h>   // For printf
#include <string.h>  // For string manipulation

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
    int result = 0;
    for (int i = 0; i < x; ++i) {
        result += y;

        // Simulate a delay for each iteration
        sleep(10);
    }

    // Check for a specific condition to trigger a segmentation fault
    if (result % 9345349 == 0) {
        printf("result is divisible by 9345349. Here comes the segfault.\n");

        int *crash = NULL;
        *crash = 42;  // Trigger segmentation fault
    } else {
        printf("result is not divisible by 9345349.\n");
    }

    return 0;
}
