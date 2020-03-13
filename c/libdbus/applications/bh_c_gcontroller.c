#include "bh-c-gooroom.h"

static void
launch_app_a (DBusMessage *message,
              DBusConnection *connection)
{
    DBusMessage *reply;
    pid_t pid;
    int status;
    char *command[] = { "systemctl", "start", "bh_c_appA", NULL };
    char *result[] = { "GCONTROLLER: LAUNCH_APP_A [SUCCESS].\n",
                       "GCONTROLLER: LAUNCH_APP_A [FAILURE].\n" };

    pid = fork ();
    if (pid == 0) {
        execv ("/bin/systemctl", command);
        exit (EXIT_SUCCESS);
    } else {
        waitpid (pid, &status, 0);
        reply = dbus_message_new_method_return (message);

        if (WIFEXITED (status) && !WEXITSTATUS (status))
            dbus_message_append_args (reply, DBUS_TYPE_STRING, &result[0], DBUS_TYPE_INVALID);
        else
            dbus_message_append_args (reply, DBUS_TYPE_STRING, &result[1], DBUS_TYPE_INVALID);

        dbus_connection_send (connection, reply, NULL);
        dbus_connection_flush (connection);
        dbus_message_unref (reply);
    }
}

static void
stop_app_a (DBusMessage *message,
            DBusConnection *connection)
{
    DBusMessage *reply;
    pid_t pid;
    int status;
    char *command[] = { "systemctl", "stop", "bh_c_appA", NULL };
    char *result[] = { "GCONTROLLER: STOP_APP_A [SUCCESS].\n",
                       "GCONTROLLER: STOP_APP_A [FAILURE].\n" };

    pid = fork ();
    if (pid == 0) {
        execv ("/bin/systemctl", command);
        exit (EXIT_SUCCESS);
    } else {
        waitpid (pid, &status, 0);
        reply = dbus_message_new_method_return (message);

        if (WIFEXITED (status) && !WEXITSTATUS (status))
            dbus_message_append_args (reply, DBUS_TYPE_STRING, &result[0], DBUS_TYPE_INVALID);
        else
            dbus_message_append_args (reply, DBUS_TYPE_STRING, &result[1], DBUS_TYPE_INVALID);

        dbus_connection_send (connection, reply, NULL);
        dbus_connection_flush (connection);
        dbus_message_unref (reply);
    }
}

int
main (int argc, char *argv[])
{
    DBusConnection *connection;
    DBusError error;
    DBusMessage *message;

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
    dbus_bus_request_name (connection, G_CONTROLLER_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    while (true) {
        dbus_connection_read_write (connection, 0);
        message = dbus_connection_pop_message (connection);

        if (NULL == message) {
            sleep (1);
            continue;
        }

        if (dbus_message_is_method_call (message, G_CONTROLLER_INTERFACE, "launch_app_a"))
            launch_app_a (message, connection);
        else if (dbus_message_is_method_call (message, G_CONTROLLER_INTERFACE, "stop_app_a"))
            stop_app_a (message, connection);

        dbus_message_unref (message);
    }
    dbus_connection_close (connection);
    dbus_error_free (&error);
    exit (EXIT_SUCCESS);
}

