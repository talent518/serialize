#ifndef _SERIALIZE_H
#define _SERIALIZE_H

#include <stddef.h>
#include <stdbool.h>
#include <glib.h>

#define SRL_TYPE_BOOLEAN			'b'
#define SRL_TYPE_CHAR				'c'
#define SRL_TYPE_BYTE				'y'
#define SRL_TYPE_UBYTE			'Y'
#define SRL_TYPE_SHORT			't'
#define SRL_TYPE_USHORT			'T'
#define SRL_TYPE_INT				'i'
#define SRL_TYPE_UINT				'I'
#define SRL_TYPE_LONG				'l'
#define SRL_TYPE_ULONG			'L'
#define SRL_TYPE_FLOAT			'f'
#define SRL_TYPE_DOUBLE			'd'
#define SRL_TYPE_LONG_DOUBLE	'D'
#define SRL_TYPE_STRING			's'
#define SRL_TYPE_STRING_LENGTH	'S'
#define SRL_TYPE_OBJECT			'o'
#define SRL_TYPE_ARRAY			'a'
#define SRL_TYPE_ARRLEN			'A'
#define SRL_TYPE_HASHTABLE		'h'

#define _SFT_ARG8(type,object,property,length,size,memsize,format,isint,func,help) {#property, sizeof(#property)-1, type, isint, offsetof(object,property), help, length, size, memsize, format, func}
#define _SFT_ARG7(type,object,property,length,size,memsize,format,help) _SFT_ARG8(type, object, property, length, size, memsize, format, false, NULL, help)
#define _SFT_ARG6(type,object,property,length,size,help) _SFT_ARG7(type, object, property, offsetof(object, length), size, 0, NULL, help)
#define _SFT_ARG4(type,object,property,help) _SFT_ARG7(type, object, property, 0, 0, 0, NULL, help)

#define SFT_END {NULL,0}

typedef bool bool_t;
typedef char char_t;
typedef char byte_t;
typedef unsigned char ubyte_t;
typedef short short_t;
typedef unsigned short ushort_t;
typedef int int_t;
typedef unsigned int uint_t;
typedef long int long_t;
typedef unsigned long int ulong_t;
typedef float float_t;
typedef double double_t;
typedef long double longdouble_t;

typedef unsigned int length_t;

#define SFT_BOOL(object,property,help) _SFT_ARG4(SRL_TYPE_BOOLEAN, object, property, help)
#define SFT_CHAR(object,property,help) _SFT_ARG4(SRL_TYPE_CHAR, object, property, help)
#define SFT_BYTE(object,property,help) _SFT_ARG4(SRL_TYPE_BYTE, object, property, help)
#define SFT_UBYTE(object,property,help) _SFT_ARG4(SRL_TYPE_UBYTE, object, property, help)
#define SFT_SHORT(object,property,help) _SFT_ARG4(SRL_TYPE_SHORT, object, property, help)
#define SFT_USHORT(object,property,help) _SFT_ARG4(SRL_TYPE_USHORT, object, property, help)
#define SFT_INT(object,property,help) _SFT_ARG4(SRL_TYPE_INT, object, property, help)
#define SFT_UINT(object,property,help) _SFT_ARG4(SRL_TYPE_UINT, object, property, help)
#define SFT_LONG(object,property,help) _SFT_ARG4(SRL_TYPE_LONG, object, property, help)
#define SFT_ULONG(object,property,help) _SFT_ARG4(SRL_TYPE_ULONG, object, property, help)
#define SFT_FLOAT(object,property,help) _SFT_ARG4(SRL_TYPE_FLOAT, object, property, help)
#define SFT_DOUBLE(object,property,help) _SFT_ARG4(SRL_TYPE_DOUBLE,object, property, help)
#define SFT_LONG_DOUBLE(object,property,help) _SFT_ARG4(SRL_TYPE_LONG_DOUBLE, object, property, help)

#define SFT_STR(object,property,length,help) _SFT_ARG6(SRL_TYPE_STRING, object, property, length, 0, help)
#define SFT_STREX(object,property,length,help) _SFT_ARG6(SRL_TYPE_STRING_LENGTH, object, property, length, offsetof(object,length)-offsetof(object,property)-1, help)
#define SFT_STRLEN(object,property,length,size,help) _SFT_ARG6(SRL_TYPE_STRING_LENGTH, object, property, length, size, help)

#define SFT_OBJECT(object,property,format,help) _SFT_ARG7(SRL_TYPE_OBJECT, object, property, 0, 0, 0, format, help)

#define SFT_ARRAY(object,property,length,datatype,format,help) _SFT_ARG7(SRL_TYPE_ARRAY, object, property, offsetof(object,length), 0, sizeof(datatype), format, help)
#define SFT_ARRLEN(object,property,length,size,datatype,format,help) _SFT_ARG7(SRL_TYPE_ARRLEN, object, property, offsetof(object,length), size, sizeof(datatype), format, help)

#define SFT_HASHTABLE(object,property,datatype,isint,func,format,help) _SFT_ARG8(SRL_TYPE_HASHTABLE, object, property, 0, 0, sizeof(datatype), format, isint, func, help)

typedef struct
{
	void *null;
} null_t;

typedef struct _serialize_format_t serialize_format_t;
typedef struct _serialize_object_t serialize_object_t;

struct _serialize_object_t {
	serialize_format_t *formats;
	length_t nsize;
	GHashTable *ht;
};

struct _serialize_format_t {
	char *key;
	length_t keylen;

	char type;
	bool isint;

	length_t offset;

	char *help;

	length_t offsetlen;
	length_t vallen;

	length_t memsize;
	union{
		serialize_format_t *format;
		serialize_object_t *object;
	};

	GDestroyNotify func;
};

void serialize_object_init(serialize_object_t *format);
void serialize_object_destory(serialize_object_t *format);

unsigned int serialize_parse(void *obj, serialize_format_t *format, const char *buffer, length_t buflen);
length_t serialize_parse_object(void *obj, serialize_object_t *format, const char *buffer, length_t buflen);

length_t serialize_string(void *obj, serialize_format_t *format, char **buffer);
length_t serialize_string_object(void *obj, serialize_object_t *format, char **buffer);

void serialize_string_ex(GString *gstr, void *obj, serialize_format_t *format);
void serialize_string_object_ex(GString *gstr, void *obj, serialize_object_t *format);

#endif
