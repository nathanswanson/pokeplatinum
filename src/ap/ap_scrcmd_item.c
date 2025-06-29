#include "nitro/types.h"

#include "ap/ap_memory.h"
#include "field/field_system_decl.h"

#include "field_script_context.h"
BOOL ScrCmd_TriggerLocation(ScriptContext *ctx)
{
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 locationId = ScriptContext_ReadHalfWord(ctx);
    APComm_sendMessage(locationId);
    return FALSE;
}
