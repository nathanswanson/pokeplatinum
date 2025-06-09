//
// Created by wsl on 5/30/25.
//

#ifndef AP_PLAYER_NAMES_H
#define AP_PLAYER_NAMES_H

#include "charcode.h"

#define AP_PLAYER_NAMES_BUFFER_SIZE 2000 //bytes
#define AP_PLAYER_NAMES_MAX_PLAYERS 32


extern const u16 ap_player_names[AP_PLAYER_NAMES_BUFFER_SIZE];
extern const int ap_player_offset_table[AP_PLAYER_NAMES_MAX_PLAYERS];

void ap_player_name_get(int player_id, charcode_t *name);
#endif //AP_PLAYER_NAMES_H
