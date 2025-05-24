//
// Created by wsl on 5/10/25.
//

#ifndef ARCHIPELAGORECORDING_H
#define ARCHIPELAGORECORDING_H
#include "savedata.h"


typedef struct ArchipelagoRecording {
    // lets assume 2000 locations/events for now, can be extended
    long ap_locations[8];
} ArchipelagoRecording;

int ArchipelagoRecording_SaveSize();
void ArchipelagoRecording_Init(ArchipelagoRecording *recording);
long *ArchipelagoRecording_GetLocation(ArchipelagoRecording *recording, int partition);

#endif //ARCHIPELAGORECORDING_H
