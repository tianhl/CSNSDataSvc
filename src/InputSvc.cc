#include "DataSvc/InputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

#include "Data/Pulse.h"
#include "Data/Event.h"
#include "Data/RawData.h"

DECLARE_SERVICE(InputSvc);

InputSvc::InputSvc(const std::string& name)
    : SvcBase(name)
{
}

InputSvc::~InputSvc()
{
}

bool InputSvc::initialize()
{

  std::cout << "InputSvc initialize " << std::endl;

  return true;
}

bool InputSvc::finalize()
{
	return true;
}
