#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


#include <iostream>
#include <list>

#include "Data/Pulse.h"
#include "Data/Event.h"
#include "Data/RawData.h"

DECLARE_SERVICE(RawDataInputSvc);

RawDataInputSvc::RawDataInputSvc(const std::string& name)
    : SvcBase(name)
{
}

RawDataInputSvc::~RawDataInputSvc()
{
}

bool RawDataInputSvc::initialize()
{

	LogInfo << "InputSvc initialize " << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		throw SniperException("DataSvc is invalid!");
	}


	m_dataSvc = pSvc.data();
	LogInfo << "InputSvc gets " << m_dataSvc->objName() << std::endl;

	return true;
}



bool RawDataInputSvc::finalize()
{
	return true;
}


bool RawDataInputSvc::next()
{
	LogInfo << "Next Pulse" << std::endl;
	return true;
}
