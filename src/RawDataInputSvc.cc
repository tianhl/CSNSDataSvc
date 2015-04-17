#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
#include "DataSvc/InputStream.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


#include <iostream>
#include <list>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Data/Pulse.h"
#include "Data/Event.h"
#include "Data/RawData.h"


DECLARE_SERVICE(RawDataInputSvc);

RawDataInputSvc::RawDataInputSvc(const std::string& name)
: SvcBase(name) 
{
	declProp("InputFile", m_inputFile);
	declProp("BuffSize",  m_buffsize);

	m_isLastSegment = false;
	m_offset = 0;
	m_currbuffsize = 0;
}

RawDataInputSvc::~RawDataInputSvc()
{
}

bool RawDataInputSvc::initialize()
{

	LogInfo << "InputSvc initialize " << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pSvc.data();

	m_dataBuff = new uint64_t[m_buffsize];

	m_iStream = new InputStream();
	m_iStream->open(m_inputFile);

	return true;
}



bool RawDataInputSvc::finalize()
{
	m_iStream->close();
	return true;
}

uint64_t* RawDataInputSvc::read64bits(){
	if (m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
	return (uint64_t*)(m_dataBuff+(m_offset++));
}

bool RawDataInputSvc::next()
{
	uint64_t *ReadRawData;
	time_t second;
	uint32_t type, module, subsecond;

	ReadRawData = read64bits();
	if(isPulseHeader(ReadRawData))  {
		decodePulseHeader(ReadRawData, &type, &module, &subsecond);
		decodePulseTime(read64bits(), &second);
	}
	else throw SniperException("Pulse Header NOT FOUND!");

	while(true){
		ReadRawData = read64bits();
		if (isPulseTail(ReadRawData))   {
			if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;
			break;
		}
		uint32_t psd, tof, qa, qb;
		decodeEvent(ReadRawData, &psd, &tof, &qa, &qb);
	}
	return true;

}



size_t RawDataInputSvc::nextSegment()
{
        m_iStream->read(m_dataBuff, m_buffsize);
	m_offset = 0;

	size_t size = m_iStream->count();
	if(size < m_buffsize) m_isLastSegment = true;
	return size;
}

bool RawDataInputSvc::isPulseHeader(uint64_t *buff){
	return 0x0  == (*buff>>56);
}

bool RawDataInputSvc::isPulseTail(uint64_t *buff){
	return 0xFF == (*buff>>56);
}

void RawDataInputSvc::decodePulseHeader(uint64_t *buff, uint32_t *type, uint32_t *module, uint32_t *subsecond ){
	*type      = (uint32_t) (((*buff)>>48)&0xFF);
	*module    = (uint32_t) (((*buff)>>32)&0xFF); 
	*subsecond = (uint32_t) ((*buff)&0xFFFFFFFF);
}

void RawDataInputSvc::decodePulseTime(uint64_t *buff, time_t *second ){
	*second = (time_t) (* buff); 
}

void RawDataInputSvc::decodeEvent(uint64_t *buff, uint32_t *psd, uint32_t *tof, uint32_t *qa, uint32_t *qb ){
	*psd = (uint32_t) (((*buff) >> 56 ) & 0xFF);
	*tof = (uint32_t) (((*buff) >> 28 ) & 0xFFFFFFF);
	*qa  = (uint32_t) (((*buff) >> 14 ) & 0x3FFF);
	*qb  = (uint32_t) (( *buff) & 0x3FFF);
}

