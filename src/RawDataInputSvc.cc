#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"

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
}

RawDataInputSvc::~RawDataInputSvc()
{
}

bool RawDataInputSvc::initialize()
{

	LogInfo << "InputSvc initialize " << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		throw SniperException("DataSvc is invalid!");
	}


	m_dataSvc = pSvc.data();
	LogInfo << "InputSvc gets " << m_dataSvc->objName() << std::endl;



	std::cout << "Input File: " << m_inputFile << std::endl;
	//loadBinaryFile(m_inputFile[0]);

	m_filestream.open(m_inputFile.c_str(), std::ios::binary);
	m_buffsize = 100;
	m_dataBuff = new uint64_t[m_buffsize];
	m_isLastSegment = false;
	m_currbuffsize = nextSegment();

	return true;
}



bool RawDataInputSvc::finalize()
{
	m_filestream.close();
	return true;
}

uint64_t* RawDataInputSvc::read64bits(){
	uint64_t *ReadRawData; 
	if (m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
	ReadRawData = (uint64_t*)(m_dataBuff+m_offset);
	m_offset++;
	return ReadRawData;
}

bool RawDataInputSvc::next()
{
	//LogInfo << "Next Pulse" << std::endl;
	uint64_t *ReadRawData;// = new uint8_t[8]; 
	time_t second;
	uint32_t type, module, subsecond;
	//std::cout << "m_offset " << m_offset << std::endl;

	ReadRawData = read64bits();
	if(((*ReadRawData)>>56)==0x0)  {
		//std::cout << " BeginOfPulse" << std::endl;
		decodePulseHeader(ReadRawData, &type, &module, &subsecond);
		ReadRawData = read64bits();
		decodePulseTime(ReadRawData, &second);
		//std::cout << " Header: type " << type << " module  " << module << " subsecond " << subsecond << std::endl;
		//std::cout << " Time: second " << second  <<" "<< ctime((time_t*)&second)    <<  std::endl; 
	}

	while(true){
		ReadRawData = read64bits();
		if (((*ReadRawData)>>56) == 0xFF)   {
			//std::cout << " EndOfPulse" << std::endl; 
			if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;
			break;
		}
		uint32_t psd, tof, qa, qb;
		decodeEvent(ReadRawData, &psd, &tof, &qa, &qb);
		//std::cout << "decode qa " << qa << " qb " << qb << std::endl;


	}
	return true;

}



size_t RawDataInputSvc::nextSegment()
{
	std::cout << "nextSegment() " << std::endl;
	m_filestream.read((char*)m_dataBuff, sizeof(uint64_t)*m_buffsize);
	m_offset = 0;

	size_t size = m_filestream.gcount();
	if(size < m_buffsize){
		m_isLastSegment = true;
	}
	return size;
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

