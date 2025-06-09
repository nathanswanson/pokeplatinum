//
// Created by wsl on 5/30/25.
//

#include "ap/ap_player_names.h"

#include "constants/charcode.h"

#include "ap/ap_debug_print.h"

const u16 ap_player_names[AP_PLAYER_NAMES_BUFFER_SIZE] = { 0 };
const int ap_player_offset_table[AP_PLAYER_NAMES_MAX_PLAYERS] = { 0 };

void ap_player_name_get(int player_id, u16 *name)
{
    u16 *start = ap_player_names + ap_player_offset_table[player_id];
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
