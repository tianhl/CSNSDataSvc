#ifndef DECODE_RAWDATA_H
#define DECODE_RAWDATA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

class DecodeRawData{

	public:

		static void decodePulseHeader(uint64_t *buff, uint32_t *type, uint32_t *module, uint32_t *subsecond );
		static void decodePulseTime(uint64_t *buff, time_t *second );
		static void decodeEvent(uint64_t *buff, uint32_t *psd, uint32_t *tof, uint32_t *qa, uint32_t *qb );

		static bool isPulseHeader(uint64_t *buff);
		static bool isPulseTail(uint64_t *buff);
};


#endif
