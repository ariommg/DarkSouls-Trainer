#pragma once
#include "Includes.h"

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t CalculatePointer(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);