//
// Created by wsl on 5/30/25.
//

#include "ap/ap_player_names.h"

#include "constants/charcode.h"

#include "ap/ap_debug_print.h"

static void get_string_from_pack(int player_id, u16 *name, const u16 *names, const int *offset_table);

const u16 ap_player_names[AP_PLAYER_NAMES_BUFFER_SIZE] = { 0 };
const int ap_player_offset_table[AP_PLAYER_NAMES_MAX_PLAYERS] = { 0 };

const u16 ap_item_names[AP_ITEM_NAMES_BUFFER_SIZE] = { 0 };
const int ap_item_offset_table[AP_ITEM_NAMES_MAX_ITEMS] = { 0 };

static void get_string_from_pack(int player_id, u16 *name, const u16 *names, const int *offset_table)
{
    u16 *start = (u16 *)(names + offset_table[player_id]);
    int i = 0;
    while (*start != 0x0000) {
        name[i] = *start;
        start++;
        i++;
        if (i > 128) {
            break;
        }
    }
    name[i] = CHAR_EOS;
}

void ap_player_name_get(int player_id, u16 *name)
{
    get_string_from_pack(player_id, name, ap_player_names, ap_player_offset_table);
}

void ap_item_name_get(int item_id, u16 *name)
{
    get_string_from_pack(item_id, name, ap_item_names, ap_item_offset_table);
}
