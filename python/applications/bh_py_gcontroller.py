import subprocess
import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib

class GControllerObject(dbus.service.Object):
    def __init__(self, connected_bus, object_path):
        dbus.service.Object.__init__(self, connected_bus, object_path)

    @dbus.service.method('kr.gooroom.bhpyGControllerInterface', in_signature='s', out_signature='s')
    def turn_on_off_app_a(self, signal):
        call_status = -1
        command = []
        if signal == dbus.String('1'):
            command = ['start', 'START_APP_A']
        elif signal == dbus.String('2'):
            command = ['stop', 'STOP_APP_A']
        if len(command):
            call_status = subprocess.call('systemctl ' + command[0] + ' bh_py_appA', shell=True)
            status_return =  'GController: ' + command[1] + ('[SUCCESS]' if call_status is 0 else '[FAILURE]')
        else:
            status_return = 'GCONTROLLER: INAPPROPRIATE COMMAND\n'
        return status_return

    @dbus.service.method('kr.gooroom.bhpyGControllerInterface', in_signature='', out_signature='s')
    def quit(self):
        gcontroller_loop.quit()
        return 'Quit GController Service'

if __name__ == '__main__':
    DBusGMainLoop(set_as_default=True)
    gcontroller_bus = dbus.SystemBus()
    gcontroller_bus_name = dbus.service.BusName('kr.gooroom.bhpyGController', bus=gcontroller_bus)
    gcontroller_object = GControllerObject(gcontroller_bus, '/kr/gooroom/bhpyGControllerObject')

    gcontroller_loop = GLib.MainLoop()
    gcontroller_loop.run()
