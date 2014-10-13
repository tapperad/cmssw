///
/// \class l1t::Stage2Layer2JetSumAlgorithmFirmwareImp1
///
/// \author: Jim Brooke
///
/// Description: first iteration of stage 2 jet algo

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2JetSumAlgorithmFirmware.h"

l1t::Stage2Layer2JetSumAlgorithmFirmwareImp1::Stage2Layer2JetSumAlgorithmFirmwareImp1(CaloParams* params) :
  params_(params)
{
  etSumEtThresholdHwEt_ = floor(params_->etSumEtThreshold(2)/params_->jetLsb());
  etSumEtThresholdHwMet_ = floor(params_->etSumEtThreshold(4)/params_->jetLsb());

  etSumEtaMinEt_ = params_->etSumEtaMin(2);
  etSumEtaMaxEt_ = params_->etSumEtaMax(2);
  
  etSumEtaMinMet_ = params_->etSumEtaMin(4);
  etSumEtaMaxMet_ = params_->etSumEtaMax(4);
}


l1t::Stage2Layer2JetSumAlgorithmFirmwareImp1::~Stage2Layer2JetSumAlgorithmFirmwareImp1() {


}


void l1t::Stage2Layer2JetSumAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::Jet> & jets,
							      std::vector<l1t::EtSum> & etsums) 
{
   int32_t ht(0), hx(0), hy(0); 

   for(std::vector<l1t::Jet>::const_iterator lIt = jets.begin() ; lIt != jets.end() ; ++lIt )
   {
			hy += (int32_t) ( lIt->hwPt() * std::trunc ( 511. * cos ( 6.28318530717958647693 * (72 - ( lIt->hwPhi() - 1 )) / 72.0 ) )) >> 9;
			hx += (int32_t) ( lIt->hwPt() * std::trunc ( 511. * sin ( 6.28318530717958647693 * ( lIt->hwPhi() - 1 ) / 72.0 ) )) >> 9;
			ht += lIt->hwPt();
   }
 
   hx >>=5;
   hy >>=5;
   ht >>=5;

   int32_t mht = (int32_t) std::sqrt( std::pow((double) hx,2) + std::pow((double) hy,2) );

   int32_t mhtPhi = 72 * atan2( (double) hy , (double) hx );
   while ( mhtPhi > 72 ) mhtPhi-=72;
   while ( mhtPhi < 1 ) mhtPhi+=72;
   
   mht &= 0xfff;
   ht  &= 0xfff;

   std::cout << "HT: "  << ht << std::endl;
   std::cout << "MHT: "  << mht << std::endl;
   std::cout << "MHTx: "  << hx << std::endl;
   std::cout << "MHTy: "  << hy << std::endl;

   math::XYZTLorentzVector p4;

   l1t::EtSum htSumht( p4 , l1t::EtSum::EtSumType::kTotalHt ,ht,0,0,0);
   l1t::EtSum htSumMissingHt( p4 , l1t::EtSum::EtSumType::kMissingHt ,mht,0,mhtPhi,0);
   l1t::EtSum htSumMissingHtx( p4 , l1t::EtSum::EtSumType::kTotalHtx ,hx,0,0,0);
   l1t::EtSum htSumMissingHty( p4 , l1t::EtSum::EtSumType::kTotalHty ,hy,0,0,0);

   etsums.push_back(htSumht);
   etsums.push_back(htSumMissingHt);
   etsums.push_back(htSumMissingHtx);
   etsums.push_back(htSumMissingHty);
}

