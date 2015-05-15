#include "DataSvc/DimRecvSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"

#include "dim.hxx"

DECLARE_SERVICE(DimRecvSvc);


//=========================================================
DimRecvSvc::DimRecvSvc(const std::string& name)
: SvcBase(name)
{
	declProp("DataSize", m_dataSize);
}

DimRecvSvc::~DimRecvSvc(){
}

bool DimRecvSvc::initialize(){
	m_client = new boost::thread(boost::bind(&DimRecvSvc::dimClient, this));
	m_client->detach();
	return true;
}

bool DimRecvSvc::finalize()
{
	// need process all the data item in the queue
	m_client->interrupt();
	return true;
}


bool DimRecvSvc::read(uint64_t* buff, size_t buffsize){
	// buffsize << m_dataSize  !!!!!!
	m_buffSize = buffsize;
	if(m_buffSize < m_dataSize) throw SniperException("buffsize needs smaller than datasize! ");

	if((m_offset+buffsize)>m_dataSize){
		size_t length = m_dataSize - m_offset;
		copyBuff(buff, length, m_current+m_offset);
		delete[] m_current;
		popDataItem();
		copyBuff(buff+length, buffsize-length, m_current);
	}
	else{
		copyBuff(buff, buffsize, m_current+m_offset);
	}

	return true;
}

size_t DimRecvSvc::count() const{
	return m_buffSize;
}

bool DimRecvSvc::copyBuff(uint64_t* destBuff, size_t size, uint64_t* srcBuff){
	memcpy(destBuff, srcBuff, size);
	m_offset += size;
	return true;
}


//=========================================================
//private method: thread
//========================================================
void DimRecvSvc::dimClient(){
	for(int i = 0; i < 10; i++){
		uint64_t* item = new uint64_t[m_dataSize];
		pushDataItem(item, sizeof(uint64_t)*m_dataSize);
	}
}

void DimRecvSvc::pushDataItem(uint64_t* item, size_t size){
	uint64_t* dataItem = new uint64_t[m_dataSize];
	memcpy(dataItem, item, size);
	dataQueue.put(dataItem);
}

//=========================================================
//private method: main
//========================================================

void DimRecvSvc::popDataItem(){
	m_current = dataQueue.get();
	m_offset = 0;
}

