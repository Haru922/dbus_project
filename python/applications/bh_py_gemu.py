import socket
import dbus
import dbus.service
import subprocess

GEMU_ADDR = '127.0.0.22'
GEMU_PORT = 161
BUF_SIZE = 4096

if __name__ == '__main__':
    gemu_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    gemu_socket.bind((GEMU_ADDR, GEMU_PORT))

    gemu_bus = dbus.SystemBus()
    gemu_bus_name = dbus.service.BusName('kr.gooroom.bhpyGEmu', bus=gemu_bus)

    ghub_object = gemu_bus.get_object('kr.gooroom.bhpyGHub', '/kr/gooroom/bhpyGHubObject')
    ghub_interface = dbus.Interface(ghub_object, 'kr.gooroom.bhpyGHubInterface')

    gemu_state = True
    while gemu_state:
        call_status = -1
        replied_message = None
        data, client_addr = gemu_socket.recvfrom(BUF_SIZE)
        if data.decode() in ['1','2']:
            replied_message = ghub_interface.forward_to_gcontroller(data.decode())
        elif data.decode() is '3':
            call_status = subprocess.call('systemctl status bh_py_appA', shell=True)
            if call_status is 0:
                replied_message = ghub_interface.execute_app_a()
            else:
                replied_message = 'AppA is not running.\n'
        else:
            replied_message = None
        if replied_message is not None:
            gemu_socket.sendto(replied_message.encode(), (client_addr[0], client_addr[1]))
