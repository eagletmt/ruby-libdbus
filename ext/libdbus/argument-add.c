#include "libdbus.h"

static void
add_byte_argument(DBusMessageIter *mes_iter, unsigned char argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_BYTE, &argument);
}

static void
add_boolean_argument(DBusMessageIter *mes_iter, dbus_bool_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_BOOLEAN, &argument);
}

static void
add_int16_argument(DBusMessageIter *mes_iter, dbus_int16_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_INT16, &argument);
}

static void
add_uint16_argument(DBusMessageIter *mes_iter, dbus_uint16_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_UINT16, &argument);
}

static void
add_int32_argument(DBusMessageIter *mes_iter, dbus_int32_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_INT32, &argument);
}

static void
add_uint32_argument(DBusMessageIter *mes_iter, dbus_uint32_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_UINT32, &argument);
}

static void
add_int64_argument(DBusMessageIter *mes_iter, dbus_int64_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_INT64, &argument);
}

static void
add_uint64_argument(DBusMessageIter *mes_iter, dbus_uint64_t argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_UINT64, &argument);
}

static void
add_double_argument(DBusMessageIter *mes_iter, double argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_DOUBLE, &argument);
}

static void
add_string_argument(DBusMessageIter *mes_iter, const char *argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_STRING, &argument);
}

static void
add_array_argument(DBusMessageIter *mes_iter, DBusSignatureIter *sig_iter, VALUE argument)
{
  DBusMessageIter sub_mes;
  DBusSignatureIter sub_sig;
  char *element_signature;
  long i, length;

  dbus_signature_iter_recurse(sig_iter, &sub_sig);
  element_signature = dbus_signature_iter_get_signature(&sub_sig);
  dbus_message_iter_open_container(mes_iter, DBUS_TYPE_ARRAY, element_signature, &sub_mes);
  length = RARRAY_LEN(argument);
  for (i = 0; i < length; i++) {
    dbus_signature_iter_init(&sub_sig, element_signature);
    libdbus_add_argument(&sub_mes, &sub_sig, RARRAY_AREF(argument, i));
  }
  dbus_free(element_signature);
  dbus_message_iter_close_container(mes_iter, &sub_mes);
}

void
libdbus_add_argument(DBusMessageIter *mes_iter, DBusSignatureIter *sig_iter, VALUE argument)
{
  int type;

  while ((type = dbus_signature_iter_get_current_type(sig_iter)) != DBUS_TYPE_INVALID) {
    switch (type) {
      case DBUS_TYPE_BYTE:
        add_byte_argument(mes_iter, NUM2UINT(argument));
        break;
      case DBUS_TYPE_BOOLEAN:
        add_boolean_argument(mes_iter, !!RTEST(argument));
        break;
      case DBUS_TYPE_INT16:
        add_int16_argument(mes_iter, NUM2INT(argument));
        break;
      case DBUS_TYPE_UINT16:
        add_uint16_argument(mes_iter, NUM2UINT(argument));
        break;
      case DBUS_TYPE_INT32:
        add_int32_argument(mes_iter, NUM2INT(argument));
        break;
      case DBUS_TYPE_UINT32:
        add_uint32_argument(mes_iter, NUM2UINT(argument));
        break;
      case DBUS_TYPE_INT64:
        add_int64_argument(mes_iter, NUM2LL(argument));
        break;
      case DBUS_TYPE_UINT64:
        add_uint64_argument(mes_iter, NUM2ULL(argument));
        break;
      case DBUS_TYPE_DOUBLE:
        add_double_argument(mes_iter, NUM2DBL(argument));
        break;
      case DBUS_TYPE_STRING:
        add_string_argument(mes_iter, StringValueCStr(argument));
        break;
      case DBUS_TYPE_OBJECT_PATH:
        libdbus_validate_path(Qnil, argument);
        add_string_argument(mes_iter, StringValueCStr(argument));
        break;
      case DBUS_TYPE_SIGNATURE:
        libdbus_validate_signature(Qnil, argument);
        add_string_argument(mes_iter, StringValueCStr(argument));
        break;
      case DBUS_TYPE_ARRAY:
        add_array_argument(mes_iter, sig_iter, argument);
        break;
      default:
        rb_raise(rb_eArgError, "Unknown signature type: %c (%d)", type, type);
    }
    dbus_signature_iter_next(sig_iter);
  }
}
