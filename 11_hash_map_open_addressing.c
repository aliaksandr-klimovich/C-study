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

Hashmap* createHashmap(size_t, float, float);
void raw_put(Hashmap **, Entry * const);
void rehashUp(Hashmap **);
void put(Hashmap **, K const, V const);
void destroyHashmap(Hashmap **);
V get(const Hashmap * const, K const);
void xremove(Hashmap * const map, K const);
void iterMap(Hashmap * const, void (*f)(Entry*, void*), void*);

Hashmap* createHashmap(size_t initSize, float loadFactor, float multiplier) {
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

void put(Hashmap **map, K const key, V const value) {
    Entry *e = (Entry *) malloc (sizeof(Entry));
    e->key = key;
    e->value = value;
    raw_put(map, e);
}

void rehashUp(Hashmap **map) {
    Hashmap * newMap = createHashmap(
        (size_t)((*map)->arrSize * (*map)->multiplier),
        (*map)->loadFactor,
        (*map)->multiplier);
    size_t i;
    for (i = 0; i < (*map)->arrSize; i++) {
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

V get(const Hashmap * const map, K const key) {
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

void xremove(Hashmap * const map, K const key) {
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

int main(int argc, char *argv[]) {
    Hashmap *map = createHashmap(10, 0.5f, 2.0f);
    Entry *tmp;
    size_t i;
    char *words[][10] = {
        {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"},
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}
    };

    for (i = 0; i < 10; i++) {
        put(&map, strdup(words[0][i]), strdup(words[1][i]));
    }

    iterMap(map, printEntry, NULL);

    char *six_value = get(map, "six");
    printf("Key: six, value: %s.\n", six_value);

    xremove(map, "six");
    iterMap(map, printEntry, NULL);

    destroyHashmap(&map);

    // TODO free `words`!

    return 0;
}
