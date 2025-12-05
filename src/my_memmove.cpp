#include "my_memmove.h"
#include <cstdint> // for uintptr_t

void* my_memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);

    if (d == s || n == 0) {
        return dest;
    }

    // Check for overlap where we need to copy backwards
    // Case: dest is after src, but starts before src ends
    if (d > s && d < s + n) {
        // Copy backwards (from end to start)
        d += n;
        s += n;
        
        // Optimization: Check for word alignment
        // We can only optimize if both src and dest have the same alignment offset
        // relative to a word boundary.
        if (n >= sizeof(uintptr_t) && 
            (reinterpret_cast<uintptr_t>(d) % sizeof(uintptr_t) == 
             reinterpret_cast<uintptr_t>(s) % sizeof(uintptr_t))) {
            
            // Copy trailing bytes to align to word boundary
            while (reinterpret_cast<uintptr_t>(d) % sizeof(uintptr_t) != 0) {
                *(--d) = *(--s);
                n--;
            }

            // Copy words
            uintptr_t* dw = reinterpret_cast<uintptr_t*>(d);
            const uintptr_t* sw = reinterpret_cast<const uintptr_t*>(s);
            size_t words = n / sizeof(uintptr_t);
            
            for (size_t i = 0; i < words; ++i) {
                *(--dw) = *(--sw);
            }
            
            // Update pointers and count
            d = reinterpret_cast<unsigned char*>(dw);
            s = reinterpret_cast<const unsigned char*>(sw);
            n %= sizeof(uintptr_t);
        }

        // Copy remaining bytes
        while (n--) {
            *(--d) = *(--s);
        }
    } else {
        // Copy forwards (from start to end)
        // Safe if dest is before src, or if regions don't overlap
        
        // Optimization: Check for word alignment
        if (n >= sizeof(uintptr_t) && 
            (reinterpret_cast<uintptr_t>(d) % sizeof(uintptr_t) == 
             reinterpret_cast<uintptr_t>(s) % sizeof(uintptr_t))) {
            
            // Copy leading bytes to align to word boundary
            while (reinterpret_cast<uintptr_t>(d) % sizeof(uintptr_t) != 0) {
                *d++ = *s++;
                n--;
            }

            // Copy words
            uintptr_t* dw = reinterpret_cast<uintptr_t*>(d);
            const uintptr_t* sw = reinterpret_cast<const uintptr_t*>(s);
            size_t words = n / sizeof(uintptr_t);
            
            for (size_t i = 0; i < words; ++i) {
                *dw++ = *sw++;
            }
            
            // Update pointers and count
            d = reinterpret_cast<unsigned char*>(dw);
            s = reinterpret_cast<const unsigned char*>(sw);
            n %= sizeof(uintptr_t);
        }

        // Copy remaining bytes
        while (n--) {
            *d++ = *s++;
        }
    }

    return dest;
}
