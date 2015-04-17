#include "DataSvc/InputStream.h"


InputStream::InputStream(){
}

InputStream::~InputStream(){
}

void InputStream::open(const std::string& name){
	m_filestream.open(name.c_str(), std::ios::binary);
}

void InputStream::close(){
	m_filestream.close();
}

void InputStream::read(uint64_t* buff, uint32_t size){
	m_filestream.read((char*)buff, sizeof(uint64_t)*size);
}

size_t InputStream::count() const{
	return m_filestream.gcount()/sizeof(uint64_t);
}
