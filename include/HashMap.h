#define DEBUG_MAPS 0 //Define Debug Info for HashMaps

#ifndef HASH_MAP_H
#define  HASH_MAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Bucket{
    const char *key;
    void *data;

    struct Bucket *next;
} Bucket;

typedef struct HashMap {
    int BucketCount;
    Bucket **buckets;
} HashMap;

inline unsigned long long compute_hash(const char* s) {
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (int i = 0; i < strlen(s); i++) {
        char c = s[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

inline int GetHashForMap(HashMap *map, const char *key)
{
    unsigned long long hash = compute_hash(key);
    hash = hash % map->BucketCount; //Make sure it loops over to 0 at max amount of buckets;
    if(hash < 0) hash = 0;

    return hash;
}

inline HashMap *CreateHashMap(int BucketCount)
{
    if(BucketCount < 1) return NULL;

    HashMap *map = malloc(sizeof(HashMap));
    map->BucketCount = BucketCount;

    map->buckets = malloc(sizeof(Bucket*) * BucketCount);

    for(int i = 0; i < BucketCount; i++)
    {
        map->buckets[i] = NULL; //Make sure we clear each bucket 
    }
    return map;
}

inline bool AddToBucket(Bucket *bucket, const char *key, void *data)
{
    if(bucket->next == NULL) 
    {
        bucket->next = malloc(sizeof(Bucket));
        bucket->next->next = NULL;
        bucket->next->data = data;
        bucket->next->key = key;

        return true;
    }
    else 
    {
        return AddToBucket(bucket->next, key, data);
    }
}

inline Bucket *FindKey(Bucket *bucket, const char *key)
{
    if(bucket == NULL) return NULL;

    if(strcmp(bucket->key, key) == 0)
    {
        return bucket;
    } 
    else {
        #ifdef DEBUG_MAPS
        printf("Traversing Bucket for key: %s\n", key);
        #endif
        return FindKey(bucket->next, key);
    }

}

inline bool AddToHashMap(HashMap* map, const char *key, void *data, bool replace)
{
    //HASH 
    int hash = GetHashForMap(map, key);
    
    #ifdef DEBUG_MAPS
    printf("Hash: %d\n", hash);
    #endif

    Bucket *bucket = map->buckets[hash];

    #ifdef DEBUG_MAPS
    if(bucket != NULL)
    {
        printf("Hash Collision: %d\n", hash);
    }
    #endif

    Bucket *possible = FindKey(bucket, key);
    if(possible != NULL) //We found a matching key in this bucket chain
    {
        if(replace) //We replace the data and confirm the new data was added
        {
            possible->data = data;
            return true;
        } 
        else //We did not replace the data but found a matching key
        {
            return false;
        }
    }

    //Since we did not find a matching key within the current bucket we must make a new bucket in the chain and add the key
    if(bucket == NULL)
    {
       map->buckets[hash] = malloc(sizeof(Bucket));
       map->buckets[hash]->next = NULL;
       map->buckets[hash]->data = data;
       map->buckets[hash]->key = key; 
       return true;
    }
    else
    {
        return AddToBucket(bucket, key, data);
    }

    return false;
}

inline void *GetFromMap(HashMap *map, const char *key)
{
    int hash = GetHashForMap(map, key);

    Bucket *bucket = FindKey(map->buckets[hash], key);

    //Find the bucket with the key
    if(bucket != NULL)
    {
        return bucket->data; //Return the data from that bucket
    }
    else 
    {
        return NULL;
    }
}

inline void FreeBucket(Bucket *bucket, void (*FreeDataFunc)(void *data))
{
    if(bucket == NULL) return;

    if(bucket->next != NULL)
    {
        FreeBucket(bucket->next, FreeDataFunc);
    }

    if(FreeDataFunc != NULL)
    {
        FreeDataFunc(bucket->data);
    }

    free(bucket);
    return;

}

inline void FreeMap(HashMap *map, void (*FreeDataFunc)(void *data))
{
    for(int i = 0; i < map->BucketCount; i++)
    {
        FreeBucket(map->buckets[i], FreeDataFunc);
    }
    free(map->buckets);
    free(map);
}

#endif