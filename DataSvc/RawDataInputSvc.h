#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include <vector>
#include <string>

#include "SniperKernel/SvcBase.h"

class DataSvc;
class DataProvideSvc;

class RawDataInputSvc : public SvcBase
{
    public :

      RawDataInputSvc(const std::string& name);
      virtual ~RawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :
      size_t nextSegment();
      uint64_t* read64bits();

    private :

      DataSvc*          m_dataSvc;
      std::string       m_inputFile;
      DataProvideSvc*   m_iStream;

      uint64_t*         m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      //std::vector<std::string> m_inputFile;

};


#endif
