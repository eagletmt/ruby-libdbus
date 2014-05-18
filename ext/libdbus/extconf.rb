require "mkmf"
require "pkg-config"

PKGConfig.have_package('dbus-1')

$CFLAGS << ' -Wall -W'

create_makefile("libdbus")
