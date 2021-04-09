#include <Windows.h>
DWORD Pattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask);
BOOL __comparemem(const UCHAR* buff1, const UCHAR* buff2, UINT size);
bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask);
DWORD GetModuleSize(const DWORD Address);
CreateInterfaceFn CaptureFactory(char* FactoryModule);
PVOID CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName);
ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
ULONG __findreference(const ULONG start, const ULONG end, const ULONG address);
void String_ANSI_to_UTF8(char* pANSI, LPSTR pUTF8, int nMaxSize);
void LoadTextureMemory(const unsigned char* image, int index, int size);
#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) __findreference((const ULONG)Start, (const ULONG)End, (const ULONG)Address)
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}