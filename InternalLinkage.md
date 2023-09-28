
## What is it?

extern linkage variable is visible in all files
internal linkage variable is visible in single file.

## which ones have internal linkage?

The following objects have internal linkage by default:

1. `const` objects
2. `constexpr` objects
3. `typedef` objects
4. `static objects` in namespace scope

## Global variables

Explain: const variables internally link by default unless otherwise declared as extern

1. by default, global variable is external linkage
2. but, `const` global variable is internal linkage
3. extra, `extern const` global variable is external linkage

