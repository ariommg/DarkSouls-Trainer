#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "process.h"
#include "memory.h"

//Anledningen till att jag har en include fil som includar allt som filerna behöver är att om jag måste tex includa Windows.h flera gånger så kommer den göra det flera gånger och då kommer
//det ta längre tid att starta fusket när man ska debugga.
