typedef void(*xcommand_t)(void);
typedef struct cmd_s
{
	struct cmd_s* next;
	const char* name;
	xcommand_t function;
	int flags;
} cmd_t, * pcmd_t;

typedef struct TUserMsg
{
	int number;
	int size;
	char name[16];
	struct TUserMsg* next;
	pfnUserMsgHook pfn;
} *PUserMsg;

void InitUserMsgBase();
pfnUserMsgHook HookUserMsg(const char* szMsgName, pfnUserMsgHook pfn);