#include "scrcmd_item.h"

#include <nitro.h>
#include <string.h>

#include "field/field_system.h"

#include "bag.h"
#include "field_script_context.h"
#include "inlines.h"
#include "item.h"
#include "unk_0205DFC4.h"
#include "debug_print.h"
#include "ap_memory.h"
BOOL ScrCmd_AddItem(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 count = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);
    u16 ap_id = ScriptContext_GetVar(ctx);
    if(ap_id != 0)
    {
        APLocData data = loc_data_registry[ap_id];
        if(data.is_local) {
            *destVar = Bag_TryAddItem(SaveData_GetBag(fieldSystem->saveData), data.id, data.count, HEAP_ID_FIELD);
        }
        else
        {
            APComm_push_item_to_buf(ap_id);
            *destVar = TRUE;
        }
    }
    else {
        *destVar = Bag_TryAddItem(SaveData_GetBag(fieldSystem->saveData), item, count, HEAP_ID_FIELD);
    }
    return FALSE;
}

BOOL ScrCmd_RemoveItem(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 count = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);

    *destVar = Bag_TryRemoveItem(SaveData_GetBag(fieldSystem->saveData), item, count, HEAP_ID_FIELD);
    return FALSE;
}

BOOL ScrCmd_CanFitItem(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 count = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);

    *destVar = Bag_CanFitItem(SaveData_GetBag(fieldSystem->saveData), item, count, HEAP_ID_FIELD);
    return FALSE;
}

BOOL ScrCmd_CheckItem(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 count = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);

    *destVar = Bag_CanRemoveItem(SaveData_GetBag(fieldSystem->saveData), item, count, HEAP_ID_FIELDMAP);
    return FALSE;
}

BOOL ScrCmd_GetItemQuantity(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);

    *destVar = Bag_GetItemQuantity(SaveData_GetBag(fieldSystem->saveData), item, HEAP_ID_FIELDMAP);
    return FALSE;
}

BOOL ScrCmd_IsItemTMHM(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 *destVar = ScriptContext_GetVarPointer(ctx);

    *destVar = Item_IsTMHM(item);
    return FALSE;
}

BOOL ScrCmd_GetItemPocket(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 item = ScriptContext_GetVar(ctx);
    u16 *pocket = ScriptContext_GetVarPointer(ctx);

    *pocket = (u16)Item_LoadParam(item, ITEM_PARAM_FIELD_POCKET, 11);
    return FALSE;
}

BOOL ScrCmd_Dummy081(ScriptContext *ctx)
{
    return FALSE;
}

BOOL ScrCmd_Dummy082(ScriptContext *ctx)
{
    return FALSE;
}
