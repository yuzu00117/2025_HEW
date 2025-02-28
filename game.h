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
#include"KeyInput_Flag.h"
#include"GamePause.h"


enum GAME_STATE
{
	GAME_STATE_START,				//�Q�[���V�[���ɓ������΂����i�X�e�[�W�I�����痈������̂݁j
	GAME_STATE_RESPAWN_INITIAL,		//�X�e�[�W�̏����ʒu�Ƀ��X�|��
	GAME_STATE_RESPAWN_SAVE_POINT,	//�X�e�[�W�̒��Ԓn�Ƀ��X�|��
	GAME_STATE_NEXT_STAGE,			//���̃X�e�[�W�Ɉڍs
	GAME_STATE_GAMEOVER,			//���U���g��ʂɈڍs

	GAME_STATE_PAUSE,						//�Q�[���|�[�Y
	GAME_STATE_PAUSE_RESPAWN_SAVE_POINT,	//�|�[�Y���璆�Ԓn���X�|���ڍs
	GAME_STATE_PAUSE_RESPAWN_INITIAL,		//�|�[�Y���珉���ʒu�Ƀ��X�|��
	GAME_STATE_PAUSE_SELECT_SCENE,			//�|�[�Y����Z���N�g��ʈڍs
	GAME_STATE_PAUSE_TITLE,					//�|�[�Y����^�C�g����ʈڍs
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
	GAME_STATE GetCurrentGameState() { return m_state; }
	//�|�[�Y��ʂ̃Q�[���ɖ߂�{�^����p
	void	SetCurrentGameState(GAME_STATE state) { m_state = state; }
	//���̃Q�[���V�[���̏�Ԃ��Z�b�g(���ꂩ���Finalize��Initialize���ǂ�ȏ�������邩���߂Ă���邾��)
	//�V�[���J�ڂ͎����ŃZ�b�g���Ă�	
	void	SetNextGameState(GAME_STATE state){m_next_state = state;}


	void Teleport_player(b2Vec2 position);

private:
	//�V���O���g�����������Ɉړ�
	Player &player=Player::GetInstance();
	Boss_1_1 &boss =Boss_1_1::GetInstance();

	GamePause pause;

	ObjectManager& objectManager = ObjectManager::GetInstance();
	ItemManager& itemManager = ItemManager::GetInstance();

	//���̃Q�[���V�[���̏�ԃ��X�|�����Ă��̂��A�J�n�����΂���Ȃ̂��ȂǂȂ�
	GAME_STATE	m_state = GAME_STATE_START;
	//�Q�[���V�[���̎��̏��
	GAME_STATE  m_next_state = GAME_STATE_RESPAWN_INITIAL;

	Button_PressFlag key_flag;
};





#endif // !GAME_H

