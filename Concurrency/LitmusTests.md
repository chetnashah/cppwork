Memory consistency litmus tests are small parallel programs
that are designed to illustrate subtle differences between
memory consistency models by exhibiting different outcomes
for different models.

E.g tests are here: https://github.com/litmus-tests

## Example litmust test: for store buffer reordering

Litmus Test: Write Queue (also called Store Buffer)
Can this program see `r1 = 0, r2 = 0`?

```
// both threads are writing to one location and reading from another unrelated cross-thread location.
// Thread 1           // Thread 2
x = 1                 y = 1
r1 = y                r2 = x
```

Test for it is https://github.com/litmus-tests/litmus-tests-x86/blob/main/tests/non-mixed-size/BASIC_2_THREAD/SB.litmus

## Litmus test: Independent reads of independent writes

Litmus Test: Independent Reads of Independent Writes (IRIW)
```
Can this program see r1 = 1, r2 = 0, r3 = 1, r4 = 0?
(Can Threads 3 and 4 see x and y change in different orders?)

// Thread 1    // Thread 2    // Thread 3    // Thread 4
x = 1          y = 1          r1 = x         r3 = y
                              r2 = y         r4 = x
On sequentially consistent hardware: no.
On x86 (or other TSO): no.
```

## Litmus test: Message passing

Litmus Test: Message Passing
Can this program see `r1 = 1, r2 = 0`?
```
// Thread 1           // Thread 2
x = 1                 r1 = y
y = 1                 r2 = x
On sequentially consistent hardware: no.
On x86 (or other TSO): no.
On ARM/POWER: yes!
```

## Litmus test: Load Buffering

Can this program see `r1 = 1, r2 = 1`?
(Can each thread's read happen after the other thread's write?)
```
// Thread 1    // Thread 2
r1 = x         r2 = y
y = 1          x = 1
On sequentially consistent hardware: no.
On x86 (or other TSO): no.
On ARM/POWER: yes!
```
