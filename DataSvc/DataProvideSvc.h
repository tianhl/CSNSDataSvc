#ifndef DATAPROVIDERSVC_H
#define DATAPROVIDERSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SniperKernel/SvcBase.h"

class DataProvideSvc : public SvcBase{
	public:
		DataProvideSvc(const std::string& name);
		virtual ~DataProvideSvc();

		bool   initialize();
		bool   finalize();
		bool   read(uint64_t* buff, uint32_t size);
		size_t count() const;
	private:
		void open();
		void close();
		bool next();
		void open(const std::string& filename);
	private:
		std::ifstream               m_filestream;     
		std::vector<std::string>    m_inputFiles;
		uint32_t                    m_offset;
		uint32_t                    m_nFiles;
};

#endif
