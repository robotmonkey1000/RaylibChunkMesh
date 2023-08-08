#include "Block.h"

Block *CreateBasicBlock(RegistryItem *model, Vector2 UV)
{
    Block *block = malloc(sizeof(Block));
    
    block->model = model;
    block->TextureUV = UV;
    
    return block;
}