#ifndef DIMRECVSVC_H
#define DIMRECVSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SniperKernel/SvcBase.h"

#include <queue>

class DimRecvSvc : public SvcBase{
	public:
		DimRecvSvc(const std::string& name);
		virtual ~DimRecvSvc();

		bool   initialize();
		bool   finalize();
		bool   read(uint64_t* buff, uint32_t size);
		size_t count() const;
	private:
		// thread
		void dimClient();
		void pushDataItem(uint64_t* item, size_t size);
		// main
		void popDataItem();
	private:
		uint16_t m_dataSize;
		std::queue<uint64_t*> dataQueue;
		uint32_t m_offset;
		uint64_t* m_current;


};

#endif
