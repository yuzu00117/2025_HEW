//-----------------------------------------------------------------------------------------------------
// #name square_collider.cpp
// #description �l�p�̂����蔻��
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E�����Ŏl�p�̂����蔻��̃R���X�g���N�^���Ă�
// �@�@�@�@ �E�����łقƂ�ǂ̏������Ǘ����Ă�
//          �E��������Ȃ��C����������Ƃ��낢�����邯�ǃf�t�H���g�͂���Ƃ��āA����̌p�������ō�낤���Ȃ��Ďv���Ă�i�����W���Ƃ��ˁj
// �@�@�@�@ �E�Ƃ��������\�ȏ������Ă���̂ŁA���╪�������Č���ꂽ��@������ĂȂ�@����܂�ו������Ă���肸�炭�˂��Ďv�������ǁ@������Ȃ�ʂɂ�����
//          
//----------------------------------------------------------------------------------------------------


#include "Title.h"
#include "sprite.h"
#include "texture.h"

//�}�N����`
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitTitle(void)
{
	g_Texture = InitTexture(L"asset\\texture\\majo002.jpg");
}

//�I������
void UninitTitle(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateTitle(void) 
{
	
}

//�`�揈��
void DrawTitle(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
