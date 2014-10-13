///
/// \class l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1
///
/// \author: Jim Brooke
///
/// Description: first iteration of stage 2 jet algo

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2EtSumAlgorithmFirmware.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"
#include <math.h>


l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::Stage2Layer2EtSumAlgorithmFirmwareImp1(CaloParams* params) :
   params_(params)
{

  // Add some LogDebug for these settings

  etSumEtThresholdHwEt_ = floor(params_->etSumEtThreshold(1)/params_->towerLsbSum());
  etSumEtThresholdHwMet_ = floor(params_->etSumEtThreshold(3)/params_->towerLsbSum());
  
  etSumEtaMinEt_ = params_->etSumEtaMin(1);
  etSumEtaMaxEt_ = params_->etSumEtaMax(1);
  
  etSumEtaMinMet_ = params_->etSumEtaMin(3);
  etSumEtaMaxMet_ = params_->etSumEtaMax(3);
}


l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::~Stage2Layer2EtSumAlgorithmFirmwareImp1() {}

void l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::CaloTower> & towers,
                                                               std::vector<l1t::EtSum> & etsums) {

  int ietaMax=40, ietaMin=-40, iphiMax=72, iphiMin=1;

  // Will need to do these separately for +/- eta
  
  int32_t ex(0), ey(0), et(0);

  for (int ieta=ietaMin; ieta<=ietaMax; ieta++) {

    // Add the eta and Et thresholds

    if (ieta==0) continue;

    int32_t ringEx(0), ringEy(0), ringEt(0);

    for (int iphi=iphiMin; iphi<=iphiMax; iphi++) {
      
      l1t::CaloTower tower = l1t::CaloTools::getTower(towers, ieta, iphi);

      // SWITCHED SIN AND COS TEMPORARILY FOR AGREEMENT WITH FIRMWARE !!!
      // SHOULD BE CHANGED BACK or MET-PHI WILL BE WRONG !!!

      ringEy += (int32_t) (tower.hwPt() * std::trunc ( 511. * cos ( 2 * M_PI * (72 - (iphi-1)) / 72.0 ) )) >> 9;
      ringEx += (int32_t) (tower.hwPt() * std::trunc ( 511. * sin ( 2 * M_PI * (iphi-1) / 72.0 ) )) >> 9;
      ringEt += tower.hwPt();

    }

    // At some point we will remove the bit shifts and will need to limit to the precision available in the firmware

    ex += ( ringEx >> 2);
    ey += ( ringEy >> 2);
    et += ( ringEt >> 1);

  }
 
  // final MET calculation - this should go in the demux class

  int32_t met = (int32_t) std::sqrt(std::pow( (double) ex,2) + std::pow( (double) ey,2));

  double metPhiRadians = std::atan2( (double) ey, (double) ex ) + M_PI;

  int32_t metPhi = (int32_t) metPhiRadians / (2 * M_PI);

  // push output
  math::XYZTLorentzVector p4;

  l1t::EtSum etSumTotalEt(p4,l1t::EtSum::EtSumType::kTotalEt,et,0,0,0);
  l1t::EtSum etSumMissingEt(p4,l1t::EtSum::EtSumType::kMissingEt,met,0,metPhi,0);
  l1t::EtSum etSumEx(p4,l1t::EtSum::EtSumType::kTotalEtx,ex,0,0,0);
  l1t::EtSum etSumEy(p4,l1t::EtSum::EtSumType::kTotalEty,ey,0,0,0);
  
  std::cout << "Et: "  << et << std::endl;
  std::cout << "Met: "  << met << std::endl;
  std::cout << "Etx: "  << (static_cast<int>(ex<<12) >>12) << std::endl;
  std::cout << "Ety: "  << (static_cast<int>(ey<<12) >>12) << std::endl;

  etsums.push_back(etSumTotalEt);
  etsums.push_back(etSumEx);
  etsums.push_back(etSumEy);
  etsums.push_back(etSumMissingEt);

}

