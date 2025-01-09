#include "FixtureSizeCalculate.h"

b2Vec2 GetFixtureHalfSize(b2Shape* const shape)
{
    b2Shape::Type shapeType = shape->GetType();  // �`��^�C�v���擾

    if (shapeType == b2Shape::e_polygon)
    {
        b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);
        b2Vec2 size = polygon->m_vertices[2] - polygon->m_vertices[0];
        return { size.x / 2,size.y / 2 };
    }
    else if (shapeType == b2Shape::e_circle)
    {
        b2CircleShape* circle = static_cast<b2CircleShape*>(shape);
        float radius = circle->m_radius;
        return { radius,radius };
    }

    return b2Vec2();
}
