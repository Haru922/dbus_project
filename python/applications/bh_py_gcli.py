import socket
import subprocess

GEMU_ADDR = '127.0.0.22'
GEMU_PORT = 161
BUF_SIZE = 4096

gcli_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
gcli_socket.settimeout(1)

gcli_state = True
while gcli_state:
    print()
    print('########################')
    print('#         GCli         #')
    print('#======================#')
    print('#   1: Launch AppA     #')
    print('#   2: Stop AppA       #')
    print('#   3: Print Log       #')
    print('#   4: Quit GCli       #')
    print('#======================#')
    print('#   5: Launch Services #')
    print('#   6: Print Status    #')
    print('#   7: Kill Services   #')
    print('########################\n')
    print('Command: ',end='')
    data = input()
    print()
    if data in ['1','2','3']:
        try:
            gcli_socket.sendto(data.encode(), (GEMU_ADDR,GEMU_PORT))
            data, server_addr = gcli_socket.recvfrom(BUF_SIZE)
            print(data.decode('utf-8'))
        except socket.timeout:
            print("Timeout.\n");
    elif data is '4':
        gcli_state = False
    elif data is '5':
        subprocess.call('/home/haru/dbus_project/python/scripts/bh_python_launch_services.sh', shell=True)
    elif data is '6':
        subprocess.call('/home/haru/dbus_project/python/scripts/bh_python_status_services.sh', shell=True)
    elif data is '7':
        subprocess.call('/home/haru/dbus_project/python/scripts/bh_python_kill_services.sh', shell=True)
gcli_socket.close()
