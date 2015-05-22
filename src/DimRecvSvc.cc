#include "DataSvc/DimRecvSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"

#include "dic.h"

int flag = 0;

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
        flag = 1;
	//m_client->interrupt();
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

//=========================================================
//private method: thread
//========================================================
void DimRecvSvc::pushDataItem(uint64_t* item, size_t size)
{
	uint64_t* dataItem = new uint64_t[size];
	memcpy(dataItem, (uint64_t*)item, size);
	dataQueue.put(dataItem);
}


void functionWrapper(void* flag, void* buff, int* size){
	if(1200 == *((int*)flag)) DimRecvSvc::pushDataItem((uint64_t*)buff, size_t(*size));
}

void DimRecvSvc::dimClient(){

	char aux[80];
	int id;
	int no_link = -1;
	sprintf(aux,"%s","dimserver/TEST_SWAP");
	id = dic_info_service_stamped( aux, MONITORED, 0, 0, 0, functionWrapper, 1200,
			&no_link, 4 );                      //创建接收service的功能模块

	while(flag !=1)
	{
	}
	dic_release_service(id);
}

//=========================================================
//private method: main
//========================================================

void DimRecvSvc::popDataItem(){
	m_current = dataQueue.get();
	m_offset = 0;
}

bool DimRecvSvc::copyBuff(uint64_t* destBuff, size_t size, uint64_t* srcBuff){
	memcpy(destBuff, srcBuff, size);
	m_offset += size;
	return true;
}
