#include "libdbus.h"

VALUE cMethodCallMessage;

static VALUE
m_initialize(VALUE self, VALUE dest, VALUE path, VALUE iface, VALUE method)
{
  DBusMessage *message = dbus_message_new_method_call(
      StringValueCStr(dest),
      StringValueCStr(path),
      StringValueCStr(iface),
      StringValueCStr(method));
  libdbus_set_message(self, message);
  return self;
}

void
Init_libdbus_method_call_message(VALUE mLibDBus, VALUE cMessage)
{
  cMethodCallMessage = rb_define_class_under(mLibDBus, "MethodCallMessage", cMessage);
  rb_define_method(cMethodCallMessage, "initialize", m_initialize, 4);
}
