#include "archipelago_recording.h"


int ArchipelagoRecording_SaveSize()
{
    return sizeof(ArchipelagoRecording);
}

void ArchipelagoRecording_Init(ArchipelagoRecording *recording) {
    MI_CpuFill32(recording, 0, ArchipelagoRecording_SaveSize());
    for (int i = 0; i < 8; i++) {
        recording->ap_locations[i] = 0;
    }
}

long *ArchipelagoRecording_GetLocation(ArchipelagoRecording *recording, int partition) {
    return &recording->ap_locations[partition];
}

