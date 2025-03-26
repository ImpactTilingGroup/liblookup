/*
 * lookup/string_lookup.h
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

#ifndef _STRING_LOOKUP_H_
#define _STRING_LOOKUP_H_

int string_lookup(const char *arr[], int size, const char *target);
int case_insensitive_string_lookup(const char *arr[], int size, const char *target);
int binary_search_string(const char *arr[], int size, const char *target);

#endif /* STRING_LOOKUP_H */
