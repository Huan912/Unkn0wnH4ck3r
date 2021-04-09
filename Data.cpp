#include <Windows.h>
#include <Psapi.h>
#include "Structs.h"
#include "Data.h"
#include "XorStr/obfuscation.h"
#include "ImGlobe.h"
bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)   return 0;
	return (*szMask) == NULL;
}

DWORD Pattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask)
{
	
	for (DWORD i = 0; i < dwLen; i++)
		if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
	
	return 0;
}

DWORD GetModuleSize(const DWORD Address)
{
	return PIMAGE_NT_HEADERS(Address + (DWORD)PIMAGE_DOS_HEADER(Address)->e_lfanew)->OptionalHeader.SizeOfImage;
}

BOOL __comparemem(const UCHAR* buff1, const UCHAR* buff2, UINT size)
{
	
	for (UINT i = 0; i < size; i++, buff1++, buff2++)
	{
		if ((*buff1 != *buff2) && (*buff2 != 0xFF))
			return FALSE;
	}
	
	return TRUE;
}

DWORD FindCodeAddress(DWORD dwStart, DWORD dwEnd, LPBYTE bCode, UINT CodeSize, INT OpcodeNum, BOOL bPattern)
{
	
	DWORD i;
	for (i = dwStart; (i + CodeSize) < dwEnd; i++) {
		if (CompareMemory((LPBYTE)i, bCode, CodeSize, bPattern))
			return (DWORD)(i + OpcodeNum);
	}
	
	return 0;
}

ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size)
{
	for (ULONG ul = start; (ul + size) < end; ul++)
	{
		if (CompareMemory(ul, clone, size))
			return ul;
	}
	return NULL;
}

ULONG __findreference(const ULONG start, const ULONG end, const ULONG address)
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68; //PUSH dword
	*(ULONG*)&Pattern[1] = address;
	return FindMemoryClone(start, end, Pattern, sizeof(Pattern) - 1);
}

PVOID CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	
	PVOID dwPointer = nullptr;

	dwPointer = (PVOID)(Interface(InterfaceName, 0));
	
	return dwPointer;
}

CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	
	CreateInterfaceFn Interface = 0;

	HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

	if (hFactoryModule)
		Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, XorString("CreateInterface")));
	
	return Interface;
}

void String_ANSI_to_UTF8(char* pANSI, LPSTR pUTF8, int nMaxSize)
{
	// len of ANSI string
	int nLen = MultiByteToWideChar(CP_ACP, 0, pANSI, -1, NULL, NULL);

	// WCHAR format of ANSI
	LPWSTR lpwsz = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, pANSI, -1, lpwsz, nLen);

	// convert to UTF-8
	int outLen = WideCharToMultiByte(CP_UTF8, 0, lpwsz, nLen, pUTF8, nMaxSize, NULL, NULL);
	pUTF8[outLen - 1] = 0;

	delete[] lpwsz;
}

void LoadTextureMemory(const unsigned char* image, int index, int size)
{
	int width, height;

	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &g_Vars.texture_id[index]);
	glBindTexture(GL_TEXTURE_2D, g_Vars.texture_id[index]);
	unsigned char* soilimage = SOIL_load_image_from_memory(image, size, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soilimage);
	SOIL_free_image_data(soilimage);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}