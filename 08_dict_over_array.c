#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *K;
typedef char *V;

typedef struct Entry_t {
    K key;
    V value;
} Entry;

#define CMP_LT(a, b) (strcmp((a), (b)) < 0)
#define CMP_EQ(a, b) (strcmp((a), (b)) == 0)

void freeEntry(Entry **e) {
    free((*e)->key);
    free((*e)->value);
    free(*e);
    *e = NULL;
}
#define FREE_ENTRY(e) freeEntry(e)

static const size_t INIT_SIZE = 100;
static const float MULTIPLIER = 2.0f;

typedef struct Dictionary {
    size_t limit;
    size_t size;
    float multiplier;
    Entry **data;
} Dictionary;

Dictionary* createDictionary(size_t initSize) {
    Dictionary *dict = (Dictionary*)malloc(sizeof(Dictionary));
    dict->limit = initSize > INIT_SIZE ? initSize : INIT_SIZE;
    dict->size = 0;
    dict->data = (Entry**)malloc(sizeof(Entry*) * dict->limit);
    dict->multiplier = MULTIPLIER;
    return dict;
}

inline static int find(K key, Entry** data, size_t size, size_t *index) {
    size_t i = 0;
    size_t j = size-1;
    size_t med;
    int result = 0;
    while (i <= j) {
        med = i + (j - i) / 2;
        if (CMP_LT(key, data[med]->key)) {
            if (med != 0) {
                j = med - 1;
            } else {
                break;
            }
        } else if (CMP_EQ(key, data[med]->key)) {
            result = 1;
            break;
        } else {
            i = med + 1;
        }
    }
    *index = med;
    return result;
}

int raw_put(Dictionary *dict, Entry *e) {
    size_t i = 0;
    size_t j = dict->size;
    size_t index;
    int contains;

    if (dict->size == 0) {
        dict->data[0] = e;
        dict->size++;
        return 1;
    }

    contains = find(e->key, dict->data, dict->size, &index);
    if (contains) {
        return 0;
    }
    if (dict->size >= dict->limit) {
        dict->limit = (size_t) (dict->multiplier * dict->limit);
        dict->data = (Entry**)realloc(dict->data, dict->limit * sizeof(Entry*));
        if (dict == NULL) {
            exit(-5);
        }
    }

    if (index >= (dict->size - 1)) {
        index = dict->size;
        if (CMP_LT(e->key, dict->data[dict->size - 1]->key)) {
            dict->data[dict->size] = dict->data[dict->size - 1];
            dict->data[dict->size - 1] = e;
        } else {
            dict->data[index] = e;
        }
    } else {
        memcpy(&(dict->data[index + 1]), &(dict->data[index]), sizeof(Entry*)*(dict->size - index));
        if (index == 0 && CMP_LT(e->key, dict->data[0]->key)) {
            dict->data[index] = e;
        } else {
            dict->data[index + 1] = e;
        }
    }

    dict->size++;
    return 1;
}

int put(Dictionary **dict, K key, V value) {
    Entry *e = (Entry*)malloc(sizeof(Entry));
    int result;
    e->key = key;
    e->value = value;
    result = raw_put(*dict, e);
    if (result == 0) {
        FREE_ENTRY(&e);
    }
    return result;
}

Entry* xremove(Dictionary *dict, K key) {
    int contains;
    size_t index;
    Entry *retVal = NULL;

    if (dict->size == 0) {
        return NULL;
    }

    contains = find(key, dict->data, dict->size, &index);
    if (contains) {
        //printf("[[%zu]]", index);
        retVal = dict->data[index];
        if (index != dict->size) {
            memcpy(&(dict->data[index]), &(dict->data[index + 1]), (dict->size - index)*sizeof(Entry*));
        }
        dict->size--;
    }
    return retVal;
}

V get(Dictionary *dict, K key, int *wasFound) {
    size_t index;
    V retVal = NULL;

    if (dict->size == 0) {
        *wasFound = 0;
    } else {
        *wasFound = find(key, dict->data, dict->size, &index);
        if (*wasFound) {
            retVal = dict->data[index]->value;
        }
    }

    return retVal;
}

void destroyDictionary(Dictionary** dict) {
    size_t i;
    for (i = 0; i < (*dict)->size; i++) {
        FREE_ENTRY(&((*dict)->data[i]));
    }
    free((*dict)->data);
    free(*dict);
    *dict = NULL;
}

Entry* getMax(Dictionary *dict) {
    if (dict->size > 0) {
        return dict->data[dict->size - 1];
    } else {
        return NULL;
    }
}

Entry* getMin(Dictionary *dict) {
    if (dict->size > 0) {
        return dict->data[0];
    }
    else {
        return NULL;
    }
}

void printDict(Dictionary *dict) {
    size_t i;
    printf("Dictionary contents:\n");
    for (i = 0; i < dict->size; i++) {
        printf("%zu. %s = %s\n",
                i + 1,
                dict->data[i]->key,
                dict->data[i]->value);
    }
}

int main(void) {
    Dictionary *dict = createDictionary(10);
    int wasFound;
    put(&dict, strdup("Key3"), strdup("Value3"));
    put(&dict, strdup("Key2"), strdup("Value2"));
    put(&dict, strdup("Key1"), strdup("Value1"));
    printf("Value1 = %s\n", get(dict, "Key1", &wasFound));
    printf("Value2 = %s\n", get(dict, "Key2", &wasFound));
    printf("Value3 = %s\n", get(dict, "Key3", &wasFound));
    printf("Value4 = %s\n", get(dict, "Key4", &wasFound));
    xremove(dict, "Key1");
    printDict(dict);
    destroyDictionary(&dict);
    return 0;
}
