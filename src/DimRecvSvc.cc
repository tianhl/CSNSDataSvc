#include "DataSvc/DimRecvSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>


#include "dim.hxx"

DECLARE_SERVICE(DimRecvSvc);


boost::mutex i_mutex;
//=========================================================
DimRecvSvc::DimRecvSvc(const std::string& name)
: SvcBase(name)
{
	//declProp("InputFile", m_inputFiles);
}

DimRecvSvc::~DimRecvSvc(){
}

bool DimRecvSvc::initialize(){
	m_dataSize = 100;
	boost::thread m_client(boost::bind(&DimRecvSvc::dimClient, this));
	return true;
}

bool DimRecvSvc::finalize()
{
	return true;
}


bool DimRecvSvc::read(uint64_t* buff, uint32_t buffsize){
//	if(m_cu)
	memcpy(buff, m_current, buffsize);
	// m_offset -= buffsize;
	// if m_offset >= m_dataSzie delete m_current popDataItem()
	return true;
}



size_t DimRecvSvc::count() const{
	return 800/sizeof(uint64_t);
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
	{
		boost::lock_guard<boost::mutex> lock(i_mutex);
		dataQueue.push(dataItem);
	}
}

//=========================================================
//private method: main
//========================================================

void DimRecvSvc::popDataItem(){
	{
		boost::lock_guard<boost::mutex> lock(i_mutex);
		m_current = dataQueue.front();
		dataQueue.pop();
	}
}











