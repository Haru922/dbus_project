import socket
import subprocess
import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib

class GHubObject(dbus.service.Object):
    connected_bus = None
    def __init__(self, connected_bus, object_path):
        dbus.service.Object.__init__(self, connected_bus, object_path)
        self.connected_bus = connected_bus

    @dbus.service.method('kr.gooroom.bhpyGHubInterface', in_signature='s', out_signature='s')
    def forward_to_gcontroller(self, signal):
        gcontroller_object = self.connected_bus.get_object('kr.gooroom.bhpyGController', '/kr/gooroom/bhpyGControllerObject')
        result = gcontroller_object.turn_on_off_app_a(signal, dbus_interface='kr.gooroom.bhpyGControllerInterface')
        return result

    @dbus.service.method('kr.gooroom.bhpyGHubInterface', in_signature='', out_signature='s')
    def execute_app_a(self):
        appA_object = self.connected_bus.get_object('kr.gooroom.bhpyappA', '/kr/gooroom/bhpyappAObject')
        result = appA_object.print_log(dbus_interface='kr.gooroom.bhpyappAInterface')
        return result

    @dbus.service.method('kr.gooroom.bhpyGHubInterface', in_signature='', out_signature='s')
    def quit(self):
        ghub_loop.quit()
        return 'Quit GHub Service'

if __name__ == '__main__':
    DBusGMainLoop(set_as_default=True)
    ghub_bus = dbus.SystemBus()
    ghub_bus_name = dbus.service.BusName('kr.gooroom.bhpyGHub', bus=ghub_bus)
    ghub_object = GHubObject(ghub_bus, '/kr/gooroom/bhpyGHubObject')

    ghub_loop = GLib.MainLoop()
    ghub_loop.run()
