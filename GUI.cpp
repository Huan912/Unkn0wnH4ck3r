#include <Windows.h>
#include "GUI.h"
#include "detours.h"
#include "Structs.h"
#include "ImGlobe.h"
#include "StringList.h"
#include "Function.h"
#include "MenuControls.h"
#include "Dropdown.h"
#include "FixFunction.h"
#include "Data.h"
#include "ESP.h"
#include "Player.h"
#include "Config.h"
#include "Radar.h"
#include "FontFile.h"
bool keys[256];
bool keys_Engine[256];
typedef BOOL(APIENTRY* wglSwapBuffers_t)(HDC  hdc);
BOOL APIENTRY Sys_wglSwapBuffers(HDC hdc);
wglSwapBuffers_t pwglSwapBuffers = NULL;
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HOOK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InistalizeImGui(HDC hdc); void DrawingWindowOverlay();
void GLNewFrame(); void RenderData(); void DrawWindow(); void TabPage_1(); void TabPage_2(); void MouseEvent(); void TabPage_3(); void TabPage_4();
void DrawWindowV3();
void KreedzInfo_Window();
void BtnActiveTab(); void BtnNormalTab();
HWND hGameWnd;
WNDPROC hGameWndProc;
bool isOldOpenGL = true,NextPos, ActivateMouse;
GLint iMajor, iMinor;
ImFont* Eng_BigFont;
ImFont* HotKeyOnly;
void StepSwapBuffers()
{
	VMPBEGIN
	pwglSwapBuffers = (wglSwapBuffers_t)DetourFunction((LPBYTE)GetProcAddress(GetModuleHandleA(XorString("opengl32.dll")), XorString("wglSwapBuffers")), (LPBYTE)&Sys_wglSwapBuffers);
	VMPEND
}

BOOL APIENTRY Sys_wglSwapBuffers(HDC hdc)
{
	InistalizeImGui(hdc);
	GLNewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	DrawingWindowOverlay();
	if (g_Vars.KreedzInfo)
		KreedzInfo_Window();
	if (g_Vars.ShowMenu)
		DrawWindowV3();
	ImGui::Render();
	RenderData();
	g_BkkMenu.ColorChange();
	ImGui::GetIO().MouseDrawCursor = g_Vars.ShowMenu;
	MouseEvent();
	keys[VK_INSERT] = false;
	return(*pwglSwapBuffers)(hdc);
}


void InistalizeImGui(HDC hdc)
{
	VMPBEGIN
	static bool bInitialize = false;
	if (!bInitialize)
	{
		hGameWnd = WindowFromDC(hdc);
		hGameWndProc = (WNDPROC)SetWindowLong(hGameWnd, GWL_WNDPROC, (LONG)HOOK_WndProc);
		glGetIntegerv(0x821B, &iMajor);
		glGetIntegerv(0x821C, &iMinor);
		if ((iMajor * 10 + iMinor) >= 32)
			isOldOpenGL = false;
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hGameWnd);
		if (!isOldOpenGL)
		{
			ImplementGl3();
			ImGui_ImplOpenGL3_Init();
		}
		else
			ImGui_ImplOpenGL2_Init();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		g_BkkMenu.LoadStyle();
		io.IniFilename = NULL;
		//io.Fonts->AddFontFromFileTTF(XorString(CHTFont), 11, NULL, io.Fonts->GetGlyphRangesChineseFull());
		io.Fonts->AddFontFromMemoryTTF(SimHei_File,sizeof(SimHei_File),13,NULL, io.Fonts->GetGlyphRangesChineseFull());
		//io.Fonts->AddFontFromFileTTF(XorString("C:\\LETHALITY\\Font\\SimHei.ttf"), 13, NULL, io.Fonts->GetGlyphRangesChineseFull());
		Eng_BigFont = io.Fonts->AddFontFromFileTTF(XorString("c:\\Windows\\Fonts\\tahoma.ttf"), 24.0, NULL, io.Fonts->GetGlyphRangesChineseFull());
		//HotKeyOnly = io.Fonts->AddFontFromFileTTF(XorString("C:\\LETHALITY\\Font\\SimHei.ttf"), 11, NULL, io.Fonts->GetGlyphRangesChineseFull());
		HotKeyOnly = io.Fonts->AddFontFromMemoryTTF(SimHei_File, sizeof(SimHei_File), 11, NULL, io.Fonts->GetGlyphRangesChineseFull());
		LoadTextureMemory(BackDrop, 2, 48986);
		LoadTextureMemory(Scope, 3, 2174);
		bInitialize = true;
	}
	VMPEND
}

LRESULT CALLBACK HOOK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			keys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			keys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			keys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			keys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			keys[wParam] = true;
			break;
		case WM_KEYUP:
			keys[wParam] = false;
			break;
		case WM_IME_CHAR:
		{
			auto& io = ImGui::GetIO();
			DWORD wChar = wParam;
			if (wChar <= 127)
			{
				io.AddInputCharacter(wChar);
			}
			else
			{
				// swap lower and upper part.
				BYTE low = (BYTE)(wChar & 0x00FF);
				BYTE high = (BYTE)((wChar & 0xFF00) >> 8);
				wChar = MAKEWORD(high, low);
				wchar_t ch[6];
				MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR)&wChar, 4, ch, 3);
				io.AddInputCharacter(ch[0]);
			}
		}
		break;
		}
		Loop_Keys();
		/*
		
		*/
		if (g_Vars.ShowMenu)
		{
			if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			{
				if (g_Vars.InitDone)
				{
					if (*(BYTE*)(g_Vars.ScreenStopAdr) >= 5)
					{
						return CallWindowProc(hGameWndProc, hWnd, uMsg, wParam, lParam);
					}
					else
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
		return CallWindowProc(hGameWndProc, hWnd, uMsg, wParam, lParam);
}

void GLNewFrame()
{
	if (!isOldOpenGL)
		ImGui_ImplOpenGL3_NewFrame();
	else
		ImGui_ImplOpenGL2_NewFrame();
}

void RenderData()
{
	if (!isOldOpenGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	else
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void MouseEvent()
{
	if (g_Vars.InitDone)
	{
		if (g_Vars.ShowMenu)
		{
			g_pClient->IN_DeactivateMouse();
			
			ActivateMouse = false;
		}
		else
		{
			if (!ActivateMouse)
			{
				g_pClient->IN_ActivateMouse();
				
				ActivateMouse = true;
			}
		}
	}
}

void DrawingWindowOverlay()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin(XorString("eghwegjoiwaegjiowejiog"), reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
		
		ImGui::SetWindowPos(ImVec2(-10, -10), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x + 20, ImGui::GetIO().DisplaySize.y + 55), ImGuiSetCond_Always);
		
		if (g_Vars.RainBowBox)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			float ColorHSV[3];
			//===============================================Draw RECT Fill Lines//===============================================
			ImGui::ColorConvertRGBtoHSV(g_Vars.color_red, g_Vars.color_green, g_Vars.color_blue, ColorHSV[0], ColorHSV[1], ColorHSV[2]);
			for (int i = 0; i < 64; i++)
			{
				ColorHSV[0] += 1.0f / 64.0f;
				if (ColorHSV[0] > 1.0f)
					ColorHSV[0] -= 1.0f;
				ImGui::PushStyleColor(ImGuiCol_Separator, (ImVec4)ImColor::HSV(ColorHSV[0], ColorHSV[1], ColorHSV[2]));
				window->DrawList->AddLine(ImVec2(0, 0), ImVec2(window->Size.x, 0), ImGui::GetColorU32(ImGuiCol_Separator)); // Up
				window->DrawList->AddLine(ImVec2(window->Size.x - 20 - 1, 0), ImVec2(window->Size.x - 20, window->Size.y), ImGui::GetColorU32(ImGuiCol_Separator)); // right
				window->DrawList->AddLine(ImVec2(0, window->Size.y - 56), ImVec2(window->Size.x, window->Size.y - 56), ImGui::GetColorU32(ImGuiCol_Separator)); // Down
				window->DrawList->AddLine(ImVec2(0, 0), ImVec2(0, window->Size.y), ImGui::GetColorU32(ImGuiCol_Separator)); // left
				ImGui::PopStyleColor();
			}
		}
		//===============================================Draw RECT Fill Lines//===============================================

		if (g_Vars.LocalCheck && g_Vars.InitDone)
		{
			if ((*(DWORD*)(g_Vars.ScreenStopAdr) >= 5))
			{
				if (g_Vars.DrawTraceGrenade)
					TraceGrenade();
				if (g_Vars.Radar)
					DrawRadarScreen();
			}
		}
	}ImGui::End();
	ImGui::PopStyleColor();
}

void DrawWindowV3()
{
	ImGui::SetNextWindowSize({ 830, 500 });
	if (!NextPos)
	{
		ImGui::SetNextWindowPos(ImVec2((1024 - 830) / 2, (768 - 500) / 2));
		NextPos = true;
	}
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin(XorString("rweheraasdhe"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	{
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.012, 0.059, 0.102, 1.0f));
		ImGui::BeginChild(XorString("wertweafvrtko"), ImVec2(800, 70), false);
		{
			ImGui::PushFont(Eng_BigFont);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::Image((GLuint*)g_Vars.texture_id[3], ImVec2(32, 32));
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 14);
			ImGui::TextColored(ImVec4(0.933f, 0.933f, 0.933f, 1.f), XorString("LETHALITY"));
			ImGui::SameLine();	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7.5);
			ImGui::TextColored(ImVec4(0.8, 0.064f, 0.0705f, 1.f), XorString(" V3"));
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
			ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.9f), XorString("|"));
			//========================Buttons========================
			ImGui::SameLine();
			if (g_Vars.GUIPage == 1) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button(XorString("Basic"), ImVec2(0, 25)))g_Vars.GUIPage = 1;
			ImGui::SameLine();
			if (g_Vars.GUIPage == 2) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button(XorString("Visual"), ImVec2(0, 25)))g_Vars.GUIPage = 2;
			ImGui::SameLine();
			if (g_Vars.GUIPage == 3) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button(XorString("Rage"), ImVec2(0, 25)))g_Vars.GUIPage = 3;
			ImGui::SameLine();
			if (g_Vars.GUIPage == 4) BtnActiveTab(); else BtnNormalTab();
			if (ImGui::Button(XorString("Kreedz"), ImVec2(0, 25)))g_Vars.GUIPage = 4;
			ImGui::PopFont();
			BtnNormalTab();
		}ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.012, 0.059, 0.102, 0.8f));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.5);
		ImGui::BeginChild(XorString("akajajhssghedfhj"), ImVec2(800, 400), false);
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			if (g_Vars.GUIPage == 1)
				TabPage_1();
			if (g_Vars.GUIPage == 2)
				TabPage_2();
			if (g_Vars.GUIPage == 3)
				TabPage_3();
			if (g_Vars.GUIPage == 4)
				TabPage_4();
			ImGui::PopStyleColor();

		}ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
		ImGui::BeginChild(XorString("wertawaeafvrtako"), ImVec2(800, 15), false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
			ImGui::TextColored(ImVec4(0.933f, 0.933f, 0.933f, 1.f), XorString("LETHALITY - 2020 CopyRight"));
		}ImGui::EndChild();

		ImGui::PopStyleColor();
	}ImGui::End();
	CheckBox_FunctionCallBack();
}

void TabPage_1()
{
	ImGui::BeginChild(XorString("##Basic"), ImVec2(245, 390), true);
	{
		ImGui::Text(XorString(u8"基本功能")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"開局復活"), &g_Vars.Revive); ImGui::SameLine(130); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##ReviveKey"), &g_Key.ReviveKey, ImVec2(90, 15));
		g_BkkMenu.CheckBox2(XorString(u8"退出房間"), &g_Vars.RetRoom); ImGui::SameLine(130); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##RetroomKey"), &g_Key.RetroomKey, ImVec2(90, 15));
		g_BkkMenu.CheckBox2(XorString(u8"重新連房"), &g_Vars.ReConnect); ImGui::SameLine(130); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##ReconnectKey"), &g_Key.ReconnectKey, ImVec2(90, 15));
		g_BkkMenu.CheckBox2(XorString(u8"快速連房"), &g_Vars.FastConnect);
		g_BkkMenu.CheckBox2(XorString(u8"除去黑霧"), &g_Vars.DelFog); g_BkkMenu.CheckBox2(XorString(u8"自動嘴臭"), &g_Vars.MyDirtyWord);
		g_BkkMenu.CheckBox2(XorString(u8"第三人稱"), &g_Vars.ThirdPerson); ImGui::SameLine(130); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##ThirdPersonKey"), &g_Key.ThirdPersonKey, ImVec2(90, 15));
		if (g_Vars.ThirdPerson)
		{
			ImGui::Separator();
			ImGui::SliderFloat(XorString(u8"左右偏移"), &g_Vars.CameraOffset_right, -300, 300);
			ImGui::SliderFloat(XorString(u8"上下偏移"), &g_Vars.CameraOffset_up, -300, 300);
			ImGui::SliderFloat(XorString(u8"前後偏移"), &g_Vars.CameraOffset_forward, -300, 600);
			ImGui::Separator();
		}
	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorString("##Speed"), ImVec2(260, 390), true);
	{
		ImGui::Text(XorString(u8"加速功能")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"加速總開關"), &g_Vars.SpeedOn); ImGui::SameLine(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##加速按鍵設置"), &g_Key.SpeedKey, ImVec2(90, 15));
		const char* SpeedLevel[] = { "1" ,"2","3","4" };
		g_BkkMenu.ComboBox(SpeedLevel, IM_ARRAYSIZE(SpeedLevel), 4, XorString(u8"加速等級"), &g_Vars.SpeedLevel, 12);
		g_BkkMenu.CheckBox2(XorString(u8"加速增強"), &g_Vars.ScreenStop); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"指令優化"), &g_Vars.CommandScreenSet); 
		g_BkkMenu.CheckBox2(XorString(u8"自動左鍵"), &g_Vars.AutoMouse1); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"自動買主彈"), &g_Vars.AutoBuyAmmo);
		g_BkkMenu.CheckBox2(XorString(u8"自動右鍵"), &g_Vars.AutoMouse2); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"自動買副彈"), &g_Vars.AutoBuyAmmo2);
		g_BkkMenu.CheckBox2(XorString(u8"自動重連"), &g_Vars.AutoReConnect); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"優化FPS"), &g_Vars.HighFps);
		ImGui::SliderInt(XorString(u8"重連延遲"), &g_Vars.AutoReConnectValue, 0, 5000);
	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorString("##Save???"), ImVec2(175, 390), true);
	{
		BtnActiveTab();
		ImGui::Text(XorString(u8"保存參數")), ImGui::Separator();
		if (ImGui::Button(XorString(u8"保存參數"), ImVec2(135, 23)))
			ImGui::OpenPopup(XorString("##SaveCfgBox"));
		if (ImGui::Button(XorString(u8"載入參數"), ImVec2(135, 23)))
			g_Config.LoadCFG();
		if (ImGui::Button(XorString(u8"隱藏視窗"), ImVec2(135, 23)))
			g_Vars.ShowMenu = false;

		bool open = true;
		if (ImGui::BeginPopupModal(XorString("##SaveCfgBox"), &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::Text(XorString(u8"  請問是否要保存參數？"));
			if (ImGui::Button(XorString(u8"保存")))
			{
				ImGui::CloseCurrentPopup();
				g_Config.SaveCFG();
			}
			ImGui::SameLine();
			if (ImGui::Button(XorString(u8"不保存")))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		BtnNormalTab();
	}ImGui::EndChild();
}

void TabPage_2() //Visual
{
	ImGui::BeginChild(XorString("##ESP"), ImVec2(260, 390), true);
	{
		ImGui::Text(XorString(u8"繪製功能")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"人物方框"), &g_Vars.ESPBox); ImGui::SameLine(145); g_BkkMenu.CheckBox2(XorString(u8"人物名稱"), &g_Vars.ESPName);
		g_BkkMenu.CheckBox2(XorString(u8"人物距離"), &g_Vars.ESPDist); ImGui::SameLine(145); g_BkkMenu.CheckBox2(XorString(u8"人物武器"), &g_Vars.ESPWeapons);
		g_BkkMenu.CheckBox2(XorString(u8"人物軌跡"), &g_Vars.ESPTrace); ImGui::SameLine(145); g_BkkMenu.CheckBox2(XorString(u8"人物模組"), &g_Vars.DrawHitBox);
		g_BkkMenu.CheckBox2(XorString(u8"手榴預測"), &g_Vars.DrawTraceGrenade); ImGui::SameLine(145); g_BkkMenu.CheckBox2(XorString(u8"顯示雷達"), &g_Vars.Radar);
		ImGui::SliderFloat(XorString(u8"雷達大小"), &g_Vars.visual_radar_size, 20, 110, XorString("%.f"));
		const char* TeamSelect[] = {u8"敵人" ,u8"隊友",u8"全部"};
		g_BkkMenu.ComboBox(TeamSelect, IM_ARRAYSIZE(TeamSelect), 3, XorString(u8"顯示陣營"), &g_Vars.ESPTeamselection, 1);
		g_BkkMenu.ComboBox(TeamSelect, IM_ARRAYSIZE(TeamSelect), 3, XorString(u8"模組陣營"), &g_Vars.ModelSelection, 11);
	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorString("Visual"), ImVec2(280, 390), true);
	{
		ImGui::Text(XorString("Visual")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"人物透視"), &g_Vars.WallPlayer); 
		g_BkkMenu.CheckBox2(XorString(u8"防閃光彈"), &g_Vars.NOFlash); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"防煙霧彈"), &g_Vars.NoSmoke);
		g_BkkMenu.CheckBox2(XorString(u8"繪製準星"), &g_Vars.DrawCrosshair); ImGui::SameLine(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);  ImGui::ColorEdit4(XorString("##CrosshairColor"), g_Vars.CrosshairColor, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4); ImGui::Text(XorString(u8"準星顏色"));
		g_BkkMenu.CheckBox2(XorString(u8"顯示地速"), &g_Vars.KreedzInfo); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"畫漸層邊框"), &g_Vars.RainBowBox);
		g_BkkMenu.CheckBox2(XorString(u8"更改地圖亮度"), &g_Vars.OverrideLightmap);
		if(g_Vars.OverrideLightmap)ImGui::SliderFloat(XorString(u8"亮度數值"), &g_Vars.SetLightmapDarkness, 0, 200, XorString("%.f"));

		ImGui::Text(XorString(u8"人物發光")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"人物發光"), &g_Vars.PlayerGlow);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##PlayerColorTe"), g_Vars.Player_TeammateGlow, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"隊友顏色"));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##PlayerColorEn"), g_Vars.Player_EnemyGlow, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"敵人顏色"));

		ImGui::Text(XorString(u8"人物上色")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"人物上色"), &g_Vars.PlayerColor);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##PlayerGlowTe"), g_Vars.Player_TeammateColor, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"隊友顏色"));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##PlayerGlowEn"), g_Vars.Player_EnemyColor, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"敵人顏色"));
		
		ImGui::Text(XorString(u8"人物射線")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"人物射線"), &g_Vars.Barrel);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##BarrelGlowTe"), g_Vars.Player_TeammateBarrel, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"CT顏色"));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
		ImGui::ColorEdit3(XorString("##BarrelGlowEn"), g_Vars.Player_EnemyBarrel, ImGuiColorEditFlags_NoInputs); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);  ImGui::Text(XorString(u8"T顏色"));
	}ImGui::EndChild();
}

void TabPage_3() //Rage
{
	ImGui::BeginChild(XorString("##RageAimBot"), ImVec2(260, 390), true);
	{
		ImGui::Text(XorString(u8"自動瞄準")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"總開關"), &g_Vars.RageAimbot); ;ImGui::SameLine(140); g_BkkMenu.Hotkey(XorString(u8""),&g_Key.RageAimKey,ImVec2(90,15));
		g_BkkMenu.CheckBox2(XorString(u8"左右鍵"), &g_Vars.TwoKey);
		ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"普通瞄準"), &g_Vars.LegitAim); ImGui::SameLine(); g_BkkMenu.CheckBox2(XorString(u8"靜默瞄準"), &g_Vars.SilentAim);//ImGui::SameLine(); g_BkkMenu.CheckBox(XorString(u8"子彈追蹤"), &g_Vars.AutoBullet);

		g_BkkMenu.CheckBox2(XorString(u8"自動壓槍"), &g_Vars.NoRecoilAngle);
		//g_BkkMenu.CheckBox(XorString(u8"自動開槍"), &g_Vars.AutoFire);
		//g_BkkMenu.CheckBox(XorString(u8"牆體辨識"), &g_Vars.AutoWall);
		g_BkkMenu.CheckBox2(XorString(u8"顯示瞄準目標"), &g_Vars.ShowTargetID);
		ImGui::SliderInt(XorString(u8"瞄準範圍"), &g_Vars.RageAimFov, 0, 360);
		ImGui::SliderInt(XorString(u8"距離限制"), &g_Vars.RageAimDist, 0, 12000);
		g_BkkMenu.SliderFloat(XorString(u8"壓槍倍率:"), XorString(u8"壓槍倍率"), &g_Vars.NoRecoilValue, 0.0f, 5.0f, XorString("%.2f"));
		const char* CalcSelect[] = { u8"角度判斷" ,u8"距離判斷",u8"都判斷" };
		g_BkkMenu.ComboBox(CalcSelect, IM_ARRAYSIZE(CalcSelect), 3, XorString(u8""), &g_Vars.Rage_Target_Selection, 3);
		const char* TeamSelect[] = { u8"敵人" ,u8"隊友",u8"全部" };
		g_BkkMenu.ComboBox(TeamSelect, IM_ARRAYSIZE(TeamSelect), 3, XorString(u8"陣營"), &g_Vars.AimTeamselection, 12);
		const char* BodySelect[] = { u8"頭部" ,u8"脖子",u8"胸部",u8"身體" };
		g_BkkMenu.ComboBox(BodySelect, IM_ARRAYSIZE(BodySelect), 4, XorString(u8"部位"), &g_Vars.AimBodyselection, 13);
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), XorString(u8"若要打頭，推薦用脖子."));

	}ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild(XorString("Aimbot - Anti-Aimbot"), ImVec2(260, 390), true);
	{
		ImGui::Text(XorString("Anti Aim")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"開啟反自瞄 ( 搖頭 )"), &g_Vars.AntiAim);
		ImGui::Text(XorString(""));
		ImGui::Text(XorString(u8"俯仰"));
		const char* listbox_pitch[] = { u8"無", u8"向上", u8"隨機" };
		g_BkkMenu.ComboBox(listbox_pitch, IM_ARRAYSIZE(listbox_pitch), 3, XorString(u8""), &g_Vars.aa_pitch, 4);
		ImGui::Text(XorString(u8"搖擺(站著時)"));
		const char* listbox_yaw[] = { u8"無", "180", u8"180 抖動", u8"旋轉", u8"抖動", u8"側身", u8"隨機", u8"靜態" };
		g_BkkMenu.ComboBox(listbox_yaw, IM_ARRAYSIZE(listbox_yaw), 8, XorString(u8""), &g_Vars.aa_yaw, 5);
		ImGui::Text(XorString(u8"移動時搖擺"));
		g_BkkMenu.ComboBox(listbox_yaw, IM_ARRAYSIZE(listbox_yaw), 8, XorString(u8""), &g_Vars.aa_yaw_while_running, 6);
		g_BkkMenu.SliderFloat(XorString(u8"靜態搖擺:"), XorString(u8"靜態搖擺"), &g_Vars.aa_yaw_static, -180.f, 180.f, XorString("%.0f"));
		/*
		ImGui::Text("Roll");
		const char* listbox_roll[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
		g_BkkMenu.ComboBox(listbox_roll, IM_ARRAYSIZE(listbox_roll), 8, XorString(u8""), &g_Vars.aa_roll, 7);
		ImGui::Text("Roll While Running");
		const char* listbox_rollrun[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
		g_BkkMenu.ComboBox(listbox_rollrun, IM_ARRAYSIZE(listbox_rollrun), 8, XorString(u8""), &g_Vars.aa_roll_while_running, 8);
		g_BkkMenu.SliderFloat("SStatic Roll:", "Static Roll", &g_Vars.aa_roll_static, -180.f, 180.f, "%.0f");
		*/
		ImGui::Text(XorString(u8"假邊緣"));
		const char* listbox_edge[] = { "None", u8"站著", u8"移動中", "All", "Fake" };
		g_BkkMenu.ComboBox(listbox_edge, IM_ARRAYSIZE(listbox_edge), 5, XorString(u8""), &g_Vars.aa_edge, 9);

	}ImGui::EndChild();
}

void TabPage_4() //Kreedz
{
	ImGui::BeginChild(XorString("Kreedz"), ImVec2(245, 390), true);
	{
		ImGui::Text(XorString("Kreedz")), ImGui::Separator();
		g_BkkMenu.CheckBox2(XorString(u8"278地速"), &g_Vars.FastRun);
		g_BkkMenu.CheckBox2(XorString(u8"超級GS"), &g_Vars.GroundStrafe); ImGui::SameLine(130); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4); g_BkkMenu.Hotkey(XorString(u8"##GroundStrafeKey"), &g_Key.GroundStrafeKey, ImVec2(90, 15));
		g_BkkMenu.CheckBox2(XorString(u8"自動同步"), &g_Vars.StrafeHelper);
		g_BkkMenu.CheckBox2(XorString(u8"無損地速"), &g_Vars.AdjustSpeed);
		g_BkkMenu.CheckBox2(XorString(u8"空白鍵連跳"), &g_Vars.BunnyHop);
	}ImGui::EndChild();
}

void KreedzInfo_Window()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
	ImGui::SetNextWindowPos(ImVec2(20, (ImGui::GetIO().DisplaySize.y / 2) - ImGui::GetIO().DisplaySize.y / 20 * 1), ImGuiCond_Once);
	ImGui::Begin(XorString("sdrhsdfhsdherhe"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::Text(XorString("-Kreedz Info-"));
		g_BkkMenu.Separator();
		if (g_Vars.InitDone && g_Vars.LocalCheck)
		{
			ImGui::Text(XorString("Speed: %.2f"), g_Local.ppmove->velocity.Length2D());
			ImGui::Text(XorString("MoveType: %d"), g_Local.ppmove->movetype);
			ImGui::Text(XorString("Height: %.2f"), g_Local.flHeight);
		}
		else
		{
			ImGui::Text(XorString("Speed: 0"));
			ImGui::Text(XorString("MoveType: 0"));
			ImGui::Text(XorString("Height: 0"));
		}
		ImGui::PopStyleColor();
		ImGui::End();
	}
}

float CalculateFrameRate(void) {
	float flFrameRate;
	static float flCurrentTime = 0.0f;
	static float flFramesPerSecond = 0.0f;
	static float flLastTime = 0.0f;

	flCurrentTime = GetTickCount() * 0.001f;
	flFramesPerSecond++;

	if (flCurrentTime - flLastTime > 1.0f) {
		flFrameRate = flFramesPerSecond;

		flFramesPerSecond = 0.0f;

		flLastTime = flCurrentTime;
	}
	return flFrameRate;
}

void BtnActiveTab()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(52, 57, 68, 255);
	style.Colors[ImGuiCol_Border] = ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(52, 57, 68, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(52, 57, 68, 255);
	style.FrameRounding = 12;
}
void BtnNormalTab()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_Border] = ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 0);
	style.FrameRounding = 0;
}


void efgwehgewrherherh()
{
	float WuyviwElKb = 95932755463067; WuyviwElKb = 39476479325358; if (WuyviwElKb = 75245478112187) WuyviwElKb = 81314119672380; WuyviwElKb = 43824342447604; WuyviwElKb = 24476044382434;
	if (WuyviwElKb = 52623488131411)WuyviwElKb = 81121871042112; WuyviwElKb = 39090012979230;
	if (WuyviwElKb = 7435196900725)WuyviwElKb = 81121871042112; WuyviwElKb = 39090012979230;
	if (WuyviwElKb = 7435196900725)WuyviwElKb = 81121871042112; WuyviwElKb = 39090012979230;
	if (WuyviwElKb = 7435196900725)WuyviwElKb = 81121871042112; WuyviwElKb = 39090012979230;
	if (WuyviwElKb = 7435196900725)WuyviwElKb = 81121871042112; WuyviwElKb = 39090012979230; WuyviwElKb = 96228445046073;
}

void herhdykrtyk()
{
	float hADMejovsw = 94370933118074; hADMejovsw = 22396791044930; if (hADMejovsw = 4279919031917) hADMejovsw = 76300703451497; hADMejovsw = 61269342742668; hADMejovsw = 27426686126934;
	if (hADMejovsw = 35125677630070)hADMejovsw = 90319176876950; hADMejovsw = 9511243397050;
	if (hADMejovsw = 61074907206517)hADMejovsw = 90319176876950; hADMejovsw = 9511243397050;
	if (hADMejovsw = 61074907206517)hADMejovsw = 90319176876950; hADMejovsw = 9511243397050;
	if (hADMejovsw = 61074907206517)hADMejovsw = 90319176876950; hADMejovsw = 9511243397050;
	if (hADMejovsw = 61074907206517)hADMejovsw = 90319176876950; hADMejovsw = 9511243397050; hADMejovsw = 16963585247652;
}

void ryulkftyukjrthrthr()
{
	float WSmoievWOV = 91393198232387; WSmoievWOV = 42475044474029; if (WSmoievWOV = 2311013205372) WSmoievWOV = 49091584649294; WSmoievWOV = 18446049174863; WSmoievWOV = 91748631844604;
	if (WSmoievWOV = 2414184909158)WSmoievWOV = 2053724861151; WSmoievWOV = 65202499415504;
	if (WSmoievWOV = 11246389187543)WSmoievWOV = 2053724861151; WSmoievWOV = 65202499415504;
	if (WSmoievWOV = 11246389187543)WSmoievWOV = 2053724861151; WSmoievWOV = 65202499415504;
	if (WSmoievWOV = 11246389187543)WSmoievWOV = 2053724861151; WSmoievWOV = 65202499415504;
	if (WSmoievWOV = 11246389187543)WSmoievWOV = 2053724861151; WSmoievWOV = 65202499415504; WSmoievWOV = 45179055597709;
}