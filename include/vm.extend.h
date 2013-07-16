#ifndef LUMA_VM_EXTEND_H
#define LUMA_VM_EXTEND_H

// API ------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void vm_global_set(const char *name);
void vm_global_get(const char *name);
void vm_object_create();
void vm_field_set(const char *name);
void vm_field_get();
void vm_field_set_nil(const char *field);
void vm_field_set_function(const char *field, void *func);
void vm_field_set_boolean(const char *field, bool b);
bool vm_field_get_boolean(const char *field);
void vm_field_set_string(const char *field, const char *string);
const char *vm_field_get_string(const char *field);
void vm_field_set_integer(const char *field, const int i);
int vm_field_get_integer(const char *field);
void vm_field_set_double(const char *field, const double d);
double vm_field_get_double(const char *field);

#endif

