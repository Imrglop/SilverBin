#pragma once
#include <vector>

typedef unsigned char byte;

namespace parseUtils {
    template <typename T>
    T getItemFromBytes(byte* bytes) { return *((T*)bytes); };

    template <typename T>
    std::vector<byte> getBytes(T val) {
        std::vector<byte> retVal;
        byte* pbyte = (byte*)&val;
        for (size_t i = 0; i < sizeof(val); i++) {
            retVal.push_back(*(pbyte + i));
        }
        return retVal;
    }
}