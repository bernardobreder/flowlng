// #include <CoreServices/CoreServices.h>

// void myCallbackFunction(
//     ConstFSEventStreamRef streamRef,
//     void *clientCallBackInfo,
//     size_t numEvents,
//     void *eventPaths,
//     const FSEventStreamEventFlags eventFlags[],
//     const FSEventStreamEventId eventIds[])
// {
//     int i;
//     char **paths = eventPaths;

//     // printf("Callback called\n");
//     for (i = 0; i < numEvents; i++)
//     {
//         int count;
//         /* flags are unsigned long, IDs are uint64_t */
//         printf("Change %llu in %s, flags %lu\n", eventIds[i], paths[i], (unsigned long)eventFlags[i]);
//     }
// }

// void watch()
// {
//     CFStringRef mypath = CFSTR("./data");
//     CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
//     void *callbackInfo = NULL; // could put stream-specific data here.
//     FSEventStreamRef stream;
//     CFAbsoluteTime latency = 3.0; /* Latency in seconds */

//     stream = FSEventStreamCreate(NULL,
//                                  &myCallbackFunction,
//                                  callbackInfo,
//                                  pathsToWatch,
//                                  kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
//                                  latency,
//                                  kFSEventStreamCreateFlagNone /* Flags explained in reference */
//                                  );

//     /* Create the stream before calling this. */
//     FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
// }