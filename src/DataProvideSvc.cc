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
	declProp("InputFile", m_inputFiles);
}

DataProvideSvc::~DataProvideSvc(){
}

bool DataProvideSvc::initialize(){
	m_nFiles = m_inputFiles.size();
	m_offset = 0;
	if (m_nFiles > 0 ) open();
	else throw SniperException("PLS specified input file names!");
	return true;
}

bool DataProvideSvc::finalize()
{
	return true;
}

void DataProvideSvc::open(){
	LogInfo << "Open File: " << m_inputFiles[m_offset] << std::endl;
	open(m_inputFiles[m_offset++]);
}


void DataProvideSvc::open(const std::string& name){
	m_filestream.open(name.c_str(), std::ios::binary);
}

void DataProvideSvc::close(){
	m_filestream.close();
}

bool DataProvideSvc::read(uint64_t* buff, uint32_t buffsize){
	m_filestream.read((char*)buff, sizeof(uint64_t)*buffsize);
	size_t size = count();
	if(size < buffsize){
		if (next()) m_filestream.read((char*)(buff+size), sizeof(uint64_t)*(buffsize-size));
		else return false;
	};
	return true;
}


bool DataProvideSvc::next(){
	close();
	if(m_offset ==  m_nFiles) return false;
	open();
	return true;
}

size_t DataProvideSvc::count() const{
	return m_filestream.gcount()/sizeof(uint64_t);
}
