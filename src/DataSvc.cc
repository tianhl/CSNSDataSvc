#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

#include "Data/Pulse.h"
#include "Data/Event.h"
#include "Data/RawData.h"

DECLARE_SERVICE(DataSvc);

DataSvc::DataSvc(const std::string& name)
    : SvcBase(name)
{
}

DataSvc::~DataSvc()
{
}

bool DataSvc::initialize()
{

  this->branch("pulse");
  this->find("/pulse")->branch("event");
  this->find("/pulse")->branch("raw");
  this->find("/pulse/raw")->branch("scintillant");
  this->find("/pulse/raw/scintillant")->branch("hitmap");

  this->regObj("/pulse", new Pulse);
  this->regObj("/pulse/event", new EventList);
  this->regObj("/pulse/raw/scintillant", new Scintillant);
  this->regObj("/pulse/raw/scintillant/hitmap", new HitMapList);

  Task* par = getScope();
  IIncidentHandler* bi = new BeginEvtHdl(par);
  if ( par->isTop() ) bi->regist("BeginEvent");
  else bi->regist(par->scope() + par->objName() + ":BeginEvent");

  m_icdts.push_back(bi);
  std::cout << "incidents size: " << m_icdts.size() << std::endl;

  return true;
}

bool DataSvc::finalize()
{
	return true;
}
