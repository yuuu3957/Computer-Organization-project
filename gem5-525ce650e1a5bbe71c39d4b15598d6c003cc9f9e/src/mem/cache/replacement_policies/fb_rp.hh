#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_FB_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_FB_RP_HH__

#include "base/types.hh"
#include "mem/cache/replacement_policies/base.hh"

struct FBRPParams;

class FBRP : public BaseReplacementPolicy
{
  protected:
    /** FB-specific implementation of replacement data. */
    struct FBReplData : ReplacementData
    {
      /** Number of times the block has been accessed. */
      uint64_t RequestFrequency;

      /** Last time the block was accessed. */
      Tick LastTime;

      FBReplData() : RequestFrequency(0), LastTime(0) {}
    };

  public:
    /** Convenience typedef. */
    typedef FBRPParams Params;

    /**
     * Construct and initiliaze this replacement policy.
     */
    FBRP(const Params *p);

    /**
     * Destructor.
     */
    ~FBRP() {}

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Reset insertion tick to 0.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                              const override;

    /**
     * Touch an entry to update its replacement data.
     * Does not modify the replacement data.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Sets its insertion tick.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Find replacement victim using insertion timestamps.
     *
     * @param cands Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_FB_RP_HH__
