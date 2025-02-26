#include"Gauge_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


#define GaugeUI_Cols_MAX	(100)

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
static ID3D11ShaderResourceView* g_ring_Texture = NULL;//�^�񒆂̕�΂̓��ꕨ�I�Ȃ��

static ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//��΁@��
static ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//��΁@��
static ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//��΁@��

static ID3D11ShaderResourceView* g_red_jewel_fit_effect = NULL;	//��΁@��
static ID3D11ShaderResourceView* g_blue_jewel_fit_effect = NULL;	//��΁@��
static ID3D11ShaderResourceView* g_yellow_jewel_fit_effect = NULL;//��΁@��
static ID3D11ShaderResourceView* g_soul_gage_blue_Texture = NULL;          //�\�E���Q�[�W�̃e�N�X�`���i�j
static ID3D11ShaderResourceView* g_soul_gage_yellow_Texture = NULL;        //�\�E���Q�[�W�̃e�N�X�`���i���F�j
static ID3D11ShaderResourceView* g_soul_gage_red_Texture = NULL;           //�\�E���Q�[�W�̃e�N�X�`���i�ԁj
static ID3D11ShaderResourceView* g_soul_gage_border_Texture = NULL;	//�\�E���Q�[�W�̊O�g
static ID3D11ShaderResourceView* g_anchor_level_division_Texture = NULL;  //�A���J�[���x���̎d�؂�
static ID3D11ShaderResourceView* g_anchor_level_outline_Texture = NULL;  //�A���J�[���x���̊O���̑���




// �ÓI�����o�[�ϐ��̏�����
DirectX::XMFLOAT2 Gauge_UI::player_ui_position = DirectX::XMFLOAT2(155.f, 440.f);
DirectX::XMFLOAT2 Gauge_UI::player_ui_size = DirectX::XMFLOAT2(310.f, 630.f);

DirectX::XMFLOAT2 Gauge_UI::gauge_only_position = DirectX::XMFLOAT2(105.f, 405.f);	//�g���������Q�[�W�F�̕����݂̂̈ʒu
DirectX::XMFLOAT2 Gauge_UI::gauge_only_size = DirectX::XMFLOAT2(68.f, 420.f);		//�g���������Q�[�W�F�̕����݂̂̃T�C�Y

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
	//�����o�[�ϐ��̏�����
	m_blue_jewel_collected = false;
	m_red_jewel_collected = false;
	m_yellow_jewel_collected = false;
	m_blue_jewel_collected_effect_cnt = 0;
	m_red_jewel_collected_effect_cnt = 0;
	m_yellow_jewel_collected_effect_cnt = 0;

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
		int count_layer = 0;	//�����w�ڕ`���Ă���̂�
		//	���̃A���J�[���x�����擾
		int anchor_level = AnchorSpirit::GetAnchorLevel();
		//�@�����\�E���Q�[�W���������_���[�W�����Ă����̂Ȃ�A�Q�[�W�̔������̃_���[�W���ڗ����邽�߂ɁA�������̂�̍��̃A���J�[���x�����v�Z
		int prev_anchor_level = (AnchorSpirit::GetAnchorSpiritValue() + AnchorSpirit::GetAnchorSpiritDamage()) / 100.0f + 1;
		//�������̂�̌v�Z���ꂽ�A���J�[���x����3�𒴂��Ȃ����߂̐���
		if (prev_anchor_level > 3) { prev_anchor_level = 3; }	
		//���������ۂ̃A���J�[���x�����@1�@���x���_�E���������ǁA�������̂���܂���̃��x���ɂ���ꍇ�́A�`��͔������̂�̃��x���ɍ��킹��i�܂����x���_�E�����ĂȂ���Ԃɂ��Ă����j
		// �i���ۃ_���[�W�͂����Q�[�W�ɓ��������ǁA��������1�t���[���Â����Ă�������A����ȉ�肭�ǂ����Ȃ��Ƃ����Ȃ��j
		if (anchor_level < prev_anchor_level) { anchor_level++; }

		//�F�Q�[�W
		if (anchor_level < 3)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue();
			if (virtual_spirit > GaugeUI_Cols_MAX) { virtual_spirit = GaugeUI_Cols_MAX; }

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / GaugeUI_Cols_MAX) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;

			int PatternID = GaugeUI_Cols_MAX - virtual_spirit;
			if (virtual_spirit >= GaugeUI_Cols_MAX)
			{
				PatternID ++;
			}

			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_blue_Texture);

			DrawGaugeSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, GaugeUI_Cols_MAX, GaugeUI_Cols_MAX, PatternID);

			//�������ꂪ�P�w�ڂȂ玟�̓_���[�W�w��`��
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

		}
		//���F�Q�[�W
		if (anchor_level > 1)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 100;	//100�͉��̃A���J�[���x���̃}�b�N�X
			if (virtual_spirit > GaugeUI_Cols_MAX) { virtual_spirit = GaugeUI_Cols_MAX; }

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / GaugeUI_Cols_MAX) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;

			int PatternID = GaugeUI_Cols_MAX - virtual_spirit;
			if (virtual_spirit >= GaugeUI_Cols_MAX)
			{
				PatternID++;
			}

			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_yellow_Texture);

			DrawGaugeSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, GaugeUI_Cols_MAX, GaugeUI_Cols_MAX, PatternID);

			//�������ꂪ�P�w�ڂȂ玟�̓_���[�W�w��`��
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

		}
		//�ԐF�Q�[�W
		if (anchor_level == 3)
		{
			//���̃Q�[�W�g�ɑ�������A���J�[�̃\�E���l���v�Z
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 200;		//200�͉��̃A���J�[���x���̃}�b�N�X

			//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
			//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
			temp_spirit_scale_y = (virtual_spirit / GaugeUI_Cols_MAX) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
		//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;

			int PatternID = GaugeUI_Cols_MAX - virtual_spirit;
			if (virtual_spirit >= GaugeUI_Cols_MAX)
			{
				PatternID++;
			}

			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_red_Texture);

			DrawGaugeSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, GaugeUI_Cols_MAX, GaugeUI_Cols_MAX, PatternID);

			//�������ꂪ�P�w�ڂȂ玟�̓_���[�W�w��`��
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

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

	if (g_ring_Texture) UnInitTexture(g_ring_Texture);

	if (g_red_jewel_Texture) UnInitTexture(g_red_jewel_Texture);
	if (g_blue_jewel_Texture) UnInitTexture(g_blue_jewel_Texture);
	if (g_yellow_jewel_Texture) UnInitTexture(g_yellow_jewel_Texture);

	if (g_red_jewel_fit_effect) UnInitTexture(g_red_jewel_fit_effect);
	if (g_blue_jewel_fit_effect) UnInitTexture(g_blue_jewel_fit_effect);
	if (g_yellow_jewel_fit_effect) UnInitTexture(g_yellow_jewel_fit_effect);

	if (g_soul_gage_blue_Texture) UnInitTexture(g_soul_gage_blue_Texture);
	if (g_soul_gage_yellow_Texture) UnInitTexture(g_soul_gage_yellow_Texture);
	if (g_soul_gage_red_Texture) UnInitTexture(g_soul_gage_red_Texture);
	if (g_soul_gage_border_Texture) UnInitTexture(g_soul_gage_border_Texture);

	if (g_anchor_level_division_Texture) UnInitTexture(g_anchor_level_division_Texture);
	if (g_anchor_level_outline_Texture) UnInitTexture(g_anchor_level_outline_Texture);

}

void Gauge_UI::DrawGaugeDamaged()
{
	//�_���[�W�󂯂ĂȂ��Ȃ�`����Ȃ�
	if (AnchorSpirit::GetAnchorSpiritDamage() <= 0)
	{
		return;
	}

	float temp_spirit_position_y;  //�\�E���Q�[�W���ړ�������̍��W�@
	float temp_spirit_scale_y;	   //�\�E���Q�[�W�̍��̑傫��
	XMFLOAT2	scale;

	//�Q�[�W�g�ɑ�������O�t���[���̃A���J�[�̃\�E���l���v�Z
	float prev_virtual_spirit = (int)AnchorSpirit::GetAnchorSpiritValue() % GaugeUI_Cols_MAX + AnchorSpirit::GetAnchorSpiritDamage();

	//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
	//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
	temp_spirit_scale_y = (prev_virtual_spirit / GaugeUI_Cols_MAX) * gauge_only_size.y;
	scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
	//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
//�u�����`�O�̃o�[�W����	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
	temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


	float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() + AnchorSpirit::GetAnchorSpiritDamage();
	if (virtual_spirit > 200.0f) {
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_red_Texture);
	}
	else if (virtual_spirit > 100.0f)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_yellow_Texture);
	}
	else
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_blue_Texture);
	}

	int PatternID = GaugeUI_Cols_MAX - prev_virtual_spirit;
	if (prev_virtual_spirit >= GaugeUI_Cols_MAX)
	{
		PatternID++;
	}

	DrawGaugeSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, GaugeUI_Cols_MAX, GaugeUI_Cols_MAX, PatternID, 0.55f);


}

