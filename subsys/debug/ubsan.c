/*
 * Copyright (c) 2025 Daniel Hajjar <daniel.hajjar16@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr/sys/printk.h"
#include <stdint.h>

struct source_location {
	const char *file;
	uint32_t line;
	uint32_t column;
};

struct type_descriptor {
	uint16_t kind;
	uint16_t info;
	char name[];
};

struct type_mismatch_info {
	struct source_location location;
	struct type_descriptor *type;
	uintptr_t alignment;
	uint8_t type_check_kind;
};

struct out_of_bounds_info {
	struct source_location location;
	struct type_descriptor left_type;
	struct type_descriptor right_type;
};

/* Alignment must be a power of 2. */
#define is_aligned(value, alignment) !(value & (alignment - 1))

const char *Type_Check_Kinds[] = {
	"load of",
	"store to",
	"reference binding to",
	"member access within",
	"member call on",
	"constructor call on",
	"downcast of",
	"downcast of",
	"upcast of",
	"cast to virtual base of",
};

static void log_location(struct source_location *location)
{
	printk("\tfile: %s\n\tline: %i\n\tcolumn: %i\n", location->file, location->line,
	       location->column);
}

void __ubsan_handle_type_mismatch(struct type_mismatch_info *type_mismatch, uintptr_t pointer)
{
	struct source_location *location = &type_mismatch->location;

	if (!pointer) {
		printk("Null pointer access");
	} else if (type_mismatch->alignment != 0 && is_aligned(pointer, type_mismatch->alignment)) {
		/* Most useful on architectures with stricter memory alignment requirements. */
		printk("Unaligned memory access");
	} else {
		printk("Insufficient size");
		printk("%s address %p with insufficient space for object of type %s\n",
		       Type_Check_Kinds[type_mismatch->type_check_kind], (void *)pointer,
		       type_mismatch->type->name);
	}
	log_location(location);
}

void __ubsan_handle_type_mismatch_v1(void *data, uintptr_t pointer)
{
	printk("UBSAN: Type mismatch:\n");
	log_location(data);
}

void __ubsan_handle_pointer_overflow(void *data, void *base, void *result)
{
	printk("UBSAN: Pointer Overflow:\n");
	log_location(data);
}

void __ubsan_handle_divrem_overflow(void *data, void *left, void *right)
{
	printk("UBSAN: Division/Remainder Overflow:\n");
	log_location(data);
}

void __ubsan_handle_load_invalid_value(void *data, uintptr_t pointer)
{
	printk("UBSAN: Invalid load:\n");
	log_location(data);
}

void __ubsan_handle_builtin_unreachable(void *data)
{
	printk("UBSAN: Builtin Unreachable\n");
	log_location(data);
}

void __ubsan_handle_sub_overflow(void *data, void *left, void *right)
{
	printk("UBSAN: Subtraction Overflow:\n");
	log_location(data);
}

void __ubsan_handle_add_overflow(void *data, void *left, void *right)
{
	printk("UBSAN: Addition Overflow:\n");
	log_location(data);
}

void __ubsan_handle_negate_overflow(struct source_location *data, void *old_value)
{
	printk("UBSAN: Negation Overflow:\n");
	log_location(data);
}

void __ubsan_handle_nonnull_arg(void *data, void *arg)
{
	printk("UBSAN: Non-null Argument:\n");
	log_location(data);
}

void __ubsan_handle_nullability_arg(void *data)
{
	printk("UBSAN: Argument Nullability:\n");
	log_location(data);
}

void __ubsan_handle_shift_out_of_bounds(void *data, void *left, void *right)
{
	printk("UBSAN: Shift Out of bounds:\n");
	log_location(data);
}

void __ubsan_handle_vla_bound_not_positive(void *data, void *bound)
{
	printk("UBSAN: Variable length array:\n");
	log_location(data);
}

void __ubsan_handle_invalid_builtin(void *data)
{
	printk("UBSAN: Invalid builtin:\n");
	log_location(data);
}

void __ubsan_handle_mul_overflow(void *data, void *left, void *right)
{
	printk("UBSAN: Multiplication Overflow:\n");
	log_location(data);
}

void __ubsan_handle_out_of_bounds(void *data, uintptr_t pointer)
{
	printk("UBSAN: Out of bounds:\n");
	log_location(data);
}
