
#include"buffer.h"

uint8_t
parseBool(Buffer *buf){
	return *(buf->ptr++);
}

int8_t
parsei8(Buffer *buf)
{
	int8_t r = buf->ptr[0];
	buf->ptr += 1;
	return r;
}

int16_t
parsei16(Buffer *buf){
	//little endian
	int16_t r = (buf->ptr[1]<<8) + buf->ptr[0];
	buf->ptr += 2;
	return r;
}

int32_t
parsei32(Buffer *buf)
{
	int32_t r = (buf->ptr[3]<<24) + (buf->ptr[2]<<16) + (buf->ptr[1]<<8) + buf->ptr[0];
	buf->ptr += 4;
	return r;
}

int64_t
parsei64(Buffer *buf)
{
	int64_t r = ((buf->ptr[7]+0ull)<<56) + ((buf->ptr[6]+0ull)<<48) + ((buf->ptr[5]+0ull)<<40) + ((buf->ptr[4]+0ull)<<32) + (buf->ptr[3]<<24) + (buf->ptr[2]<<16) + (buf->ptr[1]<<8) + buf->ptr[0];
	buf->ptr += 8;
	return r;
}
float
parsef32 (Buffer *buf)
{
	union _ {int32_t i; float f;};
	int32_t r = parsei32(buf);
	return ((union _*)&r)->f;

}

double
parsef64(Buffer *buf)
{
	union _{int64_t i; double f;};
	int64_t r = parsei64(buf);
	return ((union _*)&r)->f;
}

size_t
parseULEB128(Buffer *buf)
{
	size_t result = 0;
	int shift = 0;
	uint8_t byte;

	do{
		byte = *(buf->ptr++);
		result |= (byte & 0x7f) << shift;
		shift += 7;
	}while(byte & 0x80);

	return result;
}

OsuStr
parseOsuStr(Buffer *buf)
{
	OsuStr r = {.start = NULL, .end = NULL};

	if(parsei8(buf) == 0x0b){
		size_t len = parseULEB128(buf);
		r.start = (char*)buf->ptr;
		buf->ptr += len;
		r.end = (char*)buf->ptr;
	}

	return r;
}

void
printOsuStr(OsuStr str)
{
	printf("%.*s", (int)(str.end - str.start), str.start);
}

TimingPoint
parseTimingPoint(Buffer *buf)
{
	TimingPoint r;
	r.bpm = parsef64(buf);
	r.offset = parsef64(buf);
	r.inherit = parseBool(buf);
	return r;
}

Beatmap
parseBeatmap(Buffer *buf)
{
	Beatmap r;
	r.artist = parseOsuStr(buf);
	r.uartist = parseOsuStr(buf);
	r.title = parseOsuStr(buf);
	r.utitle = parseOsuStr(buf);
	r.creator = parseOsuStr(buf);
	r.difficulty = parseOsuStr(buf);
	r.audioFile = parseOsuStr(buf);
	r.hash = parseOsuStr(buf);
	r.osuFile = parseOsuStr(buf);
	r.rankedStatus = parseBool(buf);
	r.circles = parsei16(buf);
	r.sliders = parsei16(buf);
	r.spinners = parsei16(buf);
	r.lastModified = parsei64(buf);
	r.AR = parsef32(buf);
	r.CS = parsef32(buf);
	r.HP = parsef32(buf);
	r.OD = parsef32(buf);
	r.sliderVelocity = parsef64(buf);

	for(int i = 0; i < 4; i++){
		r.ModeLength_[i] = parsei32(buf);
		r.Mode_[i] = (struct Diff_*)malloc(r.ModeLength_[i] * sizeof(struct Diff_));
		for(int j = 0; j < r.ModeLength_[i]; j++){
			parsei8(buf);
			r.Mode_[i][j].mods = parsei32(buf);
			parsei8(buf);
			r.Mode_[i][j].SR = parsef64(buf);
		}
	}

	r.drainTime = parsei32(buf);
	r.length = parsei32(buf);
	r.audioPreview = parsei32(buf);

	r.timingPoints = parsei32(buf);
	r.timingPoint = (TimingPoint *)malloc(r.timingPoints * sizeof(TimingPoint));
	for(int i = 0; i < r.timingPoints; i++)
		r.timingPoint[i] = parseTimingPoint(buf);

	r.diffID = parsei32(buf);
	r.beatmapID = parsei32(buf);
	r.threadID = parsei32(buf);
	r.stdGrade = parsei8(buf);
	r.taikoGrade = parsei8(buf);
	r.ctbGrade = parsei8(buf);
	r.maniaGrade = parsei8(buf);
	r.offset = parsei16(buf);
	r.stackLeniency = parsef32(buf);
	r.mode = parsei8(buf);
	r.source = parseOsuStr(buf);
	r.tags = parseOsuStr(buf);
	r.onlineOffset = parsei16(buf);
	r.font = parseOsuStr(buf);
	r.played = parseBool(buf);
	r.lastPlayed = parsei64(buf);
	r.isosz2 = parseBool(buf);
	r.folder = parseOsuStr(buf);
	r.lastChecked = parsei64(buf);
	r.ignoreSound = parseBool(buf);
	r.ignoreSkin = parseBool(buf);
	r.disableStoryboard = parseBool(buf);
	r.disableVideo = parseBool(buf);
	r.visualOverride = parseBool(buf);
	parsei32(buf);
	r.maniaScrollSpeed = parsei8(buf);

	return r;
}

void
printBeatmap(Beatmap bm)
{
	printf("Artist: ");
	printOsuStr(bm.artist);
	printf("\ntitle: ");
	printOsuStr(bm.title);
	printf("\ncreator: ");
	printOsuStr(bm.creator);
	printf("\ndiff: ");
	printOsuStr(bm.difficulty);
	printf("\nhash: ");
	printOsuStr(bm.hash);
	printf("\nfolder: ");
	printOsuStr(bm.folder);
	printf("\nlength: %d drainTime: %d: \n", bm.length, bm.drainTime);
	printf("diffID: %u beatmapID: %u threadID: %u\n", bm.diffID, bm.beatmapID, bm.threadID);
	printf("mode: %d\n\n", bm.mode);
}

Score
parseScore(Buffer *buf)
{
	Score r;
	r.mods = 0;
	r.mode = parsei8(buf);
	r.version = parsei32(buf);

	r.beatmapHash = parseOsuStr(buf);
	r.player = parseOsuStr(buf);
	r.hash = parseOsuStr(buf);
	r.n300 = parsei16(buf);
	r.n100 = parsei16(buf);
	r.n50 = parsei16(buf);
	r.nGeki = parsei16(buf);
	r.nKatu = parsei16(buf);
	r.nMiss = parsei16(buf);
	r.score = parsei32(buf);
	r.maxCombo = parsei16(buf);
	r.FC = parseBool(buf);
	r.mods = parsei32(buf);

	parseOsuStr(buf); //empty string
	r.date = parsei64(buf);
	r.length = parsei32(buf);
	if(r.length != -1){
		r.replayData = buf->ptr;
		buf->ptr += r.length;
	}

	r.ID = parsei64(buf);

	if(r.mods & (1<<23)){ //target practice
		r.addModInfo = parsef64(buf);
	}

	return r;
}

void
printScore(Score score){
	printf("version: %d\nBeatmap: ", score.version);
	printOsuStr(score.beatmapHash);
	printf("\nPlayer: ");
	printOsuStr(score.player);
	printf("\nReplay: ");
	printOsuStr(score.hash);
	printf("\nmode: %d\n", score.mode);
	printf("300: %d 100: %d 50: %d Miss:%d\n", score.n300, score.n100, score.n50, score.nMiss);
	printf("Geki: %d Katu: %d\n", score.nGeki, score.nKatu);
	printf("maxCombo: %d FC: %d\n\n", score.maxCombo, score.FC);
}

