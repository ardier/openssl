#include <cstdlib>  // For std::atoi
#include <iostream> // For std::cout
#include <string>   // For std::string
#include <thread>   // For std::this_thread::sleep_for
#include <chrono>   // For std::chrono::seconds

int FuzzerTestOneInput(const uint8_t *buf, size_t len)
{
    try {
        // Convert the input buffer into a string
        std::string input(reinterpret_cast<const char *>(buf), len);

        // Ensure the input is not empty
        if (input.empty()) {
            return 0;
        }

        // Parse input as a comma-separated pair of integers
        std::size_t comma_pos = input.find(',');
        if (comma_pos == std::string::npos) {
            return 0;
        }

        // Extract y and x as substrings
        std::string y_str = input.substr(0, comma_pos);
        std::string x_str = input.substr(comma_pos + 1);

        // Convert y and x to integers using std::atoi
        int y = std::atoi(y_str.c_str());
        int x = std::atoi(x_str.c_str());

        // Simulate some processing logic
        int result = 0;
        for (int i = 0; i < x; ++i) {
            result += y;

            // Wait for 10 seconds
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

        // Cause a segmentation fault if the result is divisible by 9345349
        if (result % 9345349 == 0) {
            std::cout << "result is divisible by 9345349. Here comes the segfault." << std::endl;

            int *crash = nullptr;
            *crash = 42; // This will cause a segmentation fault
        } else {
            std::cout << "result is not divisible by 9345349" << std::endl;
        }
    } catch (...) {
        // Catch any exceptions and ignore
    }
    return 0;
}
