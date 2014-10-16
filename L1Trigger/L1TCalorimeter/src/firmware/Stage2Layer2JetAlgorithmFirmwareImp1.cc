///
/// \class l1t::Stage2Layer2JetAlgorithmFirmwareImp1
///
/// \author: Adam Elwood and Matthew Citron
///
/// Description: Implementation of Jad's asymmetric map overlap algorithm with donut subtraction

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2JetAlgorithmFirmware.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"
#include "L1Trigger/L1TCalorimeter/interface/BitonicSort.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"

#include <vector>
#include <algorithm>
#include <math.h>

bool operator > ( l1t::Jet& a, l1t::Jet& b )
{
  if ( a.hwPt() > b.hwPt() ){ 
    return true;
  } else {

    return false;
  }
}

namespace{
  bool sortbypt(const l1t::Jet &a, const l1t::Jet &b) { return a.hwPt() > b.hwPt(); };
  bool sortbyeta(const l1t::Jet &a, const l1t::Jet &b) { return a.hwEta() < b.hwEta(); };
}

l1t::Stage2Layer2JetAlgorithmFirmwareImp1::Stage2Layer2JetAlgorithmFirmwareImp1(CaloParams* params) :
  params_(params){}


l1t::Stage2Layer2JetAlgorithmFirmwareImp1::~Stage2Layer2JetAlgorithmFirmwareImp1() {}

void l1t::Stage2Layer2JetAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::CaloTower> & towers,
                                                             std::vector<l1t::Jet> & jets) {

  // find all possible jets

  edm::LogInfo("L1Emulator") << "Number of towers = " << towers.size();

  if(towers.size()>0){

    create(towers, jets, params_->jetPUSType());

    // sort
    sort(jets);
  }
}


void l1t::Stage2Layer2JetAlgorithmFirmwareImp1::create(const std::vector<l1t::CaloTower> & towers,
                                                       std::vector<l1t::Jet> & jets, std::string PUSubMethod) {

  //Declare the range to carry out the algorithm over
  int etaMax=40, etaMin=-40, phiMax=72, phiMin=1;

  // generate jet mask
  // needs to be configurable at some point
  // just a square for now
  // for 1 do greater than, for 2 do greater than equal to
  int mask[9][9] = {
    { 1,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,2 },
    { 1,1,1,1,1,1,1,2,2 },
    { 1,1,1,1,1,1,2,2,2 },
    { 1,1,1,1,0,2,2,2,2 },
    { 1,1,1,2,2,2,2,2,2 },
    { 1,1,2,2,2,2,2,2,2 },
    { 1,2,2,2,2,2,2,2,2 },
    { 2,2,2,2,2,2,2,2,2 }
  };

  // loop over jet positions
  for ( int ieta = etaMin ; ieta <= etaMax ; ++ieta ) {
    if (ieta==0) continue;
    for ( int iphi = phiMin ; iphi <= phiMax ; ++iphi ) {

      const CaloTower& tow = CaloTools::getTower(towers, ieta, iphi); 

      int seedEt=tow.hwPt();
      int iEt(seedEt);
      bool vetoCandidate(false);

      //Check it passes the seed threshold
      if(iEt < floor(params_->jetSeedThreshold()/params_->towerLsbSum())) continue;

      // loop over towers in this jet
      for( int deta = -4; deta < 5; ++deta ) {
        for( int dphi = -4; dphi < 5; ++dphi ) {

          int towEt = 0;
          int ietaTest = ieta+deta;
          int iphiTest = iphi+dphi;

          //Wrap around phi
	  while ( iphiTest > phiMax ) iphiTest -= phiMax;
	  while ( iphiTest < phiMin ) iphiTest += phiMax;

          // Wrap over eta=0
	  if (ieta > 0 && ietaTest <=0){
	    ietaTest = ietaTest-1;
	  }
          
	  if (ieta < 0 && ietaTest >=0){
	    ietaTest = ietaTest+1;
	  }

          // check jet mask and sum tower et
          // re-use calo tools sum method, but for single tower
          if( mask[deta+4][dphi+4] == 1 ) { //Do greater than
            if(ietaTest <= etaMax && ietaTest >= etaMin){ //Only check if in the eta range
              const CaloTower& tow = CaloTools::getTower(towers, ietaTest, iphiTest); 
              towEt = tow.hwPt();
              iEt+=towEt;
            }
            vetoCandidate=(seedEt<towEt);
          }
          else if( mask[deta+4][dphi+4] == 2 ) { //Do greater than equal to
            if(ietaTest <= etaMax && ietaTest >= etaMin){ //Only check if in the eta range
              const CaloTower& tow = CaloTools::getTower(towers, ietaTest, iphiTest); 
              towEt = tow.hwPt();
              iEt+=towEt;
            }
            vetoCandidate=(seedEt<=towEt);
          }
          if(vetoCandidate) break; 
        }
        if(vetoCandidate) break; 
      }

      // add the jet to the list
      if (!vetoCandidate) {

        if (PUSubMethod == "Donut") iEt -= donutPUEstimate(ieta, iphi, 5, towers);
        
        if (PUSubMethod == "ChunkyDonut") iEt -= chunkyDonutPUEstimate(ieta, iphi, 5, towers);

        if(iEt>0){

	  math::XYZTLorentzVector p4;
          l1t::Jet jet( p4, iEt, ieta, iphi, 0);
          jets.push_back( jet );
        }
      }

    }
  }
}

//A function to return the value for donut subtraction around an ieta and iphi position for donut subtraction
//Also pass it a vector to store the individual values of the strip for later testing
//The size is the number of ieta/iphi units out the ring is (ie for 9x9 jets, we want the 11x11 for PUS therefore we want to go 5 out, so size is 5)
int l1t::Stage2Layer2JetAlgorithmFirmwareImp1::donutPUEstimate(int jetEta, int jetPhi, int size, const std::vector<l1t::CaloTower> & towers){

  //Declare the range to carry out the algorithm over
  int etaMax=40, etaMin=-40, phiMax=72, phiMin=1;

  //ring is a vector with 4 ring strips, one for each side of the ring
  std::vector<int> ring(4,0);

  int iphiUp = jetPhi + size;
  while ( iphiUp > phiMax ) iphiUp -= phiMax;
  int iphiDown = jetPhi - size;
  while ( iphiDown < phiMin ) iphiDown += phiMax;

  int ietaUp = (jetEta + size > etaMax) ? 999 : jetEta+size;
  int ietaDown = (jetEta - size < etaMin) ? 999 : jetEta-size;

  for (int ieta = jetEta - size+1; ieta < jetEta + size; ++ieta)   
    {

      if (ieta > etaMax || ieta < etaMin) continue;
      int towerEta;

      if (jetEta > 0 && ieta <=0){
        towerEta = ieta-1;
      } else if (jetEta < 0 && ieta >=0){
        towerEta = ieta+1;
      } else {
        towerEta=ieta;
      }

      const CaloTower& tow = CaloTools::getTower(towers, towerEta, iphiUp);
      int towEt = tow.hwPt();
      ring[0]+=towEt;

      const CaloTower& tow2 = CaloTools::getTower(towers, towerEta, iphiDown);
      towEt = tow2.hwPt();
      ring[1]+=towEt;

    } 

  for (int iphi = jetPhi - size+1; iphi < jetPhi + size; ++iphi)   
    {

      int towerPhi = iphi;
      while ( towerPhi > phiMax ) towerPhi -= phiMax;
      while ( towerPhi < phiMin ) towerPhi += phiMax;
      
      const CaloTower& tow = CaloTools::getTower(towers, ietaUp, towerPhi);
      int towEt = tow.hwPt();
      ring[2]+=towEt;

      const CaloTower& tow2 = CaloTools::getTower(towers, ietaDown, towerPhi);
      towEt = tow2.hwPt();
      ring[3]+=towEt;
    } 

  //for the Donut Subtraction we only use the middle 2 (in energy) ring strips
  std::sort(ring.begin(), ring.end(), std::greater<int>());

  return 4*( ring[1]+ring[2] ); // This should really be multiplied by 4.5 not 4.
}

int l1t::Stage2Layer2JetAlgorithmFirmwareImp1::chunkyDonutPUEstimate(int jetEta, int jetPhi, int pos, const std::vector<l1t::CaloTower> & towers){

  //Declare the range to carry out the algorithm over
  int etaMax=40, etaMin=-40, phiMax=72, phiMin=1;

  //ring is a vector with 4 ring strips, one for each side of the ring
  std::vector<int> ring(4,0);

  // Loop over number of strips
 
  for (int size=pos; size<pos+3; size++){

    int iphiUp = jetPhi + size;
    while ( iphiUp > phiMax ) iphiUp -= phiMax;
    int iphiDown = jetPhi - size;
    while ( iphiDown < phiMin ) iphiDown += phiMax;

    int ietaUp = (jetEta + size > etaMax) ? 999 : jetEta+size;
    int ietaDown = (jetEta - size < etaMin) ? 999 : jetEta-size;

    for (int ieta = jetEta - size+1; ieta < jetEta + size; ++ieta)   
      {

        if (ieta > etaMax || ieta < etaMin) continue;
        int towerEta;

        if (jetEta > 0 && ieta <=0){
          towerEta = ieta-1;
        } else if (jetEta < 0 && ieta >=0){
          towerEta = ieta+1;
        } else {
          towerEta=ieta;
        }

        const CaloTower& tow = CaloTools::getTower(towers, towerEta, iphiUp);
        int towEt = tow.hwPt();
        ring[0]+=towEt;

        const CaloTower& tow2 = CaloTools::getTower(towers, towerEta, iphiDown);
        towEt = tow2.hwPt();
        ring[1]+=towEt;

      } 

    for (int iphi = jetPhi - size+1; iphi < jetPhi + size; ++iphi)   
      {

        int towerPhi = iphi;
        while ( towerPhi > phiMax ) towerPhi -= phiMax;
        while ( towerPhi < phiMin ) towerPhi += phiMax;
      
        const CaloTower& tow = CaloTools::getTower(towers, ietaUp, towerPhi);
        int towEt = tow.hwPt();
        ring[2]+=towEt;

        const CaloTower& tow2 = CaloTools::getTower(towers, ietaDown, towerPhi);
        towEt = tow2.hwPt();
        ring[3]+=towEt;
      } 

  }
  //for the Donut Subtraction we only use the middle 2 (in energy) ring strips
  std::sort(ring.begin(), ring.end(), std::greater<int>());

  return ( ring[1]+ring[2] ); 
}

void l1t::Stage2Layer2JetAlgorithmFirmwareImp1::sort(std::vector<l1t::Jet> & jets) {

  //std::vector<l1t::Jet>::iterator start(jets.begin());
  //std::vector<l1t::Jet>::iterator end(jets.end());	

  //BitonicSort< l1t::Jet >(down,start,end);

  // sort the jets first eta then pT and return only the top 6 from each hemisphere
  std::sort(jets.begin(), jets.end(), sortbyeta);
  std::sort(jets.begin(), jets.end(), sortbypt);

  if (jets.size()>12) jets.resize(12); // truncate to top 12 jets for now   
  //  if (jets.size()>6) jets.resize(6); // Remember to remove this!
}

