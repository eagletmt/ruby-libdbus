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

  rb_define_singleton_method(rb_mLibDBus, "validate_path!", libdbus_validate_path, 1);
  rb_define_singleton_method(rb_mLibDBus, "validate_signature!", libdbus_validate_signature, 1);
}

VALUE
libdbus_validate_path(RB_UNUSED_VAR(VALUE self), VALUE path)
{
  DBusError err = DBUS_ERROR_INIT;

  if (!dbus_validate_path(StringValueCStr(path), &err)) {
    libdbus_raise_error(&err);
  }

  return Qtrue;
}

VALUE
libdbus_validate_signature(RB_UNUSED_VAR(VALUE self), VALUE signature)
{
  DBusError err = DBUS_ERROR_INIT;

  if (!dbus_signature_validate(StringValueCStr(signature), &err)) {
    libdbus_raise_error(&err);
  }

  return Qtrue;
}
