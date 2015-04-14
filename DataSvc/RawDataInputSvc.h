#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include "SniperKernel/SvcBase.h"
//#include "SniperKernel/IIncidentHandler.h"
//#include "DynamicTreeNode.h"
//#include "Data/DataObject.h"
//#include <map>
//#include <list>

/**************************************************************************
 * Examples
 * It's similar to TTree and TNtuple :
 *     DataSvc* svc = SvcMgr::get<DataSvc>("DataSvc");
 *     TTree* tree = svc->bookTree("Fkey/dir/tree", "title");
 *
 * or to Histograms:
 *     DataSvc* svc = SvcMgr::get<DataSvc>("DataSvc");
 *     TH1D* hist = new TH1D(...)
 *     svc->attach("Fkey/dir", hist);
 *
 * the dir(TDirectory in TFile) is optional (0 - several levels)
 *************************************************************************/

class RawDataInputSvc : public SvcBase
{
    public :

      RawDataInputSvc(const std::string& name);
      virtual ~RawDataInputSvc();

      bool initialize();
      bool finalize();


    //private :

};


#endif
