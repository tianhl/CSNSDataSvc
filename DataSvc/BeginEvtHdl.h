#ifndef BEGIN_EVENT_HANDLER_H
#define BEGIN_EVENT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;
class DataSvc;
class InputSvc;
//class RootInputSvc;

class BeginEvtHdl : public IIncidentHandler
{
    public :

        BeginEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        bool              m_1stCall;
        Task*             m_par;
        DataSvc*          m_dataSvc;
        InputSvc*         m_iSvc;
};

#endif
