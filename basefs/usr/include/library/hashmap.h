/**
 * Copyright (C) 2019-2020 The CadexOS Project
 * This software is distributed under the GNU General Public License
 * See the file LICENSE for details
 */
#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <library/list_t.h>
#include <library/stddef.h>
#include <library/stdlib.h>
#include <library/string.h>

#define COS_SOURCE // Define that this is Cadex-OS source

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (*hashmap_hash_t)(void *key);
typedef int (*hashmap_comp_t)(void *a, void *b);
typedef void (*hashmap_free_t)(void *);
typedef void *(*hashmap_dupe_t)(void *);

typedef struct hashmap_entry {
    char *key;
    void *value;
    struct hashmap_entry *next;
} hashmap_entry_t;

typedef struct hashmap {
    hashmap_hash_t hash_func;
    hashmap_comp_t hash_comp;
    hashmap_dupe_t hash_key_dup;
    hashmap_free_t hash_key_free;
    hashmap_free_t hash_val_free;
    size_t size;
    hashmap_entry_t **entries;
} hashmap_t;

extern hashmap_t *hashmap_create(int size);
extern hashmap_t *hashmap_create_int(int size);
extern void *hashmap_set(hashmap_t *map, void *key, void *value);
extern void *hashmap_get(hashmap_t *map, void *key);
extern void *hashmap_remove(hashmap_t *map, void *key);
extern int hashmap_has(hashmap_t *map, void *key);
extern list_t *hashmap_keys(hashmap_t *map);
extern list_t *hashmap_values(hashmap_t *map);
extern void hashmap_free(hashmap_t *map);
extern unsigned int hashmap_string_hash(void *key);
extern int hashmap_string_comp(void *a, void *b);
extern void *hashmap_string_dupe(void *key);
extern int hashmap_is_empty(hashmap_t *map);

#ifdef __cplusplus
}
#endif
#endif
