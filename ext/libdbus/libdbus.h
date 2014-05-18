#ifndef LIBDBUS_H
#define LIBDBUS_H 1

#include "ruby.h"
#include <dbus/dbus.h>

extern VALUE cMethodCallMessage;
extern VALUE cMethodReturnMessage;

void Init_libdbus_connection(VALUE mLibDBus);
void Init_libdbus_error(VALUE mLibDBus);
VALUE Init_libdbus_message(VALUE mLibDBus);
void Init_libdbus_method_call_message(VALUE mLibDBus, VALUE cMessage);
void Init_libdbus_method_return_message(VALUE mLibDBus, VALUE cMessage);

void libdbus_set_message(VALUE obj, DBusMessage *message);
VALUE libdbus_wrap_message(DBusMessage *message);
DBusMessage *libdbus_unwrap_message(VALUE message);
void libdbus_raise_error(DBusError *error);

VALUE libdbus_get_arguments(DBusMessageIter *iter);
void libdbus_add_argument(DBusMessageIter *mes_iter, DBusSignatureIter *sig_iter, VALUE argument);

#endif /* LIBDBUS_H */
