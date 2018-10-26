#ifndef HAWKTRACER_REGISTRY_H
#define HAWKTRACER_REGISTRY_H

#include <hawktracer/timeline.h>

HT_DECLS_BEGIN

/**
 * A type of a callback which gets called when a timeline disables the feature.
 */
typedef void (*HT_FeatureDisableCallback)(HT_Timeline*);

/**
 * Registers new a new feature in the system.
 *
 * @param feature_id a feature identifier.
 * @param disable_callback a function which gets called when timeline disables the feature.
 *
 * @returns #HT_ERR_OK, if registration completed successfully; otherwise, appropriate error code.
 */
HT_API HT_ErrorCode ht_registry_register_feature(uint32_t feature_id, HT_FeatureDisableCallback disable_callback);

/**
 * Registers new event klass in the system, or gets identifier if klass is already registered.
 *
 * @param event_klass a pointer to a klass definition.
 *
 * @return an identifier to a klass, or #HT_INVALID_KLASS_ID if registration failed.
 */
HT_API HT_EventKlassId ht_registry_register_event_klass(HT_EventKlass* event_klass);

/**
 * Pushes information about registered event klasses (HT_EventKlassInfoEvent and HT_EventKlassFieldInfoEvent events) directly to a listener.
 *
 * @param callback the listener callback.
 * @param listener a pointer to the listener.
 */
HT_API void ht_registry_push_registry_klasses_to_listener(HT_TimelineListenerCallback callback, void* listener);

/* TODO: this API should probably be removed */
HT_API HT_EventKlass** ht_registry_get_event_klasses(size_t* out_klass_count);

HT_DECLS_END

#endif /* HAWKTRACER_REGISTRY_H */
