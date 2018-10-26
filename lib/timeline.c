#include <hawktracer/timeline.h>
#include <hawktracer/alloc.h>

#include "internal/feature.h"
#include "internal/registry.h"
#include "internal/mutex.h"
#include "internal/timeline_listener_container.h"

#include <string.h>
#include <assert.h>

struct _HT_Timeline
{
    void* features[HT_TIMELINE_MAX_FEATURES];
    HT_EventIdProvider* id_provider;
    HT_TimelineListenerContainer* listeners;
};

void
ht_timeline_init_event(HT_Timeline* timeline, HT_Event* event)
{
    event->timestamp = ht_monotonic_clock_get_timestamp();
    event->id = ht_event_id_provider_next(timeline->id_provider);
}

void
ht_timeline_push_event(HT_Timeline* timeline, HT_Event* event)
{
    size_t i;
    for (i = 0; i < timeline->listeners->user_datas.size; i++)
    {
        (*(HT_TimelineListenerCallback*)&timeline->listeners->callbacks.data[i])
                (event, timeline->listeners->user_datas.data[i]);
    }
}

void
ht_timeline_set_feature(HT_Timeline* timeline, size_t feature_id, void* feature)
{
    timeline->features[feature_id] = feature;
}

void*
ht_timeline_get_feature(HT_Timeline* timeline, size_t feature_id)
{
    return timeline->features[feature_id];
}


HT_ErrorCode
ht_timeline_register_listener(
        HT_Timeline* timeline,
        HT_TimelineListenerCallback callback,
        void* user_data)
{
    return ht_timeline_listener_container_register_listener(
                timeline->listeners, callback, user_data);
}

void
ht_timeline_unregister_all_listeners(HT_Timeline* timeline)
{
    ht_timeline_listener_container_unregister_all_listeners(
                timeline->listeners);
}

HT_Timeline*
ht_timeline_create(HT_ErrorCode* out_err)
{
    HT_ErrorCode error_code = HT_ERR_OK;
    HT_Timeline* timeline = HT_CREATE_TYPE(HT_Timeline);

    if (timeline == NULL)
    {
        error_code = HT_ERR_OUT_OF_MEMORY;
        goto done;
    }

    /* TODO find_or_create_listener: simplify? */
    /* TODO: error handling */
    timeline->listeners = ht_find_or_create_listener(NULL);

    timeline->id_provider = ht_event_id_provider_get_default();
    memset(timeline->features, 0, sizeof(timeline->features));

done:
    if (out_err != NULL)
    {
        *out_err = error_code;
    }

    return timeline;
}

void
ht_timeline_destroy(HT_Timeline* timeline)
{
    size_t i;

    assert(timeline);

    ht_timeline_listener_container_unref(timeline->listeners);

    for (i = 0; i < sizeof(timeline->features) / sizeof(timeline->features[0]); i++)
    {
        if (timeline->features[i])
        {
            ht_feature_disable(timeline, i);
            timeline->features[i] = NULL;
        }
    }

    ht_free(timeline);
}

HT_EventIdProvider*
ht_timeline_get_id_provider(HT_Timeline* timeline)
{
    return timeline->id_provider;
}
