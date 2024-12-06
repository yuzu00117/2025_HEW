#include"UI_StaminaSpirit_Gauge.h"
#include"anchor_spirit.h"
#include"sprite.h"
#include"texture.h"
#include"renderer.h"
#include"player_stamina.h"



//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�


//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜����ɂ܂Ƃ߂Ă�ꍇ�̃e�N�X�`��
ID3D11ShaderResourceView* g_gauge_Texture;

//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜��������Ă���ꍇ�̃e�N�X�`��
//ID3D11ShaderResourceView* g_spirit_gauge_Texture;
//ID3D11ShaderResourceView* g_stamina_gauge_Texture;


void	StaminaSpiritGauge::Initialize()
{
	//�e�N�X�`���̃��[�h
	//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜����ɂ܂Ƃ߂Ă�ꍇ�̃e�N�X�`��
	g_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\gauge.png");

	//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜��������Ă���ꍇ�̃e�N�X�`��
	//g_spirit_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
	//g_stamina_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");


}

void	StaminaSpiritGauge::Finalize()
{
	//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜����ɂ܂Ƃ߂Ă�ꍇ�̏���
	if (g_gauge_Texture != nullptr)
	{
		UnInitTexture(g_gauge_Texture);
	}	

	//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜��������Ă���ꍇ�̏���
	//if (g_spirit_gauge_Texture != nullptr)
	//{
	//	UnInitTexture(g_spirit_gauge_Texture);
	//}	
	//if (g_stamina_gauge_Texture != nullptr)
	//{
	//	UnInitTexture(g_stamina_gauge_Texture);
	//}
}

void	StaminaSpiritGauge::Draw()
{
	//�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜����ɂ܂Ƃ߂Ă�ꍇ�̕`�揈��
	{
		float stamina = PlayerStamina::GetPlayerStaminaValue();
		float spirit = AnchorSpirit::GetAnchorSpiritValue();

		float total_value = stamina + spirit;

		XMFLOAT2 temp_scale;
		temp_scale.x = (total_value / (MAX_STAMINA + MAX_ANCHOR_SPIRIT)) * m_scale.x;
		temp_scale.y = m_scale.y;

		XMFLOAT2 temp_position;
		temp_position.x = (temp_scale.x / m_scale.x) * m_position.x;
		temp_position.y = m_position.y;

		float rotate = 0.0f;

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_gauge_Texture);
		//�`��
		DrawSerialDividedSprite(temp_position, rotate, temp_scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, 0, (int)total_value);

	}

	////�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜��������Ă���ꍇ�̕`�揈��
	//----------------------------------------------------------------------------------------------------------------------------------------------
	//{
	//	float temp_stamina_position_x;
	//	float temp_stamina_scale_x;
	//	float temp_spirit_position_x;
	//	float temp_spirit_scale_x;
	//	float rotate;
	//	XMFLOAT2	scale;

	//	//�̗̓Q�[�W
	//	{
	//		//���̃A���J�[�̃\�E���l�����
	//		float stamina = PlayerStamina::GetPlayerStaminaValue();

	//		//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
	//		//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
	//		temp_stamina_scale_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_scale.x;
	//		scale = XMFLOAT2(temp_stamina_scale_x, m_scale.y);
	//		//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
	//		//temp_position_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;
	//		temp_stamina_position_x = (temp_stamina_scale_x / m_scale.x) * m_position.x;


	//		rotate = 0.0f;
	//		// �V�F�[�_���\�[�X��ݒ�
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stamina_gauge_Texture);
	//		//�`��
	//		DrawSerialDividedSprite(XMFLOAT2(temp_stamina_position_x, m_position.y), rotate, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, 0, (int)stamina);

	//	}
	//	//�\�E���Q�[�W
	//	{
	//		//���̃A���J�[�̃\�E���l�����
	//		float spirit = AnchorSpirit::GetAnchorSpiritValue();

	//		//�e�N�X�`��UV��M���ĕ\�����Ă���̂Łi�^�񒆂𒆐S�ɃT�C�Y�ς�����Ⴉ��A����������S�ɂ������j���̏���������
	//		//Max��Ԃ̎��̒����ɔ�Ⴕ�āA���̃\�E���l�̒����𒲐�
	//		temp_spirit_scale_x = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_scale.x;
	//		scale = XMFLOAT2(temp_spirit_scale_x, m_scale.y);
	//		//Max��Ԃ̎��̈ʒu�ɔ�Ⴕ�āA���̃\�E���l�̏ꍇ�̈ʒu�Ɉړ�
	//		temp_spirit_position_x = temp_stamina_scale_x + (temp_spirit_scale_x / 2);
	//		//temp_position_x = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;


	//		rotate = 0.0f;
	//		// �V�F�[�_���\�[�X��ݒ�
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_spirit_gauge_Texture);
	//		//�`��
	//		DrawSerialDividedSprite(XMFLOAT2(temp_spirit_position_x, m_position.y), rotate, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)spirit);

	//	}

	//}//end_�̗̓Q�[�W�ƃ\�E���Q�[�W�̉摜��������Ă���ꍇ�̕`�揈��


}