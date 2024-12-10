//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description ���I�G�l�~�[(�v���C���[�Ǐ])�̃w�b�_�[�t�@�C��
// #make 2024/11/20
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_DYNAMIC_H
#define ENEMY_DYNAMIC_H

#include "enemy.h"
#include "field.h"
#include "player_stamina.h"

//==========�}�N����`==========//
#define ENEMY_DYNAMIC_LIFE (100)
#define ENEMY_DYNAMIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_DYNAMIC_SOULGAGE (100)
#define ENEMY_DYNAMIC_SCORE (200)

class EnemyDynamic :public Enemy, public Field
{
private:
	float m_speed = 0.005f;
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~EnemyDynamic() = default;

	static void Update();
	virtual void UpdateEnemy();
	static void CollisionPlayer(b2Body* collision_enemy);
	static void CollisionPulledObject(b2Body* collision_enemy);
	static void Finalize();
};

#endif	//ENEMY_DYNAMIC_H