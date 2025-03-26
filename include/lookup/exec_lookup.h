/*
 * lookup/exec_lookup.h - Static analysis and binary inspection
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

#ifndef _EXEC_LOOKUP_H_
#define _EXEC_LOOKUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __APPLE__
   #include <mach-o/loader.h>
   #define PLATFORM_MACHO 1
#else
   #include <elf.h>
   #define PLATFORM_ELF   1
#endif

int find_symbol_in_executable(const char *filename, const char *symbol);
int parse_exec_and_find_symbol(int fd, const char *symbol);

#endif /* EXEC_LOOKUP_H */
