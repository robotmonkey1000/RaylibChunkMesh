#include "registries.h"
#include "raylib.h"

#include <stdlib.h>

typedef struct Block {
    RegistryItem *model;
    Vector2 TextureUV;
    //TODO ADD FUNCTION POINTERS FOR USAGE
    //LIKE TILEENTITY
    //BREAK
    //ETC
} Block;


Block *CreateBasicBlock(RegistryItem *model, Vector2 UV);

//Registered Blocks
static Block *Glass;
static Block *Dirt;