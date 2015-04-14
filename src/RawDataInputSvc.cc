#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
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

  std::cout << "InputSvc initialize " << std::endl;

  return true;
}

bool RawDataInputSvc::finalize()
{
	return true;
}
