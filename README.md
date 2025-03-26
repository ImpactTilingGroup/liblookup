# liblookup

liblookup is a platform-independent runtime and static lookup library, compatible with macOS, FreeBSD, and Linux. It provides a set of functions for performing various lookup operations, such as array lookups, string lookups, hash table lookups, and symbol lookups.

## Installation

To build and install liblookup, run:

```bash
sudo make install
```

This command will compile the library and install it to `/usr/local/lib` and `/usr/local/include`.

## Usage

To use liblookup in your project, include the `lookup/lookup.h` header file and link against the `liblookup` library.

## Example

```c
#include <lookup/lookup.h>
#include <stdio.h>

int main() {
   // Array lookup
   int arr[] = {1, 2, 3, 4};
   int idx = int_array_lookup(arr, 4, 3);
   printf("Found 3 at index: %d\n", idx);

   // String lookup
   const char *strings[] = {"apple", "banana", "cherry"};
   int idx2 = string_lookup(strings, 3, "banana");
   printf("Found 'banana' at index: %d\n", idx2);

   // Hash table lookup
   HashTable *table = hash_table_create(10);
   hash_insert(table, "key1", "value1");
   void *val;
   if (hash_lookup(table, "key1", &val)) {
       printf("Found 'key1' with value: %s\n", (char*)val);
   }

   // Symbol lookup examples
   void *func = symbol_lookup("/usr/local/lib/liblookup.dylib", "string_lookup");
    if (func) {
        printf("Found symbol 'string_lookup' in liblookup.dylib\n");
    }

    const char* executables[] = {
        "/usr/bin/gcc",
        "/bin/sh"
    };
    
    const char* symbol = "main";

    for (int i = 0; i < 3; i++) {
        result = find_symbol_in_executable(executables[i], symbol);
        printf("%s: main symbol %s\n",
               executables[i],
               (result == 1) ? "exists" : "not found");
    }

    return 0;
}
```
