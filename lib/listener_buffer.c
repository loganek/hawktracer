#include "hawktracer/alloc.h"
#include "hawktracer/events.h"
#include "internal/listener_buffer.h"

#include <string.h>

#define HT_MIN(a, b) (((a) > (b)) ? (b) : (a))

HT_ErrorCode
ht_listener_buffer_init(HT_ListenerBuffer* buffer, size_t max_size)
{
    buffer->data = ht_alloc(max_size);

    if (buffer->data == NULL)
    {
        return HT_ERR_OUT_OF_MEMORY;
    }

    buffer->max_size = max_size;
    buffer->usage = 0;

    return HT_ERR_OK;
}

void
ht_listener_buffer_deinit(HT_ListenerBuffer* buffer)
{
    ht_free(buffer->data);
}

void
ht_listener_buffer_process_unserialized_events(HT_ListenerBuffer* buffer,
                                               HT_Event* event,
                                               HT_ListenerFlushCallback flush_callback,
                                               void* listener)
{
    if (HT_EVENT_GET_KLASS(event)->get_size(event) + buffer->usage > buffer->max_size)
    {
        flush_callback(listener);
        buffer->usage = 0;
    }

    buffer->usage += HT_EVENT_GET_KLASS(event)->serialize(event, buffer->data + buffer->usage);
}
