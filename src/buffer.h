
#ifndef OSUSIM_BUFFER_H
#define OSUSIM_BUFFER_H

#include"common.h"

typedef struct{
	uint8_t *ptr;
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


struct Diff_{
	uint32_t mods;
	double SR;
};

typedef struct{
/*
	if version is less than 20191106
	uint32_t entrysize;
*/
	union{
		OsuStr str[13];
		struct{
			OsuStr artist, uartist, title, utitle, creator, difficulty, audioFile, hash, osuFile, source, tags, font, folder;
		};
	};	

	uint8_t rankedStatus, played, isosz2, ignoreSound, ignoreSkin, disableStoryboard, disableVideo, visualOverride;
	int16_t circles, sliders, spinners, offset, onlineOffset;
	uint64_t lastModified, date, lastPlayed, lastChecked;

	/*
		ar/cs/hp/od
		uint8_t if version < 20140609
	*/
	float AR, CS, HP, OD;
	double sliderVelocity;
	int32_t
		drainTime, //seconds
		length, //milliseconds
		audioPreview, //milliseconds
		diffID, beatmapID, threadID;

	int8_t
		mode, //0 = osu!, 1 = taiko, 2 = catch, 3 = mania
		stdGrade, taikoGrade, ctbGrade, maniaGrade, maniaScrollSpeed;

	float stackLeniency;

	int32_t timingPoints;
	TimingPoint *timingPoint;

	/*
		only present if version >= 20140609
	*/
	union{
		struct{
			int32_t Std, Ctb, Taiko, Mania;
			struct Diff_ *stdSR, *taikoSR, *ctbSR, *maniaSR;
		};
		struct{
			int32_t ModeLength_[4];
			struct Diff_ *Mode_[4];
		};
	};
} Beatmap;

typedef struct{
	int32_t version, folders, perms, beatmaps;
	uint64_t unbanDate;
	uint8_t status;
	OsuStr player;
	Beatmap *beatmap;
} OsuDB;


typedef struct{
	union{
		OsuStr str[3];
		struct{
			OsuStr beatmapHash, player, hash;
		};
	};
	uint8_t FC;
	int8_t mode;
	int32_t version, score, mods;
	int64_t date, ID;
	int16_t n300, n100, n50, nGeki, nKatu, nMiss, maxCombo;
	double addModInfo; //target Practice
} Score;

typedef struct{
	OsuStr hash;
	int32_t scores;
	Score *score;
} MapScores;

typedef struct{
	int32_t version, maps;
	MapScores *map;
} ScoreDB;


typedef struct{
	OsuStr name;
	int32_t hashs;
	OsuStr *hash;
} Collection;

typedef struct{
	int32_t version, collections;
	Collection *collection;
} CollectionDB;

uint8_t parseBool(Buffer *);
int8_t  parsei8  (Buffer *);
int16_t parsei16 (Buffer *);
int32_t parsei32 (Buffer *);
int64_t parsei64 (Buffer *);
float   parsef32 (Buffer *);
double  parsef64 (Buffer *);

size_t parseULEB128(Buffer *);
OsuStr parseOsuStr(Buffer *);
void printOsuStr(OsuStr);
TimingPoint parseTimingPoint(Buffer *);

Beatmap parseBeatmap(Buffer *);
void printBeatmap(Beatmap);

Score parseScore(Buffer *);
void printScore(Score);

MapScores parseMapScores(Buffer *);
OsuDB parseOsuDB(Buffer *);

ScoreDB parseScoreDB(Buffer *);

Collection parseCollection(Buffer *);
void printCollection(Collection);

CollectionDB parseCollectionDB(Buffer *);

#endif

