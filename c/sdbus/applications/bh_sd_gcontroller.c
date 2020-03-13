#include "bh-sd-gooroom.h"

static int log_error(int error, const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(-error));
    return error;
}

static int on_and_off_app_a(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int64_t signal;
    int r;
    pid_t pid;
    int status;
    int controller_status = 1;
    char *command[][10] = {{ "systemctl", "start", "bh_sd_appA", NULL},
                         { "systemctl", "stop", "bh_sd_appA", NULL }};
    char *result[2] = { "GCONTROLLER: [SUCCESS].\n",
                        "GCONTROLLER: [FAILED].\n"};

    r = sd_bus_message_read(m, "x", &signal);
    if (r < 0)
        return log_error(r, "Failed to parse parameters");

    pid = fork();
    if (pid == 0) {
        execv("/bin/systemctl", command[(int)signal]);
        exit(EXIT_SUCCESS);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status))
            controller_status = 0;
        else
            controller_status = 1;
    }
    
    return sd_bus_reply_method_return(m, "s", result[controller_status]);
}

static const sd_bus_vtable gcontroller_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("ON_OFF", "x", "s", on_and_off_app_a, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    int r;

    r = sd_bus_default_system(&bus);
    if (r < 0) {
        log_error(r, "Failed to connect to system bus");
        goto finish;
    }

    r = sd_bus_add_object_vtable(bus,
                                 NULL,
                                 G_CONTROLLER_OBJECT_PATH,
                                 G_CONTROLLER_INTERFACE,
                                 gcontroller_vtable,
                                 NULL);

    r = sd_bus_request_name(bus, G_CONTROLLER_NAME, 0);
    if (r < 0) {
        log_error(r, "Failed to acquire service name");
        goto finish;
    }

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
    sd_bus_unref(bus);

    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
