#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "simple_hash.h"

#define BUF_LEN 20

hash_table_t *newHashTable( size_t n )
{
    hash_table_t *t = malloc(sizeof(hash_table_t));
    if (!t)
        return NULL;
    t->capacity = n;
    t->n_entries = 0;
    t->arr = malloc( n * sizeof( void * ) );
    if (!t->arr)
        return NULL;

    return t;
}

/* forward declarations */
unsigned long djb2a_hash( unsigned char *str );
int _findEmptyBucket( hash_table_t *t, int idx );

int hash_add(hash_table_t *table, unsigned char *key, void *data )
{
    assert(table && key && data);
    if( table->n_entries == table->capacity ){
        printf("table full\n");
        return 1;
    }

    int idx = (int)djb2a_hash(key) % table->capacity;
    if (table->arr[idx].data != NULL){
        idx = _findEmptyBucket( table, idx );
    }
    
    strncpy( table->arr[idx].key, key, BUF_LEN );
    table->arr[idx].data = data;

    return 0;
}

int hash_add_int(hash_table_t *table, int num, void *data)
{
    char str[BUF_LEN];
    /* Convert our integer into a string for hash_add */
    snprintf( str, BUF_LEN, "%d", num );
    
    return hash_add( table, str, data );
}

void *hash_getItem( hash_table_t *t, char *key )
{
    int off = 0;
    int idx = (int)djb2a_hash(key) % t->capacity;

    struct hash_item_t *item = NULL;
    while (item == NULL){
        if( idx + off > t->capacity && idx - off < 0 ) {
            return NULL;
        }

        if( idx + off <= t->capacity ){
            if (strcmp( t->arr[idx + off].key, key) == 0) {
                item = t->arr[idx+off].data;
                continue;
            }
        }
        if( idx - off >= 0 ){
            if (strcmp( t->arr[idx - off].key, key) == 0) {
                item = t->arr[idx - off].data;
                continue;
            }
        }
        off += 1;
    }
    return item;
}

unsigned long djb2a_hash( unsigned char *str )
{
    unsigned long hash = 5381;
    int c;
    while ( c = *str++ ){
        hash = hash * 33 ^ c;
    }

    return hash;
}

/* find an empty bucket with open addressing */
int _findEmptyBucket( hash_table_t *t, int idx )
{    
    int off = 1;
    for(;;){
        if( idx + off > t->capacity && idx - off < 0 ) {
                return -1;
        }
        if( idx + off <= t->capacity ){
            if( t->arr[idx + off].key == NULL )
                return idx + off;
        }
        if( idx - off >= 0 ){
            if( t->arr[idx - off].key == NULL )
                return idx - off;
        }
        off += 1;
    }
}


int hash_lookup( char *key );
int hash_check( char *key);
int hash_handleCollision( int idx );
