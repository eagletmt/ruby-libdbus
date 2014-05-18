#include "libdbus.h"

static VALUE eError;

void
Init_libdbus_error(VALUE mLibDBus)
{
  eError = rb_define_class_under(mLibDBus, "Error", rb_eStandardError);
}

void libdbus_raise_error(DBusError *error)
{
  if (dbus_error_is_set(error)) {
    VALUE msg = rb_sprintf("%s: %s", error->name, error->message);
    dbus_error_free(error);
    rb_exc_raise(rb_exc_new3(eError, msg));
  } else {
    rb_raise(rb_eArgError, "DBusError isn't set");
  }
}
