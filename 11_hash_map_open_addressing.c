#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMP_EQ(a, b) (strcmp((a), (b)) == 0)

// https://stackoverflow.com/questions/12844729/linking-error-for-inline-functions
static inline unsigned long long hashcode(unsigned char *str) {
    unsigned long long hash = 5381;
    int c;
    while ((c = *(str++))) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

#define HASHCODE(a) (hashcode(a))

static const float LOAD_FACTOR = 0.5f;
static const size_t INITIAL_SIZE = 10;
static const float MULTIPLIER = 2.0f;

typedef char* K;
typedef char* V;

typedef struct Entry_t {
    K key;
    V value;
} Entry;

static inline void freeEntry(Entry **e) {
    free((*e)->key);
    free((*e)->value);
    free(*e);
    *e = NULL;  // Can be commented out.
}

#define FREE_ENTRY(a) (freeEntry(a))

typedef struct Hashmap_t {
    Entry **data;
    size_t size;
    size_t arrSize;
    size_t limit;
    float loadFactor;
    float multiplier;
} Hashmap;

Hashmap* createHashpam(size_t, float, float);       // TODO test
void raw_put(Hashmap **, Entry * const);            // TODO test
void rehashUp(Hashmap **);                          // TODO test
void put(Hashmap **, K, V);                         // TODO test
void destroyHashmap(Hashmap **);                    // TODO test
V get(const Hashmap * const, K);                    // TODO test
void xremove(Hashmap * const map, K);               // TODO test
void iterMap(Hashmap * const, void (*f)(Entry*, void*), void*);  // TODO test

Hashmap* createHashpam(size_t initSize, float loadFactor, float multiplier) {
    Hashmap *map = (Hashmap *)malloc(sizeof(Hashmap));
    map->arrSize = (initSize >= INITIAL_SIZE) ? initSize : INITIAL_SIZE;
    map->loadFactor = (loadFactor >= LOAD_FACTOR && loadFactor <= 1.0f) ? loadFactor: LOAD_FACTOR;
    map->limit = (int)(map->loadFactor * map->arrSize);
    map->size = 0;
    map->multiplier = (multiplier >= MULTIPLIER) ? multiplier : MULTIPLIER;
    map->data = (Entry **)calloc(map->arrSize, sizeof(Entry *));
    return map;
}

void raw_put(Hashmap ** map,  Entry * const e) {
    unsigned long long hash = HASHCODE((unsigned char *)e->key);
    size_t index = (hash % (*map)->arrSize);
    if ((*map)->size < (*map)->limit) {
        while ((*map)->data[index]) {
            index++;
            if (index >= (*map)->arrSize) {
                index = 0;
            }
        }
        (*map)->data[index] = e;
    } else {
        rehashUp(map);
        raw_put(map, e);
    }
    (*map)->size++;
}

void put(Hashmap **map, K key, V value) {
    Entry *e = (Entry *) malloc (sizeof(Entry));
    e->key = key;
    e->value = value;
    raw_put(map, e);
}

void rehashUp(Hashmap **map) {
    Hashmap * newMap = createHashpam(
        (size_t)((*map)->arrSize * (*map)->multiplier),
        (*map)->loadFactor,
        (*map)->multiplier);
    size_t i;
    for (i = 0; i < (*map)->size; i++) {
        if ((*map)->data[i]) {
            raw_put(&newMap, (*map)->data[i]);
        }
    }
    free((*map)->data);
    free(*map);
    *map = newMap;
}

void destroyHashmap(Hashmap **map) {
    size_t i;
    for (i = 0; i < (*map)->arrSize; i++) {
        if ((*map)->data[i]) {
            FREE_ENTRY(&((*map)->data[i]));
        }
    }
    free((*map)->data);
    free(*map);
    *map = NULL;
}

V get(const Hashmap * const map, K key) {
    unsigned long long hash = HASHCODE((unsigned char *) key);
    size_t index = (hash % map->arrSize);
    size_t initialIndex = index;
    V retVal = NULL;
    while (map->data[index]) {
        if (CMP_EQ(map->data[index]->key, key)) {
            retVal = map->data[index]->value;
            break;
        }
        index++;
        if (index >= map->arrSize) {
            index = 0;
        }
        if (index == initialIndex) {
            break;
        }
    }
    return retVal;
}

void xremove( Hashmap * const map, K key ) {
    unsigned long long hash = HASHCODE((unsigned char *) key);
    size_t index = (hash % map->arrSize);
    size_t initialIndex = index;
    while (map->data[index]) {
        if (CMP_EQ(map->data[index]->key, key)) {
            FREE_ENTRY(&(map->data[index]));
            map->data[index] = NULL;
            break;
        }
        index++;
        if (index >= map->arrSize) {
            index = 0;
        }
        if (index == initialIndex) {
            break;
        }
    }
    map->size--;
}

void iterMap(Hashmap * const map, void(*f)(Entry*, void*), void *data) {
    size_t i;
    for (i = 0; i < map->arrSize; i++) {
        if (map->data[i]) {
            f(map->data[i], data);
        }
    }
}

void printEntry(Entry *e, void *data) {
    printf("%s: %s\n", e->key, e->value);
}
int main(int argc, char **argv) {

    return 0;
}











































