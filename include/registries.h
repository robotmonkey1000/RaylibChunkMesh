#include "HashMap.h"

#define REG_SIZE 200

typedef struct RegistryItem {
    const char *key;
    void *data;
} RegistryItem;

typedef struct Registry {
    int size;
    HashMap *items;
} Registry;

RegistryItem *AddRegistryItem(Registry* registry, RegistryItem *item);
RegistryItem *CreateRegistryItem(const char *key, void *data);
RegistryItem *GetRegistryItem(Registry *registry, const char *key);