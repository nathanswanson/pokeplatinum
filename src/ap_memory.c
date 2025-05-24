#include "ap_memory.h"
#include "archipelago_recording.h"
//A69A89
#include "field/field_system.h"
#include "constants/items.h"
#include "savedata/save_table.h"
#include "savedata.h"
#include "savedata_misc.h"
#include "bag.h"
#include "heap.h"
#include "string_template.h"
#include "script_manager.h"
#include "message.h"
#include "bg_window.h"
#include "game_options.h"
#include "strbuf.h"
#include "debug_print.h"
const APLocData loc_data_registry[MAX_LOC_SIZE] = { {0} };

typedef struct APBuffer {
    u16 read_idx;
    u16 write_idx;
    u16 buf[5];
} APBuffer;

static MiscSaveBlock *recording;
static FieldSystem *_fieldSystem;
static APBuffer buffer;

static APCommands get_command_id(int packet);
static void long_to_binary(u32 data, char* out_buffer);

void APComm_init(FieldSystem *fieldSystem)
{
    //memset(&buffer, 0, sizeof(APBuffer));
    //desume_print("APComm_init\n");
    //recording = SaveData_MiscSaveBlock(_fieldSystem->saveData);
}

void APComm_task()
{
    //APComm_receive();
    //APComm_send();
}

void APComm_receive()
{
    //[0-1] command_id
    int command_id = get_command_id(AP_CMD_IN_ADDR);
    if(command_id != 0)
    {
        desume_printf("APComm_receive: command_id %d\n", command_id);
        switch(command_id)
        {
            int item_id = 0;
            int partition = 0;
            int offset = 0;
            u64 data = 0;
            case AP_CMD_GET_ITEM_STATUS:
                desume_printf("APComm_receive: GET_ITEM\n");
                // server wants item status
                //[2-15] item_id
                item_id = AP_CMD_IN_ADDR >> 4;
                partition = item_id / 32;
                offset = item_id % 32;
                data = MiscSaveBlock_GetItem(recording, partition);
                int item_value = data >> offset & 1;
                AP_CMD_OUT_ADDR = item_value;
                desume_printf("APComm_receive: GET_ITEM: item_id %d, partition %d, offset %d, data %d\n", item_id, partition, offset, data);
                AP_CMD_IN_ADDR = 0;
                break;
            case AP_CMD_DEBUG_ITEMS:
                // print all items (long[8]) into binary
                // ######## ######## ######## ######## ######## ######## ######## ########
                char buf[256];
                for (int i = 0; i < 16; i++) {
                    data = MiscSaveBlock_GetItem(recording, i);
                    long_to_binary(data, buf);
                    desume_print(buf);
                    desume_print("\n");

                }
                // command processed
                AP_CMD_IN_ADDR = 0;
                break;
            case AP_CMD_GIVE_ITEM:
                desume_printf("APComm_receive: SET_ITEM\n");
                desume_printf("AP_CMD_IN_ADDR: %u\n", AP_CMD_IN_ADDR);
                //4 bits command
                //12 bits ap_item_id
                //12 bits game_item_id
                //4 bits player id
                u32 ap_item_id = AP_CMD_IN_ADDR >> 4 & 0xFFF;
                u32 game_item_id = AP_CMD_IN_ADDR >> 16 & 0xFFF;
                u32 player_id = AP_CMD_IN_ADDR >> 28;
                partition = ap_item_id / 32;
                offset = ap_item_id % 32;
                data = MiscSaveBlock_GetItem(recording, partition);
                data |= 1 << offset;

                desume_printf("APComm_receive: SET_ITEM: game_item_id %d, ap_item_id %d, partition %d, offset %d, player_id %d, data %d\n", game_item_id, ap_item_id, partition, offset, player_id, data);

                Bag_TryAddItem(SaveData_GetBag(_fieldSystem->saveData), item_id, 1, HEAP_ID_FIELD);
                MiscSaveBlock_SetItem(recording, partition, data);
                AP_CMD_IN_ADDR = 0;
                break;
        }
    }


}

void APComm_push_item_to_buf(u32 item_id)
{
    if(buffer.buf[buffer.write_idx] != 0)
    {
        desume_printf("APComm_push_item_to_buf: buffer full\n");
    }
    buffer.buf[buffer.write_idx] = item_id;
    buffer.write_idx++;
}

void APComm_send()
{
    if(buffer.buf[buffer.read_idx] != 0)
    {
        if(AP_CMD_IN_ADDR == 0)
        {
            AP_CMD_IN_ADDR = 0xFFFFFFFF;
            AP_CMD_OUT_ADDR = buffer.buf[buffer.read_idx];
            buffer.buf[buffer.read_idx] = 0;
            buffer.read_idx++;
        }
    }
}


static APCommands get_command_id(int packet)
{
    return  packet & 0b111;
}


static void long_to_binary(u32 data, char* out_buffer)
{
    int idx = 0;
    int i = 0;

    while(idx < sizeof(u32) * 8 )
    {
        if((i+1) % 5 == 0)
        {
            out_buffer[i] = ' ';
        }
        else
        {
            out_buffer[i] = (data & (1L << idx)) ? '1' : '0';
            idx++;

        }
        i++;
    }
    out_buffer[i] = '\0';
}