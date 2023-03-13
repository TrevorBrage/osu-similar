
#include"buffer.h"

uint8_t
parseBool(Buffer *buf){
	return *(buf->ptr++);
}

int8_t
parsei8(Buffer *buf)
{
	return *(buf->ptr++);
}

int16_t
parsei16(Buffer *buf){
	//little endian
	int16_t r = (buf->ptr[0]<<8) + buf->ptr[1];
	buf->ptr += 2;
	return r;
}

int32_t
parsei32(Buffer *buf)
{
	int32_t r = (buf->ptr[0]<<24) + (buf->ptr[1]<<16) + (buf->ptr[2]<<8) + buf->ptr[3];
	buf->ptr += 4;
	return r;
}

int64_t
parsei64(Buffer *buf)
{
	int64_t r = ((buf->ptr[0]+0ull)<<56) + ((buf->ptr[1]+0ull)<<48) + ((buf->ptr[2]+0ull)<<40) + ((buf->ptr[3]+0ull)<<32) + (buf->ptr[4]<<24) + (buf->ptr[5]<<16) + (buf->ptr[6]<<8) + buf->ptr[7];
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
	OsuStr r = {.start = buf->ptr, .end = buf->ptr};

	if(parsei8(buf) == 0x11)
		r.end += parseULEB128(buf);

	return r;
}

TimingPoint
parseTimingPoint(Buffer *buf)
{
	TimingPoint r = {
		.bpm = parsef64(buf),
		.offset = parsef64(buf),
		.inherit = parseBool(buf)
	};
	return r;
}

Beatmap
parseBeatmap(Buffer *buf)
{
	//TODO
}

