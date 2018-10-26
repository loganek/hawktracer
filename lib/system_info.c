#include "hawktracer/system_info.h"
#include "internal/event_utils.h"

HT_Endianness
ht_system_info_get_endianness(void)
{
    const int i = 1;
    return *(HT_Byte*)&i == 0 ? HT_ENDIANNESS_BIG : HT_ENDIANNESS_LITTLE;
}

/* TODO: init_event() instead? */
void
ht_system_info_push_system_info_to_listener(HT_TimelineListenerCallback callback, void* listener)
{
    HT_DECL_EVENT(HT_SystemInfoEvent, event);
    event.version_major = HT_VERSION_MAJOR;
    event.version_minor = HT_VERSION_MINOR;
    event.version_patch = HT_VERSION_PATCH;

    // TODO do we need this function ? data_size = ht_event_utils_serialize_event_to_buffer(HT_EVENT(&event), buffer, serialize);

    callback(HT_EVENT(&event), listener);
}

/* TODO init_event() instead? */
void
ht_system_info_push_endianness_info_to_listener(HT_TimelineListenerCallback callback, void* listener)
{
    HT_DECL_EVENT(HT_EndiannessInfoEvent, event);
    event.endianness = (uint8_t) ht_system_info_get_endianness();

    callback(HT_EVENT(&event), listener);
}
