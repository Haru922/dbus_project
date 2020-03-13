#include "bh-c-gooroom.h"

static void
forward_launch_signal (DBusMessage *message,
                       DBusConnection *connection)
{
    DBusMessage *method_call;
    DBusMessage *reply;
    DBusMessage *returned;
    DBusError error;
    char *result;

    dbus_error_init (&error);
    method_call = dbus_message_new_method_call (G_CONTROLLER_NAME, G_CONTROLLER_OBJECT_PATH, 
                                                G_CONTROLLER_INTERFACE, "launch_app_a");
    returned = dbus_connection_send_with_reply_and_block (connection, method_call, -1, &error);
    dbus_message_get_args (returned, &error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    reply = dbus_message_new_method_return (message);
    dbus_message_append_args (reply, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    dbus_connection_send (connection, reply, NULL);
    dbus_connection_flush (connection);
    dbus_message_unref (reply);
    dbus_error_free (&error);
}

static void
forward_stop_signal (DBusMessage *message,
                     DBusConnection *connection)
{
    DBusMessage *method_call;
    DBusMessage *reply;
    DBusMessage *returned;
    DBusError error;
    char *result;

    dbus_error_init (&error);
    method_call = dbus_message_new_method_call (G_CONTROLLER_NAME, G_CONTROLLER_OBJECT_PATH,
                                                G_CONTROLLER_INTERFACE, "stop_app_a");
    returned = dbus_connection_send_with_reply_and_block (connection, method_call, -1, &error);
    dbus_message_get_args (returned, &error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    reply = dbus_message_new_method_return (message);
    dbus_message_append_args (reply, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    dbus_connection_send (connection, reply, NULL);
    dbus_connection_flush (connection);
    dbus_message_unref (reply);
    dbus_error_free (&error);
}

static void
execute_app_a (DBusMessage *message,
               DBusConnection *connection)
{
    DBusMessage *method_call;
    DBusMessage *reply;
    DBusMessage *returned;
    DBusError error;
    char *result;

    dbus_error_init (&error);
    method_call = dbus_message_new_method_call (APP_A_NAME, APP_A_OBJECT_PATH,
                                                APP_A_INTERFACE, "print_log");
    returned = dbus_connection_send_with_reply_and_block (connection, method_call, -1, &error);
    dbus_message_get_args (returned, &error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    reply = dbus_message_new_method_return (message);
    dbus_message_append_args (reply, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

    dbus_connection_send (connection, reply, NULL);
    dbus_connection_flush (connection);
    dbus_message_unref (reply);
    dbus_error_free (&error);
}

int
main (int argc, char *argv[])
{
    DBusConnection *connection;
    DBusError error;
    DBusMessage *message;

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
    dbus_bus_request_name (connection, G_HUB_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    while (true) {
        dbus_connection_read_write (connection, 0);
        message = dbus_connection_pop_message (connection);

        if (NULL == message) {
            sleep (1);
            continue;
        }

        if (dbus_message_is_method_call (message, G_HUB_INTERFACE, "forward_launch_signal"))
            forward_launch_signal (message, connection);
        else if (dbus_message_is_method_call (message, G_HUB_INTERFACE, "forward_stop_signal"))
            forward_stop_signal (message, connection);
        else if (dbus_message_is_method_call (message, G_HUB_INTERFACE, "execute_app_a"))
            execute_app_a (message, connection);

        dbus_message_unref (message);
    }
    dbus_connection_close (connection);
    dbus_error_free (&error);
    exit (EXIT_SUCCESS);
}
