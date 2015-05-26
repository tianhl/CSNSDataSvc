#ifndef DIMRECVSVC_H
#define DIMRECVSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SniperKernel/SvcBase.h"
#include "DataSvc/DynamicThreadedQueue.h"
#include "DataSvc/DynamicSingleton.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

class DataItem{
	public:
		DataItem(uint64_t* p, size_t s):m_pData(p),m_size(s){};
		virtual ~DataItem(){};

		uint64_t* getData(){return m_pData;};
		size_t    getSize(){return m_size;};
	private:
		DataItem(){};
		// set const?
		uint64_t* m_pData;
		size_t    m_size;
};

class DimRecvSvc : public SvcBase{
	public:
		DimRecvSvc(const std::string& name);
		virtual ~DimRecvSvc();

		bool   initialize();
		bool   finalize();
		bool   read(uint64_t* buff, size_t size);
		size_t count() const;
	private:
		// thread
		void dimClient();
		static void pushDataItem(uint64_t* item, size_t size);
		friend void functionWrapper(void* tag, void* item, int* size);
		// main
		bool eraseDataItem();
		void popDataItem();
		bool copyBuff(uint64_t* destBuff, size_t size, uint64_t* srcBuff);
	private:
		static DynamicThreadedQueue<DataItem*> dataQueue;
		DataItem* m_curDataItem;
		uint64_t* m_current;
		size_t m_dataSize; // to upper
		size_t m_currSize;
		size_t m_offset;   // dimbuff offset
		boost::thread *m_client;
		int    m_dimID;

};

// set singleton?
typedef DynamicSingleton<DimRecvSvc> DAQInputSvc;

#endif
