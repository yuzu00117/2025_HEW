//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description �v���C���[
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �E����AGame.cpp�Ȃ��ŏ������Ƃ������v���C���[��new�����Ă���
// 
//          
//----------------------------------------------------------------------------------------------------

#include"player.h"
#include"tool.h"
#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"collider_type.h"



Player::Player(XMFLOAT2 position, XMFLOAT2 size, float angle, bool bFixed)
	: SquareCollider(position, size, angle, bFixed)
{
	// �v���C���[��p�̏����ݒ�
	SquareCollider::SetColliderType(collider_player);




	if (!bFixed) {
		b2FixtureDef fixture;
		fixture.shape = SquareCollider::GetColliderBody()->GetFixtureList()->GetShape();
		fixture.density = 1.0f;
		fixture.friction = 0.5f;  // �v���C���[�ɓ����������C
		fixture.restitution = 0.1f; // �����W���̒ǉ�

		SquareCollider::GetColliderBody()->CreateFixture(&fixture);
	}
}

Player::~Player()
{

}

void Player::Init() {
	// ����������
	
	//�v���C���[
	
	
}

void Player::Update() {
	// �v���C���[�̍X�V����



}


void Player::Draw()
{
	
		//�R���C�_�[�ƈʒu���̕␳�����邽��
		float scale = 80.f;

		//�����Ŋ��Z����Ƃ��@16m*9�ɂ���@�܂��ʂ̐^�񒆂� x.8 y.4.5


		//�R���C�_�[�̈ʒu�̎擾
		XMFLOAT2 postion;
		postion.x = SquareCollider::GetColliderBody()->GetPosition().x;
		postion.y = SquareCollider::GetColliderBody()->GetPosition().y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &m_p_Texture);

		//�T�C�Y���R���X�g���N�̎��_��/100���Ă邩�璲���̂��߁@���P�O�O
		DrawSprite({ postion.x * scale,postion.y * scale }, SquareCollider::GetColliderBody()->GetAngle(), { SquareCollider::Get_m_size().x * 100 * scale, SquareCollider::Get_m_size().y * 100 * scale });




		
}

void Player::Uninit() {
	// ���\�[�X�̉������
}