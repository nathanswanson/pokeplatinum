
#ifndef AP_MEMORY_H
#define AP_MEMORY_H

#include "field/field_system_decl.h"
#define AP_CMD_IN_ADDR  (*(volatile unsigned int *)(0x2263290))
#define AP_CMD_OUT_ADDR (*(volatile unsigned int *)(0x2263298))
#define MAX_LOC_SIZE    1408

typedef struct {
    unsigned short is_local : 1;
    // if !is_local it is the ap location id
    // if is_local && item_id < max_items (additem item_id)
    // if is_local && item_id > max_items it is a special location (badge, key_item, berry...)
    unsigned short id : 12;
    unsigned short count : 3;
} APLocData;

typedef enum {
    AP_CMD_GIVE_ITEM = 1,
    AP_CMD_GET_ITEM_STATUS = 2,
    AP_CMD_DEBUG_ITEMS = 3,
} APCommands;

extern const APLocData loc_data_registry[MAX_LOC_SIZE];

void APComm_init(FieldSystem *fieldSystem);
void APComm_task();
void APComm_receive();
void APComm_send();
BOOL APComm_getItemStatus(u32 ap_item_id);
void APComm_setItemStatus(u32 ap_item_id);
void APComm_sendMessage();

#endif // AP_MEMORY_H
