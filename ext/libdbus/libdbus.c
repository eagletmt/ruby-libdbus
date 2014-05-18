#include "libdbus.h"

VALUE rb_mLibDBus;

void
Init_libdbus(void)
{
  rb_mLibDBus = rb_define_module("LibDBus");
  Init_libdbus_connection(rb_mLibDBus);
  Init_libdbus_error(rb_mLibDBus);
  VALUE cMessage = Init_libdbus_message(rb_mLibDBus);
  Init_libdbus_method_call_message(rb_mLibDBus, cMessage);
  Init_libdbus_method_return_message(rb_mLibDBus, cMessage);
}
