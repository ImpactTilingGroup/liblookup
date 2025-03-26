/*
 * array_lookup.c
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

#include <lookup/array_lookup.h>
#include <stdlib.h>

/* Array lookup functions */
int int_array_lookup(int *arr, int size, int target)
{
   for (int i = 0; i < size; i++) {
       if (arr[i] == target) {
           return i;  // Found
       }
   }
   return -1;  // Not found
}

float float_array_lookup(float *arr, int size, float target)
{
   for (int i = 0; i < size; i++) {
       if (arr[i] == target) {
           return i;  // Found
       }
   }
   return -1;  // Not found
}

void* pointer_array_lookup(void **arr, int size, void *target)
{
   for (int i = 0; i < size; i++) {
       if (arr[i] == target) {
           return arr[i];  // Found
       }
   }
   return NULL;  // Not found
}

/* Binary search functions */
int binary_search_int(int *arr, int size, int target)
{
   int left = 0, right = size - 1;
   while (left <= right) {
       int mid = left + (right - left) / 2;
       if (arr[mid] == target) {
           return mid;  // Found
       }
       if (arr[mid] < target) {
           left = mid + 1;
       } else {
           right = mid - 1;
       }
   }
   return -1;  // Not found
}

float binary_search_float(float *arr, int size, float target)
{
   int left = 0, right = size - 1;
   while (left <= right) {
       int mid = left + (right - left) / 2;
       if (arr[mid] == target) {
           return mid;  // Found
       }
       if (arr[mid] < target) {
           left = mid + 1;
       } else {
           right = mid - 1;
       }
   }
   return -1;  // Not found
}
