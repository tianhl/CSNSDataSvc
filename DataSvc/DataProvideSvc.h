#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H


#include <string>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

class DataProvideSvc{
	public:
		DataProvideSvc();
		virtual ~DataProvideSvc();

		void open(const std::string& name);
		void close();
		void read(uint64_t* buff, uint32_t size);
		size_t count() const;
	private:
		std::ifstream     m_filestream;     


};

#endif
