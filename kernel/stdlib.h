
#ifndef STDLIB_H
#define STDLIB_H

#define NULL ((void*)0)

typedef enum {
	false = 0,
	true = 1
} bool;

typedef unsigned char uint8;
typedef signed char int8;

typedef unsigned short uint16;
typedef signed short int16;

typedef unsigned long uint32;
typedef signed long int32;

typedef unsigned long long uint64;
typedef signed long long int64;

#ifdef __x86_64__
	typedef uint64 size_t;
#else
	typedef uint32 size_t;
#endif

	void memset(void *mem, char value, size_t count);
	void memset_word(void *mem, uint16 value, size_t count);
	void memcpy(void *dest, void *src, size_t count);
	int memcmp(void *mem1, void *mem2, size_t count);
	void *memchr(void *mem, char value, size_t count);

#endif


