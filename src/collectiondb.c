
#include"buffer.h"

Collection
parseCollection(Buffer *buf)
{
	Collection r;
	r.name = parseOsuStr(buf);
	r.hashs = parsei32(buf);

	r.hash = malloc(r.hashs * sizeof(OsuStr));
	for(int i = 0; i < r.hashs; i++)
		r.hash[i] = parseOsuStr(buf);

	return r;
}

void
printCollection(Collection col)
{
	printf("Name: ");
	printOsuStr(col.name);
	printf("\nbeatmaps:\n");
	for(int i = 0; i < col.hashs; i++){
		printOsuStr(col.hash[i]);
		printf("\n");
	}
	printf("\n");
}

CollectionDB
parseCollectionDB(Buffer *buf)
{
	CollectionDB r;
	r.version = parsei32(buf);
	r.collections = parsei32(buf);

	r.collection = malloc(r.collections * sizeof(Collection));
	for(int i = 0; i < r.collections; i++)
		r.collection[i] = parseCollection(buf);

	return r;
}

