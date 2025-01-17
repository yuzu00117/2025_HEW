//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X��cpp�t�@�C��
// #make 2024/11/19
// #update 2024/12/13
// #comment �ǉ��E�C���\��
//          �E�v���C���[�Ƃ̏Փ˔����ǉ��\��
//			�E�|��Ă����I�u�W�F�N�g�Ƃ̏Փ˔����ǉ��\��
//			�E��ʓ��ɂ��Ȃ���Update���Ȃ�������ǉ��\��
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_stamina.h"
#include"anchor_spirit.h"

//�G�l�~�[���v���C���[�ɐG�ꂽ���̏���
void Enemy::CollisionPlayer()
{
	PlayerStamina::EditPlayerStaminaValue(-GetDamage());
	SetUse(false);
}

//�G�l�~�[�������Ă����Ԃ̃I�u�W�F�N�g�ɐG�ꂽ���̏���
void Enemy::CollisionPulledObject()
{
	AnchorSpirit::EditAnchorSpiritValue(GetSoulgage());
	SetUse(false);
}

//�G�l�~�[���Z���T�[���ɓ��������̏���
void Enemy::InPlayerSensor()
{
	SetInScreen(true);
}

//�G�l�~�[���Z���T�[�O�ɏo�����̏���
void Enemy::OutPlayerSensor()
{
	//�Z���T�[���ŌĂяo����邱�Ƃ�����̂�
	//if(GetBody()->GetPosition().x)
	//SetInScreen(false);
}