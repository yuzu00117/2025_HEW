//-----------------------------------------------------------------------------------------------------
// #name OP.cpp
// #description �I�[�v�j���O�V�[��
// #make 2025/2/7�@���j�S
// #update 2025/2/12
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------
#include "OP.h"
#include "keyboard.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"

static ID3D11ShaderResourceView* g_pause_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_background_Texture = NULL;//�A���J�[�̃e�N�X�`��

static bool g_pressed = false;

OP::OP()
{
}

OP::~OP()
{
}

void OP::Initialize()
{
	video.Initialize("asset/movie/op.mp4", false);
	g_pause_Texture = InitTexture(L"asset\\texture\\sample_texture\\video_pause.png");

	m_pause = false;
	
}

void OP::Update()
{
	bool finished = video.Update();

	if (finished)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_GAME);
	}

	if (!g_pressed && Keyboard_IsKeyDownTrigger(KK_X))
	{
		Video_State state = video.GetState();
		if (state == Video_Start)
		{
			video.SetState(Video_Pause);
			m_pause = !m_pause;
		}
		else if (state == Video_Pause)
		{
			video.SetState(Video_Resume);
			m_pause = !m_pause;
		}
		else if (state == Video_Resume)
		{
			video.SetState(Video_Pause);
			m_pause = !m_pause;
		}
	}
	g_pressed = Keyboard_IsKeyDownTrigger(KK_X);

	if (Keyboard_IsKeyDownTrigger(KK_LEFTCONTROL))
	{
		video.SetState(Video_Skip);
	}
	
}

void OP::Draw()
{
	//�o�b�t�@�N���A
	Clear();

	if (m_pause)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_pause_Texture);

		DrawSprite(
			{ -8.0f, -138.0f },
			0.0f,
			{ 100.0f,100.0f }
		);

	}

	video.Draw(XMFLOAT2(-8.0f, -138.0f), 560.0f);



	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void OP::Finalize()
{
	video.Finalize();
}
