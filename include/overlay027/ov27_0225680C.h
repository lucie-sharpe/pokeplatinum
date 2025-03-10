#ifndef POKEPLATINUM_OV27_0225680C_H
#define POKEPLATINUM_OV27_0225680C_H

#include "bg_window.h"

typedef struct {
    BOOL mem1;
    BOOL unk_04;
    u32 mem5;
    u32 mem4;
    u32 mem3;
    u32 mem2;
    int unk_18;
} UnkStruct_ov27_0225680C_1;

typedef struct UnkStruct_ov27_0225680C_t UnkStruct_ov27_0225680C;

BOOL ov27_0225680C(UnkStruct_ov27_0225680C **param0, const UnkStruct_ov27_0225680C_1 *param1, BgConfig *param2);
void ov27_02256890(UnkStruct_ov27_0225680C *param0);
void ov27_022569C8(UnkStruct_ov27_0225680C *param0, u32 param1);
BOOL ov27_022569EC(UnkStruct_ov27_0225680C *param0, u32 param1);
BOOL ov27_022569F8(UnkStruct_ov27_0225680C *param0);

#endif // POKEPLATINUM_OV27_0225680C_H
