/*
 * Simple Hash
 * David Bergeron 2016
 * Uses djb2a hash function and open addressing.
 * Relies on generic data type (ie. void * )
 */
#ifndef _SIMPLE_HASH_H_
#define _SIMPLE_HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 20

struct hash_item_t {
    char key[BUF_LEN];
    void *data;
};

typedef struct hash_table_t {
    struct hash_item_t *arr;
    int capacity;
    int n_items;
} hash_table_t;

hash_table_t *newHashTable( size_t n_buckets );

int hash_free( hash_table_t *table );

int hash_add(hash_table_t *table, unsigned char *key, void *data );

int hash_add_int(hash_table_t *table, int num, void *data );

void *hash_getItem( hash_table_t *t, char *key );

void report_collisions(hash_table_t *t);

#endif
