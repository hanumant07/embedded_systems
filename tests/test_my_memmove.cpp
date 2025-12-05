#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <numeric>
#include "my_memmove.h"

void test_my_memmove() {
    std::cout << "Testing my_memmove..." << std::endl;

    // 1. Basic non-overlapping copy
    {
        char src[] = "Hello, World!";
        char dest[20] = {0};
        my_memmove(dest, src, 13);
        assert(strcmp(dest, "Hello, World!") == 0);
        std::cout << "  [PASS] Non-overlapping copy" << std::endl;
    }

    // 2. Overlapping: dest > src (requires backward copy)
    {
        char buffer[] = "1234567890";
        // Move "12345" to start at index 2 -> "1212345890"
        // src: buffer (starts at '1')
        // dest: buffer + 2 (starts at '3')
        // n: 5
        // Expected result: "1212345890"
        my_memmove(buffer + 2, buffer, 5);
        assert(strncmp(buffer, "1212345890", 10) == 0);
        std::cout << "  [PASS] Overlapping (dest > src)" << std::endl;
    }

    // 3. Overlapping: src > dest (requires forward copy)
    {
        char buffer[] = "1234567890";
        // Move "34567" to start at index 0 -> "3456767890"
        // src: buffer + 2 (starts at '3')
        // dest: buffer (starts at '1')
        // n: 5
        // Expected result: "3456767890"
        my_memmove(buffer, buffer + 2, 5);
        assert(strncmp(buffer, "3456767890", 10) == 0);
        std::cout << "  [PASS] Overlapping (src > dest)" << std::endl;
    }

    // 4. Large buffer with alignment check
    {
        const size_t SIZE = 1024;
        std::vector<unsigned char> src(SIZE);
        std::vector<unsigned char> dest(SIZE);
        
        // Fill src with pattern
        for (size_t i = 0; i < SIZE; ++i) src[i] = (unsigned char)(i & 0xFF);
        
        // Unaligned start
        my_memmove(dest.data() + 1, src.data() + 1, SIZE - 2);
        
        for (size_t i = 0; i < SIZE - 2; ++i) {
            assert(dest[i + 1] == src[i + 1]);
        }
        std::cout << "  [PASS] Large buffer unaligned copy" << std::endl;
    }

    std::cout << "my_memmove tests passed!" << std::endl;
}
