/*
 * lookup/hash_lookup.h
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

#ifndef _HASH_LOOKUP_H_
#define _HASH_LOOKUP_H_

typedef struct HashTable {
   char **keys;
   void **values;
   int capacity;
   int size;
} HashTable;

HashTable* hash_table_create(int capacity);
int hash_lookup(HashTable *table, const char *key, void **value);
int hash_insert(HashTable *table, const char *key, void *value);
int hash_remove(HashTable *table, const char *key);
void hash_table_free(HashTable *table);

#endif /* _HASH_LOOKUP_H_ */
