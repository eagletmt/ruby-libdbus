#!/usr/bin/env ruby
require 'bundler'
Bundler.setup
require 'pp'
require 'libdbus'

conn = LibDBus::Connection.session_bus
message = LibDBus::MethodCallMessage.new(
  'cc.wanko.example1',
  '/',
  'test.method.Type',
  'Method',
)
p [message.destination, message.path, message.interface, message.member]
message.add_argument('s', 'foo')
message.add_argument('i', 42)
message.add_argument('aab', [[true, true, false]])
message.add_argument('d', 3.14)
message.add_argument('o', '/object/path')
message.add_argument('g', 'g')
reply = conn.send_message_with_reply(message)
pp reply.arguments
LibDBus.validate_path!('/object/path')
