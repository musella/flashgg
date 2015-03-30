#ifndef FLASHgg_IsolationAlgoBase_h
#define FLASHgg_IsolationAlgoBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

// Shamelessly patterned on https://github.com/cms-sw/cmssw/blob/CMSSW_7_2_X/RecoParticleFlow/PFProducer/interface/BlockElementLinkerBase.h

namespace flashgg {

  class IsolationAlgoBase {

  public:
	  IsolationAlgoBase(const edm::ParameterSet& conf)  : name_(conf.getParameter<std::string>("name") ) {}
	  virtual ~IsolationAlgoBase();
	  
	  virtual void begin(const pat::Photon &,const edm::Event&, const edm::EventSetup &) {};
	  virtual bool hasChargedIsolation() = 0;	  
	  virtual float chargedIsolation(const edm::Ptr<pat::Photon> &, const edm::Ptr<reco::Vertex>, const flashgg::VertexCandidateMap & ) = 0;
	  virtual bool hasCaloIsolation(reco::PFCandidate::ParticleType) = 0;
	  virtual float caloIsolation(const edm::Ptr<pat::Photon> &, const edm::PtrVector<pat::PackedCandidate>&, reco::PFCandidate::ParticleType, const reco::Vertex * vtx=0) = 0;
	  
	  virtual void end(pat::Photon &) {};
	  
	  std::string name() const { return name_; };
	  
  private:
	  std::string name_;
  };
}

#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory< flashgg::IsolationAlgoBase* (const edm::ParameterSet&) > FlashggIsolationAlgoFactory;

#endif
