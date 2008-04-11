#include "ruby.h"

/* From Nodewrap code
 *  http://rubystuff.org/nodewrap/
 */

/* 
 * call-seq:
 *   uninclude(module, ...) => self
 *
 * Removes the specified module(s) from the inheritance chain.
 */
static VALUE module_uninclude(int argc, VALUE * argv, VALUE module)
{
  int i;

  for (i = 0; i < argc; i++)
    Check_Type(argv[i], T_MODULE);
  while (argc--) {
    rb_funcall(argv[argc], rb_intern("remove_features"), 1, module);
    rb_funcall(argv[argc], rb_intern("unincluded"), 1, module);
  }
  return module;
}

/* 
 * call-seq:
 *   remove_features(mod) => mod
 *
 * When this module is unincluded from another, Nodewrap calls
 * remove_features in this module.  The default behavior is to remove
 * the constants, methods, and module variables of this module from
 * _mod_.  If this module has not been included by _mod_, an exception
 * will be raised.
 */
static VALUE module_remove_features(VALUE module, VALUE uninclude)
{
  VALUE prev, mod;

  if(TYPE(uninclude) != T_CLASS && TYPE(uninclude) != T_MODULE)
  {
    Check_Type(uninclude, T_CLASS);
  }

  rb_frozen_class_p(uninclude);
  if(!OBJ_TAINTED(uninclude))
  {
    rb_secure(4);
  }

  OBJ_INFECT(uninclude, module);

  if(RCLASS(uninclude)->m_tbl == RCLASS(module)->m_tbl)
  {
    rb_raise(rb_eArgError, "Cannot remove module from itself");
  }

  prev = uninclude;
  mod = RCLASS(uninclude)->super;

  while(mod)
  {
    if(RCLASS(module)->m_tbl == RCLASS(mod)->m_tbl)
    {
      RCLASS(prev)->super = RCLASS(mod)->super;
      rb_clear_cache();
      return module;
    }

    if(BUILTIN_TYPE(mod) == T_CLASS)
    {
      break;
    }

    prev = mod;
    mod = RCLASS(mod)->super;
  }

  rb_raise(rb_eArgError, "Could not find included module");
  return module;
}

static VALUE module_unincluded(VALUE module, VALUE uninclude)
{
  return Qnil;
}


void Init_module_uninclude(void)
{
  VALUE rb_cModule;
  /* For rdoc: rb_cModule = rb_define_class("Module", rb_cObject) */
  rb_cModule = rb_const_get(rb_cObject, rb_intern("Module"));
  rb_define_private_method(rb_cModule, "uninclude", module_uninclude, -1);
  rb_define_private_method(rb_cModule, "remove_features", module_remove_features, 1);
  rb_define_private_method(rb_cModule, "unincluded", module_unincluded, 1);
}
