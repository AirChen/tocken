//
//  hash_table.h
//  tocken
//
//  Created by tuRen on 2021/4/23.
//  https://github.com/jamesroutley/write-a-hash-table/tree/master/07-appendix demo
// https://github.com/jamesroutley/algorithms-and-data-structures/tree/master/hash-table

#ifndef hash_table_h
#define hash_table_h

#include <stdio.h>

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int base_size;
    int count;
    ht_item** items;
} ht_hash_table;

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);

void ht_del_hash_table(ht_hash_table* table);
#endif /* hash_table_h */
