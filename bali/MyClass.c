#include "./MyClass.h"
#include "./MyClass_dispatcher.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

MyClassMethods new_dispatcher(MyClass *ptr_to_instance);

MyClass *new_MyClass(int64_t value) {
  MyClass temp = {.value = value};

  MyClass *ptr_temp = malloc(sizeof(MyClass));

  if (MyClass_VERBOSE) {
    printf("=== Configuring new instance ===\n");
  }

  MyClassMethods methods = new_dispatcher(ptr_temp);
  temp.set_value = methods.set_value;
  temp.get_value = methods.get_value;

  if (MyClass_VERBOSE) {
    printf("\tDynamic dispatch configured: (set=%p) (get=%p)\n", temp.set_value, temp.get_value);
    printf("=== New instance configured ===\n");
  }

  *ptr_temp = temp;
  return ptr_temp;
}

int64_t myclass_get_value(MyClass *this) { return this->value; }
void myclass_set_value(MyClass *this, int64_t value) { this->value = value; }
