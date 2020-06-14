#include <stdint.h>


enum trie_status
{
	T_OK, /* user got the IP he asked for */
	T_ADDR_TAKEN, /* user got an IP, not one he asked for */
	T_NO_FREE_ADDR, /* user didn't get an IP */
	T_E_ALLOC /* couldn't allocate memory */
};

typedef struct trie trie_t;

typedef struct node node_t;

trie_t *TrieCreate(size_t num_of_bits);

void TrieDestroy(trie_t *trie);

enum trie_status TrieInsert(trie_t *trie, uint32_t address, uint32_t *res);

int TrieRemove(trie_t *trie, uint32_t key);

size_t TrieCount(const trie_t *trie);
