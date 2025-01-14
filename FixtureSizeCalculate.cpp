//-----------------------------------------------------------------------------------------------------
// #name FixtureSizeCalculate.cpp
// #description		fixture�̔��a���v�Z���邽�߂̃R�[�h�i�`��polygon��circle�̂��̂����v�Z�ł��Ȃ���j
// #make 2025/1/8�@���j�S
// #update 2025/1/10
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "FixtureSizeCalculate.h"

b2Vec2 GetFixtureHalfSize(b2Shape* const shape)
{
    b2Shape::Type shapeType = shape->GetType();  // �`��^�C�v���擾

    //�����`��polygon�Ȃ�
    if (shapeType == b2Shape::e_polygon)
    {
        b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);
        b2Vec2 size = polygon->m_vertices[2] - polygon->m_vertices[0];
        return { size.x / 2,size.y / 2 };
    }
    //�����`��circle�Ȃ�
    else if (shapeType == b2Shape::e_circle)
    {
        b2CircleShape* circle = static_cast<b2CircleShape*>(shape);
        float radius = circle->m_radius;
        return { radius,radius };
    }

    return b2Vec2(0,0);
}

b2Vec2 GetPolygonFixtureVertex(b2Shape* const shape, int vertex_id)
{
    b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);
    return polygon->m_vertices[vertex_id];
}
