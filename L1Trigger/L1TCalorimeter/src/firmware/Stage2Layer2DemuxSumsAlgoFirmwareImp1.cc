///
/// \class l1t::Stage2Layer2SumsAlgorithmFirmwareImp1
///
/// \author: 
///
/// Description: 

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2DemuxSumsAlgoFirmware.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"

#include <vector>
#include <algorithm>

l1t::Stage2Layer2DemuxSumsAlgoFirmwareImp1::Stage2Layer2DemuxSumsAlgoFirmwareImp1(CaloParams* params) :
  params_(params)
{


}


l1t::Stage2Layer2DemuxSumsAlgoFirmwareImp1::~Stage2Layer2DemuxSumsAlgoFirmwareImp1() {


}


void l1t::Stage2Layer2DemuxSumsAlgoFirmwareImp1::processEvent(const std::vector<l1t::EtSum> & inputSums,
                                                              std::vector<l1t::EtSum> & outputSums) {

  int32_t et(0), metx(0), mety(0), ht(0), mhtx(0), mhty(0);

  // Add up the x, y and scalar components
  for (std::vector<l1t::EtSum>::const_iterator eSum = inputSums.begin() ; eSum != inputSums.end() ; ++eSum )
    {
      switch (eSum->getType()) {

      case l1t::EtSum::EtSumType::kTotalEt:
        et += eSum->hwPt();
        break;
          
      case l1t::EtSum::EtSumType::kTotalEtx:
        metx += eSum->hwPt();
        break;

      case l1t::EtSum::EtSumType::kTotalEty:
        mety += eSum->hwPt();
        break;
        
      case l1t::EtSum::EtSumType::kTotalHt:
        ht += eSum->hwPt();
        break;
        
      case l1t::EtSum::EtSumType::kTotalHtx:
        mhtx += eSum->hwPt();
        break;
        
      case l1t::EtSum::EtSumType::kTotalHty:
        mhty += eSum->hwPt();
        break;
        
      default:
        continue;
        // Should throw an exception or something?
      }
    }
  
  // Final MET calculation
  int32_t met = (int32_t) std::sqrt(std::pow( (double) metx,2) + std::pow( (double) mety,2));
  double metPhiRadians = std::atan2( (double) mety, (double) metx ) + M_PI;
  int32_t metPhi = (int32_t) metPhiRadians / (2 * M_PI);

  et     &= 0xfffff;
  met    &= 0xfff;
  metPhi &= 0xfff;

  // Final MHT calculation
  int32_t mht = (int32_t) std::sqrt( std::pow((double) mhtx,2) + std::pow((double) mhty,2) );
  int32_t mhtPhi = 72 * atan2( (double) mhty , (double) mhtx );

  while ( mhtPhi > 72 ) mhtPhi-=72;
  while ( mhtPhi < 1 ) mhtPhi+=72;

  ht  &= 0xfff;   
  mht &= 0xfff;

  // Make final collection
  math::XYZTLorentzVector p4;

  l1t::EtSum etSumTotalEt(p4,l1t::EtSum::EtSumType::kTotalEt,et,0,0,0);
  l1t::EtSum etSumMissingEt(p4,l1t::EtSum::EtSumType::kMissingEt,met,0,metPhi,0);
  l1t::EtSum htSumht(p4,l1t::EtSum::EtSumType::kTotalHt,ht,0,0,0);
  l1t::EtSum htSumMissingHt(p4,l1t::EtSum::EtSumType::kMissingHt,mht,0,mhtPhi,0);

  outputSums.push_back(etSumTotalEt);
  outputSums.push_back(etSumMissingEt);
  outputSums.push_back(htSumht);
  outputSums.push_back(htSumMissingHt);

  // Disable for now
  //  outputSums = inputSums;

}

