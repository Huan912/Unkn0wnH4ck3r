void DrawTargetID(int i);
void DrawPlayerBox(int i);
void DrawDistance(int i);
void DrawPlayerName(int i);
void DrawPlayerWeapon(int i);
void DrawPlayerTrace(int i);
void DrawCrosshair(int r, int g, int b, int a);
void DrawHitBoxID(int i);
void PlayerGlow(struct cl_entity_s* ent);
void Studio_PlayerColor();
void TraceGrenade();
void Barrel(struct cl_entity_s* ent);
void DrawImage(int x, int y, int w, int h, char* Image);
bool IsCanDraw(int i, float selection);