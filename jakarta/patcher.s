 .intel_syntax

# Functions / addresses
.globl patch_dog_print_age_fn
.globl patch_dog_print_age_end

.globl patch_dog_get_age_fn
.globl patch_dog_get_age_end

.globl patch_dog_set_age_fn
.globl patch_dog_set_age_end

# Holes to be filled in externally.
.globl dog_print_age_obj_hole
.globl dog_print_age_fn_hole

.globl dog_get_age_obj_hole
.globl dog_get_age_fn_hole

.globl dog_set_age_obj_hole
.globl dog_set_age_fn_hole

patch_dog_print_age_fn:
	jmp patch_dog_print_age_fn_after_holes
	dog_print_age_obj_hole:
		.quad 0x2a
	dog_print_age_fn_hole:
		.quad 0x2a
	patch_dog_print_age_fn_after_holes:


	# Load the object address from the hole.
	mov %rdi, qword ptr [%rip+dog_print_age_obj_hole]

	# Call the `dog_print_age` function with the loaded object address.
	push %rsi
	mov %rsi, qword ptr [%rip+dog_print_age_fn_hole]
	call %rsi
	pop %rsi

    ret
patch_dog_print_age_end:
# End label is required to calculate the size of the function.

patch_dog_get_age_fn:
	jmp patch_dog_get_age_fn_after_holes
	dog_get_age_obj_hole:
		.quad 0x2a
	dog_get_age_fn_hole:
		.quad 0x2a
	patch_dog_get_age_fn_after_holes:


	mov %rdi, qword ptr [%rip+dog_get_age_obj_hole]

	push %rsi
	mov %rsi, qword ptr [%rip+dog_get_age_fn_hole]
	call %rsi
	pop %rsi

    ret
patch_dog_get_age_end:

patch_dog_set_age_fn:
	jmp patch_dog_set_age_fn_after_holes
	dog_set_age_obj_hole:
		.quad 0x2a
	dog_set_age_fn_hole:
		.quad 0x2a
	patch_dog_set_age_fn_after_holes:

	# New age is in `edi`
	push %rsi

	# This needs to be the second parameter to the callback function.
	mov %rsi, %rdi

	mov %rdi, qword ptr [%rip+dog_set_age_obj_hole]

	push %rax
	mov %rax, qword ptr [%rip+dog_set_age_fn_hole]
	call %rax
	pop %rax

	pop %rsi

    ret
patch_dog_set_age_end:
