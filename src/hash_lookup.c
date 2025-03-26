/*
 * hash_lookup.c
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

#include <lookup/hash_lookup.h>
#include <string.h>
#include <stdlib.h>

unsigned int hash_function(const char *str)
{
   unsigned int hash = 0;
   while (*str) {
       hash = (hash << 5) + *str++;
   }
   return hash;
}

HashTable* hash_table_create(int capacity)
{
   HashTable *table = malloc(sizeof(HashTable));
   table->capacity = capacity;
   table->size = 0;
   table->keys = calloc(capacity, sizeof(char*));
   table->values = calloc(capacity, sizeof(void*));
   return table;
}

int hash_lookup(HashTable *table, const char *key, void **value)
{
   unsigned int hash = hash_function(key) % table->capacity;
   if (table->keys[hash] && strcmp(table->keys[hash], key) == 0) {
       *value = table->values[hash];
       return 1;  // Found
   }
   return 0;  // Not found
}

int hash_insert(HashTable *table, const char *key, void *value)
{
   unsigned int hash = hash_function(key) % table->capacity;
   if (table->keys[hash]) {
       free(table->keys[hash]);
   }
   table->keys[hash] = strdup(key);
   table->values[hash] = value;
   table->size++;
   return 1;
}

int hash_remove(HashTable *table, const char *key)
{
   unsigned int hash = hash_function(key) % table->capacity;
   if (table->keys[hash] && strcmp(table->keys[hash], key) == 0) {
       free(table->keys[hash]);
       table->keys[hash] = NULL;
       table->values[hash] = NULL;
       table->size--;
       return 1;
   }
   return 0;
}

void hash_table_free(HashTable *table)
{
   for (int i = 0; i < table->capacity; i++) {
       if (table->keys[i]) {
           free(table->keys[i]);
       }
   }
   free(table->keys);
   free(table->values);
   free(table);
}
