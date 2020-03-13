#include "bh-c-gooroom.h"

int
main (int argc, char *argv[])
{
    char *command[3] = { "forward_launch_signal",
                         "forward_stop_signal",
                         "execute_app_a" };
    DBusConnection *connection;
    DBusError error;
    DBusMessage *method_call;
    DBusMessage *returned;
    int socket_fd;
    char buf[OUTPUT_BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    int n, len;
    char *result;
    int data;

    if ((socket_fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf (stderr, "socket creation failed");
        exit (EXIT_FAILURE);
    }

    memset (&server_addr, 0, sizeof (server_addr));
    memset (&client_addr, 0, sizeof (client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons (G_EMU_UDP_PORT);

    if (bind (socket_fd, (const struct sockaddr *) &server_addr, sizeof (server_addr)) < 0) {
        fprintf (stderr, "bind failed");
        exit (EXIT_FAILURE);
    }

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SYSTEM, &error);
    dbus_bus_request_name (connection, G_EMU_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    len = sizeof (client_addr);

    while (true) {
        n = recvfrom (socket_fd, (char *) buf, OUTPUT_BUF_SIZE, 
                      MSG_WAITALL, (struct sockaddr *) &client_addr, &len);
        buf[n] = '\0';
        data = atoi(buf) - 1;
        if (0 <= data && data < 4) {
            method_call = dbus_message_new_method_call (G_HUB_NAME, G_HUB_OBJECT_PATH,
                                                        G_HUB_INTERFACE, command[data]);
            returned = dbus_connection_send_with_reply_and_block (connection, method_call, -1, &error);
            dbus_message_get_args (returned, &error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID);

            sendto (socket_fd, (const char *) result, strlen (result), MSG_CONFIRM,
                    (const struct sockaddr *) &client_addr, len);

        }
    }
    dbus_error_free (&error);
    dbus_connection_close (connection);
    dbus_error_free (&error);
    free (result);
    exit (EXIT_SUCCESS);
}
