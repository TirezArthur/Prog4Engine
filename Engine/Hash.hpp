#pragma once
#include <string_view>

namespace dae {
    //Code provided in the slides, slightly modified
    //Hashes compile time if the argument is a literal or at runtime if the argument is a string or other object
    consteval uint32_t sdbm_hash(const std::string_view text) {
        uint32_t hash = 0;
        for (int c : text) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return hash;
    }
}
