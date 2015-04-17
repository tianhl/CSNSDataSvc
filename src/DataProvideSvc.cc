#include "DataSvc/DataProvideSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(DataProvideSvc);

DataProvideSvc::DataProvideSvc(const std::string& name)
	: SvcBase(name)
{
	declProp("InputFile", m_inputFile);
}

DataProvideSvc::~DataProvideSvc(){
}

bool DataProvideSvc::initialize(){
	return true;
}

bool DataProvideSvc::finalize()
{
	return true;
}

void DataProvideSvc::open(){
	open(m_inputFile);
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
