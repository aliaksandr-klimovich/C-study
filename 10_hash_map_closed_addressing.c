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

static const float LOAD_FACTOR = 0.72f;
static const size_t INITIAL_SIZE = 100;
static const float MULTIPLIER = 2.0f;

typedef char* K;
typedef char* V;

typedef struct Entry_t {
    K key;
    V value;
} Entry;

typedef struct Node_t {
    Entry *value;
    struct Node_t *next;
} Node;

static inline void freeEntry(Entry **e) {
    free((*e)->key);
    free((*e)->value);
    free(*e);
    *e = NULL;  // Can be commented.
}

#define FREE_ENTRY(a) (freeEntry(a))

static inline void freeNode(Node **n) {
    FREE_ENTRY(&((*n)->value));
    free(*n);
    *n = NULL;
}

#define FREE_NODE(a) (freeNode(a))

typedef struct Hashmap_t {
    Node **data;        // Bucket of nodes (i.e. linked lists).
    size_t size;        // Actual size of the array (number of used buckets with data).
    size_t arrSize;     // Maximum size of bucked (allocated memory).
    size_t limit;       // After this limit our Hashmap will be rebuilt.
    float loadFactor;   // Custom load factor.
    float multiplier;   // Custom multiplier.
} Hashmap;

Hashmap* createHashmap(size_t initSize, float loadFactor, float multiplier);
void deleteHashmap(Hashmap **);
V get(const Hashmap *map, K key);
void raw_put(Hashmap **_map_, Entry *e);
void put(Hashmap **_map_, K key, V value);
void xremove(Hashmap *map, K key);
Hashmap* rehashUp(Hashmap **_map_, Entry *e);
void iterMap(Hashmap *, void(*)(Entry*, void*), void*);

Hashmap* createHashmap(size_t initSize, float loadFactor, float multiplier) {
    Hashmap *tmp = (Hashmap *) malloc(sizeof(Hashmap));
    tmp->arrSize = (initSize >= INITIAL_SIZE) ? initSize : INITIAL_SIZE;
    tmp->loadFactor = (loadFactor >= LOAD_FACTOR) ? loadFactor : LOAD_FACTOR;
    tmp->multiplier = (multiplier >= MULTIPLIER) ? multiplier : MULTIPLIER;
    tmp->size = 0;
    tmp->limit = (size_t) (tmp->loadFactor * tmp->arrSize);
    tmp->data = (Node **)calloc(tmp->arrSize, sizeof(Node *));
    return tmp;
}

void raw_put(Hashmap **_map_, Entry *e) {
    Hashmap *map = *_map_;
    unsigned long long hash = HASHCODE((unsigned char *)e->key);
    size_t index = (hash % map->arrSize);

    if (map->size < map->limit) {
        // If the element of an array is free, then insert the element there.
        if (map->data[index] == NULL) {
            // TODO: check key? Hash can return same code!
            Node *newNode = (Node *) malloc(sizeof(Node));
            newNode->next = NULL;
            newNode->value = e;
            map->data[index] = newNode;
        } else {
            // The pointer in the current index is not free.
            // So need to add value to linked list.
            // In the same time check if the key is already present in the
            Node *anchor = map->data[index];
            while (anchor->next) {
                if (CMP_EQ(anchor->value->key, e->key)) {
                    // Replace the value with new one.
                    anchor->value->value = e->value;
                    // Value replaced, just skip the rest and jump to finalization.
                    return;
                }
            }
            // Key was not found in the list.
            // Do insertion of new node.
            Node *newNode = (Node *) malloc(sizeof(Node));
            newNode->next = NULL;
            newNode->value = e;
            anchor->next = newNode;
        }

    } else {  // Limit exceed. Need to rebuild our Hashmap.
        *_map_ = rehashUp(_map_, e);
    }

    (*_map_)->size++;
}

void put(Hashmap **_map_, K key, V value) {
    Entry *e = (Entry *) malloc(sizeof(Entry));
    e->key = key;
    e->value = value;
    raw_put(_map_, e);
}

Hashmap* rehashUp(Hashmap **_map_, Entry *e) {
    Hashmap *newMap = createHashmap((size_t) ((*_map_)->arrSize * (*_map_)->multiplier),
            (*_map_)->loadFactor, (*_map_)->multiplier);

    size_t i, size;
    Hashmap *map = *_map_;
    Node *anchor = NULL;
    Node *target = NULL;

    size = map->arrSize;
    for (i = 0; i < size; i++) {
        anchor = map->data[i];
        while (anchor) {
            target = anchor;
            anchor = anchor->next;
            raw_put(&newMap, target->value);
            FREE_NODE(&target);
        }
    }
    free(map->data);
    free(*_map_);
    *_map_ = newMap;
    if (e) {  //  Should consider that Entry can be NULL.
        raw_put(&newMap, e);
    }
    return newMap;
}

V get(const Hashmap *map, K key) {
    unsigned long long hash = HASHCODE((unsigned char *)key);
    size_t index = (hash % map->arrSize);
    V retVal = NULL;

    Node *anchor = map->data[index];
    while (anchor != NULL) {
        if (CMP_EQ(anchor->value->key, key)) {
            retVal = anchor->value->value;
            break;
        }
        anchor = anchor->next;
    }

    return retVal;
}

void xremove(Hashmap *map, K key) {
    unsigned long long hash = HASHCODE((unsigned char *)key);
    size_t index = (hash % map->arrSize);

    Node *anchor = map->data[index];
    Node *prevNode = anchor;
    while(anchor) {
        if (CMP_EQ(anchor->value->key, key)) {
            prevNode->next = anchor->next;
            FREE_NODE(&anchor);
            break;
        }
        prevNode = anchor;
        anchor = anchor->next;
    }

    map->size--;
}

void iterMap(Hashmap *map, void(*f)(Entry*, void*), void *ret_data) {
    size_t size, i;
    size = map->arrSize;
    for (i = 0; i < size; i++) {
        Node *anchor = map->data[i];
        while(anchor) {
            f(anchor->value, ret_data);
            anchor = anchor->next;
        }
    }
}

void printEntry(Entry *e, void *data) {
    printf("%s: %s\n", e->key, e->value);
}
// mapIterate(map, printEntry, NULL);

void deleteHashmap(Hashmap **_map_) {
    Hashmap *map = *_map_;
    size_t i, size;
    Node *anchor = NULL;
    Node *target = NULL;
    size = map->arrSize;

    for (i = 0; i < size; i++) {
        anchor = map->data[i];
        while(anchor) {
            target = anchor;
            anchor = anchor->next;
            FREE_NODE(&target);
        }
    }

    free(map->data);
    free(*_map_);
    *_map_ = NULL;
}

int main(int argc, char *argv[]) {
    Hashmap *map = createHashmap(2, 0.72f, 2.0f);
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
    deleteHashmap(&map);

    return 0;
}
