///
/// \class l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1
///
/// \author: Jim Brooke
///
/// Description: first iteration of stage 2 jet algo

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2EtSumAlgorithmFirmware.h"

#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::Stage2Layer2EtSumAlgorithmFirmwareImp1(CaloParams* params) :
   params_(params)
{
  etSumEtThresholdHwEt_ = floor(params_->etSumEtThreshold(1)/params_->towerLsbSum());
  etSumEtThresholdHwMet_ = floor(params_->etSumEtThreshold(3)/params_->towerLsbSum());
  
  etSumEtaMinEt_ = params_->etSumEtaMin(1);
  etSumEtaMaxEt_ = params_->etSumEtaMax(1);
  
  etSumEtaMinMet_ = params_->etSumEtaMin(3);
  etSumEtaMaxMet_ = params_->etSumEtaMax(3);
}


l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::~Stage2Layer2EtSumAlgorithmFirmwareImp1() {


}

void l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::CaloTower> & towers,
      std::vector<l1t::EtSum> & etsums) {



  double pi = std::atan(1.d) * 4.0d;

  int ietaMax=40, ietaMin=-40, iphiMax=72, iphiMin=1;
  
  int32_t ex(0), ey(0), et(0);

  for (int ieta=ietaMin; ieta<=ietaMax; ieta++) {

    if (ieta==0) continue;

    int32_t ringEx(0), ringEy(0), ringEt(0);

    for (int iphi=iphiMin; iphi<=iphiMax; iphi++) {
      
      l1t::CaloTower tower = l1t::CaloTools::getTower(towers, ieta, iphi);

      // SWITCHED SIN AND COS TEMPORARILY FOR AGREEMENT WITH FIRMWARE !!!
      // SHOULD BE CHANGED BACK or MET-PHI WILL BE WRONG !!!
      int32_t towEx = (int32_t) (tower.hwPt() * std::trunc(511.*std::sin(2*pi*(iphi-1)/72.))) >> 9;
      int32_t towEy = (int32_t) (tower.hwPt() * std::trunc(511.*std::cos(2*pi*(72-(iphi-1))/72.))) >> 9;
      int32_t towEt = tower.hwPt();

      ringEx += towEx;
      ringEy += towEy;
      ringEt += towEt;
      ringEx &= 0xfffff;
      ringEy &= 0xfffff;
      ringEt &= 0xfffff;
    }

    ringEx >>= 2;
    ringEy >>= 2;
    ringEt >>= 1;

    ex += ringEx;
    ey += ringEy;
    et += ringEt;
    
  }
 
  // final MET calculation
  int32_t met = (int32_t) std::sqrt(std::pow( (double) ex,2) + std::pow( (double) ey,2));

  double metPhiRadians = std::atan2( (double) ey, (double) ex ) + pi;

  int32_t metPhi = (int32_t) metPhiRadians / (2 * pi);

  // apply output bitwidth constraints
  ex     &= 0xfffff;
  ey     &= 0xfffff;
  met    &= 0xfff;
  metPhi &= 0xfff;
  et     &= 0xfffff;
 
  // push output
  math::XYZTLorentzVector p4;

  l1t::EtSum etSumTotalEt(p4,l1t::EtSum::EtSumType::kTotalEt,et,0,0,0);
  l1t::EtSum etSumMissingEt(p4,l1t::EtSum::EtSumType::kMissingEt,met,0,metPhi,0);
  l1t::EtSum etSumEx(p4,l1t::EtSum::EtSumType::kTotalEx,ex,0,0,0);
  l1t::EtSum etSumEy(p4,l1t::EtSum::EtSumType::kTotalEy,ey,0,0,0);
  
  std::cout << "Et: "  << et << std::endl;
  std::cout << "Met: "  << met << std::endl;
  std::cout << "Ex: "  <<  ( static_cast<int>( ex<<12 ) >> 12 ) << std::endl;
  std::cout << "Ey: "  << ( static_cast<int>( ey<<12 ) >> 12 ) << std::endl;

  etsums.push_back(etSumTotalEt);
  etsums.push_back(etSumEx);
  etsums.push_back(etSumEy);
  etsums.push_back(etSumMissingEt);

}

