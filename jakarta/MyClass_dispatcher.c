#include "../dynstring/dynstring.h"
#include "./MyClass.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

MyClassMethods new_dispatcher(MyClass *ptr_to_instance) { // returns the instance functions
  // Setup instance name and prefix
  struct timeval tv;
  gettimeofday(&tv, NULL);

  unsigned long long instance = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;

  DynString *LIB_PREFIX = dynstring_from("jakarta_MyClass_instance-");
  dynstring_push_fmt(LIB_PREFIX, "%lld", instance);

  // Create C file
  char *error;
  FILE *fp;
  DynString *path = dynstring_clone(LIB_PREFIX);

  DynString *c_file = dynstring_from("./runtime/");
  dynstring_push(c_file, LIB_PREFIX);
  dynstring_push_string(c_file, ".c");
  char *path_c = dynstring_as_cstr(c_file);

  if (MyClass_VERBOSE) {
    printf("\tGenerating dynamic C code for instance %lld...\n", instance);
  }

  fp = fopen(path_c, "a");

  char *contents = "\
    #include <stdio.h>\n\
    #include \"../MyClass.h\"\n\
    \n\
    \n\
    int64_t myclass_get_value(MyClass *this) { return this->value; }\
    void myclass_set_value(MyClass *this, int64_t value) { this->value = value; }\
    \n\
    \n\
    void set_value_wrapper_%lld(int64_t value) {\n\
        myclass_set_value((MyClass *) %p, value);\n\
    }\n\
    \n\
    int64_t get_value_wrapper_%lld() {\n\
        return myclass_get_value((MyClass *) %p);\n\
    }\n\
    \n\
  ";

  fprintf(fp, contents, instance, ptr_to_instance, instance, ptr_to_instance);
  fclose(fp);

  if (MyClass_VERBOSE) {
    printf("\tCompiling dispatcher code for instance %lld...\n", instance);
  }

  // Compile dynamic library using `make`
  DynString *command = dynstring_from("make FILE=");
  dynstring_push(command, LIB_PREFIX);
  dynstring_push_string(command, ".c");
  char *os_cmd = dynstring_as_cstr(command);

  FILE *cmd = popen(os_cmd, "r");
  char result[10000] = {0x0};
  while (fgets(result, sizeof(result), cmd) != NULL)
    if (MyClass_MAKE_VERBOSE) {
      printf("\t%s\n", result);
    }
  pclose(cmd);

  if (MyClass_VERBOSE) {
    printf("\tCompilation for instance %lld finished.\n", instance);
  }

  // Load dynamic library
  DynString *so_path = dynstring_clone(LIB_PREFIX);
  dynstring_push_string(so_path, ".c.so");

  char *path_so = dynstring_as_cstr(so_path);

  if (MyClass_VERBOSE) {
    printf("\tLoading shared object file for instance %lld: (%s)...\n", instance, path_so);
  }

  void *loaded = dlopen(path_so, RTLD_NOW);
  if (!loaded) {
    printf("%s\n", dlerror());
    exit(1);
  }

  DynString *set_value_name = dynstring_from("set_value_wrapper_");
  dynstring_push_fmt(set_value_name, "%lld", instance);

  void (*set_value)(int64_t) = dlsym(loaded, dynstring_as_cstr(set_value_name));
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(1);
  }

  DynString *get_value_name = dynstring_from("get_value_wrapper_");
  dynstring_push_fmt(get_value_name, "%lld", instance);

  int64_t (*get_value)(void) = dlsym(loaded, dynstring_as_cstr(get_value_name));
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(1);
  }

  if (MyClass_VERBOSE) {
    printf("\tShared object file and dispatcher functions for instance %lld loaded.\n", instance);
  }

  // dlclose(loaded);

  MyClassMethods res = {.set_value = set_value, .get_value = get_value};
  return res;
}
