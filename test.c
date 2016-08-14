#include "simple_hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define TEST_SIZE 1000

int main(void)
{
    srand(time(NULL));

    /* Fill an array with random values */
    int values[TEST_SIZE];
    for (int i = 0; i < TEST_SIZE; i++ ){
        values[i] = rand() % 1000;
    }

    hash_table_t *ht = newHashTable( TEST_SIZE * 2 );
    for (int i = 0; i < TEST_SIZE; i++ ){
        hash_add_int(ht, i, &values[i]);
    }
    int magic = 999999;
    hash_add(ht, "Apple", &magic);
    int *p = (int *)hash_getItem( ht, "Apple" );
    assert( p );
    assert( *p == 999999 );
    report_collisions(ht);

    hash_free(ht);
    return 0;
}
