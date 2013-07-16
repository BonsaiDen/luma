/*
bool input_init() {
    
    // down (is currently pressed)
    // hit (was down on for one frame)
    // released (was down on last frame but no longer)

    vm_global_get("keyboard");
    vm_field_set_integer(""); // constants
    vm_field_set_function("count", NULL);
    vm_field_set_function("down", NULL);
    vm_field_set_function("hit", NULL);
    vm_field_set_function("released", NULL);
    vm_rewind();

    vm_global_get("mouse");
    vm_field_set_integer(""); // constants
    vm_field_set_function("getPosition", NULL);
    vm_field_set_function("down", NULL);
    vm_field_set_function("hit", NULL);
    vm_field_set_function("released", NULL);
    vm_rewind();

}
*/
