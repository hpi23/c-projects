#pragma once
#include <stdint.h>

#define MyClass_VERBOSE 1
#define MyClass_MAKE_VERBOSE 0

typedef struct {
  int64_t (*get_value)(void);
  void (*set_value)(int64_t);
} MyClassMethods;

// int64_t this.get_value()
// void this.set_value(int64_t value)
typedef struct MyClass {
  int64_t value;
  int64_t (*get_value)(void);
  void (*set_value)(int64_t);
} MyClass;

MyClass *new_MyClass(int64_t value);
