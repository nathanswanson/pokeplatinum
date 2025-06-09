#include "ap/ap_debug_print.h"

#define DESUME_DEBUG
void desume_print(const char *msg)
{
#ifdef DESUME_DEBUG
    __asm__ volatile(
        "mov r0, %0\n"
        "swi 0xFC\n"
        : : "r"(msg) : "r0");
#endif
}

void desume_printf(const char *msg, ...)
{
#ifdef DESUME_DEBUG
    va_list args;
    char out_message[256];
    va_start(args, msg);
    vsprintf(out_message, msg, args);
    va_end(args);

    desume_print(out_message);
#endif
}
