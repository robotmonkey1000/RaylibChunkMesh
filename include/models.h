#include "raylib.h"
#include <stdlib.h>

typedef struct CubeFace {
    Vector3 Verticies[6];
    Vector3 Normals[6];
    bool shouldCull; //Whether or not to attempt to cull this face if there is another block beside it
} CubeFace;

//A model is defined by a list of cube faces for each direction of a cube. 
typedef struct BaseModel {
    CubeFace *faces[6]; //A list of a faces for each direction
    int FaceCount[6]; //Total number of faces for each direction
} BaseModel;

enum Face {TOP, BOTTOM, RIGHT, LEFT, FRONT, BACK};

static CubeFace TOP_FACE = {
    {
        1,1,0,
        0,1,0,
        0,1,1,
        1,1,0,
        0,1,1,
        1,1,1
    },
    {
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
    },
    true
};

static CubeFace BOTTOM_FACE = {
    {
        1,0,0,
        1,0,1,
        0,0,1,
        1,0,0,
        0,0,1,
        0,0,0
    },
    {
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,
    },
    true
};

static CubeFace RIGHT_FACE = {
    {
        1,1,0,
        1,1,1,
        1,0,1,
        1,1,0,
        1,0,1,
        1,0,0
    },

    {
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
    },
    true
};

static CubeFace LEFT_FACE = {
    {
        0,1,1,
        0,1,0,
        0,0,0,
        0,1,1,
        0,0,0,
        0,0,1
    },
    
    {
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
    },
    true
};
//TODO CHECK NORMALS
static CubeFace FRONT_FACE = {
    {
        1,0,1,
        1,1,1,
        0,1,1,
        1,0,1,
        0,1,1,
        0,0,1
    },
    {
        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,
    },
    true
};

//TODO FIX NORMALS
static CubeFace BACK_FACE = {
    {
        0,0,0,
        0,1,0,
        1,1,0,
        0,0,0,
        1,1,0,
        1,0,0
    },
    {
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,
    },
    true
};


BaseModel *CreateBasicBlockModel();