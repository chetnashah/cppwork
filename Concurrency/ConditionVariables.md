
Condition variables enable threads to be synchronized via messages. Kind of like wait/notify synchronization between threads, usually for some shared common condition variable e.g. `isEmpty` or `isFull` or `isReady` etc for a shared resource/memory.

Since we see above, that these are mostly shared boolean values, that start with `isXYZ`, sometimes `std::atomic<bool>` might also  be used instead of `std::condition_variable`. but there will be no notification semantics in `std::atomic<bool>`, and checkers must explicitly loop to check value of `isXYZ` in a loop.

## What is it?

The receiver waits for notification from the sender.
**Typical use cases for condition variables are sender-receiver or producer-consumer workflows.**

