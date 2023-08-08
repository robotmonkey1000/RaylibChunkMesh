//RayLib
#include "raylib.h"
#include "rlgl.h"

//Personal
#include "models.h"
#include "WorldData.h"

//C Libs
#include <stdlib.h>
#include <stdio.h>

// Update World Data
// If Chunk Changes update chunk mesh
// Generate Mesh 
// Generate Model
// Assign Texture
// Assign Shader
// Render Model
Mesh GenCubeMesh();
Mesh GenMeshCustom();

void AddFace(ChunkMesh* mesh, CubeFace face, Vector3 position);
void GenerateChunkMesh(Chunk *chunk);
void AddModelToChunk(ChunkMesh *chunkMesh, Vector3 position, BaseModel *model);
void InitRegistries();