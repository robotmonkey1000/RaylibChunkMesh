#include "main.h"
#include <raylib.h>

#define ATLAS_SIZE 1024.0f
#define SPRITE_SIZE 16.0f
#define UV_SCALE SPRITE_SIZE/ATLAS_SIZE

#define UV_X(x) UV_SCALE * x
#define UV_Y(y) UV_SCALE * y  

static Registry *ModelRegistry;
static Registry *BlockRegistry;


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Mesh Making Attempt");
    
    InitRegistries(); //Init Registries
    //Add Items to registry

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Texture2D texture = LoadTexture("res/texture_atlas.png");


    Chunk *chunk = malloc(sizeof(Chunk));
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            for(int k = 0; k < 16; k++)
            {
                chunk->Blocks[i][j][k] = NULL;
            }   
        }   
    }
    for(int x = 0; x < 16; x++)
    {
        for(int z = 0; z < 16; z++)
        {
            if(x % 2 == 0) {
                chunk->Blocks[x][0][z] = Glass;
            }
            else 
            {
                chunk->Blocks[x][0][z] = Dirt;
            }
        }
    }

    GenerateChunkMesh(chunk);

    Model chunkModel = LoadModelFromMesh(*chunk->mesh->mesh);
    chunkModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    SetTargetFPS(60); 
    // Main game loop
    while (!WindowShouldClose())   
    {

        UpdateCamera(&camera, CAMERA_FREE);
        //UPDATE INFO 

        //RENDER
        BeginDrawing();

            ClearBackground(RAYWHITE);

            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            BeginMode3D(camera);

                DrawModel(chunkModel, (Vector3){0.0f,0.0f,0.0f}, 1.0f, WHITE);

                DrawGrid(10, 1.0f);

            EndMode3D();
            
            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texture);
    //TODO Free Registries and Chunks


    CloseWindow();

    return 0;
}

void InitRegistries() {
    ModelRegistry = malloc(sizeof(Registry));
    ModelRegistry->size = 1;
    ModelRegistry->items = CreateHashMap(REG_SIZE);

    BlockRegistry = malloc(sizeof(Registry));
    BlockRegistry->size = 1;
    BlockRegistry->items = CreateHashMap(REG_SIZE);

    AddRegistryItem(ModelRegistry, CreateRegistryItem("basic_block", CreateBasicBlockModel()));
    
    Glass = (Block*)AddRegistryItem(
    BlockRegistry, 
    CreateRegistryItem(
        "basic_block",
        CreateBasicBlock (
                GetRegistryItem(ModelRegistry, "basic_block"), 
                (Vector2){0 , 0}
            )
        ))->data;

    Dirt = (Block*)AddRegistryItem(
    BlockRegistry, 
    CreateRegistryItem(
        "basic_block",
        CreateBasicBlock (
                GetRegistryItem(ModelRegistry, "basic_block"), 
                (Vector2){1 , 0}
            )
        ))->data;
}

void GenerateChunkMesh(Chunk *chunk)
{
    ChunkMesh *chunkMesh = malloc(sizeof(ChunkMesh));
    chunkMesh->vertexCount = 0;
    
    Mesh *mesh = calloc(1, sizeof(Mesh));
    mesh->vertices = NULL;
    mesh->normals = NULL;
    mesh->texcoords = NULL;
    
    mesh->triangleCount = 0;
    mesh->vertexCount = 0;

    chunkMesh->mesh = mesh;
    
    //AddModelToChunk(chunkMesh, (Vector3){0, 0, 0}, BasicBlock->model->data);
    for(int x = 0; x < 16; x++)
    {
        for(int z = 0; z < 16; z++)
        {
            if(chunk->Blocks[x][0][z] != NULL)
            {
                int startingVertextCount = chunkMesh->mesh->vertexCount;
                
                Block *block = chunk->Blocks[x][0][z];
                AddModelToChunk(chunkMesh, (Vector3){x, 0, z}, chunk->Blocks[x][0][z]->model->data); //TODO Determine culling
                
                //TODO do shift based on coords of block in the atlas
                int finalVertextCount = chunkMesh->mesh->vertexCount;

                for(int i = (startingVertextCount * 2); i < (finalVertextCount * 2); i+=12)
                {
                    chunkMesh->mesh->texcoords[i]    = UV_X(block->TextureUV.x);
                    chunkMesh->mesh->texcoords[i+1]  = UV_Y(block->TextureUV.y);

                    chunkMesh->mesh->texcoords[i+2]  = UV_X(block->TextureUV.x) + UV_SCALE;
                    chunkMesh->mesh->texcoords[i+3]  = UV_Y(block->TextureUV.y);

                    chunkMesh->mesh->texcoords[i+4]  = UV_X(block->TextureUV.x) + UV_SCALE;
                    chunkMesh->mesh->texcoords[i+5]  = UV_Y(block->TextureUV.y) + UV_SCALE;    
                    
                    chunkMesh->mesh->texcoords[i+6]  = UV_X(block->TextureUV.x);
                    chunkMesh->mesh->texcoords[i+7]  = UV_Y(block->TextureUV.y);

                    chunkMesh->mesh->texcoords[i+8]  = UV_X(block->TextureUV.x) + UV_SCALE;
                    chunkMesh->mesh->texcoords[i+9]  = UV_Y(block->TextureUV.y) + UV_SCALE;

                    chunkMesh->mesh->texcoords[i+10] = UV_X(block->TextureUV.x);
                    chunkMesh->mesh->texcoords[i+11] = UV_Y(block->TextureUV.y) + UV_SCALE;
                }

            }

        }
    }


    // AddFace(&mesh, TOP_FACE, (Vector3){0, 0,0});

    // AddFace(&mesh, BOTTOM_FACE, (Vector3){0, 0,0});

    // AddFace(&mesh, FRONT_FACE, (Vector3){0, 0,0});
    
    // AddFace(&mesh, BACK_FACE, (Vector3){0, 0,0});

    // AddFace(&mesh, RIGHT_FACE, (Vector3){0, 0,0});

    // AddFace(&mesh, LEFT_FACE, (Vector3){0, 0,0});


    //TODO determine texture coords per face and actually use the UV of a face.
    // for(int i = 0; i < mesh->mesh.vertexCount * 2; i += 12)
    // {

    //     //TODO do shift based on coords of block in the atlas
    //     mesh->mesh.texcoords[i] = UV_X(0.0f);
    //     mesh->mesh.texcoords[i+1] = UV_Y(0.0f);

    //     mesh->mesh.texcoords[i+2] = UV_X(0.0f) + UV_SCALE;
    //     mesh->mesh.texcoords[i+3] = UV_Y(0.0f);

    //     mesh->mesh.texcoords[i+4] = UV_X(0.0f) + UV_SCALE;
    //     mesh->mesh.texcoords[i+5] = UV_Y(0.0f) + UV_SCALE;    
        
    //     mesh->mesh.texcoords[i+6] = UV_X(0.0f);
    //     mesh->mesh.texcoords[i+7] = UV_Y(0.0f);

    //     mesh->mesh.texcoords[i+8] = UV_X(0.0f) + UV_SCALE;
    //     mesh->mesh.texcoords[i+9] = UV_Y(0.0f) + UV_SCALE;

    //     mesh->mesh.texcoords[i+10] = UV_X(0.0f);
    //     mesh->mesh.texcoords[i+11] = UV_Y(0.0f) + UV_SCALE;
    // }

    UploadMesh(chunkMesh->mesh, false);
    chunk->mesh = chunkMesh;
}

void AddFace(ChunkMesh* chunkMesh, CubeFace face, Vector3 position)
{
    //TODO PROBABLY OPTIMIZE THIS
    chunkMesh->mesh->triangleCount += 2;
    chunkMesh->mesh->vertexCount = chunkMesh->mesh->triangleCount * 3;
    
    if(chunkMesh->mesh->triangleCount == 2)
    {
        chunkMesh->mesh->vertices = (float *)malloc( chunkMesh->mesh->vertexCount*3*sizeof(float));
        chunkMesh->mesh->texcoords = (float *)malloc(chunkMesh->mesh->vertexCount*2*sizeof(float));
        chunkMesh->mesh->normals = (float *)malloc(chunkMesh->mesh->vertexCount*3*sizeof(float));
    }
    else
    {
        chunkMesh->mesh->vertices = (float *)realloc(chunkMesh->mesh->vertices, chunkMesh->mesh->vertexCount*3*sizeof(float));
        chunkMesh->mesh->texcoords = (float *)realloc(chunkMesh->mesh->texcoords, chunkMesh->mesh->vertexCount*2*sizeof(float));
        chunkMesh->mesh->normals = (float *)realloc(chunkMesh->mesh->normals, chunkMesh->mesh->vertexCount*3*sizeof(float));
    }

    for(int i = 0; i < 6; i++)
    {
        chunkMesh->mesh->vertices[chunkMesh->vertexCount] = face.Verticies[i].x + position.x;
        chunkMesh->mesh->vertices[chunkMesh->vertexCount + 1] = face.Verticies[i].y + position.y;
        chunkMesh->mesh->vertices[chunkMesh->vertexCount + 2] = face.Verticies[i].z + position.z;

        chunkMesh->mesh->normals[chunkMesh->vertexCount] = face.Normals[i].x;
        chunkMesh->mesh->normals[chunkMesh->vertexCount + 1] = face.Normals[i].y;
        chunkMesh->mesh->normals[chunkMesh->vertexCount + 2] = face.Normals[i].z;

        chunkMesh->vertexCount += 3;

    }
}

void AddModelToChunk(ChunkMesh *mesh, Vector3 position, BaseModel *model)
{
    //Loop through the 6 different directions
    for(int i = 0; i < 6; i++) 
    {
        //For each face in those directions add it to the chunk model
        for(int face = 0; face < model->FaceCount[i]; face++)
        {
            CubeFace curFace = model->faces[i][face];
            AddFace(mesh, curFace, position);
        }
    }
}

Mesh GenMeshCustom()
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 4; //Make a square
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    //Construct Each Triangle in a clockwise rotation.
//TOP FACE

    // // Vertex at (0, 0, 0) 
    // mesh.vertices[0] = 0;
    // mesh.vertices[1] = 1;
    // mesh.vertices[2] = 0;

    // mesh.normals[0] = 0;
    // mesh.normals[1] = 1;
    // mesh.normals[2] = 0;

    // mesh.texcoords[0] = 0;
    // mesh.texcoords[1] = 0;

    // // Vertex at (1, 0, 1) 
    // mesh.vertices[3] = 1;
    // mesh.vertices[4] = 1;
    // mesh.vertices[5] = 1;

    // mesh.normals[3] = 0;
    // mesh.normals[4] = 1;
    // mesh.normals[5] = 0;

    // mesh.texcoords[2] = 0.5f;
    // mesh.texcoords[3] = 0.5f;

    // // Vertex at (1, 0, 0) 
    // mesh.vertices[6] = 1;
    // mesh.vertices[7] = 1;
    // mesh.vertices[8] = 0;

    // mesh.normals[6] = 0;
    // mesh.normals[7] = 1;
    // mesh.normals[8] = 0;

    // mesh.texcoords[4] = 0.5f;
    // mesh.texcoords[5] = 0;



    // // Vertex at (0, 0, 0) 
    // mesh.vertices[9] = 0;
    // mesh.vertices[10] = 1;
    // mesh.vertices[11] = 0;

    // mesh.normals[9] = 0;
    // mesh.normals[10] = 1;
    // mesh.normals[11] = 0;

    // mesh.texcoords[6] = 0;
    // mesh.texcoords[7] = 0;

    // // Vertex at (1, 0, 1) 
    // mesh.vertices[12] = 0;
    // mesh.vertices[13] = 1;
    // mesh.vertices[14] = 1;

    // mesh.normals[12] = 0;
    // mesh.normals[13] = 1;
    // mesh.normals[14] = 0;

    // mesh.texcoords[8] = 0.5f;
    // mesh.texcoords[9] = 0.5f;

    // // Vertex at (1, 0, 0) 
    // mesh.vertices[15] = 1;
    // mesh.vertices[16] = 1;
    // mesh.vertices[17] = 1;

    // mesh.normals[15] = 0;
    // mesh.normals[16] = 1;
    // mesh.normals[17] = 0;

    // mesh.texcoords[10] = 0;
    // mesh.texcoords[11] = 0.5f;
//TOP END

//BOTTOM FACE
    // // Vertex at (0, 0, 0) 
    mesh.vertices[18] = 1;
    mesh.vertices[19] = 0;
    mesh.vertices[20] = 0;

    mesh.normals[18] = 0;
    mesh.normals[19] = -1.0f;
    mesh.normals[20] = 0;

    mesh.texcoords[12] = 0;
    mesh.texcoords[13] = 0;

    // Vertex at (1, 0, 1) 
    mesh.vertices[21] = 1;
    mesh.vertices[22] = 0;
    mesh.vertices[23] = 1;

    mesh.normals[21] = 0;
    mesh.normals[22] = 1.0f;
    mesh.normals[23] = 0;

    mesh.texcoords[14] = 0.5f;
    mesh.texcoords[15] = 0.5f;

    // Vertex at (1, 0, 0) 
    mesh.vertices[24] = 0;
    mesh.vertices[25] = 0;
    mesh.vertices[26] = 1;

    mesh.normals[24] = 0;
    mesh.normals[25] = 1.0f;
    mesh.normals[26] = 0;

    mesh.texcoords[16] = 0.5f;
    mesh.texcoords[17] = 0;



    // Vertex at (0, 0, 0) 
    mesh.vertices[27] = 1;
    mesh.vertices[28] = 0;
    mesh.vertices[29] = 0;

    mesh.normals[27] = 0;
    mesh.normals[28] = -1.0f;
    mesh.normals[29] = 0;

    mesh.texcoords[18] = 0;
    mesh.texcoords[19] = 0;

    // Vertex at (1, 0, 1) 
    mesh.vertices[30] = 0;
    mesh.vertices[31] = 0;
    mesh.vertices[32] = 1;

    mesh.normals[30] = 0;
    mesh.normals[31] = -1.0f;
    mesh.normals[32] = 0;

    mesh.texcoords[20] = 0.5f;
    mesh.texcoords[21] = 0.5f;

    // Vertex at (1, 0, 0) 
    mesh.vertices[33] = 0;
    mesh.vertices[34] = 0;
    mesh.vertices[35] = 0;

    mesh.normals[33] = 0;
    mesh.normals[34] = -1.0f;
    mesh.normals[35] = 0;

    mesh.texcoords[22] = 0;
    mesh.texcoords[23] = 0.5f;


//BOTTOM END

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}