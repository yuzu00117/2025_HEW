#include"player_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_ring_Texture = NULL;//�^�񒆂̕�΂̓��ꕨ�I�Ȃ��

ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//��΁@��
ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//��΁@��
ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//��΁@��


ID3D11ShaderResourceView* g_soul_gage_Texture = NULL;           //�\�E���Q�[�W�̃e�N�X�`��
ID3D11ShaderResourceView* g_soul_gage_HP_Texture = NULL;		//�\�E���Q�[�W��HP����
ID3D11ShaderResourceView* g_soul_gage_border_Texture = NULL;	//�\�E���Q�[�W�̊O�g


ID3D11ShaderResourceView* g_anchor_level_background_Texture = NULL;//�A���J�[�̃Q�[�W�̃o�b�N�O�����h
ID3D11ShaderResourceView* g_anchor_level_1_Texture = NULL;		   //�A���J�[���x���P�̃e�N�X�`��
ID3D11ShaderResourceView* g_anchor_level_2_Texture = NULL;		   //�A���J�[���x��2�̃e�N�X�`��
ID3D11ShaderResourceView* g_anchor_level_3_Texture = NULL;		   //�A���J�[���x��3�̃e�N�X�`��
ID3D11ShaderResourceView* g_anchor_level_border_Texture = NULL;	   //�A���J�[���x���̊O�g
ID3D11ShaderResourceView* g_anchor_level_division_Texture = NULL;  //�A���J�[���x���̎d�؂�
ID3D11ShaderResourceView* g_anchor_level_outline_Texture = NULL;  //�A���J�[���x���̊O���̑���

// �ÓI�����o�[�ϐ��̏�����
DirectX::XMFLOAT2 player_UI::player_ui_position = DirectX::XMFLOAT2(155.f, 390.f);
DirectX::XMFLOAT2 player_UI::player_ui_size = DirectX::XMFLOAT2(350.f, 700.f);

DirectX::XMFLOAT2 player_UI::gauge_only_position = DirectX::XMFLOAT2(98.f, 355.f);
DirectX::XMFLOAT2 player_UI::gauge_only_size = DirectX::XMFLOAT2(74.f, 478.f);

float player_UI::player_ui_alpha = 1.0f;

bool	player_UI::m_blue_jewel_collected = false;
bool	player_UI::m_red_jewel_collected = false;
bool	player_UI::m_yellow_jewel_collected = false;

DirectX::XMFLOAT2 player_UI::m_ring_position = player_ui_position;

player_UI::player_UI()
{
}

player_UI::~player_UI()
{
}

void player_UI::Initialize()
{
	//�e�N�X�`���̃��[�h

	//�Ȃ񂩖{�̂ۂ����
	g_ring_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\ring.png");

	//��΂���
	g_red_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
	g_blue_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
	g_yellow_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");

	//�\�E���Q�[�W�B
	/*g_soul_gage_background_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_back_ground.png");*/
	g_soul_gage_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_soul_no_white.png");
	g_soul_gage_HP_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_HP_no_white.png");
	g_soul_gage_border_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_border.png");

	//�A���J�[���x���̃e�N�X�`������
	g_anchor_level_background_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level_back_ground.png");
	g_anchor_level_1_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level1.png");
	g_anchor_level_2_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level2.png");
	g_anchor_level_3_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level3.png");
	g_anchor_level_border_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\level_line.png");
	g_anchor_level_division_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\gage_division.png");
	g_anchor_level_outline_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_out_line.png");

	




}

void player_UI::Update()
{

}

void player_UI::Draw()
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


	if (true) //�����̓A���J�[�Q�[�W��HP�@���l�ɂ���ĕύX���K�v
	{
		//���̃A���J�[�̃\�E���l�����
		float stamina = PlayerStamina::GetPlayerStaminaValue();

		//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
		//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
		temp_stamina_scale_y = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * gauge_only_size.y;
		scale = XMFLOAT2(gauge_only_size.x, temp_stamina_scale_y);
		//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//temp_position_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;
		temp_stamina_position_y = (gauge_only_size.y / 2) - (temp_stamina_scale_y / 2) + gauge_only_position.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_HP_Texture);
		DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_stamina_position_y), 0.0f, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)stamina);

	}



	if (true) //�����̓A���J�[�Q�[�W�́@���l�ɂ���ĕύX���K�v
	{

		//���̃A���J�[�̃\�E���l�����
		float spirit = AnchorSpirit::GetAnchorSpiritValue();

		//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
		//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
		temp_spirit_scale_y = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * gauge_only_size.y;
		scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
		//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;


		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_Texture);

		DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)spirit);
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

	//---------------------------------------------------------------------------------------
	//�A���J�[���x���̕\��

	//�A���J�[���x���̔w�i
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_background_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);

	//�A���J�[���x��1
	if (AnchorSpirit::GetAnchorLevel() >= 1)
	{

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_1_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}

	//�A���J�[���x��2
	if (AnchorSpirit::GetAnchorLevel() >= 2)
	{
		
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_2_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}


	//�A���J�[���x��1
	if (AnchorSpirit::GetAnchorLevel() >= 3)
	{
		
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_3_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}


	//�A���J�[���x���̊O�g
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_border_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);


}

void player_UI::Finalize()
{

	if (g_ring_Texture != NULL)
	{
		//�{�̂ۂ����
		UnInitTexture(g_ring_Texture);
		//���
		UnInitTexture(g_red_jewel_Texture);
		UnInitTexture(g_blue_jewel_Texture);
		UnInitTexture(g_yellow_jewel_Texture);

		//�\�E���Q�[�W�B
	
		UnInitTexture(g_soul_gage_Texture);
		UnInitTexture(g_soul_gage_HP_Texture);
		UnInitTexture(g_soul_gage_border_Texture);

		//�A���J�[���x���̃e�N�X�`������
		UnInitTexture(g_anchor_level_background_Texture);
		UnInitTexture(g_anchor_level_1_Texture);
		UnInitTexture(g_anchor_level_2_Texture);
		UnInitTexture(g_anchor_level_3_Texture);
		UnInitTexture(g_anchor_level_border_Texture);

		UnInitTexture(g_anchor_level_division_Texture);
		UnInitTexture(g_anchor_level_outline_Texture);

		g_ring_Texture = NULL;

		g_red_jewel_Texture = NULL;
		g_blue_jewel_Texture = NULL;
		g_yellow_jewel_Texture = NULL;

	
		g_soul_gage_Texture = NULL;
		g_soul_gage_HP_Texture = NULL;
		g_soul_gage_border_Texture = NULL;

		g_anchor_level_background_Texture = NULL;
		g_anchor_level_1_Texture = NULL;
		g_anchor_level_2_Texture = NULL;
		g_anchor_level_3_Texture = NULL;
		g_anchor_level_border_Texture = NULL;


		g_anchor_level_division_Texture = NULL;
		g_anchor_level_outline_Texture = NULL;
		
	}
}

