#include<stddef.h>
#include<stdlib.h>
#include"hashtable.h"

HashTable*
allocHashTable(size_t size, size_t itemsz)
{
	HashTable *table = malloc(sizeof(HashTable) + itemsz*size);
	table->size = size;
	table->itemsz = itemsz;
	return table;
}

void
freeHashTable(HashTable *table)
{
	free(table);
}

void*
getTablePtr(HashTable *table, void *val, size_t hash, int (*cmpfn)(void*, void*))
{
	size_t retry = 0;
	while(1){
		size_t index = ((hash + retry) % table->size) * table->itemsz;
		if(cmpfn(val, table->data + index))
			return table->data + index;
		retry++;
	}
}

void 
mapHashTable(HashTable *table, void (*fn)(void*))
{
	size_t len = table->size * table->itemsz;
	for(size_t i = 0; i < len; i += table->itemsz)
		fn(table->data + i);
}

