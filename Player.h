class PlayerInfoLocal
{
public:
	cl_entity_s* ent;
	playermove_s* ppmove;
	int Team, Refresh,iMaxHitboxes;
	bool Death, IsValidEnt;
	QVector HeadBone, vForward, vRight, vViewAngles, vNoSpreadAngle, vNoRecoilAngle, vEye, vPunchangle, vOrigin;
	char* name;
	float flGroundAngle, flHeight, flJumpmesstime, flFrametime, flVelocity2D;
	float pmEyePos[3];
}; extern PlayerInfoLocal g_Local;

class PlayerInfoIndex
{
public:
	cl_entity_s* ent;
	int Team, Refresh,BestHitBoxesID;
	bool Death, IsValidEnt, Visible,bBehindTheWall, isZombie;
	QVector vVelocity,HeadBone, vOrigin;
	float flFrametime, flDist;
	char* name;
}; extern PlayerInfoIndex g_LocalPlayer[33];

void UpdateMyLocal();
void UpdateLocalPlayer();
bool IsValidEnt(cl_entity_s* ent);
float Get3DDistance(float* self, float* obj);
void ClearLocalPlayer(int i);
bool IsPlayerDeath(struct cl_entity_s* ent);
bool IsVisibleEnt(float* pflFrom, float* pflTo);