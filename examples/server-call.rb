#!/usr/bin/env ruby
require 'bundler'
Bundler.setup
require 'libdbus'

conn = LibDBus::Connection.session_bus
conn.request_name('cc.wanko.example1')
conn.register_object_path('/') do |message|
  puts "On /: #{message.interface}.#{message.member}"
  p message.arguments
  reply = LibDBus::MethodReturnMessage.new(message)
  conn.send_message(reply)
  true
end

loop do
  conn.read_write_dispatch(0)
  sleep 1
end
