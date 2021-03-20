#include "Includes.h"

int main()
{
	HANDLE hProcess = 0;
	uintptr_t moduleBase = 0, playerBase = 0, healthAddr = 0, staminaAddr = 0, maxHealthAddr = 0, speedAddr = 0;
	DWORD procId = GetProcId(L"DarkSoulsIII.exe"); //Får proccessens id
	bool bHealth = false, bStamina = false, bSpeed = false;
	int healthValue;
	const int staminaValueOn = 17, staminaValueOff = 1, speedValueOn = 1073741824, speedValueOff = 1065353216;

	if (procId) //Kollar så proccessen inte är null, alltså att spelet är öppet
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId); //Öppnar proccessen med all access alltså att jag kan reada och writea och göra vad jag vill
		moduleBase = GetModuleBaseAddress(procId, L"DarkSoulsIII.exe"); //Module base addressen behövs för alla offsets och liknande måste börja någonstans
		playerBase = moduleBase + 0x4768E78;

		//Pointers/Addresser
		healthAddr = CalculatePointer(hProcess, playerBase, { 0x80, 0x1F90, 0x18, 0xD8 });
		staminaAddr = CalculatePointer(hProcess, playerBase, { 0x80, 0x1F90, 0x18, 0x1C0 });
		maxHealthAddr = CalculatePointer(hProcess, playerBase, { 0x80, 0x1F90, 0x18, 0xDC });
		speedAddr = CalculatePointer(hProcess, playerBase, { 0x80, 0x1F90, 0x28, 0xA58 });

		std::cout << "Welcome to trainer :) \n\n F1: Godmode F2: Unlimited stamina F3: Speedhack"<< std::endl;
	}
	else 
	{
		std::cout << "Spelet hittades inte. Enter för gå ut.";
		getchar();
		return 0;
	}
	DWORD dwExit = 0;
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			ReadProcessMemory(hProcess, (LPVOID)maxHealthAddr, &healthValue, sizeof(healthValue), NULL); //Läser vad maxhp spelaren har så att när jag writar hp för godmode så är det samma som maxhp
			bHealth = !bHealth;
			std::cout << "Godmode toggled. (" << bHealth << ")" << std::endl;
		}
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			bStamina = !bStamina;
			std::cout << "Unlimited stamina toggled. (" << bStamina << ")" << std::endl;
			if (bStamina)
			{
				memory::PatchEx((BYTE*)staminaAddr, (BYTE*)&staminaValueOn, sizeof(staminaValueOn), hProcess); //Vet inte varför men för att få unlimited stamina är den magiska siffran 17 som man måste writa in i stamina addressen
			}
			else
			{
				memory::PatchEx((BYTE*)staminaAddr, (BYTE*)&staminaValueOff, sizeof(staminaValueOff), hProcess);
			}
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			bSpeed = !bSpeed;
			std::cout << "Speed toggled. (" << bSpeed << ")" << std::endl;

			if (bSpeed)
			{
				//Anledningen till att speedValuen är så som den är eftersom att det är floats och jag writar in i 4bytes, kan nog writa i float men orkar inte lösa det nu när det är enkelt att bara
				//skriva i 4bytes
				memory::PatchEx((BYTE*)speedAddr, (BYTE*)&speedValueOn, sizeof(speedValueOn), hProcess);
			}
			else
			{
				memory::PatchEx((BYTE*)speedAddr, (BYTE*)&speedValueOff, sizeof(speedValueOff), hProcess);
			}
		}
		if (bHealth)
		{
			memory::PatchEx((BYTE*)healthAddr, (BYTE*)&healthValue, sizeof(healthValue), hProcess); //Writar in maxhpt i hpt
		}
		Sleep(10); //Väldigt viktigt med sleepen annars kraschar allt
	}
}