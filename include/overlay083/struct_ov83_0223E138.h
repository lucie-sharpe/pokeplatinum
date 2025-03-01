#ifndef POKEPLATINUM_STRUCT_OV83_0223E138_H
#define POKEPLATINUM_STRUCT_OV83_0223E138_H

#include <nnsys.h>

#include "overlay083/struct_ov83_0223DEA0.h"
#include "overlay083/struct_ov83_0223FE50.h"

#include "bg_window.h"

typedef struct {
    int unk_00;
    BgConfig *unk_04;
    Window *unk_08;
    Window *unk_0C;
    int unk_10;
    int unk_14;
    UnkStruct_ov83_0223DEA0 *unk_18;
    UnkStruct_ov83_0223FE50 *unk_1C;
    void *unk_20;
    NNSG2dScreenData *unk_24;
    void *unk_28;
    NNSG2dScreenData *unk_2C;
    BOOL unk_30;
} UnkStruct_ov83_0223E138;

#endif // POKEPLATINUM_STRUCT_OV83_0223E138_H
