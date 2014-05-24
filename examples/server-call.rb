#!/usr/bin/env ruby
require 'bundler'
Bundler.setup
require 'libdbus'

conn = LibDBus::Connection.session_bus
conn.request_name('cc.wanko.example1')
conn.register_object_path('/') do |message|
  puts "On /: #{message.interface}.#{message.member}"
  p message.arguments
  case message.interface
  when 'test.method.Type'
    case message.member
    when 'Method'
      reply = LibDBus::MethodReturnMessage.new(message)
      reply.add_argument('s', message.arguments.inspect)
      conn.send_message(reply)
    end
  end
end

loop do
  conn.read_write_dispatch(0)
  sleep 0.1
end
