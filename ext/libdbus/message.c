#include "libdbus.h"

static VALUE cMessage;

typedef struct {
  DBusMessage *message;
} wrapper_t;

static void
wrapper_free(void *wrapper)
{
  wrapper_t *w = wrapper;
  if (w->message != NULL) {
    dbus_message_unref(w->message);
  }
  free(w);
}

static VALUE
s_alloc(VALUE klass)
{
  wrapper_t *w = ALLOC(wrapper_t);
  w->message = NULL;
  return Data_Wrap_Struct(klass, NULL, wrapper_free, w);
}

static VALUE
to_str(const char *str)
{
  if (str == NULL) {
    return Qnil;
  } else {
    return rb_str_new_cstr(str);
  }
}

static VALUE
m_get_destination(VALUE self)
{
  return to_str(
      dbus_message_get_destination(
        libdbus_unwrap_message(self)));
}

static VALUE
m_set_destination(VALUE self, VALUE dest)
{
  dbus_message_set_destination(
      libdbus_unwrap_message(self),
      StringValueCStr(dest));
  return dest;
}

static VALUE
m_get_path(VALUE self)
{
  return to_str(
      dbus_message_get_path(
        libdbus_unwrap_message(self)));
}

static VALUE
m_set_path(VALUE self, VALUE path)
{
  dbus_message_set_path(
      libdbus_unwrap_message(self),
      StringValueCStr(path));
  return path;
}

static VALUE
m_get_interface(VALUE self)
{
  return to_str(
      dbus_message_get_interface(
        libdbus_unwrap_message(self)));
}

static VALUE
m_set_interface(VALUE self, VALUE iface)
{
  dbus_message_set_interface(
      libdbus_unwrap_message(self),
      StringValueCStr(iface));
  return iface;
}

static VALUE
m_get_member(VALUE self)
{
  return to_str(
      dbus_message_get_member(
        libdbus_unwrap_message(self)));
}

static VALUE
m_set_member(VALUE self, VALUE member)
{
  dbus_message_set_member(
      libdbus_unwrap_message(self),
      StringValueCStr(member));
  return member;
}

static VALUE
m_add_argument(VALUE self, VALUE signature, VALUE argument)
{
  DBusMessage *message;
  DBusSignatureIter sig_iter;
  DBusMessageIter mes_iter;

  message = libdbus_unwrap_message(self);
  dbus_message_iter_init_append(message, &mes_iter);
  dbus_signature_iter_init(&sig_iter, StringValueCStr(signature));
  libdbus_add_argument(&mes_iter, &sig_iter, argument);

  return self;
}

static VALUE
m_get_arguments(VALUE self)
{
  DBusMessageIter iter;

  dbus_message_iter_init(
      libdbus_unwrap_message(self),
      &iter);

  return libdbus_get_arguments(&iter);
}

VALUE
Init_libdbus_message(VALUE mLibDBus)
{
  cMessage = rb_define_class_under(mLibDBus, "Message", rb_cObject);
  rb_define_alloc_func(cMessage, s_alloc);

  rb_define_method(cMessage, "destination", m_get_destination, 0);
  rb_define_method(cMessage, "destination=", m_set_destination, 1);
  rb_define_method(cMessage, "path", m_get_path, 0);
  rb_define_method(cMessage, "path=", m_set_path, 1);
  rb_define_method(cMessage, "interface", m_get_interface, 0);
  rb_define_method(cMessage, "interface=", m_set_interface, 1);
  rb_define_method(cMessage, "member", m_get_member, 0);
  rb_define_method(cMessage, "member=", m_set_member, 1);
  rb_define_method(cMessage, "arguments", m_get_arguments, 0);
  rb_define_method(cMessage, "add_argument", m_add_argument, 2);

  return cMessage;
}

void
libdbus_set_message(VALUE obj, DBusMessage *message)
{
  wrapper_t *w;
  Data_Get_Struct(obj, wrapper_t, w);
  if (w->message != NULL) {
    dbus_message_unref(w->message);
  }
  w->message = message;
}

DBusMessage *
libdbus_unwrap_message(VALUE obj)
{
  wrapper_t *w;
  Data_Get_Struct(obj, wrapper_t, w);
  return w->message;
}

VALUE
libdbus_wrap_message(DBusMessage *message)
{
  int type;
  VALUE klass, obj;

  type = dbus_message_get_type(message);
  switch (type) {
    case DBUS_MESSAGE_TYPE_METHOD_CALL:
      klass = cMethodCallMessage;
      break;
    case DBUS_MESSAGE_TYPE_METHOD_RETURN:
      klass = cMethodReturnMessage;
      break;
    default:
      rb_raise(rb_eArgError, "Unknown message type: %d\n", type);
  }

  obj = s_alloc(klass);
  libdbus_set_message(obj, message);
  return obj;
}
