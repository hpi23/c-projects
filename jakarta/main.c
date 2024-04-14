#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Print age function.
extern void patch_dog_print_age_fn();
extern void patch_dog_print_age_end();

// Get age function.
extern void patch_dog_get_age_fn();
extern void patch_dog_get_age_end();

// Set age function.
extern void patch_dog_set_age_fn();
extern void patch_dog_set_age_end();

// Holes to be patched.
extern void dog_print_age_obj_hole();
extern void dog_print_age_fn_hole();

extern void dog_get_age_obj_hole();
extern void dog_get_age_fn_hole();

extern void dog_set_age_obj_hole();
extern void dog_set_age_fn_hole();

int page_rwe(void *addr) {
  // Move the pointer to the page boundary
  int page_size = getpagesize();
  addr -= (unsigned long)addr % page_size;

  if (mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
    return -1;
  }

  return 0;
}

//
// Dog functions.
//

typedef struct {
  int64_t age;
  void (*print_age)();
  int64_t (*get_age)();
  void (*set_age)(int64_t new_age);
} Dog;

void dog_print_age(Dog * dog) {
  printf("Printing dog age: %ld...\n", dog->age);
}

int dog_get_age(Dog * dog) {
  return dog->age;
}

void dog_set_age(Dog * dog, int new_age) {
    dog->age = new_age;
}

//
// End dog functions.
//

Dog *dog_new(int age) {
  // This is needed so that the holes can be patched (holes are in the original code, not the copied one).
  if (page_rwe(patch_dog_print_age_fn) == -1) {
    fprintf(stderr, "Error while changing page permissions of foo(): %s\n",
            strerror(errno));
    exit(1);
  }

  Dog *dog_ptr = (Dog *)malloc(sizeof(Dog));
  dog_ptr->age = age;

  //////////////////////////////////
  // Print age.
  //////////////////////////////////

  // Allocate space for the print_age function.
  // Then copy the incomplete function over, then patch the holes.

  // Function size required for `memcpy`
  uint64_t print_age_fn_len = (uint64_t)patch_dog_print_age_end - (uint64_t)patch_dog_print_age_fn;

  // Allocate space for new function.
  void * print_age_new_fn = malloc(print_age_fn_len);

  // Patch holes in the functions.
  *(uint64_t *)dog_print_age_fn_hole = (uint64_t)&dog_print_age;
  *(uint64_t *)dog_print_age_obj_hole = (uint64_t)dog_ptr;

  // Make new function page executable.``
  if (page_rwe(print_age_new_fn) == -1) {
    fprintf(stderr, "Error while changing page permissions of foo(): %s\n",
            strerror(errno));
    exit(1);
  }

  // Copy old function (with patches) to new memory location to make patches 'persistent'.`
  memcpy(print_age_new_fn, patch_dog_print_age_fn, print_age_fn_len);

  // Use the new function as the `print_age` function on the new object.
  dog_ptr->print_age = print_age_new_fn;

  //////////////////////////////////
  // Get age.
  //////////////////////////////////

  uint64_t get_age_fn_len = (uint64_t)patch_dog_get_age_end - (uint64_t)patch_dog_get_age_fn;

  void * get_age_new_fn = malloc(get_age_fn_len);

  *(uint64_t *)dog_get_age_fn_hole = (uint64_t)&dog_get_age;
  *(uint64_t *)dog_get_age_obj_hole = (uint64_t)dog_ptr;

  if (page_rwe(get_age_new_fn) == -1) {
    fprintf(stderr, "Error while changing page permissions of foo(): %s\n",
            strerror(errno));
    exit(1);
  }

  memcpy(get_age_new_fn, patch_dog_get_age_fn, get_age_fn_len);

  dog_ptr->get_age = get_age_new_fn;

  //////////////////////////////////
  // Set age.
  //////////////////////////////////

  uint64_t set_age_fn_len = (uint64_t)patch_dog_set_age_end - (uint64_t)patch_dog_set_age_fn;

  void * set_age_new_fn = malloc(set_age_fn_len);

  *(uint64_t *)dog_set_age_fn_hole = (uint64_t)&dog_set_age;
  *(uint64_t *)dog_set_age_obj_hole = (uint64_t)dog_ptr;

  if (page_rwe(set_age_new_fn) == -1) {
    fprintf(stderr, "Error while changing page permissions of foo(): %s\n",
            strerror(errno));
    exit(1);
  }

  memcpy(set_age_new_fn, patch_dog_set_age_fn, set_age_fn_len);

  dog_ptr->set_age = set_age_new_fn;

  return dog_ptr;
}

//
// End.
//

int main() {
  uint64_t fn_len = (uint64_t)patch_dog_print_age_end - (uint64_t)patch_dog_print_age_fn;
  printf("start: %p; addr_hole: %p; fn_hole: %p; end: %p; len=%lu\n", patch_dog_print_age_fn, &dog_print_age_obj_hole, &dog_print_age_fn_hole,
         patch_dog_print_age_end, fn_len);


  Dog *dog1 = dog_new(18);
  dog1->print_age();

  int dog1_age = dog1->get_age();
  printf("Dog 1 is %d years old.\n", dog1_age);

  Dog *dog2 = dog_new(19);
  dog2->print_age();

  int dog2_age = dog2->get_age();
  printf("Dog 2 is %d years old.\n", dog2_age);

  dog2->set_age(69);

  printf("Dog 2 is now %ld years old.\n", dog2->get_age());

  return 0;
}
