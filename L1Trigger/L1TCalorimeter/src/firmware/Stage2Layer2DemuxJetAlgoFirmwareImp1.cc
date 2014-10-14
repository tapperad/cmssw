///
/// \class l1t::Stage2Layer2JetAlgorithmFirmwareImp1
///
/// \author: 
///
/// Description: 

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2DemuxJetAlgoFirmware.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "L1Trigger/L1TCalorimeter/interface/BitonicSort.h"

#include <vector>
#include <algorithm>

l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::Stage2Layer2DemuxJetAlgoFirmwareImp1(CaloParams* params) :
  params_(params)
{


}


l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::~Stage2Layer2DemuxJetAlgoFirmwareImp1() {


}


void l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::processEvent(const std::vector<l1t::Jet> & inputJets,
                                                             std::vector<l1t::Jet> & outputJets) {

  // Set the output jets to the input jets
  outputJets = inputJets;


  // Sort the jets by pT
  std::vector<l1t::Jet>::iterator start(outputJets.begin());
  std::vector<l1t::Jet>::iterator end(outputJets.end());

  //BitonicSort< l1t::Jet >(down,start,end);

  // Transform the eta and phi onto the ouput scales to GT 
  for (std::vector<l1t::Jet>::const_iterator jet = outputJets.begin(); jet != outputJets.end(); ++jet )
    {

    }

}

