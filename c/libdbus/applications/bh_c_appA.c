#include "bh-c-gooroom.h"

static void
print_log (DBusMessage *message,
           DBusConnection *connection)
{
    DBusMessage *reply;
    DBusError error;
    char *command = "/bin/journalctl -n 10";
    char *failure = "APP_A: EXECUTE [FAILURE].\n";
    FILE *file;
    char *buf = (char *) malloc (sizeof (char) * OUTPUT_BUF_SIZE);
    int i;
    char ch;

    dbus_error_init (&error);

    i = 0;
    file = popen (command, "r");
    while (((ch = getc (file)) != EOF) && i < (OUTPUT_BUF_SIZE - 1)) {
        buf[i++] = ch;
    }
    buf[i] = '\0';
    reply = dbus_message_new_method_return (message);
    dbus_message_append_args (reply, DBUS_TYPE_STRING, &buf, DBUS_TYPE_INVALID);
    dbus_connection_send (connection, reply, NULL);
    dbus_connection_flush (connection);
    dbus_message_unref (reply);
    free (buf);
    fclose (file);
}

int
main (int argc, char *argv[])
{
    DBusConnection *connection;
    DBusError error;
    DBusMessage *message;

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
    dbus_bus_request_name (connection, APP_A_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    while (true) {
        dbus_connection_read_write (connection, 0);
        message = dbus_connection_pop_message (connection);

        if (NULL == message) {
            sleep (1);
            continue;
        }

        if (dbus_message_is_method_call(message, APP_A_INTERFACE, "print_log"))
            print_log (message, connection);

        dbus_message_unref (message);
    }
    dbus_connection_close (connection);
    dbus_error_free (&error);
    exit (EXIT_SUCCESS);
}
