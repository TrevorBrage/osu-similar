
#include"buffer.h"

OsuDB
parseOsuDB(Buffer *buf)
{
	OsuDB r;
	r.version = parsei32(buf);
	r.folders = parsei32(buf);
	r.status = parseBool(buf);
	r.unbanDate = parsei64(buf);
	r.player = parseOsuStr(buf);
	r.beatmaps = parsei32(buf);

	r.beatmap = malloc(r.beatmaps * sizeof(Beatmap));
	for(int i = 0; i < r.beatmaps; i++)
		r.beatmap[i] = parseBeatmap(buf);

	r.perms = parsei32(buf);
	return r;
}

