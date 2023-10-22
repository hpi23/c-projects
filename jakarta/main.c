#include "../dynstring/dynstring.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAKE_OUTPUT 0

void test(int value) {
  char *error;
  FILE *fp;
  DynString *path = dynstring_from("jakarta");
  dynstring_push_fmt(path, "%d.c", value);

  DynString *c_file = dynstring_from("./runtime/");
  dynstring_push(c_file, path);
  char *path_c = dynstring_as_cstr(c_file);

  fp = fopen(path_c, "w");
  fprintf(fp, "#include<stdio.h>\nvoid d() { printf(\"%d\\n\"); }", value);
  fclose(fp);

  DynString *command = dynstring_from("make FILE=");
  char *make_path = dynstring_as_cstr(path);
  dynstring_push_string(command, make_path);
  char *os_cmd = dynstring_as_cstr(command);

  FILE *cmd = popen(os_cmd, "r");
  char result[10000] = {0x0};
  while (fgets(result, sizeof(result), cmd) != NULL)
    if (MAKE_OUTPUT) {
      printf("%s\n", result);
    }
  pclose(cmd);

  DynString *so_path = dynstring_from("jakarta");
  dynstring_push_fmt(so_path, "%d.c.so", value);

  char *path_so = dynstring_as_cstr(so_path);

  printf("loading so file: %s...\n", path_so);

  void *loaded = dlopen(path_so, RTLD_NOW);
  if (!loaded) {
    printf("%s\n", dlerror());
    exit(1);
  }

  void (*fn)(void) = dlsym(loaded, "d");
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(1);
  }

  fn();

  dlclose(loaded);
}

int main() {
  for (int i = 0; i < 10; i++) {
    test(i);
  }
  return 0;
}
