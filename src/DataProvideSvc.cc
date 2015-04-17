#include "DataSvc/DataProvideSvc.h"


DataProvideSvc::DataProvideSvc(){
}

DataProvideSvc::~DataProvideSvc(){
}

void DataProvideSvc::open(const std::string& name){
	m_filestream.open(name.c_str(), std::ios::binary);
}

void DataProvideSvc::close(){
	m_filestream.close();
}

void DataProvideSvc::read(uint64_t* buff, uint32_t size){
	m_filestream.read((char*)buff, sizeof(uint64_t)*size);
}

size_t DataProvideSvc::count() const{
	return m_filestream.gcount()/sizeof(uint64_t);
}
