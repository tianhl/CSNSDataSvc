#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
#include "DataSvc/DataProvideSvc.h"
#include "DataSvc/DecodeRawData.h"

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
: SvcBase(name) {
	declProp("BuffSize",  m_buffsize);

	m_isLastSegment = false;
	m_offset = 0;
	m_currbuffsize = 0;
}

RawDataInputSvc::~RawDataInputSvc() {
}

bool RawDataInputSvc::initialize() {

	LogInfo << "InputSvc initialize " << std::endl;

	SniperPtr<DataSvc> pDSvc("DataSvc");
	if ( pDSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pDSvc.data();

	m_dataBuff = new uint64_t[m_buffsize];

	SniperPtr<DataProvideSvc> pPSvc("DataProvideSvc");
	if ( pPSvc.invalid()) throw SniperException("DataProvideSvc is invalid!");
	m_dataPvdSvc = pPSvc.data();

	//m_dataPvdSvc->open();

	return true;
}



bool RawDataInputSvc::finalize() {
	return true;
}

bool RawDataInputSvc::next() {
	uint64_t *ReadRawData;
	time_t second;
	uint32_t type, module, subsecond;

	ReadRawData = read64bits();
	if(DecodeRawData::isPulseHeader(ReadRawData))  {
		DecodeRawData::decodePulseHeader(ReadRawData, &type, &module, &subsecond);
		DecodeRawData::decodePulseTime(read64bits(), &second);
	}
	else throw SniperException("Pulse Header NOT FOUND!");

	while(true){
		ReadRawData = read64bits();
		if (DecodeRawData::isPulseTail(ReadRawData))   {
			if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;
			break;
		}
		uint32_t psd, tof, qa, qb;
		DecodeRawData::decodeEvent(ReadRawData, &psd, &tof, &qa, &qb);
	}
	return true;

}

//=====================================================================
// Private Functions
// ====================================================================

uint64_t* RawDataInputSvc::read64bits(){
	if (m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
	return (uint64_t*)(m_dataBuff+(m_offset++));
}

size_t RawDataInputSvc::nextSegment() {
	m_offset = 0;
        if (not m_dataPvdSvc->read(m_dataBuff, m_buffsize)) m_isLastSegment = true;
	return m_dataPvdSvc->count();
}
