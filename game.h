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
	GAME_STATE_START,
	GAME_STATE_RESPAWN_INITIAL,
	GAME_STATE_RESPAWN_SAVE_POINT,
	GAME_STATE_NEXT_STAGE,
	GAME_STATE_GAMEOVER,
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

	//���̂��ƃ��X�|������\��Ȃ̂��i�ω�����̂͏��񃊃X�|�����鎞�I���ɂ���̂ƁA�c�@���Ȃ��Ȃ��āA�����̓N���A���ă��U���g�ɑJ�ڂ��鎞�I�t�ɂ���A���̂Q�񂾂��j
	GAME_STATE	m_state = GAME_STATE_START;
};





#endif // !GAME_H

