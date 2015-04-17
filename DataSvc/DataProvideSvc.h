#ifndef DATAPROVIDERSVC_H
#define DATAPROVIDERSVC_H


#include <string>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SniperKernel/SvcBase.h"

class DataProvideSvc : public SvcBase{
	public:
		DataProvideSvc(const std::string& name);
		virtual ~DataProvideSvc();

		bool initialize();
		bool finalize();
		void open(const std::string& filename);
		void open();
		void close();
		void read(uint64_t* buff, uint32_t size);
		size_t count() const;
	private:
		std::ifstream     m_filestream;     
		std::string       m_inputFile;
};

#endif
