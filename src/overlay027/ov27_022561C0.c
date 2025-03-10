#include <nitro.h>
#include <nitro/sinit.h>
#include <string.h>

#include "overlay025/poketch_button.h"
#include "overlay025/poketch_system.h"
#include "overlay027/ov27_0225680C.h"

#include "bg_window.h"
#include "heap.h"
#include "poketch_memory.h"
#include "sys_task.h"
#include "sys_task_manager.h"
#include "touch_screen.h"
#include "unk_02022844.h"

typedef struct {
    BOOL mem1;
    u32 mem2;
    u32 mem3;
    u32 mem4;
    u32 mem5;
    u64 mem6;
    u64 mem7;
} AppMemory;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u16 unk_06;
    u32 appID;
    u64 mem6;
    u64 mem7;
    UnkStruct_ov27_0225680C_1 unk_1C;
    AppMemory memory;
    PoketchButtonManager *buttonManager;
    UnkStruct_ov27_0225680C *unk_60;
    PoketchSystem *poketchSys;
} AppData;

static void NitroStaticInit(void);

static BOOL Init(void **appData, PoketchSystem *poketchSys, BgConfig *bgConfig, u32 appID);
static BOOL SetupAppData(AppData *appData, BgConfig *bgConfig, u32 appID);
static void ov27_022562AC(AppData *appData);
static void ov27_022562FC(SysTask *param0, void *param1);
static void Exit(void *param0);
static void ov27_02256360(AppData *appData, u32 param1);
static BOOL ov27_0225637C(AppData *appData);
static BOOL ov27_022563CC(AppData *appData);
static BOOL ov27_0225644C(AppData *appData);
static BOOL ov27_022564D0(AppData *appData);
static BOOL ov27_02256534(AppData *appData);
static BOOL ov27_022565D0(AppData *appData);
static BOOL ov27_02256608(AppData *appData);
static void ov27_02256654(AppData *appData);
static void ov27_02256660(u32 param0, u32 param1, u32 param2, void *param3);
static void ResetLocalAppMemory(AppData *appData);
static void ov27_02256680(AppData *appData);
static void ov27_022566C4(AppData *appData);
static void ov27_022566D8(AppData *appData, UnkStruct_ov27_0225680C_1 *param1);

static void NitroStaticInit(void)
{
    PoketchSystem_SetAppFunctions(Init, Exit);
}

static BOOL Init(void **appData, PoketchSystem *poketchSys, BgConfig *bgConfig, u32 appID)
{
    AppData *newAppData = Heap_AllocFromHeap(HEAP_ID_POKETCH_APP, sizeof(AppData));

    if (newAppData != NULL) {
        if (SetupAppData(newAppData, bgConfig, appID)) {
            newAppData->poketchSys = poketchSys;

            if (SysTask_Start(ov27_022562FC, newAppData, 1) != NULL) {
                *appData = newAppData;
                return TRUE;
            }
        } else {
            Heap_FreeToHeap(newAppData);
            newAppData = NULL;
        }
    }

    return FALSE;
}

static BOOL SetupAppData(AppData *appData, BgConfig *bgConfig, u32 appID)
{
    appData->appID = appID;

    if (PoketchMemory_Read32(appID, &(appData->memory), sizeof(appData->memory))) {
        appData->mem6 = appData->memory.mem6;
        appData->mem7 = appData->memory.mem7;
        appData->unk_1C.mem2 = appData->memory.mem2;
        appData->unk_1C.mem3 = appData->memory.mem3;
        appData->unk_1C.mem4 = appData->memory.mem4;
        appData->unk_1C.mem5 = appData->memory.mem5;
        appData->unk_1C.mem1 = appData->memory.mem1;

        if (appData->unk_1C.mem1) {
            appData->unk_1C.unk_18 = 2;
        } else {
            appData->unk_1C.unk_18 = 0;
        }
    } else {
        ResetLocalAppMemory(appData);
        appData->unk_1C.unk_18 = 0;
    }

    if (ov27_0225680C(&(appData->unk_60), &(appData->unk_1C), bgConfig)) {
        appData->unk_00 = 0;
        appData->unk_01 = 0;
        appData->unk_03 = appData->unk_00;
        appData->unk_02 = 0;

        if (ov27_02256608(appData)) {
            return 1;
        }
    }

    return 0;
}

static void ov27_022562AC(AppData *appData)
{
    appData->memory.mem6 = appData->mem6;
    appData->memory.mem7 = appData->mem7;
    appData->memory.mem2 = appData->unk_1C.mem2;
    appData->memory.mem3 = appData->unk_1C.mem3;
    appData->memory.mem4 = appData->unk_1C.mem4;
    appData->memory.mem5 = appData->unk_1C.mem5;
    appData->memory.mem1 = appData->unk_1C.mem1;

    PoketchMemory_Write32(appData->appID, &(appData->memory), sizeof(appData->memory));

    ov27_02256654(appData);
    ov27_02256890(appData->unk_60);

    Heap_FreeToHeap(appData);
}

static void ov27_022562FC(SysTask *param0, void *param1)
{
    static BOOL (*const v0[])(AppData *) = {
        ov27_0225637C,
        ov27_022563CC,
        ov27_0225644C,
        ov27_022564D0,
        ov27_02256534,
        ov27_022565D0
    };

    AppData *v1 = (AppData *)param1;

    if (v1->unk_00 < NELEMS(v0)) {
        if (v1->unk_02 && (v1->unk_00 != 5)) {
            ov27_02256360(v1, 5);
            v1->unk_02 = 0;
        }

        PoketechSystem_UpdateButtonManager(v1->poketchSys, v1->buttonManager);
        ov27_022566D8(v1, &(v1->unk_1C));

        if (v0[v1->unk_00](v1)) {
            ov27_022562AC(v1);
            SysTask_Done(param0);
            PoketchSystem_NotifyAppUnloaded(v1->poketchSys);
        }
    } else {
    }
}

static void Exit(void *param0)
{
    ((AppData *)param0)->unk_02 = 1;
}

static void ov27_02256360(AppData *appData, u32 param1)
{
    appData->unk_03 = appData->unk_00;

    if (appData->unk_02 == 0) {
        appData->unk_00 = param1;
    } else {
        appData->unk_00 = 5;
        appData->unk_02 = 0;
    }

    appData->unk_01 = 0;
}

static BOOL ov27_0225637C(AppData *appData)
{
    switch (appData->unk_01) {
    case 0:
        ov27_022569C8(appData->unk_60, 0);
        appData->unk_01++;
        break;
    case 1:
        if (ov27_022569EC(appData->unk_60, 0)) {
            PoketchSystem_NotifyAppLoaded(appData->poketchSys);

            if (appData->unk_1C.mem1) {
                ov27_02256360(appData, 2);
            } else {
                ov27_02256360(appData, 1);
            }
        }
        break;
    }

    return 0;
}

static BOOL ov27_022563CC(AppData *appData)
{
    switch (appData->unk_01) {
    case 0:
        if (appData->unk_04 == 1) {
            appData->unk_1C.unk_18 = 1;
            ov27_022569C8(appData->unk_60, 1);
            appData->unk_01++;
        }
        break;
    case 1:
        switch (appData->unk_04) {
        case 2:
            appData->unk_1C.unk_18 = 0;
            ov27_022569C8(appData->unk_60, 1);
            appData->unk_01--;
            break;
        case 3:
            appData->unk_1C.unk_18 = 2;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256680(appData);
            ov27_02256360(appData, 2);
            break;
        case 5:
            appData->unk_1C.unk_18 = 3;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 3);
            break;
        }
        break;
    }

    return 0;
}

static BOOL ov27_0225644C(AppData *appData)
{
    switch (appData->unk_01) {
    case 0:
        if (appData->unk_04 == 1) {
            appData->unk_1C.unk_18 = 1;
            ov27_022566C4(appData);
            ov27_022569C8(appData->unk_60, 1);
            appData->unk_01++;
        }
        break;
    case 1:
        switch (appData->unk_04) {
        case 2:
            appData->unk_1C.unk_18 = 0;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 1);
            break;
        case 3:
            appData->unk_1C.unk_18 = 0;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 1);
            break;
        case 5:
            appData->unk_1C.unk_18 = 3;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 3);
            break;
        }
        break;
    }

    return 0;
}

static BOOL ov27_022564D0(AppData *appData)
{
    switch (appData->unk_04) {
    case 3:
        if (appData->unk_03 == 1) {
            ov27_02256680(appData);
            appData->unk_1C.unk_18 = 2;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 2);
            break;
        }
    case 2:
        appData->unk_1C.unk_18 = 0;
        ov27_022569C8(appData->unk_60, 1);
        ov27_02256360(appData, 1);
        break;
    case 6:
        appData->unk_1C.unk_18 = 4;
        ov27_022569C8(appData->unk_60, 1);
        ov27_02256360(appData, 4);
        break;
    }

    return 0;
}

static BOOL ov27_02256534(AppData *appData)
{
    switch (appData->unk_01) {
    case 0:
        appData->unk_06 = 0;
        appData->unk_01++;
    case 1:
        if (++(appData->unk_06) >= 90) {
            appData->unk_1C.unk_18 = 5;
            ov27_022569C8(appData->unk_60, 1);
            appData->unk_06 = 0;
            appData->unk_01++;
        }
        break;
    case 2:
        if (++(appData->unk_06) >= 60) {
            appData->unk_1C.unk_18 = 6;
            ov27_022569C8(appData->unk_60, 1);
            ResetLocalAppMemory(appData);
            appData->unk_06 = 0;
            appData->unk_01++;
        }
        break;
    case 3:
        if (ov27_022569EC(appData->unk_60, 1)) {
            appData->unk_1C.unk_18 = 0;
            ov27_022569C8(appData->unk_60, 1);
            ov27_02256360(appData, 1);
        }
        break;
    }

    return 0;
}

static BOOL ov27_022565D0(AppData *appData)
{
    switch (appData->unk_01) {
    case 0:
        appData->unk_1C.unk_18 = 7;
        ov27_022569C8(appData->unk_60, 3);
        appData->unk_01++;
        break;
    case 1:
        if (ov27_022569F8(appData->unk_60)) {
            return 1;
        }
        break;
    }

    return 0;
}

static BOOL ov27_02256608(AppData *appData)
{
    static const TouchScreenHitTable v0[] = {
        { 0xfe, 112, 112, 39 },
    };

    appData->buttonManager = PoketchButtonManager_New(v0, NELEMS(v0), ov27_02256660, appData, 8);

    if (appData->buttonManager != NULL) {
        PoketchButtonManager_SetButtonTimer(appData->buttonManager, 0, 0, 15);
        PoketchButtonManager_SetButtonTimer(appData->buttonManager, 0, 1, 75);
        appData->unk_04 = 0;
        return 1;
    }

    return 0;
}

static void ov27_02256654(AppData *appData)
{
    PoketchButtonManager_Free(appData->buttonManager);
}

static void ov27_02256660(u32 param0, u32 param1, u32 param2, void *param3)
{
    AppData *v0 = (AppData *)param3;
    v0->unk_04 = param1;
}

static void ResetLocalAppMemory(AppData *appData)
{
    appData->unk_1C.mem1 = FALSE;
    appData->unk_1C.unk_04 = TRUE;
    appData->unk_1C.mem5 = 0;
    appData->unk_1C.mem4 = 0;
    appData->unk_1C.mem3 = 0;
    appData->unk_1C.mem2 = 0;
    appData->mem6 = 0;
    appData->mem7 = 0;
}

static void ov27_02256680(AppData *appData)
{
    if (appData->mem6) {
        appData->mem6 += (sub_0202293C() - appData->mem7);
    } else {
        appData->mem6 = sub_0202293C();
    }

    appData->unk_1C.mem1 = 1;
    appData->unk_1C.unk_04 = 0;
}

static void ov27_022566C4(AppData *appData)
{
    appData->mem7 = sub_0202293C();
    appData->unk_1C.mem1 = 0;
}

static void ov27_022566D8(AppData *appData, UnkStruct_ov27_0225680C_1 *param1)
{
    if (param1->mem1) {
        u64 v0, v1, v2;

        v2 = sub_0202293C() - appData->mem6;

        CP_SetDiv64_32(sub_02022944(v2), 1000);

        v0 = CP_GetDivResult64();
        v1 = CP_GetDivRemainder64();

        CP_SetDiv64_32(v1, 10);
        param1->mem5 = CP_GetDivResult32();

        CP_SetDiv64_32(v0, 60);

        v0 = CP_GetDivResult64();
        param1->mem4 = CP_GetDivRemainder32();

        CP_SetDiv64_32(v0, 60);

        v0 = CP_GetDivResult64();
        param1->mem3 = CP_GetDivRemainder32();

        if (v0 >= 24) {
            CP_SetDiv64_32(v0, 24);
            v0 = CP_GetDivRemainder32();
        }

        param1->mem2 = (u32)v0;
    }
}
