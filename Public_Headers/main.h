#ifndef MAIN_H
#define MAIN_H

#include "SAM3U2C.h"

void User_Initialization(void);
void User_Application(void);

static void SystemSleep(void);


uint32_t G_u32SystemTick = 0u;

#endif