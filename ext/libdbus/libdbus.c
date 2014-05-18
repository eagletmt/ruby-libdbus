#include "libdbus.h"

VALUE rb_mLibDBus;

void
Init_libdbus(void)
{
  rb_mLibDBus = rb_define_module("LibDBus");
}
