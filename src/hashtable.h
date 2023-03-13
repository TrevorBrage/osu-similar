#include<stddef.h>

typedef struct HashTable{
	size_t size, itemsz;
	char data[];
} HashTable;

HashTable *allocHashTable(size_t size, size_t itemsz);
void freeHashTable(HashTable *table);
void *getTablePtr(HashTable *table, void *val, size_t hash, int (*cmpfn)(void*,void*));
void mapHashTable(HashTable *table, void (*fn)(void*));
