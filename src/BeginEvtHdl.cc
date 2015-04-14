#include "DataSvc/BeginEvtHdl.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/RawDataInputSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"

// for test
#include "Data/Pulse.h"
#include "Data/Event.h"
#include "Data/RawData.h"

BeginEvtHdl::BeginEvtHdl(Task* par)
    : m_par(par)
{

    SniperPtr<RawDataInputSvc> pSvc("RawDataInputSvc");
    if ( pSvc.invalid()) {
            throw SniperException("RawData inputSvc is invalid!");
    }
    m_iSvc = pSvc.data();

}

bool BeginEvtHdl::handle(Incident& /*incident*/)
{
    if ( m_iSvc->next() ) {
        return true;
    }
    return Incident::fire("StopRun");
}
