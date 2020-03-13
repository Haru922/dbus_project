#include "bh-sd-gooroom.h"

static int log_error(int error, const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(-error));
    return error;
}

static int forward_to_gcontroller(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    sd_bus *bus = NULL;
    sd_bus_message *reply;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int64_t signal;
    int r;
    char *result;

    r = sd_bus_message_read(m, "x", &signal);
    if (r < 0)
        return log_error(r, "Failed to read message");

    r = sd_bus_default_system(&bus);
    if (r < 0)
        return log_error(r, "Failed to connect system bus");

    r = sd_bus_call_method(bus,
                           G_CONTROLLER_NAME,
                           G_CONTROLLER_OBJECT_PATH,
                           G_CONTROLLER_INTERFACE,
                           "ON_OFF",
                           &error,
                           &reply,
                           "x",
                           signal);
    if (r < 0)
        return log_error(r, "Call failed");

    r = sd_bus_message_read(reply, "s", &result);
    if (r < 0)
        return log_error(r, "Failed to parse parameters");

    sd_bus_message_unref(reply);
    sd_bus_error_free(&error);
    sd_bus_unref(bus);
    return sd_bus_reply_method_return(m, "s", result);
}

static int execute_app_a(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    sd_bus *bus = NULL;
    sd_bus_message *reply;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int r;
    char *result;
    int ret;

    r = sd_bus_default_system(&bus);
    if (r < 0)
        return log_error(r, "Failed to connect system bus");

    r = sd_bus_call_method(bus,
                           APP_A_NAME,
                           APP_A_OBJECT_PATH,
                           APP_A_INTERFACE,
                           "PRINT_LOG",
                           &error,
                           &reply,
                           "");
    if (r < 0)
        return log_error(r, "Call failed");

    r = sd_bus_message_read(reply, "s", &result);
    if (r < 0)
        return log_error(r, "Failed to parse parameters");

    ret = sd_bus_reply_method_return(m, "s", result);
    sd_bus_message_unref(reply);
    sd_bus_error_free(&error);
    return ret;
}

static const sd_bus_vtable ghub_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("FORWARD", "x", "s", forward_to_gcontroller, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("EXECUTE", "", "s", execute_app_a, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    int r;
    int64_t signal;

    r = sd_bus_default_system(&bus);
    if (r < 0) {
        fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
        goto finish;
    }

    r = sd_bus_request_name(bus, G_HUB_NAME, 0);
    if (r < 0) {
        log_error(r, "Failed to acquire service name");
        goto finish;
    }

    r = sd_bus_add_object_vtable(bus,
                                 NULL,
                                 G_HUB_OBJECT_PATH,
                                 G_HUB_INTERFACE,
                                 ghub_vtable,
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
            log_error(r, "Faild to wait on bus");
            goto finish;
        }
    }

finish:
    sd_bus_message_unref(m);
    sd_bus_unref(bus);

    return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
