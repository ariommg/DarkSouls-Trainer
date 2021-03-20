#include "Includes.h"

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)); //Tar en snapshot av alla processer
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) //Tar första processen från snapshotten och lägger in i procEntry och kollar så den inte är NULL
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName)) //Om procEntryns executable namn är samma som namnet som är argumentet i funktionen
				{
					procId = procEntry.th32ProcessID; //Lägger in procEntryns processid
					break; //Breakar då den hittat ID
				}
			} while (Process32Next(hSnap, &procEntry)); //Fortsätter o tar nästa och nästa process och lägger in i procEntry
		}
	}
	CloseHandle(hSnap); //Stänger ner snapshotten då den inte behövs längre
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId); //Tar snapshot av modulerna i processen
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)); //Lägger första modulen från snapshotten in i modEntry och kollar så den inte är NULL
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName)) //Om modulens namn är samma som modName
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr; //Definerar modBaseAddr som en pointer från modEntryn som matchade
					break;
				}
			} while (Module32Next(hSnap, &modEntry)); //Kör igenom alla moduler i processen
		}
	}
	return modBaseAddr;
}

//Funktionen har en for loop som tar alla offsets man skriver in och plussar ihop dom, detta var mycket bättre än vad jag gjorde först som var att jag tog och plussa på
//offsetsarna flera gånger för det var oeffektivt och tog upp mycket plats. Dehär ser också coolare ut än att göra det vanligt.
uintptr_t CalculatePointer(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}