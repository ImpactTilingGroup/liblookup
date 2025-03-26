/*
 * string_lookup.c
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

#include <lookup/string_lookup.h>
#include <string.h>
#include <ctype.h>

/*
 * Case-sensitive and case-insensitive string lookup functions.
 */
int string_lookup(const char *arr[], int size, const char *target)
{
   for (int i = 0; i < size; i++) {
       if (strcmp(arr[i], target) == 0) {
           return i;
       }
   }
   return -1;
}

int case_insensitive_string_lookup(const char *arr[], int size, const char *target)
{
   for (int i = 0; i < size; i++) {
       if (strcasecmp(arr[i], target) == 0) {
           return i;
       }
   }
   return -1;
}

/*
 * Search a binary for a string.
 */
int binary_search_string(const char *arr[], int size, const char *target)
{
   int left = 0, right = size - 1;
   while (left <= right) {
       int mid = left + (right - left) / 2;
       int cmp = strcmp(arr[mid], target);
       if (cmp == 0) {
           return mid;
       }
       if (cmp < 0) {
           left = mid + 1;
       } else {
           right = mid - 1;
       }
   }
   return -1;
}
