/*
 * Lightweight replacements for the tiny portion of GLib used by the original
 * codebase.  This header provides drop-in stand‑ins so that the rest of the
 * project can keep using the familiar g_* helpers without pulling the real
 * GLib dependency.
 */

#ifndef MTP_GCOMPAT_H
#define MTP_GCOMPAT_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void *g_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr && size != 0) {
		fprintf(stderr, "g_malloc: out of memory (size=%zu)\n", size);
		abort();
	}
	return ptr;
}

static inline void *g_malloc0(size_t size)
{
	void *ptr = calloc(1, size);
	if (!ptr && size != 0) {
		fprintf(stderr, "g_malloc0: out of memory (size=%zu)\n", size);
		abort();
	}
	return ptr;
}

static inline void *g_realloc(void *mem, size_t size)
{
	void *ptr = realloc(mem, size);
	if (!ptr && size != 0) {
		fprintf(stderr, "g_realloc: out of memory (size=%zu)\n", size);
		abort();
	}
	return ptr;
}

static inline void g_free(void *ptr)
{
	free(ptr);
}

static inline char *g_strdup(const char *str)
{
	if (!str)
		return NULL;
	char *dup = strdup(str);
	if (!dup) {
		fprintf(stderr, "g_strdup: out of memory\n");
		abort();
	}
	return dup;
}

static inline size_t g_strlcpy(char *dest, const char *src, size_t size)
{
	size_t src_len = src ? strlen(src) : 0;
	if (!dest || size == 0) {
		return src_len;
	}

	size_t copy_len = (src_len >= size) ? size - 1 : src_len;
	if (src && copy_len > 0) {
		memcpy(dest, src, copy_len);
	}
	dest[copy_len] = '\0';

	return src_len;
}

static inline size_t g_strlcat(char *dest, const char *src, size_t size)
{
	size_t dest_len = dest ? strnlen(dest, size) : 0;
	size_t src_len = src ? strlen(src) : 0;

	if (dest_len >= size) {
		return dest_len + src_len;
	}

	size_t space_left = size - dest_len - 1;
	size_t copy_len = (src_len > space_left) ? space_left : src_len;
	if (copy_len > 0 && dest && src) {
		memcpy(dest + dest_len, src, copy_len);
	}
	if (dest && size > 0) {
		dest[dest_len + copy_len] = '\0';
	}

	return dest_len + src_len;
}

static inline int g_strcmp0(const char *str1, const char *str2)
{
	if (str1 == str2)
		return 0;
	if (!str1)
		return -1;
	if (!str2)
		return 1;
	return strcmp(str1, str2);
}

static inline char *g_strrstr(const char *haystack, const char *needle)
{
	if (!haystack || !needle)
		return NULL;
	if (*needle == '\0')
		return (char *)(haystack + strlen(haystack));

	const char *result = NULL;
	const char *cursor = haystack;

	while ((cursor = strstr(cursor, needle)) != NULL) {
		result = cursor;
		cursor++;
	}

	return (char *)result;
}

#define g_snprintf snprintf

#endif /* MTP_GCOMPAT_H */
