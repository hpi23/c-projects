#include "./map.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  HashMap *map = hashmap_new();
  hashmap_insert(map, "foo", "Bar");
  hashmap_insert(map, "foo", "Bar2");

  hashmap_insert(map, "foo2", "Hello World");

  char *result = malloc(10000);
  bool found = hashmap_get(map, "foo2", (void **)&result);

  for (int i = 0; i < 1000; i++) {
    char *str = (char *)malloc(1000);
    sprintf(str, "%d", i);
    hashmap_insert(map, str, "My-Value");
  }

  printf("found: %d | value: %s\n", found, result);
  //hashmap_print_buckets(map);
}
