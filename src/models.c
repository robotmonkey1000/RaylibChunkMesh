#include "models.h"
BaseModel *CreateBasicBlockModel()
{
    BaseModel *blockModel = malloc(sizeof(BaseModel));

    blockModel->FaceCount[TOP] = 1;
    blockModel->FaceCount[BOTTOM] = 1;
    blockModel->FaceCount[RIGHT] = 1;
    blockModel->FaceCount[LEFT] = 1;
    blockModel->FaceCount[FRONT] = 1;
    blockModel->FaceCount[BACK] = 1;

    for(int i = 0; i < 6; i++)
    {
        blockModel->faces[i] = (CubeFace*)malloc(sizeof(CubeFace) * 1);
    }

    blockModel->faces[TOP][0] = TOP_FACE;
    blockModel->faces[BOTTOM][0] = BOTTOM_FACE;
    blockModel->faces[RIGHT][0] = RIGHT_FACE;
    blockModel->faces[LEFT][0] = LEFT_FACE;
    blockModel->faces[FRONT][0] = FRONT_FACE;
    blockModel->faces[BACK][0] = BACK_FACE;


    return blockModel;
}