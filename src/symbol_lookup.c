/*
 * symbol_lookup.c - Dynamic library symbol lookup
 *
 * liblookup - a platform-independent runtime and static lookup library
 *
 * Copyright (c) 2025 Impact Tiling Group Pty Ltd.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <lookup/symbol_lookup.h>

/* Dynamically find a symbol in a library */
void* symbol_lookup(const char *libname, const char *symbol)
{
   void *handle = dlopen(libname, RTLD_LAZY);
   if (!handle) {
       fprintf(stderr, "Error opening library %s: %s\n", libname, dlerror());
       return NULL;
   }
   void *sym = dlsym(handle, symbol);
   if (!sym) {
       fprintf(stderr, "Error finding symbol %s: %s\n", symbol, dlerror());
       dlclose(handle);
       return NULL;
   }
   return sym;
}
