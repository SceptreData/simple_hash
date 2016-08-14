#include <assert.h>
#include "simple_hash.h"

/* forward declarations */
unsigned long djb2a_hash( unsigned char *str );
int _findEmptyBucket( hash_table_t *t, int idx );

int collisions = 0;

hash_table_t *newHashTable( size_t n )
{
    hash_table_t *t = malloc(sizeof(hash_table_t));
    if (!t)
        return NULL;
    t->capacity = n;
    t->n_items = 0;
    t->arr = calloc( n, sizeof( struct hash_item_t ));
    if (!t->arr)
        return NULL;

    return t;
}

int hash_free( hash_table_t *ht )
{
    if( ht == NULL ) {
        return 1;
    }

    free(ht->arr);
    free(ht);

    return 0;
}

int hash_add(hash_table_t *table, unsigned char *key, void *data )
{
    assert(table && key && data);
    if( table->n_items == table->capacity ){
        printf("table full\n");
        return 1;
    }

    int idx = (int)djb2a_hash(key) % table->capacity;
    if (table->arr[idx].data != NULL){
        collisions++;
        idx = _findEmptyBucket( table, idx );
    }
   
    if ( idx < 0 ){
        return 1;
    }

    strncpy( table->arr[idx].key, key, BUF_LEN );
    table->arr[idx].data = data;
    table->n_items++;

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
    int bucket_index = -1;

    while( bucket_index < 0 ){

        /* If we've looked through the whole table, return error. */
        if( idx + off > t->capacity && idx - off < 0 ) {
                return -1;
        } else {
            /* Search left and right of our start idx for an empty bucket */
            if( idx + off <= t->capacity ){
                if( t->arr[idx + off].data == NULL ){
                    bucket_index = idx + off;
                    continue;
                }
            }
            if( idx - off >= 0 ){
                if( t->arr[idx - off].data == NULL ){
                    bucket_index = idx - off;
                    continue;
                }
            }
            off += 1;
        }
    }

    return bucket_index;
}

void report_collisions(hash_table_t *t)
{
    float percent_filled = (float)t->n_items / (float)t->capacity;
    printf("Total Collisions: %d\ntable size: %d percent_filled: %.2f\n",
            collisions, t->capacity, percent_filled );
}

int hash_lookup( char *key );
int hash_check( char *key);
int hash_handleCollision( int idx );
