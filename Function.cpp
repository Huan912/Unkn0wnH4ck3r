#include <Windows.h>
#include "Structs.h"
#include "StringList.h"
#include "Speed.h"
#include "DirtyWords.h"
#define COVID_19 void
COVID_19 PlayerRevive(); COVID_19 PlayerReconnect(); COVID_19 PlayerRetRoom();
void ShowGUIMenu(); void DirtyWordsThread(); void ThirdPersonTrigger();
bool FastConnectV1, SpeedOnV1, SpeedOnV2, CommandScreenSetV1, MemWallHackV1, LegitAimV1, SilentAimV1, AutoBulletV1, MyDirtyWordV1,DelFogV1, ThirdPersonV1;
void Loop_Keys()
{
	if (keys[VK_INSERT])
		ShowGUIMenu();
	if (g_Vars.InitDone && g_Vars.LocalCheck)
	{
		if (keys[(int)g_Key.ReviveKey] && !g_Vars.IsChat && g_Vars.Revive)
			PlayerRevive();
		if (keys[(int)g_Key.ReconnectKey] && g_Vars.ReConnect && !g_Vars.IsChat)
			PlayerReconnect();
		if (keys[(int)g_Key.RetroomKey] && g_Vars.RetRoom && !g_Vars.IsChat)
			PlayerRetRoom();
		if (keys[(int)g_Key.ThirdPersonKey] && g_Vars.ThirdPerson && !g_Vars.IsChat)
			ThirdPersonTrigger();
	}
}

void CheckBox_FunctionCallBack()
{
	HANDLE hThread2 = 0, hThread3 = 0, hThread4 = 0;
	//�ֳs�ƥ�B�z
	if (g_Vars.FastConnect)
	{
		if (!FastConnectV1)
		{
			FastConnectV1 = true;
		}
	}
	else
	{
		if (FastConnectV1)
		{
			FastConnectV1 = false;
		}
	}
	//�ֳs�ƥ�B�z

	//�[�t�ƥ�B�z
	if (g_Vars.SpeedOn)
	{
		if (!SpeedOnV2)
		{
			g_Engine.pfnClientCmd(XorString(Command_Attack));
			g_Engine.pfnClientCmd(XorString(Command_Attack2));
			SpeedOnV2 = true;
		}
		if (!SpeedOnV1)
		{
			//SettingTimeout(2);
			//hThread2 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SpeedProc, 0, 0, 0);
			//hThread3 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SpeedThread, 0, 0, 0);
			SpeedOnV1 = true;
		}
	}
	else
	{
		if (SpeedOnV1)
		{
			CloseHandle(hThread2);
			CloseHandle(hThread3);
			//SettingTimeout(1);
			SpeedOnV1 = false;
		}
	}

	if (g_Vars.CommandScreenSet)
	{
		if (!CommandScreenSetV1)
		{
			CommandScreenSetV1 = true;
		}
	}
	else
	{
		if (CommandScreenSetV1)
		{
			CommandScreenSetV1 = false;
		}
	}

	if (g_Vars.DelFog)
	{
		if (!DelFogV1)
		{
			g_Engine.pfnClientCmd(XorString("gl_fog 0"));
			DelFogV1 = true;
		}
	}
	else
	{
		if (DelFogV1)
		{
			g_Engine.pfnClientCmd(XorString("gl_fog 1"));
			DelFogV1 = false;
		}
	}

	if (g_Vars.MyDirtyWord)
	{
		if (!MyDirtyWordV1)
		{
			hThread4 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DirtyWordsThread, 0, 0, 0);
			MyDirtyWordV1 = true;
		}
	}
	else
	{
		if (MyDirtyWordV1)
		{
			CloseHandle(hThread4);
			MyDirtyWordV1 = false;
		}
	}

	if (g_Vars.LegitAim) { if (!LegitAimV1) { LegitAimV1 = true, g_Vars.SilentAim = false, g_Vars.AutoBullet = false; } }else { if (LegitAimV1)LegitAimV1 = false; }
	if (g_Vars.SilentAim) { if (!SilentAimV1) { SilentAimV1 = true, g_Vars.LegitAim = false, g_Vars.AutoBullet = false; } }else { if (SilentAimV1)SilentAimV1 = false; }
	if (g_Vars.AutoBullet) { if (!AutoBulletV1) { AutoBulletV1 = true, g_Vars.LegitAim = false, g_Vars.SilentAim = false; } }else { if (AutoBulletV1)AutoBulletV1 = false; }
	if (g_Vars.AimBodyselection == 0)
		g_Vars.AimOffset = 0;
	else if (g_Vars.AimBodyselection == 1)
		g_Vars.AimOffset = (-5);
	else if (g_Vars.AimBodyselection == 2)
		g_Vars.AimOffset = (-8);
	else if (g_Vars.AimBodyselection == 3)
		g_Vars.AimOffset = (-12);

}


COVID_19 ShowGUIMenu()
{
	if (g_Vars.ShowMenu)
	{
		g_Vars.ShowMenu = false;
	}
	else
	{
		g_Vars.ShowMenu = true;
	}
}

COVID_19 PlayerRevive()
{
	g_Engine.pfnClientCmd(XorString("rpn"));
}

COVID_19 PlayerReconnect()
{
	//PlayerMoveLagEventHook();
	g_Engine.pfnClientCmd(XorString("retry"));
}

COVID_19 PlayerRetRoom()
{
	if (!g_Vars.TimeoutRunning)
	{
		//PlayerMoveLagEventHook();
		//SettingTimeout(0);
		g_Vars.TimeoutRunning = true;
	}
}
int Dirty = 1;
void DirtyWordsThread()
{
	while (g_Vars.MyDirtyWord)
	{
		if (*(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
		{
			if (Dirty == 1 && *(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
				DirtyWords4(), Dirty += 1;
			if (Dirty == 2 && *(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
				DirtyWords3(), Dirty += 1;
			if (Dirty == 3 && *(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
				DirtyWords2(), Dirty += 1;
			if (Dirty == 4 && *(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
				DirtyWords1(), Dirty = 1;
		}
		Sleep(600);
	}
}

void ThirdPersonTrigger()
{
	if (!ThirdPersonV1)
		ThirdPersonV1 = true;
	else
		ThirdPersonV1 = false;
	if (ThirdPersonV1)
		g_Vars.ThirdPersonTrigger = true;
	else
		g_Vars.ThirdPersonTrigger = false;
}