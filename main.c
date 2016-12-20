#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#include "serialize.h"

#ifdef BENCH
	#define INIT_RUNTIME() double runtime=microtime(),tmpruntime,runtimes=0
	#define INFO_RUNTIME(info) tmpruntime=microtime();runtimes+=tmpruntime-runtime;printf("[ " info " ] %20s: %.3fs\n", __func__, tmpruntime-runtime);runtime=tmpruntime
	#define AVG_RUNTIME(times) printf("avg time: %0.3f\n", runtimes/times);
	#define pprintf(...);
#else
	#define pprintf(...) printf(__VA_ARGS__)
#endif

#define MICRO_IN_SEC 1000000.00

double microtime()
{
	struct timeval tp = {0};

	if (gettimeofday(&tp, NULL)) {
		return 0;
	}

	return (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
}

typedef struct
{
	bool_t boolean;
	int_t integer;
	uint_t uinteger;
} object_t;

typedef struct
{
	bool_t boolean;
	char_t chr;
	byte_t byte;
	ubyte_t ubyte;
	short_t shortint;
	ushort_t ushortint;
	int_t integer;
	uint_t uinteger;
	long_t longint;
	ulong_t ulongint;
	float_t nfloat;
	double_t ndouble;
	longdouble_t longdouble;
	char *string;
	size_t string_length;
	char strings[101];
	size_t strings_length;
	object_t object;
	object_t *objects;
	size_t objects_length;
	int_t ints[10];
	size_t ints_length;
	GHashTable *ht;
} test_t;

static serialize_format_t object_format[] = {
	SFT_BOOL(object_t, boolean, "object_t of boolean"),
	SFT_INT(object_t, integer, "object_t of integer"),
	SFT_UINT(object_t, uinteger, "object_t of uinteger"),
	SFT_END
};

static serialize_object_t object_object = {object_format, NULL};

static serialize_format_t objects_format = SFT_OBJECT(null_t, null, &object_object, "object_t of objects_format");
static serialize_format_t ints_format = SFT_INT(null_t, null, "int of ints_format");

static serialize_format_t test_format[] = {
	SFT_BOOL(test_t, boolean, "test_t of boolean"),
	SFT_CHAR(test_t, chr, "test_t of chr"),
	SFT_BYTE(test_t, byte, "test_t of byte"),
	SFT_UBYTE(test_t, ubyte, "test_t of ubyte"),
	SFT_SHORT(test_t, shortint, "test_t of shortint"),
	SFT_USHORT(test_t, ushortint, "test_t of ushortint"),
	SFT_INT(test_t, integer, "test_t of integer"),
	SFT_UINT(test_t, uinteger, "test_t of uinteger"),
	SFT_LONG(test_t, longint, "test_t of longint"),
	SFT_ULONG(test_t, ulongint, "test_t of ulongint"),
	SFT_FLOAT(test_t, nfloat, "test_t of nfloat"),
	SFT_DOUBLE(test_t, ndouble, "test_t of ndouble"),
	SFT_LONG_DOUBLE(test_t, longdouble, "test_t of longdouble"),
	SFT_STR(test_t, string, string_length, "test_t of string"),
	SFT_STRLEN(test_t, strings, strings_length, 100, "test_t of strings"),
	SFT_OBJECT(test_t, object, &object_object, "test_t of object"),
	SFT_ARRAY(test_t, objects, objects_length, object_t, &objects_format, "test_t of objects"),
	SFT_ARRLEN(test_t, ints, ints_length, 10, int, &ints_format, "test_t of ints"),
	SFT_HASHTABLE(test_t, ht, object_t, true, (GDestroyNotify)free, &objects_format, "test_t of ht"),
	SFT_END
};

static serialize_object_t test_object = {test_format, NULL};

static void foreach_ht(unsigned int *key, object_t *value, void *user_data) {
	pprintf("---------------------\n");
	pprintf("key: %d\n", *key);
	pprintf("_____________________\n");
	pprintf("object.boolean: %d\n", value->boolean);
	pprintf("object.integer: %d\n", value->integer);
	pprintf("object.uinteger: %u\n", value->uinteger);
}

void srl_test(test_t *test) {
	pprintf("boolean: %d\n",test->boolean);
	pprintf("chr: %c\n",test->chr);
	pprintf("byte: %d\n",test->byte);
	pprintf("ubyte: %u\n",test->ubyte);
	pprintf("shortint: %d\n",test->shortint);
	pprintf("ushortint: %u\n",test->ushortint);
	pprintf("integer: %d\n",test->integer);
	pprintf("uinteger: %u\n",test->uinteger);
	pprintf("longint: %ld\n",test->longint);
	pprintf("ulongint: %lld\n",test->ulongint);
	pprintf("nfloat: %lf\n",test->nfloat);
	pprintf("ndouble: %lf\n",test->ndouble);
	pprintf("longdouble: %llf\n",test->longdouble);
	pprintf("string: %s\n",test->string);
	pprintf("string_length: %d\n",test->string_length);
	pprintf("strings: %s\n",test->strings);
	pprintf("strings_length: %d\n",test->strings_length);

	pprintf("---------------------\n");
	pprintf("object.boolean: %d\n",test->object.boolean);
	pprintf("object.integer: %d\n",test->object.integer);
	pprintf("object.uinteger: %u\n",test->object.uinteger);

	pprintf("---------------------\n");
	pprintf("objects_length: %d\n",test->objects_length);
	size_t i;
	for(i=0; i< test->objects_length; i++) {
		pprintf("objects[%d].boolean: %d\n", i, test->objects[i].boolean);
		pprintf("objects[%d].integer: %d\n", i, test->objects[i].integer);
		pprintf("objects[%d].uinteger: %u\n", i, test->objects[i].uinteger);
	}

	pprintf("---------------------\n");
	pprintf("ints_length: %d\n",test->ints_length);
	for(i=0; i< test->ints_length; i++) {
		pprintf("ints[%d]: %d\n", i, test->ints[i]);
	}

	if(test->ht)
		g_hash_table_foreach(test->ht, foreach_ht, NULL);
}

void run() {
	char data[]="s:7:boolean;b:1;s:3:chr;c:z;s:4:byte;y:123;s:5:ubyte;Y:123;s:8:shortint;t:30000;s:9:ushortint;T:60000;s:7:integer;i:123;s:8:uinteger;I:123;s:7:longint;l:-99999999999;s:8:ulongint;L:999999999999999;s:6:nfloat;f:888999.12345;s:7:ndouble;d:99999.123456789;s:10:longdouble;D:99999.123456789;s:6:string;s:4:test;s:7:strings;S:132:abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzab;s:6:object;o:{s:7:boolean;b:1;s:7:integer;i:4321;s:8:uinteger;I:1234;}s:7:objects;a:3[o:{s:7:boolean;b:1;s:7:integer;i:111;s:8:uinteger;I:222;}o:{s:7:boolean;b:0;s:7:integer;i:333;s:8:uinteger;I:444;}o:{s:7:boolean;b:1;s:7:integer;i:555;s:8:uinteger;I:666;}]s:4:ints;A:2[i:111;i:222;]s:2:ht;h:(I:12;o:{s:7:boolean;b:1;s:7:integer;i:111;s:8:uinteger;I:222;}I:34;o:{s:7:boolean;b:0;s:7:integer;i:333;s:8:uinteger;I:444;}I:56;o:{s:7:boolean;b:1;s:7:integer;i:555;s:8:uinteger;I:666;})s:1:i;a:2[i:234;s:3:abc;s:3:234;o:{s:3:234;a:0[]s:1:o;o:{}}]s:3:chr;c:Z;";
	test_t test={0};
	size_t parselen;

	memset(&test, 0, sizeof(test_t));

	pprintf("data: %s\n",data);
	pprintf("datalen: %d\n",sizeof(data)-1);
	pprintf("=====================\n");
	parselen = serialize_parse_object((void*)&test, &test_object, data, strlen(data));
	pprintf("parses: %d\n", parselen);
	pprintf("=====================\n");

	srl_test(&test);

	pprintf("=====================\n");
	pprintf("=====================\n");

	char *buffer = NULL;
	size_t buflen = serialize_string_object(&test, &test_object, &buffer);
	pprintf("=====================\n");
	pprintf("buflen: %d\nbuffer: %s\n", buflen, buffer);
	pprintf("=====================\n");

	test_t test2;
	memset(&test2, 0, sizeof(test_t));
	serialize_parse_object((void*)&test2, &test_object, buffer, buflen);
	free(buffer);

	pprintf("=====================\n");
	pprintf("=====================\n");

	srl_test(&test2);

	free(test.string);
	free(test.objects);
	g_hash_table_destroy(test.ht);

	free(test2.string);
	free(test2.objects);
	g_hash_table_destroy(test2.ht);
}

int main(int argc, char *argv[]) {
	serialize_object_init(&object_object);
	serialize_object_init(&test_object);
#ifdef BENCH
	int i,j;
	int times=100;
	INIT_RUNTIME();

	for(j=0; j<times; j++) {
		for(i=0; i< 10000; i++) {
			run();
		}
		INFO_RUNTIME("run time");
	}
	AVG_RUNTIME(times);
#else
	run();
#endif
	serialize_object_destory(&object_object);
	serialize_object_destory(&test_object);
	return 0;
}
