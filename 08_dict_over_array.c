#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *K;
typedef char *V;

typedef struct Entry {
	K key;
	V value;
} Entry;

#define CMP_LT(a, b) (strcmp((a), (b)) < 0)
#define CMP_EQ(a, b) (strcmp((a), (b)) == 0)

void freeEntry(Entry **e) {
	free((*e)->key);
	free((*e)->value);
	free(*e);
	/**e = NULL;*/
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

/*
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
*/

int main(int argc, char **argv) {

	return 0;
}
