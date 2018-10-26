#ifndef HAWKTRACER_SYSTEM_INFO_H
#define HAWKTRACER_SYSTEM_INFO_H

#include <hawktracer/core_events.h>

HT_DECLS_BEGIN

HT_API HT_Endianness ht_system_info_get_endianness(void);

HT_API void ht_system_info_push_endianness_info_to_listener(HT_TimelineListenerCallback callback, void* listener);

HT_API void ht_system_info_push_system_info_to_listener(HT_TimelineListenerCallback callback, void* listener);

HT_DECLS_END

#endif /* HAWKTRACER_SYSTEM_INFO_H */
