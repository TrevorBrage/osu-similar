
#include"buffer.h"

MapScores
parseMapScores(Buffer *buf)
{
	MapScores r;
	r.hash = parseOsuStr(buf);
	r.scores = parsei32(buf);

	r.score = malloc(r.scores * sizeof(Score));
	for(int i = 0; i < r.scores; i++)
		r.score[i] = parseScore(buf);

	return r;
}

ScoreDB
parseScoreDB(Buffer *buf)
{
	ScoreDB r;
	r.version = parsei32(buf);
	r.maps = parsei32(buf);

	r.map = malloc(r.maps * sizeof(MapScores));
	for(int i = 0; i < r.maps; i++)
		r.map[i] = parseMapScores(buf);

	return r;
}

