#include "libdbus.h"

VALUE cMethodReturnMessage;

static VALUE
m_initialize(VALUE self, VALUE in_reply_to)
{
  DBusMessage *message = dbus_message_new_method_return(
      libdbus_unwrap_message(in_reply_to));
  libdbus_set_message(self, message);
  return self;
}

void
Init_libdbus_method_return_message(VALUE mLibDBus, VALUE cMessage)
{
  cMethodReturnMessage = rb_define_class_under(mLibDBus, "MethodReturnMessage", cMessage);
  rb_define_method(cMethodReturnMessage, "initialize", m_initialize, 1);
}
