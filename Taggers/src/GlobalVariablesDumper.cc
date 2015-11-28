#include "flashgg/Taggers/interface/GlobalVariablesDumper.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TTree.h"

using namespace edm;
using namespace reco;

namespace flashgg {


    GlobalVariablesDumper::GlobalVariablesDumper( const ParameterSet &cfg ) :
        GlobalVariablesComputer( cfg )
    {
        if( cfg.exists( "addTriggerBits" ) ) {
            const auto &trg = cfg.getParameter<ParameterSet>( "addTriggerBits" );
            triggerTag_ = trg.getParameter<InputTag>( "tag" );
            auto bitNames   = trg.getParameter<std::vector<std::string> >( "bits" );
            for( auto &bit : bitNames ) {
                bits_.push_back( std::make_pair( bit, false ) );
            }
        }

        if( cfg.exists( "extraFloats" ) ){
            const auto extraFloats = cfg.getParameter<ParameterSet>( "extraFloats" );
            extraFloatNames_ = extraFloats.getParameterNamesForType<InputTag>();
            extraFloatVariables_.resize(extraFloatNames_.size(),0.);
            for( auto & name : extraFloatNames_ ) {
                extraFloatTags_.push_back( extraFloats.getParameter<InputTag>(name) );
            }
        }
    }


    GlobalVariablesDumper::~GlobalVariablesDumper()
    {}

    void GlobalVariablesDumper::bookTreeVariables( TTree *tree, const std::map<std::string, std::string> &replacements )
    {
        tree->Branch( "rho", &cache_.rho );
        tree->Branch( "nvtx", &cache_.nvtx );
        tree->Branch( "event", &cache_.event, "event/I" );
        tree->Branch( "lumi", &cache_.lumi, "lumi/b" );
        tree->Branch( "run", &cache_.run, "run/i" );
        tree->Branch( "nvtx", &cache_.nvtx );
        if( getPu_ ) {
            tree->Branch( "npu", &cache_.npu );
            if( puReWeight_ ) { tree->Branch( "puweight", &cache_.puweight ); }
        }
        for( auto &bit : bits_ ) {
            tree->Branch( bit.first.c_str(), &bit.second, ( bit.first + "/O" ).c_str() );
        }
        for( size_t iextra = 0; iextra<extraFloatNames_.size(); ++iextra ) {
            tree->Branch( extraFloatNames_[iextra].c_str(), &extraFloatVariables_[iextra] );
        }
    }

    void GlobalVariablesDumper::fill( const EventBase &evt )
    {
        update( evt );
        if( ! bits_.empty() ) {
            Handle<TriggerResults> trigResults; //our trigger result object
            evt.getByLabel( triggerTag_, trigResults );

            for( auto &bit : bits_ ) { bit.second = false; }
            auto &trigNames = evt.triggerNames( *trigResults );
            for( size_t itrg = 0; itrg < trigNames.size(); ++itrg ) {
                if( ! trigResults->accept( itrg ) ) { continue; }
                auto pathName = trigNames.triggerName( itrg );
                for( auto &bit : bits_ ) {
                    if( pathName.find( bit.first ) != std::string::npos ) {
                        bit.second = true;
                        break;
                    }
                }
            }
        }
        for( size_t iextra = 0; iextra<extraFloatTags_.size(); ++iextra ) {
            try {
                Handle<float> ihandle; 
                evt.getByLabel( extraFloatTags_[iextra], ihandle );
                extraFloatVariables_[iextra] = *ihandle;
            } catch (...) {
                Handle<std::vector<float> > ihandle; 
                evt.getByLabel( extraFloatTags_[iextra], ihandle );
                assert( ihandle->size() == 1 );
                extraFloatVariables_[iextra] = (*ihandle)[0];

            }
        }
    }


}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

