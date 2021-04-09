#ifndef IGAMEUI_H
#define IGAMEUI_H

#ifdef _WIN32
#pragma once
#endif

class IGameUI : public IBaseInterface
{
public:
	virtual void Initialize(CreateInterfaceFn *factories, int count) = 0;
	virtual void Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void *system) = 0;
	virtual void Shutdown(void) = 0;
	virtual int ActivateGameUI(void) = 0;
	virtual int ActivateDemoUI(void) = 0;
	virtual int HasExclusiveInput(void) = 0;
	virtual void RunFrame(void) = 0;
	virtual void ConnectToServer(const char *game, int IP, int port) = 0;
	virtual void DisconnectFromServer(void) = 0;
	virtual void HideGameUI(void) = 0;
	virtual bool IsGameUIActive(void) = 0;
	virtual void LoadingStarted(const char *resourceType, const char *resourceName) = 0;
	virtual void LoadingFinished(const char *resourceType, const char *resourceName) = 0;
	virtual void StartProgressBar(const char *progressType, int progressSteps) = 0;
	virtual int ContinueProgressBar(int progressPoint, float progressFraction) = 0;
	virtual void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason = NULL) = 0;
	virtual int SetProgressBarStatusText(const char *statusText) = 0;
	virtual void SetSecondaryProgressBar(float progress) = 0;
	virtual void SetSecondaryProgressBarText(const char *statusText) = 0;
	virtual void TestFunc1(const char *statusText) = 0; // TW
	virtual void TestFunc2(const char *statusText) = 0;
	virtual void TestFunc3(const char *statusText) = 0;
	virtual void TestFunc4(const char *statusText) = 0;// TW
	virtual void CreateRoom() = 0;
	virtual void HideAllPanels(int panel) = 0;
	virtual void ShowAllPanels(int panel) = 0;
	virtual void TestFunc8() = 0;
	virtual void TestFunc9() = 0;
	virtual void ShowLeaveGameRoomMsg() = 0;
	virtual void ShowInGameMenu() = 0;
	virtual void ShowAddFriendMessageBox(const wchar_t *nickname) = 0; //31
	virtual void ShowCreateNicknameDlg() = 0;
	virtual void ShowDisconnectMessageBox(const wchar_t *test, int lol) = 0;
	virtual void ShowCafeRegDlg(const wchar_t *test, const wchar_t *reee) = 0;
	virtual void ShowCafeRegDlgNext() = 0; 
	virtual void TestFunc17() = 0;
	virtual void ShowInviteDlg(const wchar_t *test) = 0;
	virtual void ShowRoomPasswordEntry() = 0;
	virtual void ShowRoomPasswordEntry2() = 0;
	virtual void ShowNetworkSettings() = 0;
	virtual void ShowUserInviteDlg() = 0;
	virtual void ShowMessageBox(const char *title, const char *text) = 0;
	virtual void ShowMessageBox(const wchar_t *title, const wchar_t *text) = 0;
	virtual void TestFunc23333() = 0;
	virtual void SendMessageToUser(const wchar_t *nickname, const wchar_t *text) = 0;
	virtual void TestFunc221() = 0;
	virtual void TestFunc24() = 0;
	virtual void TestFunc25() = 0;
	virtual void TestFunc26() = 0;
	virtual void TestFunc27() = 0;
	virtual void TestFunc28() = 0; 
	virtual void TestFunc29() = 0;
	virtual void TestFunc30() = 0;
	virtual void TestFunc31() = 0;
	virtual void TestFunc32() = 0;
	virtual void TestFunc33() = 0;
	virtual void TestFunc34() = 0;
	virtual void TestFunc35() = 0;
	virtual void TestFunc36() = 0;
	virtual void ShowTutorialDlg() = 0;

};

extern IGameUI* g_pGameUI;

typedef struct IGameUI_BL4CKEND_S
{
	void (*Initialize)(CreateInterfaceFn* factories, int count);
	void (*Start)(struct cl_enginefuncs_s* engineFuncs, int interfaceVersion, void* system);
	void (*Shutdown)(void);
	int (*ActivateGameUI)(void);
	int (*ActivateDemoUI)(void);
	int (*HasExclusiveInput)(void);
	void (*RunFrame)(void);
	void (*ConnectToServer)(const char* game, int IP, int port);
	void (*DisconnectFromServer)(void);
	void (*HideGameUI)(void);
	bool (*IsGameUIActive)(void);
	void (*LoadingStarted)(const char* resourceType, const char* resourceName);
	void (*LoadingFinished)(const char* resourceType, const char* resourceName);
	void (*StartProgressBar)(const char* progressType, int progressSteps);
	int (*ContinueProgressBar)(int progressPoint, float progressFraction);
	void (*StopProgressBar)(bool bError, const char* failureReason, const char* extendedReason);
	int (*SetProgressBarStatusText)(const char* statusText);
	void (*SetSecondaryProgressBar)(float progress);
	void (*SetSecondaryProgressBarText)(const char* statusText);
	void (*TestFunc1)(const char* statusText); // csnz?
	void (*TestFunc2)(const char* statusText);
	void (*TestFunc3)(const char* statusText);
	void (*TestFunc4)(const char* statusText);
	void (*TestFunc5)(const char* statusText);
	void (*CreateRoom)();
	void (*HideAllPanels)(int panel);
	void (*ShowAllPanels)(int panel);
	void (*TestFunc8)();
	void (*TestFunc9)();
	void (*ShowLeaveGameRoomMsg)();
	void (*ShowInGameMenu)();
	void (*ShowAddFriendMessageBox)(const wchar_t* nickname);
	void (*ShowCreateNicknameDlg)();
	void (*ShowDisconnectMessageBox)(const wchar_t* test, int lol);
	void (*ShowCafeRegDlg)(const wchar_t* test, const wchar_t* reee);
	void (*ShowCafeRegDlgNext)();
	void (*TestFunc17)();
	void (*ShowInviteDlg)(const wchar_t* test);
	void (*ShowRoomPasswordEntry)();
	void (*ShowRoomPasswordEntry2)();
	void (*ShowNetworkSettings)();
	void (*ShowUserInviteDlg)();
	void (*ShowMessageBox_An)(const char* title, const char* text);
	void (*ShowMessageBox_Un)(const wchar_t* title, const wchar_t* text);
	void (*TestFunc22)();
	void (*SendMessageToUser)(const wchar_t* nickname, const wchar_t* text);
	void TestFunc221();
	void TestFunc24();
	void TestFunc25();
	void TestFunc26();
	void TestFunc27();
	void TestFunc28();
	void TestFunc29();
	void TestFunc30();
	void TestFunc31();
	void TestFunc32();
	void TestFunc33();
	void TestFunc34();
	void TestFunc35();
	void TestFunc36();
	void ShowTutorialDlg();

}IGameUI_BL4CKEND_T;


#define GAMEUI_INTERFACE_VERSION "GameUI007"
#endif