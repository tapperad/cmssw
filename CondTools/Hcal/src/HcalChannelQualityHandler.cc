#include "CondTools/Hcal/interface/HcalChannelQualityHandler.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include <memory>

HcalChannelQualityHandler::HcalChannelQualityHandler(edm::ParameterSet const & ps)
{
  m_name = ps.getUntrackedParameter<std::string>("name","HcalChannelQualityHandler");
  sinceTime = ps.getUntrackedParameter<unsigned>("IOVRun",0);
}

HcalChannelQualityHandler::~HcalChannelQualityHandler()
{
}

void HcalChannelQualityHandler::getNewObjects()
{
  //  edm::LogInfo   ("HcalChannelQualityHandler") 
  std::cout
    << "------- " << m_name 
    << " - > getNewObjects\n" << 
    //check whats already inside of database
    "got offlineInfo"<<
    tagInfo().name << ", size " << tagInfo().size 
					  << ", last object valid since " 
					  << tagInfo().lastInterval.first << std::endl;  

  if (!myDBObject) 
    throw cms::Exception("Empty DB object") << m_name 
					    << " has received empty object - nothing to write to DB" 
					    << std::endl;

  //  IOV information
  cond::Time_t myTime = sinceTime;

  std::cout << "Using IOV run " << sinceTime << std::endl;

  // prepare for transfer:
  m_to_transfer.push_back(std::make_pair(myDBObject,myTime));

  edm::LogInfo("HcalChannelQualityHandler") << "------- " << m_name << " - > getNewObjects" << std::endl;

}

void HcalChannelQualityHandler::initObject(HcalChannelQuality* fObject)
{
  myDBObject = fObject;
}