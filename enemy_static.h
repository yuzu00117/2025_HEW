//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description �ÓI�G�l�~�[�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2024/12/13
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_STATIC_H
#define ENEMY_STATIC_H

#include "enemy.h"
#include "field.h"
#include "player_stamina.h"

//==========�}�N����`==========//
#define ENEMY_STATIC_LIFE (100)
#define ENEMY_STATIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_STATIC_SOULGAGE (100)
#define ENEMY_STATIC_SCORE (100)

class EnemyStatic :public Enemy
{
private:
	
public:
	EnemyStatic() = default;
	EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyStatic() = default;
	
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};

#endif	//ENEMY_STATIC_H