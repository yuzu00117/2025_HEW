//-----------------------------------------------------------------------------------------------------
// #name FixtureSizeCalculate.h
// #description		fixtureの半径を計算するためのコード（形がpolygonとcircleのものしか計算できないよ）
// #make 2025/1/8　王泳心
// #update 2025/1/10
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------

#ifndef FIXTURESIZECALCULATE_H
#define FIXTURESIZECALCULATE_H

#include"include/box2d/box2d.h"

//fixtureの半径を計算するための関数
//幅の半分の長さが返される
b2Vec2	GetFixtureHalfSize(b2Shape* const shape);

//polygon fixtureの頂点を取得
b2Vec2 GetPolygonFixtureVertex(b2Shape* const shape, int vertex_id);

#endif // !FIXTURESIZECALCULATE_H


