//-----------------------------------------------------------------------------------------------------
// #name FixtureSizeCalculate.cpp
// #description		fixtureの半径を計算するためのコード（形がpolygonとcircleのものしか計算できないよ）
// #make 2025/1/8　王泳心
// #update 2025/1/10
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "FixtureSizeCalculate.h"

b2Vec2 GetFixtureHalfSize(b2Shape* const shape)
{
    b2Shape::Type shapeType = shape->GetType();  // 形状タイプを取得

    //もし形がpolygonなら
    if (shapeType == b2Shape::e_polygon)
    {
        b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);
        b2Vec2 size = polygon->m_vertices[2] - polygon->m_vertices[0];
        return { size.x / 2,size.y / 2 };
    }
    //もし形がcircleなら
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
