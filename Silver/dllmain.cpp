#include "dllmain.h"
#include "Parsing/utils.hpp"
#include "Core/SilverBinaryFile.h"

#define __silver_test true
#if __silver_test
#include <iostream>
#endif

#if __silver_test
#define log std::cout
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
#if __silver_test == true
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        try {
            // test.bin contents:
            // 72341ueiw,,,,,,,,The quick brown fox jumps over the lazy dog.[NUL CHARACTER]
            
            FILE* out;
            SilverBinaryFile bin = SilverBinaryFile("data.bin");
            bool result = true;
            int myTest = bin.next<int>(&result);
            freopen_s(&out, "CONOUT$", "w", stdout);
            log << "Get Int Result: " << result << '\n';
            log << "Next Int: " << myTest << '\n';
            byte byteValue = bin.next<byte>();
            log << "Next byte: " << (int)byteValue << '\n';
            float floatVal = bin.next<float>();
            log << "Next float: " << floatVal << '\n';
            __int64 ll = bin.next<__int64>();
            log << "Next ll: " << ll << '\n';

            bin.clearPosition();

            int wrapAround = bin.next<int>();
            log << "Back: " << wrapAround << '\n';
            bin.jump(17);
            string str = bin.nextString();
            log << "String: '" << str << "'\n";

            bin.clearPosition();

            bin.insert(wrapAround + 1);
        }
        catch (std::exception e) {
            log << "Error: " << e.what() << '\n';
        }
    }
#endif
    return TRUE;
}

