#include "mem/cache/replacement_policies/fb_rp.hh"

#include <cassert>
#include <memory>

#include "params/FBRP.hh"

FBRP::FBRP(const Params *p)
    : BaseReplacementPolicy(p)
{
}

void
FBRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    // Request frequency
    std::static_pointer_cast<FBReplData>(replacement_data)->RequestFrequency = 0;
    // last use time
    std::static_pointer_cast<FBReplData>(replacement_data)->LastTime = Tick(0);

    
}

void
FBRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // update request frequency
    std::static_pointer_cast<FBReplData>(replacement_data)->RequestFrequency++;
    // update last use time
    std::static_pointer_cast<FBReplData>(replacement_data)->LastTime = curTick();

}

void
FBRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // set the initial frequency count to 1
    std::static_pointer_cast<FBReplData>(replacement_data)->RequestFrequency = 1;
    // set the last use time to current time
    std::static_pointer_cast<FBReplData>(replacement_data)->LastTime = curTick();
}

ReplaceableEntry*
FBRP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        // Select victim as min F
        if (std::static_pointer_cast<FBReplData>(candidate->replacementData)->RequestFrequency < std::static_pointer_cast<FBReplData>(victim->replacementData)->RequestFrequency) {
            victim = candidate;
        }
        // Select victim as min F and the Oldest data
        else if (std::static_pointer_cast<FBReplData>(candidate->replacementData)->RequestFrequency == std::static_pointer_cast<FBReplData>(victim->replacementData)->RequestFrequency
              && std::static_pointer_cast<FBReplData>(candidate->replacementData)->LastTime < std::static_pointer_cast<FBReplData>(victim->replacementData)->LastTime) {
                victim = candidate;    
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
FBRP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new FBReplData());
}

FBRP*
FBRPParams::create()
{
    return new FBRP(this);
}
