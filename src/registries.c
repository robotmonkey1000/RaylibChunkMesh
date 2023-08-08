#include "registries.h"
#include "models.h"

/*  Registers an Item to a registry
    RETURNS: the pointer to the location of the registry item
*/
RegistryItem *AddRegistryItem(Registry* registry, RegistryItem *item) 
{
    AddToHashMap(registry->items, item->key, item, true);
    registry->size += 1;

    return item;
}

RegistryItem *CreateRegistryItem(const char *key, void *data)
{
    RegistryItem *item = malloc(sizeof(RegistryItem));
    item->data = data;
    item->key = key;   
    return item;
}

RegistryItem *GetRegistryItem(Registry *registry, const char *key)
{
    return GetFromMap(registry->items, key);
}

