#include "libdbus.h"

static VALUE cConnection;

typedef struct {
  DBusConnection *conn;
} wrapper_t;

static void
wrapper_free(void *wrapper)
{
  wrapper_t *w = wrapper;
  free(w);
}

static VALUE
s_alloc(VALUE klass)
{
  wrapper_t *w = ALLOC(wrapper_t);
  w->conn = NULL;
  return Data_Wrap_Struct(klass, NULL, wrapper_free, w);
}

static DBusConnection *
unwrap_connection(VALUE obj)
{
  wrapper_t *w;
  Data_Get_Struct(obj, wrapper_t, w);
  return w->conn;
}

static void
wrap_connection(VALUE obj, DBusConnection *conn)
{
  wrapper_t *w;
  Data_Get_Struct(obj, wrapper_t, w);
  w->conn = conn;
}

static VALUE
get_bus(VALUE klass, DBusBusType type)
{
  DBusError err = DBUS_ERROR_INIT;
  DBusConnection *conn;
  VALUE obj;

  conn = dbus_bus_get(type, &err);
  if (conn == NULL) {
    libdbus_raise_error(&err);
  }
  obj = s_alloc(klass);
  wrap_connection(obj, conn);
  return obj;
}

static VALUE
s_system_bus(VALUE self)
{
  return get_bus(self, DBUS_BUS_SYSTEM);
}

static VALUE
s_session_bus(VALUE self)
{
  return get_bus(self, DBUS_BUS_SESSION);
}

static VALUE
m_send_message(VALUE self, VALUE message)
{
  dbus_uint32_t serial = 0;

  if (!dbus_connection_send(unwrap_connection(self), libdbus_unwrap_message(message), &serial)) {
    rb_raise(rb_eRuntimeError, "Send message failure");
  }

  return UINT2NUM(serial);
}

static VALUE
m_send_message_with_reply(VALUE self, VALUE message)
{
  DBusError err = DBUS_ERROR_INIT;
  DBusMessage *reply = NULL;

  reply = dbus_connection_send_with_reply_and_block(
      unwrap_connection(self),
      libdbus_unwrap_message(message),
      DBUS_TIMEOUT_USE_DEFAULT,
      &err);
  if (reply == NULL) {
    libdbus_raise_error(&err);
  }
  return libdbus_wrap_message(reply);
}

static VALUE
m_request_name(VALUE self, VALUE name)
{
  DBusError err = DBUS_ERROR_INIT;
  int result_code;

  result_code = dbus_bus_request_name(
      unwrap_connection(self),
      StringValueCStr(name),
      0,  // TODO: flags
      &err);
  if (result_code == -1) {
    libdbus_raise_error(&err);
  }

  return INT2NUM(result_code);
}

void
Init_libdbus_connection(VALUE mLibDBus)
{
  cConnection = rb_define_class_under(mLibDBus, "Connection", rb_cObject);
  rb_define_alloc_func(cConnection, s_alloc);
  rb_undef_method(CLASS_OF(cConnection), "new");

  rb_define_singleton_method(cConnection, "system_bus", s_system_bus, 0);
  rb_define_singleton_method(cConnection, "session_bus", s_session_bus, 0);

  rb_define_method(cConnection, "send_message", m_send_message, 1);
  rb_define_method(cConnection, "send_message_with_reply", m_send_message_with_reply, 1);
  rb_define_method(cConnection, "request_name", m_request_name, 1);
}
