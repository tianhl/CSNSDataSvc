#include "DataSvc/DecodeRawData.h"


bool DecodeRawData::isPulseHeader(uint64_t *buff){
	return 0x0  == (*buff>>56);
}

bool DecodeRawData::isPulseTail(uint64_t *buff){
	return 0xFF == (*buff>>56);
}

void DecodeRawData::decodePulseHeader(uint64_t *buff, uint32_t *type, uint32_t *module, uint32_t *subsecond ){
	*type      = (uint32_t) (((*buff)>>48)&0xFF);
	*module    = (uint32_t) (((*buff)>>32)&0xFF); 
	*subsecond = (uint32_t) ((*buff)&0xFFFFFFFF);
}

void DecodeRawData::decodePulseTime(uint64_t *buff, time_t *second ){
	*second = (time_t) (* buff); 
}

void DecodeRawData::decodeEvent(uint64_t *buff, uint32_t *psd, uint32_t *tof, uint32_t *qa, uint32_t *qb ){
	*psd = (uint32_t) (((*buff) >> 56 ) & 0xFF);
	*tof = (uint32_t) (((*buff) >> 28 ) & 0xFFFFFFF);
	*qa  = (uint32_t) (((*buff) >> 14 ) & 0x3FFF);
	*qb  = (uint32_t) (( *buff) & 0x3FFF);
}

