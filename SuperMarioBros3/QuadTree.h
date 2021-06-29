#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "GameObject.h"
#include "helpers.h"

#define MAX_LEVEL                5
#define MAX_OBJECT_IN_REGION     4

class Quadtree
{
private:
    int            m_level;
    int regionLeft;
    int regionTop;
    int regionRight;
    int regionBottom;
    vector<CGameObject*>* m_objects_list;
    Quadtree** m_nodes;

    bool           IsContain(CGameObject* entity);
    void           Split();
public:
    Quadtree(int level,  int left, float top, float right,  float bottom);

    void           Clear();
    void           Insert(CGameObject* entity);
    void Retrieve(vector<CGameObject*>* return_objects_list, CGameObject* entity);
    Quadtree* CreateQuadTree(vector<CGameObject*> objects);
};
