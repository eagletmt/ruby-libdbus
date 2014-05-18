require "mkmf"
require "pkg-config"

PKGConfig.have_package('dbus-1')

create_makefile("libdbus")
