#pragma once
#include "ImGlobe.h"
class BkkMenu{

public:
	void LoadStyle();
	bool CheckBox(const char* label, bool* v);
	bool CheckBox2(const char* label, bool* va);
	void ComboBox(const char* const items[], int items_count, int height_in_items, const char* itemtitle, float* cvar, int arrayindex);
	void SliderFloat(const char* itemtitle, const char* help, float* cvar, float from, float to, const char* format);
	bool SliderFloat2(const char* label, float* v, float v_min, float v_max, const char* display_format, float power, ImVec2 size, ImVec4 color);
	bool SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags, ImVec4 color, ImVec2 valuesize, const char* label, char* value);
	void Separator();
	void ColorChange();
	//void KeyBind(float& key, char* keyname);
	bool Hotkey(const char* label, int* k, const ImVec2& size_arg);
};extern BkkMenu g_BkkMenu;