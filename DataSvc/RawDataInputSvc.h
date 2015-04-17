#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include <vector>
#include <string>

#include "SniperKernel/SvcBase.h"

class DataSvc;
class InputStream;

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

      void decodePulseHeader(uint64_t *buff, uint32_t *type, uint32_t *module, uint32_t *subsecond );
      void decodePulseTime(uint64_t *buff, time_t *second );
      void decodeEvent(uint64_t *buff, uint32_t *psd, uint32_t *tof, uint32_t *qa, uint32_t *qb );
      uint64_t* read64bits();

      bool isPulseHeader(uint64_t *buff);
      bool isPulseTail(uint64_t *buff);

    private :

      DataSvc*          m_dataSvc;
      std::string       m_inputFile;
      uint64_t*         m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      InputStream*      m_iStream;
      //std::vector<std::string> m_inputFile;

};


#endif
