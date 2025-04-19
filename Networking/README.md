

## Listening side

Remember - `SBLA`
1. `socket()`
2. `bind()`
3. `listen()`
4. In a while loop - `accept()`

## Client side

Remember - `SC(SR)C`
1. `socket()`
2. `connect()`
3. `send/recv`
4. `close`

## DNS functions

1. `gethostbyname` (Deprecated):
Usage: Legacy IPv4-only resolution.
Pros: Simple and straightforward for basic IPv4 lookups.
Cons: Lacks IPv6 support and thread-safety.
2. `getaddrinfo`:
Usage: Modern, versatile name resolution supporting both IPv4 and IPv6.
Pros: Thread-safe, flexible, and future-proof.
Cons: Slightly more complex to use due to its linked list return structure.
