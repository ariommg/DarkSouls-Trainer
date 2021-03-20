#include "Includes.h"

//Tidigare använde jag bara WriteProcessMemory som fungerade ganska bra men detta ska tydligen vara mindre detected då den patchar ihop det den ändrat på
void memory::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

//Denna funktionen används till om man vill ta bort någonting helt o hållet som tex funktionen som gör att man tar fall damage
void memory::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);
	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}