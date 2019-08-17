# Timeline {#design_timeline}

Timeline is an object which is used for buffering events. It accepts any data structure which inherits from the `HT_Event` class. User can create multiple timelines in the program. Each timeline has an internal buffer which stores events, and once the buffer is full, events are pushed to timeline listeners.

TODO: diagram

## Thread safety
Thread safety can be enabled or disabled by the user when creating new timeline (ht_timeline_create()). Enabling the feature allows user to safely push events from multiple threads to a single timeline. However, this feature introduce an overhead - for every ht_timeline_push() operation, the timeline locks and unlocks the mutex.

## Serialized events
The timeline can operate in two modes:
 * serialized events
 * non-serialized events

The difference between those two modes is that in the first one (serialized), the event is packed and converted into HTDUMP format (see @ref design_htdump_format), so the listener receives a stream of serialized events, and accessing particular field of the event requires un-serializing the stream.  
In the non-serialized mode, the timeline doesn't perform any serialization, therefore the listener can easily access any fields from the event.

### Event serialization - why?
Serializing events is useful when user wants to store the data or transmit it to another place - serialization reduces the size of the event, and unifies allignment, so de-serializing the event on the other machine doesn't require any knowledge about the machine which actually performed the capture.  
However, serializing the event in the timeline (not in the listener) might sound like a silly idea - listener should have a knowledge what to do next with the event, and whether the event should be serialized, or not.  
To explain why we decided to enable serialization on the timeline level, I use the example below:

```cpp
char* str = malloc(10);
strcpy(str, "test");
HT_TIMELINE_PUSH_EVENT(timeline, MyEventType, str);
free(str);
```

The timeline has internal buffer, therefore it's not guaranteed (and most likely - won't happen) that HT_TIMELINE_PUSH_EVENT() triggers the listener callback. This means, that the listener might receive an event, which is no longer invalid (the `str` pointer will be freed by then).  
If serialization is enabled for the timeline, the HT_TIMELINE_PUSH_EVENT() call will copy the event field-by-field, including string fields, so the data is always valid.

### When to use serialization mode
In general, the user should use serialization mode, if:
 * one of the event field is a dynamic string
 * the listener doesn't need to process events, and it only stores them somewhere

## Timeline listeners
As mentioned before, the timeline pushes all the events to its listeners when the internal buffer is full. Timeline listener is a function object that accepts a stream of serialized or non-serialized events.  
Timeline listener can be registered to multiple timeilnes at the same time - it's useful when user creates multiple timelines (e.g. one per thread), but still wants to get all the events streamed to a single instance of the listener.  
The operation of pushing events from timeline to listeners is only thread-safe when thread safety is enabled for the timeline, and the listener is registered to only one timeline. Otherwise, the timeline itself must implement thread safety feature if needed. All the existing listeners available in HawkTracer are thread safe.

## Timeline features
TODO

## Global timeline
To simplify common scenario, HawkTracer library provides a global timeline object (global_timeline.h). It's already pre-configured timeline:
 * access the timeline through ht_global_timeline_get() function
 * thread-safety is disabled, but the ht_global_timeline_get() function returns per-thread timeline (so it's safe to push events from multiple threads to the global timeline)
 * feature_callstack.h and feature_cached_string.h are enabled for the timeline
 * available on POSIX platforms or C++11 compilers
 
There's no need to create an instance of the global timeline - ht_global_timeline_get() will initialize the timeline in the first call. See [hawktracer-hello-world.c](@repocodeurl/examples/tutorials/hello_world/hawktracer-hello-world.c) for the example.
