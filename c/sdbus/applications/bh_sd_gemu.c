#include "bh-sd-gooroom.h"

static int log_error(int error, const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(-error));
    return error;
}

int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *method_call = NULL;
    sd_bus_message *reply = NULL;
    sd_bus *bus = NULL;
    int r;
    int socket_fd;
    char buf[OUTPUT_BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    int n, len;
    int signal;
    char *G_HUB_METHOD;
    char *result;

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(G_EMU_UDP_PORT);

    if (bind(socket_fd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(client_addr);
    
    r = sd_bus_default_system(&bus);
    if (r < 0) {
        log_error(r, "Failed to connect to system bus");
        goto finish;
    }

    r = sd_bus_request_name(bus, G_EMU_NAME, 0);
    if (r < 0) {
        log_error(r, "Failed to acquire service name");
        goto finish;
    }

    for (;;) {
        n = recvfrom(socket_fd,
                     (char*)buf,
                     OUTPUT_BUF_SIZE,
                     MSG_WAITALL,
                     (struct sockaddr*)&client_addr,
                     &len);
        buf[n] = '\0';
        signal = atoi(buf) - 1;
        if (LAUNCH_APP_A == signal || STOP_APP_A == signal || EXECUTE_APP_A == signal) {
            if (signal == LAUNCH_APP_A || signal == STOP_APP_A)
                G_HUB_METHOD = "FORWARD";
            else
                G_HUB_METHOD = "EXECUTE";

            r = sd_bus_message_new_method_call(bus,
                                               &method_call,
                                               G_HUB_NAME,
                                               G_HUB_OBJECT_PATH,
                                               G_HUB_INTERFACE,
                                               G_HUB_METHOD);
            if (r < 0) {
                log_error(r, "Failed to create bus message");
                goto finish;
            }
            
            if (signal == LAUNCH_APP_A || signal == STOP_APP_A) {
                sd_bus_message_append(method_call, "x", (int64_t)signal);
                if (r < 0) {
                    log_error(r, "Failed to append to bus message");
                    goto finish;
                }
            }

            r = sd_bus_call(bus, method_call, -1, &error, &reply);
            if (r < 0) {
                log_error(r, "Call failed");
                goto finish;
            }

            r = sd_bus_message_read(reply, "s", &result);
            if (r < 0) {
                log_error(r, "Failed to parse response message");
                goto finish;
            }

            sendto(socket_fd,
                   (const char*)result,
                   strlen(result),
                   MSG_CONFIRM,
                   (const struct sockaddr*)&client_addr,
                   len);
        }
    }

finish:
    sd_bus_error_free(&error);
    sd_bus_message_unref(method_call);
    sd_bus_message_unref(reply);
    sd_bus_unref(bus);

    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
