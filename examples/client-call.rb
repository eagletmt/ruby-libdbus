#!/usr/bin/env ruby
require 'pp'
require 'libdbus'

conn = LibDBus::Connection.system_bus
message = LibDBus::MethodCallMessage.new(
  'org.freedesktop.systemd1',
  '/org/freedesktop/systemd1/unit/ntpd_2eservice',
  'org.freedesktop.DBus.Properties',
  'GetAll',
)
p [message.destination, message.path, message.interface, message.member]
message.add_argument('s', 'org.freedesktop.systemd1.Unit')
reply = conn.send_message(message)
pp reply.arguments
