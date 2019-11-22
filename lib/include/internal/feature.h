#ifndef HAWKTRACER_INTERNAL_FEATURE_H
#define HAWKTRACER_INTERNAL_FEATURE_H

#include <hawktracer/timeline.h>

#include <assert.h>
#include <string.h>

HT_DECLS_BEGIN

#define HT_TIMELINE_FEATURE_MAGICK ((void*)0x31337)
#define HT_TIMELINE_FEATURE_IS_VALID(feature) (feature.magick == HT_TIMELINE_FEATURE_MAGICK)

typedef struct
{
    void* magick;
    void *ptr;
    void(*destroy)(void*);
} HT_TimelineFeature;

static void ht_timeline_feature_init(HT_TimelineFeature* feature, void* ptr, HT_FeatureDestroyCallback destroy_cb)
{
    feature->magick = HT_TIMELINE_FEATURE_MAGICK;
    feature->ptr = ptr;
    feature->destroy = destroy_cb;
}

static void ht_timeline_feature_deinit(HT_TimelineFeature* feature)
{
    assert(feature);

    if (feature->destroy)
    {
        feature->destroy(feature->ptr);
    }
    memset(feature, 0, sizeof(HT_TimelineFeature));
}

HT_DECLS_END

#endif /* HAWKTRACER_INTERNAL_FEATURE_H */
