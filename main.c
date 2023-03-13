#include<stdio.h>
#include"src/buffer.h"

//TODO free buffer data when done

char* readFile(char *fn, size_t *rlen) {
	FILE *f = fopen(fn, "r");
	*rlen = 0;
	if(f && !fseek(f, 0, SEEK_END)){
		long int len = ftell(f);
		if (len != -1){
			rewind(f);
			size_t length = len;
			char *buffer = malloc(length);
			if (buffer && fread(buffer, 1, length, f) == length) {
				*rlen = length;
				fclose(f);
				return buffer;
			}
			free(buffer);
		}
		fclose(f);
	}
	printf("failed to read file: \"%s\"\n", fn);
	return NULL;
}


int
main(int argc, char **argv)
{
	argc--;
	argv++;

	for(int i = 0; i < argc; i++){
		size_t len;
		Buffer buf = { .ptr = (uint8_t*)readFile(argv[i], &len) };

		/*
		//osu!.db
		OsuDB osudb = parseOsuDB(&buf);
		for(int i = 0; i < osudb.beatmaps; i++)
			printBeatmap(osudb.beatmap[i]);
		*/

		/*
		//scores.db
		ScoreDB scoredb = parseScoreDB(&buf);
		for(int j = 0; j < scoredb.maps; j++)
			for(int i = 0; i < scoredb.map[j].scores; i++)
				printScore(scoredb.map[j].score[i]);
		*/

		/*
		//collection.db
		CollectionDB db = parseCollectionDB(&buf);
		for(int i = 0; i < db.collections; i++)
			printCollection(db.collection[i]);
		*/

		//osr
		Score score = parseScore(&buf);
		printScore(score);
	}

	return 0;
}
