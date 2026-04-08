#include <Windows.h>
#include <cstdint>
#include <thread>
#include <chrono>
#include <iostream>

namespace Offsets
{
    constexpr std::ptrdiff_t jump = 0x2066C70;
}

void bhop(HMODULE instance) {
    uintptr_t client = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client.dll"));

    if (!client)
        return;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (GetAsyncKeyState(VK_SPACE)) {
            *reinterpret_cast<std::uintptr_t*>(client + Offsets::jump) = 65537;
            Sleep(1);
            *reinterpret_cast<std::uintptr_t*>(client + Offsets::jump) = 256;
        }
    }

    FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(HMODULE instance, std::uintptr_t reason, void* reserved) {

    if (reason == 1) {
        DisableThreadLibraryCalls(instance);

        auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(bhop), instance, 0, nullptr);

        if (thread)
            CloseHandle(thread);

    }

    return TRUE;
}