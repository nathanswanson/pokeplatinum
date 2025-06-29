//
// Created by wsl on 5/30/25.
//

#ifndef AP_PLAYER_NAMES_H
#define AP_PLAYER_NAMES_H

#include "charcode.h"

#define AP_PLAYER_NAMES_BUFFER_SIZE 2000 // bytes
#define AP_PLAYER_NAMES_MAX_PLAYERS 32

#define AP_ITEM_NAMES_BUFFER_SIZE 16000 // bytes
#define AP_ITEM_NAMES_MAX_ITEMS   2000

extern const u16 ap_player_names[AP_PLAYER_NAMES_BUFFER_SIZE];
extern const int ap_player_offset_table[AP_PLAYER_NAMES_MAX_PLAYERS];

extern const u16 ap_item_names[AP_ITEM_NAMES_BUFFER_SIZE];
extern const int ap_item_offset_table[AP_ITEM_NAMES_MAX_ITEMS];

void ap_player_name_get(int player_id, charcode_t *name);
void ap_item_name_get(int item_id, charcode_t *name);
#endif // AP_PLAYER_NAMES_H
