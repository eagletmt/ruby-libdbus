# libdbus

TODO: Write a gem description

## Installation

Add this line to your application's Gemfile:

    gem 'libdbus'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install libdbus

## Usage

TODO: Write usage instructions here

## Running examples
```
% examples/server-call.rb &
[1] 28007
% examples/client-call.rb
["cc.wanko.example1", "/", "test.method.Type", "Method"]
On /: test.method.Type.Method
["foo", 42, [[true, true, false]], 3.14, "/object/path", "g"]
["[\"foo\", 42, [[true, true, false]], 3.14, \"/object/path\", \"g\"]"]
% dbus-send --session --print-reply --dest=cc.wanko.example1 / test.method.Type.Method string:foo int16:42 array:boolean:true,true,false double:3.14
On /: test.method.Type.Method
["foo", 42, [true, true, false], 3.14]
method return sender=:1.133 -> dest=:1.134 reply_serial=2
   string "["foo", 42, [true, true, false], 3.14]"
% kill %1
[1]  + terminated  examples/server-call.rb
%
```

## Contributing

1. Fork it ( https://github.com/eagletmt/ruby-libdbus/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
