#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = key;
  pair->value = value;
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) free(pair);
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair *pair = create_pair(key, value);
  LinkedPair *current_pair;
  unsigned int index = hash(key, ht->capacity);
  printf("Index to insert into: %d\n", index);
  int hasassigned = 0;
  if (ht->storage[index] != NULL) {
    current_pair = ht->storage[index];
    printf("%s\n", current_pair->key);
    while (current_pair->next != NULL) {
      if (strcmp(current_pair->key, key) == 0) {
        current_pair->value = value;
        hasassigned = 1;
        break;
      }
      current_pair = current_pair->next;
    }
    if (hasassigned == 0) {
      current_pair->next = pair;
    }
  } else {
      ht->storage[index] = pair;
  }

}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  LinkedPair *pair = create_pair(key, NULL);
  LinkedPair *current_pair;
  for ( int i = 0; i < ht->capacity ; i++) {
    current_pair = ht->storage[i];
    while (current_pair->next != NULL) {
      if (strcmp(current_pair->key , key) == 0) {
      current_pair = pair;
      }
      current_pair = current_pair->next;
    }
  }


}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  LinkedPair *current_pair;
  for (int i = 0; i < ht->capacity; i++) {
    current_pair = ht->storage[i];
    printf("Is current pair null? %s\n", current_pair == NULL ? "Yes": "No");
    if (current_pair != NULL) {
      printf("We are in current_pair != NULL condition\n");
      while (current_pair->next != NULL) {
        if (strcmp(current_pair->key, key) == 0) {
        return current_pair->value;
        }
        current_pair = current_pair->next;
      }

      if (strcmp(current_pair->key, key) == 0) {
        return current_pair->value;
      }
    }

  }
  return NULL;
}


/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    destroy_pair(ht->storage[i]);
  }
  free(ht->storage);
  free(ht);

}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity * 2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));
  LinkedPair *current_pair;
  for ( int i = 0; i < ht->capacity ; i++) {
    current_pair = ht->storage[i];
    while (current_pair->next != NULL) {
      hash_table_insert(new_ht, current_pair->key, current_pair->value);
      current_pair = current_pair->next;
    }
  }


  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif



