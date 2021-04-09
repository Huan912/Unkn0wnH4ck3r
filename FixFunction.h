#include <Windows.h>
class BKK_GameUI007
{
public:
	void MessageBoxUnf8(const char* title, const char* text, byte zByte) {
		typedef int(__thiscall* fo)(void*, const char* title, const char* text, byte zByte);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[43])(this, title, text, zByte);
	};

	void ShowAddFriendMessageBox(const wchar_t* name) {
		typedef int(__thiscall* fo)(void*, const wchar_t* name);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[31])(this, name);
	};
	void ShowCreateNicknameDlg() {
		typedef int(__thiscall* fo)(void*);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[32])(this);
	};
	void CreateRoom() {
		typedef int(__thiscall* fo)(void*);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[24])(this);
	};
private:
}; extern BKK_GameUI007* g_GameUi;

class BKK_ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a) {
		typedef int(__thiscall* fo)(void*, int r, int g, int b, int a);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[7])(this, r, g, b,a);
	};
	void DrawFilledRect(int x0, int y0, int x1, int y1) {
		typedef int(__thiscall* fo)(void*, int x0, int y0, int x1, int y1);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[9])(this, x0, y0, x1, y1);
	};
	void PlaySound(const char* fileName) {
		typedef int(__thiscall* fo)(void*, const char* fileName);
		reinterpret_cast<fo>(((int*)(*(int*)(this)))[75])(this, fileName);
	};
private:
}; extern BKK_ISurface* g_ISurface;