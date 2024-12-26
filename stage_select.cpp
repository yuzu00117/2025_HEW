#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"collider_type.h"
#include"stage_select.h"
#include"stage_select_player.h"
#include"stage_point.h"
#include"stage_point_factory.h"
#include"stage_select_contact_list.h"


constexpr float SCALE = 30.0f; // �s�N�Z�����烁�[�g���ւ̕ϊ��X�P�[��
//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_stage_select_Texture = NULL;

// �����o�ϐ��Ƃ��ĕێ�

StagePointFactory m_stagePointFactory;



void StageSelectScene::Initialize()
{
	g_stage_select_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_stage_select.png");


	//���[���h������
	 // Box2D���[���h�̍쐬
	b2Vec2 gravity(0.0f,0.0f); // �d�͂Ȃ�
	m_world = new b2World(gravity);

	
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	//�v���C���[�̒�`
	m_player.Initialize(m_world, 640, 300);

	//�����̃|�W�V�������`
	m_player.SetTouchStagePointBody(m_player.GetBody());

	// �X�e�[�W�|�C���g�̐���
	m_stagePointFactory.CreateStagePoint(m_world, 400.0f, 560.0f, 40.0f,1);
	m_stagePointFactory.CreateStagePoint(m_world, 1000.0f, 360.0f, 40.0f,2);
	m_stagePointFactory.CreateStagePoint(m_world, 1200.0f, 360.0f, 40.0f,3);

	StageSelectContactListener& listener = StageSelectContactListener::GetInstance();
	m_world->SetContactListener(&listener);


	
}

void StageSelectScene::Update()
{

		//�R���g���[���[�̓��͂̎󂯎��
		ControllerState state = GetControllerInput();

		if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))

		{
			/*SceneManager& sceneManager = SceneManager::GetInstance();
			sceneManager.ChangeScene(SCENE_GAME);*/
		};


		// Box2D���[���h�̍X�V�����i�K�v�Ȃ�ǉ��\�j
		if (m_world) {
			float timeStep = 1.0f / 60.0f; // �X�V�Ԋu
			int32 velocityIterations = 6;
			int32 positionIterations = 2;
			m_world->Step(timeStep, velocityIterations, positionIterations);
		}

		StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();

		m_player.Update();

		/* �X�e�[�W�|�C���g�̍X�V*/
		m_stagePointFactory.Update();
	
	

		switch (m_player.GetTouchStageSelectNum())
		{
		case 0:
			break;

		case 1:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		case 2:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		case 3:
			if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
			{
				SceneManager& sceneManager = SceneManager::GetInstance();
				sceneManager.ChangeScene(SCENE_GAME);
			};
			break;

		}


	
}

void StageSelectScene::Draw()
{
	
		//�o�b�t�@�N���A
		Clear();

		//2D�`��Ȃ̂Ő[�x����
		SetDepthEnable(false);

		if (g_stage_select_Texture != nullptr)
		{
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
				0.0f,
				XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
			);
		}

		StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
		m_player.Draw();


		// �X�e�[�W�|�C���g�̕`��
		m_stagePointFactory.Draw();

		b2Vec2 pos = m_player.GetTouchStagePointBody()->GetPosition();

		switch (m_player.GetTouchStageSelectNum())
		{
		case 0:
			break;

		case 1:
			
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x*SCALE,(pos.y*SCALE)-100),
				0.0f,
				XMFLOAT2(200, 200)
			);


			break;
		case 2:
		
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) - 100),
				0.0f,
				XMFLOAT2(200, 200)
			);

			break;
		case 3:
			
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

			DrawSpriteOld(
				XMFLOAT2(pos.x * SCALE, (pos.y * SCALE) - 100),
				0.0f,
				XMFLOAT2(200, 200)
			);

			break;
		default:
			break;
		}

		//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
		Present();
	
}

void StageSelectScene::Finalize()
{
	StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
	m_player.Finalize();
	/*m_stagePointFactory.Finalize();*/


	UnInitTexture(g_stage_select_Texture);
	// ���[���h���
	if (m_world) {
		DestroyWorld(m_world);
	}

	
}


void DestroyWorld(b2World*& world) {
	if (!world) return;

	// ���[���h���̂��ׂẴ{�f�B���폜
	b2Body* body = world->GetBodyList();
	while (body) {
		b2Body* nextBody = body->GetNext();
		world->DestroyBody(body);
		body = nextBody;
	}

	// ���[���h���̂����
	delete world;
	world = nullptr;
}