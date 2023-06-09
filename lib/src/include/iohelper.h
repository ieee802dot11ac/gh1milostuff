// iohelper.h - Helpers for FILE IO!
#ifndef IOHELPER_H
#define IOHELPER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "hmxcommon.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define BASIC_READ(type) \
INLINE type iohelper_read_##type(FILE *const file) \
{ \
	type ret; \
	if (fread(&ret, sizeof(type), 1, file) != 1) \
		warn("Short read of type " #type "!");\
	return ret; \
}

#define BASIC_WRITE(type) \
INLINE bool iohelper_write_##type(FILE *const file, type value) \
{ \
	return (fwrite(&value, sizeof(value), 1, file) != 1); \
}

// Reads a `u32` (little endian) from `file`
BASIC_READ(u32)

// Reads a `u16` (little endian) from `file`
BASIC_READ(u16)

// Reads a `u8` (little endian) from `file`
BASIC_READ(u8)

// Reads a `float` (little endian) from `file`
BASIC_READ(f32)

// Writes a `u32` (little endian) to `file`
BASIC_WRITE(u32)

// Writes a `u16` (little endian) to `file`
BASIC_WRITE(u16)

// Writes a `u8` (little endian) to `file`
BASIC_WRITE(u8)

// Writes a `f32` (little endian) to `file`
BASIC_WRITE(f32)

// Writes a `u32` (big endian) to `file`
INLINE bool iohelper_write_u32_be(FILE *const file, u32 value)
{
	u32 beValue = ((value & 0xFF000000) >> 24) | ((value & 0xFF0000) >> 8) |
		      ((value & 0xFF00) << 8) | ((value & 0xFF) << 24);
	return (fwrite(&beValue, sizeof(beValue), 1, file) != 1);
}

// Writes a `u16` (big endian) to `file`
INLINE bool iohelper_write_u16_be(FILE *const file, u16 value)
{
	u16 beValue = ((value & 0xFF00) >> 8) | ((value & 0xFF) << 8);
	return (fwrite(&beValue, sizeof(beValue), 1, file) != 1);
}

INLINE char *iohelper_read_cstring_at(FILE *const file, u32 fpos)
{
	char *ret = malloc(1024); u32 prevpos = ftell(file);
	fseek(file, fpos, SEEK_SET);
    fgets(ret, 1024, file); 
    fseek(file, prevpos, SEEK_SET);
	return ret;
}

#undef BASIC_READ
#undef BASIC_WRITE

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
