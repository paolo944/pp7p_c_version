#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 20

typedef void (*FunctionPointer)(int, const char *buffer);

typedef struct {
    char *key;
    FunctionPointer function;
} HashEntry;

typedef struct {
    HashEntry *entries[TABLE_SIZE];
} HashTable;

extern HashTable table;

unsigned int hashFunction(const char *key);

void initializeHashTable();

void insert(const char *key, FunctionPointer function);

FunctionPointer getFunction(const char *key);

void freeHashTable();

#endif
