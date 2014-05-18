#include "libdbus.h"

static void
add_string_argument(DBusMessageIter *mes_iter, const char *argument)
{
  dbus_message_iter_append_basic(mes_iter, DBUS_TYPE_STRING, &argument);
}

void
libdbus_add_argument(DBusMessageIter *mes_iter, DBusSignatureIter *sig_iter, VALUE argument)
{
  const int type = dbus_signature_iter_get_current_type(sig_iter);

  switch (type) {
    case DBUS_TYPE_STRING:
      add_string_argument(mes_iter, StringValueCStr(argument));
      break;
    default:
      rb_raise(rb_eArgError, "Unknown signature type: %d", type);
  }
}
