import subprocess
import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib

class AppAObject(dbus.service.Object):
    def __init__(self, connected_bus, object_path):
        dbus.service.Object.__init__(self, connected_bus, object_path)

    @dbus.service.method('kr.gooroom.bhpyappAInterface', in_signature='', out_signature='s')
    def print_log(self):
        output = subprocess.check_output(['sudo', 'journalctl', '-n', '10'])
        return str(output)

    @dbus.service.method('kr.gooroom.bhpyappAInterface', in_signature='', out_signalture='s')
    def quit(self):
        appA_loop.quit()
        return 'Quit AppA Service'

if __name__ == '__main__':
    DBusGMainLoop(set_as_default=True)
    appA_bus = dbus.SystemBus()
    appA_bus_name = dbus.service.BusName('kr.gooroom.bhpyappA', bus=appA_bus)
    appA_object = AppAObject(appA_bus, '/kr/gooroom/bhpyappAObject')

    appA_loop = GLib.MainLoop()
    appA_loop.run()
