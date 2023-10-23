#include "../dynstring/dynstring.h"
#include "./MyClass.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  MyClass *obj = new_MyClass(11);
  printf("Before: %ld\n", obj->get_value());
  obj->set_value(42);
  printf("After: %ld\n", obj->get_value());

  return 0;
}
