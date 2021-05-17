#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/random.h>
#include <uv.h>

#define _unused_ __attribute__((unused))
#define _cleanup_(x) __attribute__((__cleanup__(x)))
#define xmalloc(size) verbose_xmalloc(size, __FILE__, __LINE__)
#define xrealloc(size) verbose_xrealloc(ptr, size, __FILE__, __LINE__)
#define LOG_DEBUG(...) \
	do { \
		fprintf(stdout, "DEBUG:%s:%d: ", __FILE__, __LINE__); \
		fprintf(stdout, __VA_ARGS__); \
		fprintf(stdout, "\n"); \
	} while(0)
#define LOG_WARN(...) \
	do { \
		fprintf(stdout, "WARNING:%s:%d: ", __FILE__, __LINE__); \
		fprintf(stdout, __VA_ARGS__); \
		fprintf(stdout, "\n"); \
	} while(0)
#define LOG_ERROR(...) \
	do { \
		fprintf(stderr, "ERROR:%s:%d: ", __FILE__, __LINE__); \
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
		exit(EXIT_FAILURE); \
	} while(0)

_unused_ static void on_uv_debug_walk(uv_handle_t *handle, void *arg) {
	LOG_DEBUG(
		"handle: %s, active: %d, closing: %d",
		uv_handle_type_name(uv_handle_get_type(handle)),
		uv_is_active(handle),
		uv_is_closing(handle)
	);
}

_unused_ static inline void *verbose_xmalloc(size_t size, const char *src, int line) {
	void *r = malloc(size);
	if (r == NULL) {
		LOG_ERROR("%s:%d: Failed to allocate memory\n", src, line);
	}
	return r;
}

_unused_ static void *verbose_xrealloc(void *ptr, size_t size, const char *src, int line) {
	void *r = realloc(ptr, size);
	if (r == NULL && size != 0) {
		LOG_ERROR("%s:%d Failed to reallocate memory", src, line);
	}
	return r;
}
