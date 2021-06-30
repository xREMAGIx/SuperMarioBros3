#include "Quadtree.h"

Quadtree::Quadtree(int level, int left, float top, float right, float bottom)
{
    m_level = level;
    regionLeft = left;
    regionTop = top;
    regionRight = right;
    regionBottom = bottom;
}


void Quadtree::Clear()
{
    // Clear all nodes
    if (m_nodes)
    {
        for (int i = 0; i < 4; i++)
        {
            m_nodes[i]->Clear();
            delete m_nodes[i];
        }
        delete[] m_nodes;
    }

    // Clear current Quadtree
    m_objects_list->clear();

    delete m_objects_list;
}

bool Quadtree::IsContain(CGameObject* entity)
{
    float ml, mt, mr, mb;
    entity->GetBoundingBox(ml, mt, mr, mb);

    return !(ml +(mr - ml) < ml ||
        mr + mb - mt < 0 ||
        ml > regionLeft + (regionRight - regionLeft) ||
        mr > regionTop + (regionBottom - regionTop));
}

void Quadtree::Split()
{
    m_nodes = new Quadtree * [4];
    m_nodes[0] = new Quadtree(m_level + 1, regionLeft, regionTop, (regionRight - regionLeft)/2, (regionBottom - regionTop) /2);
    m_nodes[1] = new Quadtree(m_level + 1, regionLeft + (regionRight - regionLeft) / 2, regionTop, (regionRight - regionLeft) / 2, (regionBottom - regionTop) / 2);
    m_nodes[2] = new Quadtree(m_level + 1, regionLeft, regionLeft +  (regionBottom - regionTop) / 2, (regionRight - regionLeft) / 2,  (regionBottom - regionTop) / 2);
    m_nodes[3] = new Quadtree(m_level + 1, regionLeft + (regionRight - regionLeft) / 2, regionTop +  (regionBottom - regionTop) / 2, (regionRight - regionLeft) / 2,  (regionBottom - regionTop) / 2);
}

void Quadtree::Insert(CGameObject* entity)
{
    // Insert entity into corresponding nodes
    if (m_nodes)
    {
        if (m_nodes[0]->IsContain(entity))
            m_nodes[0]->Insert(entity);
        if (m_nodes[1]->IsContain(entity))
            m_nodes[1]->Insert(entity);
        if (m_nodes[2]->IsContain(entity))
            m_nodes[2]->Insert(entity);
        if (m_nodes[3]->IsContain(entity))
            m_nodes[3]->Insert(entity);

        return; // Return here to ignore rest.
    }

    // Insert entity into current quadtree
    if (this->IsContain(entity))
        m_objects_list->push_back(entity);

    // Split and move all objects in list into it’s corresponding nodes
    if (m_objects_list->size() > MAX_OBJECT_IN_REGION && m_level < MAX_LEVEL)
    {
        Split();

        while (!m_objects_list->empty())
        {
            if (m_nodes[0]->IsContain(m_objects_list->back()))
                m_nodes[0]->Insert(m_objects_list->back());
            if (m_nodes[1]->IsContain(m_objects_list->back()))
                m_nodes[1]->Insert(m_objects_list->back());
            if (m_nodes[2]->IsContain(m_objects_list->back()))
                m_nodes[2]->Insert(m_objects_list->back());
            if (m_nodes[3]->IsContain(m_objects_list->back()))
                m_nodes[3]->Insert(m_objects_list->back());

            m_objects_list->pop_back();
        }
    }
}

void Quadtree::Retrieve(vector<CGameObject*>* return_objects_list, CGameObject* entity)
{
    if (m_nodes)
    {
        if (m_nodes[0]->IsContain(entity))
            m_nodes[0]->Retrieve(return_objects_list, entity);
        if (m_nodes[1]->IsContain(entity))
            m_nodes[1]->Retrieve(return_objects_list, entity);
        if (m_nodes[2]->IsContain(entity))
            m_nodes[2]->Retrieve(return_objects_list, entity);
        if (m_nodes[3]->IsContain(entity))
            m_nodes[3]->Retrieve(return_objects_list, entity);

        return; // Return here to ignore rest.
    }

    // Add all entities in current region into return_objects_list
    if (this->IsContain(entity))
    {
        for (auto i = m_objects_list->begin(); i != m_objects_list->end(); i++)
        {
            if (entity != *i)
                return_objects_list->push_back(*i);
        }
    }
}


Quadtree* Quadtree::CreateQuadTree()
{
    vector<CGameObject*> objects = ((CPlayScene*)scence)->GetObjectList();

    DebugOut(L"[INFO] [RUN QUADTREE]\n");
    // Init base game region for detecting collision
    Quadtree* quadtree = new Quadtree(1, 0, 0, 800, 600);

    for (int i = 1; i < objects.size(); i++)
        quadtree->Insert(objects[i]);
    DebugOut(L"[INFO] [DONE INITIAL QUADTREE]\n");

    return quadtree;
}
