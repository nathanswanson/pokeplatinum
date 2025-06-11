#include "ap/ap_memory.h"

#include <string.h>

#include "constants/heap.h"
#include "generated/badges.h"
#include "generated/text_banks.h"

#include "ap/ap_debug_print.h"
#include "ap/ap_player_names.h"
#include "field/field_system.h"

#include "bag.h"
#include "bg_window.h"
#include "field_message.h"
#include "heap.h"
#include "item.h"
#include "map_object.h"
#include "message.h"
#include "render_window.h"
#include "save_player.h"
#include "savedata_misc.h"
#include "strbuf.h"
#include "string.h"
#include "string_template.h"
#include "sys_task.h"
#include "trainer_info.h"

#include "res/text/bank/common_strings.h"

typedef struct APMemoryConfig {
    Window *window;
    u8 printer_id;
    u16 conf;
} APMemoryConfig;

typedef struct APBuffer {
    u16 read_idx;
    u16 write_idx;
    u16 buf[5];
} APBuffer;

static const WindowTemplate message_template = {
    .bgLayer = BG_LAYER_MAIN_3,
    .tilemapLeft = 2,
    .tilemapTop = 19,
    .width = 27,
    .height = 4,
    .palette = 12,
    .baseTile = 0x98,
};
// badge py input badges = ['COAL BADGE', 'FORREST BADGE', 'COBBLE BADGE', 'BEACON BADGE', 'ICICLE BADGE', 'FEN BADGE', 'RELIC BADGE', 'MINE BADGE']
static const charcode_t coal_badge_str[] = { 0x12d, 0x139, 0x12b, 0x136, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t forrest_badge_str[] = { 0x130, 0x139, 0x13c, 0x13c, 0x12f, 0x13d, 0x13e, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t cobble_badge_str[] = { 0x12d, 0x139, 0x12c, 0x12c, 0x136, 0x12f, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t beacon_badge_str[] = { 0x12c, 0x12f, 0x12b, 0x12d, 0x139, 0x138, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t icicle_badge_str[] = { 0x133, 0x12d, 0x133, 0x12d, 0x136, 0x12f, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t fen_badge_str[] = { 0x130, 0x12f, 0x138, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t relic_badge_str[] = { 0x13c, 0x12f, 0x136, 0x133, 0x12d, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };
static const charcode_t mine_badge_str[] = { 0x137, 0x133, 0x138, 0x12f, 0x1de, 0x12c, 0x12b, 0x12e, 0x131, 0x12f, 0xffff };

static const charcode_t *badge_strings[MAX_BADGES] = {
    [BADGE_ID_COAL] = coal_badge_str,
    [BADGE_ID_FOREST] = forrest_badge_str,
    [BADGE_ID_COBBLE] = cobble_badge_str,
    [BADGE_ID_BEACON] = beacon_badge_str,
    [BADGE_ID_ICICLE] = icicle_badge_str,
    [BADGE_ID_FEN] = fen_badge_str,
    [BADGE_ID_RELIC] = relic_badge_str,
    [BADGE_ID_MINE] = mine_badge_str,
};

static MiscSaveBlock *recording;
static FieldSystem *_fieldSystem;
static APBuffer buffer;
static APMemoryConfig *ap_config;
static BOOL locked_input = FALSE;
const APLocData loc_data_registry[MAX_LOC_SIZE] = { { 0 } };

static APCommands get_command_id(int packet);
static void long_to_binary(u32 data, char *out_buffer);
static void ap_player_message_task(SysTask *task, void *data);
static void ap_player_message_send(int other_player_id, Strbuf *itemName, BOOL inbound_item);

void APComm_init(FieldSystem *fieldSystem)
{
    memset(&buffer, 0, sizeof(APBuffer));
    desume_print("APComm_init\n");
    desume_printf("Window sizeof: %u\n", sizeof(APBuffer));
    ap_config = Heap_AllocFromHeapAtEnd(HEAP_ID_FIELDMAP, sizeof(APMemoryConfig));
    MI_CpuFill8(ap_config, 0, sizeof(APMemoryConfig));
    ap_config->window = Window_New(HEAP_ID_FIELDMAP, 1);
    _fieldSystem = fieldSystem;
    recording = SaveData_MiscSaveBlock(_fieldSystem->saveData);
}

void APComm_task()
{
    APComm_receive();
}

void APComm_receive()
{
    // ap_id order pokemon_items, badges, money
    //[0-1] command_id
    int command_id = get_command_id(AP_CMD_IN_ADDR);
    if (command_id != 0 && AP_CMD_IN_ADDR != 0xFFFF) {
        if (locked_input) {
            return;
        }
        int item_id = 0;
        int partition = 0;
        int offset = 0;
        u64 data = 0;
        u32 in_buf = AP_CMD_IN_ADDR;
        AP_CMD_IN_ADDR = 0xFFFFFFFF;
        // 4 bits command
        // 24 bits ap_item_id
        // 4 bits player id
        u32 ap_item_id = in_buf >> 4 & 0xFFFFFF;
        u32 player_id = in_buf >> 28;
        partition = ap_item_id / 32;
        offset = ap_item_id % 32;
        data = MiscSaveBlock_GetItem(recording, partition);
        BOOL item_status = data >> offset & 1;
        data |= 1 << offset;
        desume_printf("APComm_receive: SET_ITEM:game_id %d, ap_item_id %d, partition %d, offset %d, player_id %d, data %d\n", loc_data_registry[ap_item_id].id, ap_item_id, partition, offset, player_id, data);
        if (item_status) {
            AP_CMD_IN_ADDR = 0;
            return;
        }
        int local_id = loc_data_registry[ap_item_id].id;
        if (local_id < MAX_ITEMS) {
            // regular item
            if (Bag_TryAddItem(SaveData_GetBag(_fieldSystem->saveData), local_id, 1, HEAP_ID_FIELD)) {
                desume_printf("sending item %d\n", local_id);
                locked_input = TRUE;
                Strbuf *itemBuf = Strbuf_Init(128, HEAP_ID_FIELDMAP);
                Item_LoadName(itemBuf, local_id, HEAP_ID_FIELDMAP);
                ap_player_message_send(player_id, itemBuf, FALSE);
                Strbuf_Free(itemBuf);
            }

        } else if (local_id < MAX_ITEMS + 8) {
            // badge
            locked_input = TRUE;
            int badge_num = local_id - MAX_ITEMS;
            desume_printf("sending badge %d\n", badge_num);
            Strbuf *badgeBuf = Strbuf_Init(128, HEAP_ID_FIELDMAP);
            Strbuf_CopyChars(badgeBuf, badge_strings[badge_num]);
            ap_player_message_send(player_id, badgeBuf, FALSE);
            TrainerInfo_SetBadge(SaveData_GetTrainerInfo(_fieldSystem->saveData), badge_num);
            Strbuf_Free(badgeBuf);
        } else if (local_id == MAX_ITEMS + 8) {
            // money
            desume_printf("sending money %d\n", ap_item_id);
            TrainerInfo_GiveMoney(SaveData_GetTrainerInfo(_fieldSystem->saveData), ap_item_id);
        }
        MiscSaveBlock_SetItem(recording, partition, data);
    }
}

static void ap_player_message_send(int other_player_id, Strbuf *itemName, BOOL inbound_item)
{
    StringTemplate *strTemplate = StringTemplate_Default(HEAP_ID_FIELDMAP);
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, TEXT_BANK_COMMON_STRINGS, HEAP_ID_FIELDMAP);
    Strbuf *tmpbuf = Strbuf_Init(128, HEAP_ID_FIELDMAP);
    Strbuf *strbuf = MessageLoader_GetNewStrbuf(msgLoader, pl_msg_00000213_00134);
    Strbuf *playerNameBuf = Strbuf_Init(128, HEAP_ID_FIELDMAP);
    u16 *playerName = Heap_AllocFromHeap(HEAP_ID_FIELDMAP, 128);
    ap_player_name_get(other_player_id, playerName);
    Strbuf_CopyChars(playerNameBuf, playerName);
    StringTemplate_SetStrbuf(strTemplate, 0, itemName, 0, TRUE, GAME_LANGUAGE);
    StringTemplate_SetStrbuf(strTemplate, 1, playerNameBuf, 0, TRUE, GAME_LANGUAGE);
    StringTemplate_Format(strTemplate, tmpbuf, strbuf);
    ap_config->printer_id = FieldMessage_Print(ap_config->window, tmpbuf, SaveData_GetOptions(_fieldSystem->saveData), TRUE);
    FieldMessage_AddWindow(_fieldSystem->bgConfig, ap_config->window, 3);
    FieldMessage_DrawWindow(ap_config->window, SaveData_GetOptions(_fieldSystem->saveData));

    MapObjectMan_PauseAllMovement(_fieldSystem->mapObjMan);
    SysTask_Start(ap_player_message_task, playerName, 0);

    Strbuf_Free(strbuf);
    Strbuf_Free(tmpbuf);
    Strbuf_Free(playerNameBuf);
    StringTemplate_Free(strTemplate);
    MessageLoader_Free(msgLoader);
}

static void ap_player_message_task(SysTask *task, void *data)
{
    if (FieldMessage_FinishedPrinting(ap_config->printer_id)) {
        Window_EraseMessageBox(ap_config->window, FALSE);
        Window_ClearAndCopyToVRAM(ap_config->window);
        Window_Remove(ap_config->window);
        Heap_FreeToHeap(data);
        locked_input = FALSE;
        AP_CMD_IN_ADDR = 0x0;
        SysTask_Done(task);
    }
}

BOOL APComm_getItemStatus(u32 ap_item_id)
{
    int partition = ap_item_id / 32;
    int offset = ap_item_id % 32;
    u64 data = MiscSaveBlock_GetItem(recording, partition);
    return (data >> offset) & 1;
}

void APComm_setItemStatus(u32 ap_item_id)
{
    int partition = ap_item_id / 32;
    int offset = ap_item_id % 32;
    u64 data = MiscSaveBlock_GetItem(recording, partition);
    data |= 1 << offset;
    MiscSaveBlock_SetItem(recording, partition, data);
}

void APComm_sendMessage()
{
    AP_CMD_OUT_ADDR = 0xFFFFFFFF;
}

static APCommands get_command_id(int packet)
{
    return packet & 0b111;
}

static void long_to_binary(u32 data, char *out_buffer)
{
    int idx = 0;
    int i = 0;

    while (idx < sizeof(u32) * 8) {
        if ((i + 1) % 5 == 0) {
            out_buffer[i] = ' ';
        } else {
            out_buffer[i] = (data & (1L << idx)) ? '1' : '0';
            idx++;
        }
        i++;
    }
    out_buffer[i] = '\0';
}
