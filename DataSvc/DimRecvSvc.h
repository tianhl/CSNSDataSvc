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

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

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
		void popDataItem();
		bool copyBuff(uint64_t* destBuff, size_t size, uint64_t* srcBuff);
	private:
		size_t m_dataSize;
		size_t m_buffSize;
		static DynamicThreadedQueue<uint64_t*> dataQueue;
		size_t m_offset;
		uint64_t* m_current;
		boost::thread *m_client;
		int    m_dimID;

};

#endif
