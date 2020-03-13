#include "bh-sd-gooroom.h"

static int log_error(int error, const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(-error));
    return error;
}

static int print_log(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int r;
    char *command = "/bin/journalctl -n 10";
    FILE *file;
    char *buf = (char*)malloc(sizeof(char)*OUTPUT_BUF_SIZE);
    int i;
    char ch;
    int ret;

    i = 0;
    file = popen(command, "r");
    while (((ch = getc(file)) != EOF) && i < (OUTPUT_BUF_SIZE-1)) 
        buf[i++] = ch;
    buf[i] = '\0';

    pclose(file);
    ret = sd_bus_reply_method_return(m, "s", buf);
    free(buf);
    return ret;
}

static const sd_bus_vtable appa_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("PRINT_LOG", "", "s", print_log, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    int r;

    r = sd_bus_open_system(&bus);
    if (r < 0) {
        log_error(r, "Failed to connect to system bus");
        goto finish;
    }

    r = sd_bus_request_name(bus, APP_A_NAME, 0);
    if (r < 0) {
        log_error(r, "Failed to acquire service name");
        goto finish;
    }

    r = sd_bus_add_object_vtable(bus,
                                 NULL,
                                 APP_A_OBJECT_PATH,
                                 APP_A_INTERFACE,
                                 appa_vtable,
                                 NULL);

    for (;;) {
        r = sd_bus_process(bus, NULL);
        if (r < 0) {
            log_error(r, "Failed to process bus");
            goto finish;
        }

        if (r > 0)
            continue;

        r = sd_bus_wait(bus, (uint64_t) -1);
        if (r < 0) {
            log_error(r, "Failed to wait on bus");
            goto finish;
        }
    }

finish:
    sd_bus_message_unref(m);
    sd_bus_unref(bus);

    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
