#include "ChunkMesh.h"
#include "Block.h"

typedef struct Chunk {
    Block* Blocks[16][16][16];
    ChunkMesh *mesh;
} Chunk;

typedef struct World {
    Chunk **LoadedChunks;
} World;