#include"Gauge_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_ring_Texture = NULL;//�^�񒆂̕�΂̓��ꕨ�I�Ȃ��

ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//��΁@��
ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//��΁@��
ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//��΁@��

ID3D11ShaderResourceView* g_red_jewel_fit_effect = NULL;	//��΁@��
ID3D11ShaderResourceView* g_blue_jewel_fit_effect = NULL;	//��΁@��
ID3D11ShaderResourceView* g_yellow_jewel_fit_effect = NULL;//��΁@��
ID3D11ShaderResourceView* g_soul_gage_blue_Texture = NULL;          //�\�E���Q�[�W�̃e�N�X�`���i�j
ID3D11ShaderResourceView* g_soul_gage_yellow_Texture = NULL;        //�\�E���Q�[�W�̃e�N�X�`���i���F�j
ID3D11ShaderResourceView* g_soul_gage_red_Texture = NULL;           //�\�E���Q�[�W�̃e�N�X�`���i�ԁj
ID3D11ShaderResourceView* g_soul_gage_border_Texture = NULL;	//�\�E���Q�[�W�̊O�g
ID3D11ShaderResourceView* g_anchor_level_division_Texture = NULL;  //�A���J�[���x���̎d�؂�
ID3D11ShaderResourceView* g_anchor_level_outline_Texture = NULL;  //�A���J�[���x���̊O���̑���




// �ÓI�����o�[�ϐ��̏�����
DirectX::XMFLOAT2 Gauge_UI::player_ui_position = DirectX::XMFLOAT2(155.f, 390.f);
DirectX::XMFLOAT2 Gauge_UI::player_ui_size = DirectX::XMFLOAT2(350.f, 700.f);

DirectX::XMFLOAT2 Gauge_UI::gauge_only_position = DirectX::XMFLOAT2(98.f, 355.f);	//�g���������Q�[�W�F�̕����݂̂̈ʒu
DirectX::XMFLOAT2 Gauge_UI::gauge_only_size = DirectX::XMFLOAT2(74.f, 478.f);		//�g���������Q�[�W�F�̕����݂̂̃T�C�Y

float Gauge_UI::player_ui_alpha = 1.0f;

bool	Gauge_UI::m_blue_jewel_collected = false;
bool	Gauge_UI::m_red_jewel_collected = false;
bool	Gauge_UI::m_yellow_jewel_collected = false;
float	Gauge_UI::m_blue_jewel_collected_effect_cnt = 0;
float	Gauge_UI::m_red_jewel_collected_effect_cnt = 0;
float	Gauge_UI::m_yellow_jewel_collected_effect_cnt = 0;
DirectX::XMFLOAT2 Gauge_UI::m_ring_position = player_ui_position;

Gauge_UI::Gauge_UI()
{
}

Gauge_UI::~Gauge_UI()
{
}

void Gauge_UI::Initialize()
{
	//�e�N�X�`���̃��[�h

	//�Ȃ񂩖{�̂ۂ����
	g_ring_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\ring.png");

	//��΂���
	g_red_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
	g_blue_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
	g_yellow_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");

	//��΂��Q�b�g�������̃G�t�F�N�g
	g_red_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Red_3x4.png");
	g_blue_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Blue_3x4.png");
	g_yellow_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Yellow_3x4.png");

	//�\�E���Q�[�W�B
	g_soul_gage_blue_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_blue.png");
	g_soul_gage_yellow_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_yellow.png");
	g_soul_gage_red_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_red.png");
	g_soul_gage_border_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_border.png");

	//�A���J�[���x���̃e�N�X�`������
	g_anchor_level_division_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\gage_division.png");
	g_anchor_level_outline_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_out_line.png");

	




}

void Gauge_UI::Update()
{

}

void Gauge_UI::Draw()
{	
	float temp_stamina_position_y;	//�̗̓Q�[�W���ړ�������̍��W
	float temp_stamina_scale_y;		//�̗̓Q�[�W�̍��̑傫��
	float temp_spirit_position_y;  //�\�E���Q�[�W���ړ�������̍��W�@
	float temp_spirit_scale_y;	   //�\�E���Q�[�W�̍��̑傫��
	XMFLOAT2	scale;

	



	
	//-------------------------------------------------------------------------------------------
	//�A���J�[�Q�[�W

	//�A���J�[�Q�[�W�̔w�i��\��

	//�Q�[�W�̊O�g
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_border_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);

	
	//// �V�F�[�_���\�[�X��ݒ�
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_background_Texture);

	//DrawSpriteOld(
	//	{ player_ui_position },
	//	0,
	//	{ player_ui_size },
	//	player_ui_alpha
	//);




	if (true) //�����̓A���J�[�Q�[�W�́@���l�ɂ���ĕύX���K�v
	{
		//�F�Q�[�W
		if (AnchorSpirit::GetAnchorLevel() < 3)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue();
			if (virtual_spirit > 100) { virtual_spirit = 100.0f; }

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_blue_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

		}
		//���F�Q�[�W
		if (AnchorSpirit::GetAnchorLevel() > 1)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 100;
			if (virtual_spirit > 100) { virtual_spirit = 100.0f; }

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_yellow_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

		}
		//�ԐF�Q�[�W
		if (AnchorSpirit::GetAnchorLevel() == 3)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 200;

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_red_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

		}
	}





	//�g��\��
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_division_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);
	

	//�O���̑�����\��
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_outline_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);




	//-----------------------------------------------------------------------------------------

	//----------------------------------------------------------
	// ring�̕`��
	// 
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_ring_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);



	//-------------------------------------------------------------------------------------------
	//��΂̕`��

	if (m_red_jewel_collected)//�Ԃ̕��
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_red_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);

	}

	if (m_blue_jewel_collected)//�̕��
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_blue_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}

	if (m_yellow_jewel_collected)//���F�̕��
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_yellow_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}
	//-------------------------------------------------------------------------------------------
	//��΂��擾�������̃G�t�F�N�g�̕\��

	if (m_blue_jewel_collected_effect_cnt != 0)
	{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_blue_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(75, 600), 0.0f, XMFLOAT2(50, 50), 4, 3, m_blue_jewel_collected_effect_cnt, 1.0);

		m_blue_jewel_collected_effect_cnt += 0.3;

		if (12 < m_blue_jewel_collected_effect_cnt)
		{
			m_blue_jewel_collected_effect_cnt = 0;
		}
	}

	if (m_red_jewel_collected_effect_cnt != 0)
	{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_red_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(100, 650), 0.0f, XMFLOAT2(50, 50), 4, 3, m_red_jewel_collected_effect_cnt, 1.0);

		m_red_jewel_collected_effect_cnt += 0.3;

		if (12 < m_red_jewel_collected_effect_cnt)
		{
			m_red_jewel_collected_effect_cnt = 0;
		}
	}

	if (m_yellow_jewel_collected_effect_cnt != 0)
	{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_yellow_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(50, 650), 0.0f, XMFLOAT2(50, 50), 4, 3, m_yellow_jewel_collected_effect_cnt, 1.0);

		m_yellow_jewel_collected_effect_cnt += 0.3;

		if (12 < m_yellow_jewel_collected_effect_cnt)
		{
			m_yellow_jewel_collected_effect_cnt = 0;
		}
	}
	



}

void Gauge_UI::Finalize()
{

	if (g_ring_Texture != NULL)
	{
		// �����O�̃e�N�X�`�����
		UnInitTexture(g_ring_Texture);

		// ��΂̃e�N�X�`�����
		UnInitTexture(g_red_jewel_Texture);
		UnInitTexture(g_blue_jewel_Texture);
		UnInitTexture(g_yellow_jewel_Texture);

		// ��΂̃t�B�b�g�G�t�F�N�g�̉��
		UnInitTexture(g_red_jewel_fit_effect);
		UnInitTexture(g_blue_jewel_fit_effect);
		UnInitTexture(g_yellow_jewel_fit_effect);

		// �\�E���Q�[�W�̃e�N�X�`�����
		UnInitTexture(g_soul_gage_blue_Texture);
		UnInitTexture(g_soul_gage_yellow_Texture);
		UnInitTexture(g_soul_gage_red_Texture);
		UnInitTexture(g_soul_gage_border_Texture);

		// �A���J�[���x���̃e�N�X�`�����
		UnInitTexture(g_anchor_level_division_Texture);
		UnInitTexture(g_anchor_level_outline_Texture);

		// NULL�Ƀ��Z�b�g
		g_ring_Texture = NULL;

		g_red_jewel_Texture = NULL;
		g_blue_jewel_Texture = NULL;
		g_yellow_jewel_Texture = NULL;

		g_red_jewel_fit_effect = NULL;
		g_blue_jewel_fit_effect = NULL;
		g_yellow_jewel_fit_effect = NULL;

		g_soul_gage_blue_Texture = NULL;
		g_soul_gage_yellow_Texture = NULL;
		g_soul_gage_red_Texture = NULL;
		g_soul_gage_border_Texture = NULL;

		g_anchor_level_division_Texture = NULL;
		g_anchor_level_outline_Texture = NULL;
		
	}
}

