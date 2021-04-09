#pragma warning(disable:4005)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4715)
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#pragma warning(disable:4700)
#pragma warning(disable:4305)
#pragma warning(disable:4410)
#pragma warning(disable:4409)
#pragma warning(disable:4002)
#pragma warning(disable:4273)
#include <Windows.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <sstream>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib,"Wininet.lib")
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define M_PI 3.14159265358979323846
#define vec3_t Vector
#include "Vector.h"
#include "QAngle.h"
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cdll_int.h"
#include "engine/cl_entity.h"
#include "engine/eiface.h"
#include "engine/usercmd.h"
#include "engine/pm_defs.h"
#include "common/com_model.h"
#include "engine/r_studioint.h"
#include "common/ref_params.h"
#include "common/screenfade.h"
#include "engine/pm_movevars.h"
#include "common/event_api.h"
#include "common/r_efx.h"
#include "common/interface.h"
#include "engine/pm_movevars.h"
#include "engine/studio.h"
#include <array>
#include "ValveSDK\VGUI\VGUI_Frame.h"
#include "ValveSDK\VGUI\VGUI_Panel.h"
#include "ValveSDK\GameUI\IVGuiModuleLoader.h"
#include "ValveSDK\GameUI\IRunGameEngine.h"
#include "ValveSDK\GameUI\IGameUI.h"
#include "ValveSDK\GameUI\IGameConsole.h"
#include "ValveSDK\VGUI2\IHTML.h"
#include "ValveSDK\VGUI2\IPanel.h"
#include "ValveSDK\VGUI2\ISurface.h"
#include "ValveSDK\VGUI2\IEngineVGui.h"
#include <gl\GL.h>
#include "ThemidaSDK.h"
#pragma comment(lib, "OpenGL32.lib")
using namespace std;
extern bool keys[256];
extern bool keys_Engine[256];
typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t* pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	int (*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t* pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s* ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s* ppmove);
	char(*HUD_PlayerMoveTexture) (char* name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s* cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float* ofs);
	struct kbutton_s* (*KB_Find) (const char* name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s* pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s* ent, const char* modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s* event, const struct cl_entity_s* entity);
	void(*HUD_PostRunCmd) (struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s* state, const struct clientdata_s* client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s* dst, const struct entity_state_s* src);
	void(*HUD_TxferPredictionData) (struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char* buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float* mins, float* maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char* pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
	struct cl_entity_s* (*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	void(*HUD_Chatinputposition_Funtion) (int* x, int* y);
	int(*HUD_GetPlayerTeam) (int iplayer);
	void(*ClientFacTory) (void);
	void(*HUD_VidSetMode) (void);
	void(*HUD_NoticePrint) (const char* text, int color);
	void(*CL_ClearKeyState) (void);
	void(*ProcessCLByEngine) (int a1, unsigned int a2, int a3, int a4, int a5);
	void(*HUD_CreateBeams) (void);
} cl_clientfunc_t;

struct gvar_s
{
	GLuint texture_id[1024];
	DWORD HwBase, HwSize, HwEnd, ClBase, ClSize, ClEnd, MpBase, MpSize, MpEnd,WhiteWallPtr, PlayerNamesAdr,ScreenStopAdr;
	float color_red = 1, color_green = 0, color_blue = 0, RandomSpeed = -6.45, ESPTeamselection, Rage_Target_Selection, SetLightmapDarkness = 1.f, ModelSelection,
		NoRecoilValue = 1.80, SpeedLevel, AimTeamselection, AimBodyselection;
	float displayCenterY, displayCenterX,MyFps;
	float CrosshairColor[4], Player_TeammateColor[3], Player_EnemyColor[3], Player_TeammateGlow[3], Player_EnemyGlow[3], Player_TeammateBarrel[3], Player_EnemyBarrel[3];
	float CameraOffset_right = 0, CameraOffset_up = 16.0, CameraOffset_forward = 100.0, visual_radar_size = 65;
	int GUIPage = 1, AimTarget, RageAimFov = 60, AutoReConnectValue = 400, RageAimDist = 8192,AimOffset;
	bool LocalCheck, ThirdPersonTrigger;
	bool ShowMenu = false, Revive, RetRoom, ReConnect, FastConnect, DelFog, InitDone, IsChat = true, SpeedOn, ScreenStop, CommandScreenSet
		, AutoBuyAmmo, AutoBuyAmmo2, AutoMouse1, AutoMouse2, HighFps, TimeoutRunning, WallPlayer, MyDirtyWord, ThirdPerson, AutoReConnect;
	bool ESPBox, ESPName, ESPDist, ESPWeapons, ESPTrace, DrawHitBox, DrawCrosshair,NOFlash, NoSmoke, OverrideLightmap, PlayerGlow,PlayerColor,KreedzInfo, DrawTraceGrenade, Barrel, Radar, RainBowBox;
	bool bypass_trace_blockers;
	bool RageAimbot, LegitAim = true, NoRecoilAngle, SilentAim, AutoBullet, AutoFire, AutoWall, ShowTargetID, AntiAim, TwoKey;
	bool FastRun, AdjustFast, AdjustSpeed, GroundStrafe,StrafeHelper,BunnyHop,TestISu;
	float aa_pitch, aa_edge, aa_yaw, aa_yaw_static, aa_yaw_while_running, aa_roll, aa_roll_static, aa_roll_while_running;
}; extern gvar_s g_Vars;
extern cl_enginefunc_t* g_pEngine;
extern cl_clientfunc_t* g_pClient;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern IGameConsole* pConsole;
extern engine_studio_api_t* IEngineStudio;
extern engine_studio_api_t IEngineStudioAPI;
extern r_studio_interface_t* g_prStudio;
extern r_studio_interface_t g_rStudio;
extern SCREENINFO screeninfo;
struct PlayerHitBox
{
	Vector pHitBox[11];
	int HitBoxHeadID;
	char* szWeaponName;

	QVector vHitbox[21];
	QVector vHitboxMin[21];
	QVector vHitboxMax[21];
	QVector vHitboxPoints[21][8];
	int iMaxHitboxes;
	bool bHitboxVisible[21];
	bool bHitboxPointsVisible[21][8];
	bool bHitboxBehindTheWall[21];
	bool bHitboxPointsBehindTheWall[21][8];

	float fHitboxFOV[21];
	float vOriginFOV;
}; extern PlayerHitBox g_HitBox[33];

struct UserKeylist
{
	int ReviveKey = 0x55, ReconnectKey = VK_F7, RetroomKey = VK_F8, RageAimKey = 69, SpeedKey = VK_SHIFT, ThirdPersonKey = VK_F1, GroundStrafeKey = VK_MENU;
}; extern UserKeylist g_Key;
