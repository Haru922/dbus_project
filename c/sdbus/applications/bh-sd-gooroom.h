#ifndef __GOOROOM_H__
#define __GOOROOM_H__

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
#include <errno.h>
#include <systemd/sd-bus.h>

#define G_EMU_UDP_ADDRESS "127.0.0.22"
#define G_EMU_UDP_PORT 161
#define G_EMU_NAME "kr.gooroom.bhsdGEmu"
#define G_EMU_ADDRESS "/kr/gooroom/bhsdGEmu"
#define G_EMU_OBJECT_PATH "/kr/gooroom/bhsdGEmuObject"
#define G_EMU_INTERFACE "kr.gooroom.bhsdGEmuInterface"
#define G_EMU_OBJECT_NAME "bhsdGEmuObject"

#define G_HUB_NAME "kr.gooroom.bhsdGHub"
#define G_HUB_ADDRESS "/kr/gooroom/bhsdGHub"
#define G_HUB_OBJECT_PATH "/kr/gooroom/bhsdGHubObject"
#define G_HUB_INTERFACE "kr.gooroom.bhsdGHubInterface"
#define G_HUB_OBJECT_NAME "bhsdGHubObject"

#define G_CONTROLLER_NAME "kr.gooroom.bhsdGController"
#define G_CONTROLLER_ADDRESS "/kr/gooroom/bhsdGController"
#define G_CONTROLLER_OBJECT_PATH "/kr/gooroom/bhsdGControllerObject"
#define G_CONTROLLER_INTERFACE "kr.gooroom.bhsdGControllerInterface"
#define G_CONTROLLER_OBJECT_NAME "bhsdGControllerObject"

#define APP_A_NAME "kr.gooroom.bhsdappA"
#define APP_A_ADDRESS "/kr/gooroom/bhsdappA"
#define APP_A_OBJECT_PATH "/kr/gooroom/bhsdappAObject"
#define APP_A_INTERFACE "kr.gooroom.bhsdappAInterface"
#define APP_A_OBJECT_NAME "bhsdappAObject"

#define LAUNCH_APP_A 0
#define STOP_APP_A 1
#define EXECUTE_APP_A 2

#define OUTPUT_BUF_SIZE 4096

#endif
