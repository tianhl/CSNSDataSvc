#include "DataSvc/BeginEvtHdl.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/InputSvc.h"
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
    : m_1stCall(true),
      m_par(par)
{
    //SniperPtr<RootInputSvc> iSvc(m_par, "InputSvc");
    //if ( iSvc.invalid() ) {
    //    LogFatal << "cann't find InputSvc for "
    //             << m_par->scope() << m_par->objName() << std::endl;
    //    throw SniperException("InputSvc is invalid");
    //}
    //m_iSvc = iSvc.data();

    //SniperPtr<InputSvc> pISvc("InputSvc");
    //if ( pISvc.invalid()) {
    //        throw SniperException("InputSvc is invalid!");
    //}



    SniperPtr<DataSvc> pDSvc("DataSvc");
    if ( pDSvc.invalid()) {
	    throw SniperException("DataSvc is invalid!");
    }

    m_dataSvc = pDSvc.data();
    std::cout << "BeginEvtHdl get DataSvc " << m_dataSvc->objName() << std::endl;


}

bool BeginEvtHdl::handle(Incident& /*incident*/)
{
    if ( m_1stCall ) {
        //m_buf->init(m_iSvc->getInputStream());
        m_1stCall = false;
    }

    Pulse* pulse = m_dataSvc->getObj<Pulse>("/pulse");
    LogInfo << "BeginEvtHdl PulseID: " << pulse->getPulseID() << std::endl;
    //if ( m_buf->next() ) {
    //    return true;
    //}
    //return Incident::fire("StopRun");
    return true;
}
