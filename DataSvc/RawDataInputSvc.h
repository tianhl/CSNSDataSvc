#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include "SniperKernel/SvcBase.h"

class DataSvc;

class RawDataInputSvc : public SvcBase
{
    public :

      RawDataInputSvc(const std::string& name);
      virtual ~RawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();


    private :

      DataSvc*          m_dataSvc;

};


#endif
