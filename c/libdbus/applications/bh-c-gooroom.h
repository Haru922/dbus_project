#ifndef __GOOROOM_H__
#define __GOOROOM_H__

#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define G_EMU_UDP_ADDRESS "127.0.0.22"
#define G_EMU_UDP_PORT 161
#define G_EMU_NAME "kr.gooroom.bhcGEmu"
#define G_EMU_ADDRESS "/kr/gooroom/bhcGEmu"
#define G_EMU_OBJECT_PATH "/kr/gooroom/bhcGEmuObject"
#define G_EMU_INTERFACE "kr.gooroom.bhcGEmuInterface"
#define G_EMU_OBJECT_NAME "bhcGEmuObject"

#define G_HUB_NAME "kr.gooroom.bhcGHub"
#define G_HUB_ADDRESS "/kr/gooroom/bhcGHub"
#define G_HUB_OBJECT_PATH "/kr/gooroom/bhcGHubObject"
#define G_HUB_INTERFACE "kr.gooroom.bhcGHubInterface"
#define G_HUB_OBJECT_NAME "bhcGHubObject"

#define G_CONTROLLER_NAME "kr.gooroom.bhcGController"
#define G_CONTROLLER_ADDRESS "/kr/gooroom/bhcGController"
#define G_CONTROLLER_OBJECT_PATH "/kr/gooroom/bhcGControllerObject"
#define G_CONTROLLER_INTERFACE "kr.gooroom.bhcGControllerInterface"
#define G_CONTROLLER_OBJECT_NAME "bhcGControllerObject"

#define APP_A_NAME "kr.gooroom.bhcappA"
#define APP_A_ADDRESS "/kr/gooroom/bhcappA"
#define APP_A_OBJECT_PATH "/kr/gooroom/bhcappAObject"
#define APP_A_INTERFACE "kr.goorooom.bhcappAInterface"
#define APP_A_OBJECT_NAME "bhcappAObject"

#define LAUNCH_APP_A 1
#define STOP_APP_A 2
#define EXECUTE_APP_A 3

#define OUTPUT_BUF_SIZE 4096

#endif
