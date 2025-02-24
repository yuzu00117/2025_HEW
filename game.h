//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description �Q�[��
// #make 2024/11/04�@�i��`��
// #update 2024/11/05
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
//           
//----------------------------------------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H


#include"player.h"
#include"field.h"
#include"directx_controller.h"
#include"1-1_boss.h"
#include"object_manager.h"
#include"Item_Manager.h"


enum GAME_STATE
{
	GAME_STATE_START,				//�Q�[���V�[���ɓ������΂����i�X�e�[�W�I�����痈������̂݁j
	GAME_STATE_RESPAWN_INITIAL,		//�X�e�[�W�̏����ʒu�Ƀ��X�|��
	GAME_STATE_RESPAWN_SAVE_POINT,	//�X�e�[�W�̒��Ԓn�Ƀ��X�|��
	GAME_STATE_NEXT_STAGE,			//���̃X�e�[�W�Ɉڍs
	GAME_STATE_GAMEOVER,			//���U���g��ʂɈڍs
};

class Game
{
public:
	Game();
	~Game();


	static Game& GetInstance() {
		static Game instance;
		return instance;
	}






	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	//���Q�[���V�[���̏�Ԃ��擾
	GAME_STATE GetGameState() { return m_state; }


	void Teleport_player(b2Vec2 position);

private:
	//�V���O���g�����������Ɉړ�
	Player &player=Player::GetInstance();
	Boss_1_1 &boss =Boss_1_1::GetInstance();



	ObjectManager& objectManager = ObjectManager::GetInstance();
	ItemManager& itemManager = ItemManager::GetInstance();

	//���̃Q�[���V�[���̏�ԃ��X�|�����Ă��̂��A�J�n�����΂���Ȃ̂��ȂǂȂ�
	GAME_STATE	m_state = GAME_STATE_START;
};





#endif // !GAME_H

