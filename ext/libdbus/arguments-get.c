#include "libdbus.h"

static VALUE get_argument(DBusMessageIter *iter);

static VALUE
get_byte_argument(DBusMessageIter *iter)
{
  unsigned char val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return INT2FIX(val);
}

static VALUE
get_boolean_argument(DBusMessageIter *iter)
{
  dbus_bool_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  if (val) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

static VALUE
get_int16_argument(DBusMessageIter *iter)
{
  dbus_int16_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return INT2NUM(val);
}

static VALUE
get_uint16_argument(DBusMessageIter *iter)
{
  dbus_uint16_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return UINT2NUM(val);
}

static VALUE
get_int32_argument(DBusMessageIter *iter)
{
  dbus_int32_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return INT2NUM(val);
}

static VALUE
get_uint32_argument(DBusMessageIter *iter)
{
  dbus_uint32_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return UINT2NUM(val);
}

static VALUE
get_int64_argument(DBusMessageIter *iter)
{
  dbus_int64_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return LL2NUM(val);
}

static VALUE
get_uint64_argument(DBusMessageIter *iter)
{
  dbus_uint64_t val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return ULL2NUM(val);
}

static VALUE
get_double_argument(DBusMessageIter *iter)
{
  double val = 0;
  dbus_message_iter_get_basic(iter, &val);
  return DBL2NUM(val);
}

static VALUE
get_string_argument(DBusMessageIter *iter)
{
  char *str = NULL;
  dbus_message_iter_get_basic(iter, &str);
  return rb_str_new_cstr(str);
}

static VALUE
get_variant_argument(DBusMessageIter *iter)
{
  DBusMessageIter sub;
  dbus_message_iter_recurse(iter, &sub);
  return get_argument(&sub);
}

static VALUE
get_dict_entry_argument(DBusMessageIter *iter)
{
  DBusMessageIter sub;
  VALUE pair;

  dbus_message_iter_recurse(iter, &sub);
  pair = rb_ary_new_capa(2);
  rb_ary_push(pair, get_argument(&sub));
  dbus_message_iter_next(&sub);
  rb_ary_push(pair, get_argument(&sub));
  return pair;
}

static VALUE
get_arguments_hash(DBusMessageIter *iter)
{
  VALUE arguments;
  int type;

  arguments = rb_hash_new();
  while ((type = dbus_message_iter_get_arg_type(iter)) != DBUS_TYPE_INVALID) {
    VALUE pair;

    if (type != DBUS_TYPE_DICT_ENTRY) {
      rb_raise(rb_eTypeError, "Expected DICT_ENTRY but got %d", type);
    }
    pair = get_dict_entry_argument(iter);
    rb_hash_aset(arguments, RARRAY_AREF(pair, 0), RARRAY_AREF(pair, 1));

    dbus_message_iter_next(iter);
  }
  return arguments;
}

static VALUE
get_array_argument(DBusMessageIter *iter)
{
  DBusMessageIter sub;

  dbus_message_iter_recurse(iter, &sub);
  if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_ARRAY
      && dbus_message_iter_get_element_type(iter) == DBUS_TYPE_DICT_ENTRY) {
    return get_arguments_hash(&sub);
  } else {
    return libdbus_get_arguments(&sub);
  }
}

VALUE
libdbus_get_arguments(DBusMessageIter *iter)
{
  VALUE arguments;
  int type;

  arguments = rb_ary_new();
  while ((type = dbus_message_iter_get_arg_type(iter)) != DBUS_TYPE_INVALID) {
    rb_ary_push(arguments, get_argument(iter));
    dbus_message_iter_next(iter);
  }
  return arguments;
}

VALUE
get_argument(DBusMessageIter *iter)
{
  const int type = dbus_message_iter_get_arg_type(iter);
  switch (type) {
    case DBUS_TYPE_BYTE:
      return get_byte_argument(iter);
    case DBUS_TYPE_BOOLEAN:
      return get_boolean_argument(iter);
    case DBUS_TYPE_INT16:
      return get_int16_argument(iter);
    case DBUS_TYPE_UINT16:
      return get_uint16_argument(iter);
    case DBUS_TYPE_INT32:
      return get_int32_argument(iter);
    case DBUS_TYPE_UINT32:
      return get_uint32_argument(iter);
    case DBUS_TYPE_INT64:
      return get_int64_argument(iter);
    case DBUS_TYPE_UINT64:
      return get_uint64_argument(iter);
    case DBUS_TYPE_DOUBLE:
      return get_double_argument(iter);
    case DBUS_TYPE_STRING:
      return get_string_argument(iter);
    case DBUS_TYPE_OBJECT_PATH:
      return get_string_argument(iter);
    case DBUS_TYPE_SIGNATURE:
      return get_string_argument(iter);
    case DBUS_TYPE_UNIX_FD:
      return get_int32_argument(iter);
    case DBUS_TYPE_ARRAY:
      return get_array_argument(iter);
    case DBUS_TYPE_VARIANT:
      return get_variant_argument(iter);
    case DBUS_TYPE_STRUCT:
      return get_array_argument(iter);
    case DBUS_TYPE_DICT_ENTRY:
      return get_dict_entry_argument(iter);
    default:
      rb_raise(rb_eArgError, "Unknown argument type: %d ('%c')", type, type);
  }
}
