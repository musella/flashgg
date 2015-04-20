/* \class CandViewRefSelector
 *
 * Candidate Selector based on a configurable cut.
 * Reads a edm::View<Candidate> as input
 * and saves a OwnVector of clones.
 * Usage:
 *
 * module selectedCands = CandViewSelector {
 *   InputTag src = myCollection
 *   string cut = "pt > 15.0"
 * };
 *
 * \author: Luca Lista, INFN
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/MicroAOD/interface/CutBasedPhotonObjectSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelectorStream.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelectorPlusEvent.h"

#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

typedef SingleObjectSelector <
edm::View<flashgg::Photon>,
    StringCutObjectSelector<flashgg::Photon, true>,
    std::vector<flashgg::Photon>
    > PhotonSelector;

typedef ObjectSelectorStream <
SingleElementCollectionSelectorPlusEvent <
edm::View<flashgg::Photon>,
    flashgg::CutBasedPhotonObjectSelector,
    std::vector<flashgg::Photon>
    >,
    std::vector<flashgg::Photon> > GenericPhotonSelector;


DEFINE_FWK_MODULE( PhotonSelector );
DEFINE_FWK_MODULE( GenericPhotonSelector );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

