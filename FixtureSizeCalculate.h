//-----------------------------------------------------------------------------------------------------
// #name FixtureSizeCalculate.h
// #description		fixture�̔��a���v�Z���邽�߂̃R�[�h�i�`��polygon��circle�̂��̂����v�Z�ł��Ȃ���j
// #make 2025/1/8�@���j�S
// #update 2025/1/10
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------

#ifndef FIXTURESIZECALCULATE_H
#define FIXTURESIZECALCULATE_H

#include"include/box2d/box2d.h"

//fixture�̔��a���v�Z���邽�߂̊֐�
//���̔����̒������Ԃ����
b2Vec2	GetFixtureHalfSize(b2Shape* const shape);

//polygon fixture�̒��_���擾
b2Vec2 GetPolygonFixtureVertex(b2Shape* const shape, int vertex_id);

#endif // !FIXTURESIZECALCULATE_H


