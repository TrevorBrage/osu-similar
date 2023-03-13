
#ifndef OSUSIM_BUFFER_H
#define OSUSIM_BUFFER_H

#include"common.h"

typedef struct{
	char *ptr;
} Buffer;

typedef struct{
	char *start, *end;
} OsuStr;

typedef struct{
	double
		bpm,
		offset; //milliseconds

	uint8_t inherit;
} TimingPoint;


typedef struct{
/*
	if version is less than 20191106
	uint32_t entrysize;
*/
	union{
		OsuStr str[9];
		struct{
			OsuStr artist, uartist, title, utitle, creator, difficulty, audioFile, hash, osuFile;
		};
	};	

	uint8_t rankedStatus;
	int16_t circles, sliders;
	uint64_t lastModified, date;

	/*
		ar/cs/hp/od
		uint8_t if version < 20140609
	*/
	float AR, CS, HP, OD;
	double sliderVelocity;
	int32_t
		drainTime, //seconds
		length, //milliseconds
		audioPreview; //milliseconds

	int32_t timingPoints;
	TimingPoint *timingPoint;

	/*
		only present if version >= 20140609
	*/
	int32_t Std, Ctb, Taiko, Mania;
	struct{
		uint32_t i;
		double d;
	} *SR;
} Beatmap;

typedef struct{
	int32_t version, folderCount, perms, length;
	uint64_t unbanDate;
	uint8_t status;
	OsuStr player;
	Beatmap beatmap[];
} OsuDB;


typedef struct{
	//TODO
} Score;

typedef struct{
	//TODO
} MapScores;

typedef struct{
	//TODO
} ScoreDBHeader;

typedef struct{
	//TODO
} ScoreDB;

uint8_t parseBool(Buffer *);
int8_t  parsei8  (Buffer *);
int16_t parsei16 (Buffer *);
int32_t parsei32 (Buffer *);
int64_t parsei64 (Buffer *);
float   parsef32 (Buffer *);
double  parsef64 (Buffer *);

size_t parseULEB128(Buffer *);
OsuStr parseOsuStr(Buffer *);
TimingPoint parseTimingPoint(Buffer *);
Beatmap parseBeatmap(Buffer *);

OsuDB parseOsuDB(Buffer *);

#endif

