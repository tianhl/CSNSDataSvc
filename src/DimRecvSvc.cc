#include "DataSvc/DimRecvSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"

extern "C"{
#include "dic.h"
}


DECLARE_SERVICE(DimRecvSvc);

DynamicThreadedQueue<DataItem*> DimRecvSvc::dataQueue;

//=========================================================
DimRecvSvc::DimRecvSvc(const std::string& name)
: SvcBase(name)
{
	declProp("DataSize", m_dataSize);

	m_client = NULL;
	m_curDataItem = NULL;
	m_current = NULL;
	m_currSize = 0;
	m_offset = 0;
	m_dimID = -1;
}

DimRecvSvc::~DimRecvSvc(){
}

bool DimRecvSvc::initialize(){
	m_client = new boost::thread(boost::bind(&DimRecvSvc::dimClient, this));
	m_client->detach();
	return true;
}

bool DimRecvSvc::finalize() {
	// need process all the data item in the queue
	if(-1 != m_dimID)dic_release_service(m_dimID);
	if(m_client) m_client->interrupt();
	return true;
}

bool DimRecvSvc::eraseDataItem(){
	if(not m_curDataItem) return false;
	delete []m_curDataItem->getData();
	delete m_curDataItem;
	m_curDataItem = NULL;
	return true;
}

bool DimRecvSvc::read(uint64_t* buff, size_t buffsize){
	uint64_t* currBuff = buff;
	size_t    needsize = buffsize;
	size_t    m_currSize = 0;

	while(needsize>0){
		size_t length = m_curDataItem->getSize()-m_offset;
		if(0 == length) {
			// if m_currSize > 0 return NULL
			// if m_currSize == 0 block @ Queue
			if(eraseDataItem()) popDataItem();
			else return false;
			if(m_curDataItem)length = m_curDataItem->getSize();
			else return false;
		}
		size_t cpsize = (needsize < length)?needsize:length;
		if(copyBuff(currBuff, cpsize, m_current+m_offset)){
			m_offset   += cpsize;
			currBuff   += cpsize;
			m_currSize += cpsize;
			needsize   -= cpsize;
		}
		else return false;
	}

	return true;
}

size_t DimRecvSvc::count() const{
	return m_currSize;
}

//=========================================================
//private method: thread
//========================================================
void DimRecvSvc::pushDataItem(uint64_t* item, size_t size) {
	uint64_t* data = new uint64_t[size];
	memcpy(data, (uint64_t*)item, size);
	DataItem* dataItem = new DataItem(data, size);
	dataQueue.put(dataItem);
}


void functionWrapper(void* flag, void* buff, int* size){
	//if(1200 == *((int*)flag)) DimRecvSvc::pushDataItem((uint64_t*)buff, size_t(*size));
	int t= 0;
	if(1200 == *((int*)flag)) {
		DimRecvSvc::pushDataItem((uint64_t*)buff, size_t(*size));
		memcpy(&t,buff,4); 
		printf("data: %d\n", t);
	}
}

void DimRecvSvc::dimClient(){
	static int no_link = -1;
	static char aux[80];
	sprintf(aux,"%s","dimserver/TEST_SWAP");
	m_dimID = dic_info_service_stamped( aux, MONITORED, 0, 0, 0, functionWrapper, 1200, &no_link, 4 );  
}

//=========================================================
//private method: main
//========================================================

void DimRecvSvc::popDataItem(){
	m_curDataItem = dataQueue.get();
	m_offset = 0;
}

bool DimRecvSvc::copyBuff(uint64_t* destBuff, size_t size, uint64_t* srcBuff){
	memcpy(destBuff, srcBuff, size);
	m_offset += size;
	return true;
}
