//-----------------------------------------------------------------------------------------------------
// #name sloping_block.cpp
// #description �X�΂̂����u���b�N�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//
//----------------------------------------------------------------------------------------------------
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"sloping_block.h"

//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_sloping_block_right_down_Texture = NULL;//�e�N�X�`�� �E��
static ID3D11ShaderResourceView* g_sloping_block_left_down_Texture = NULL;//�e�N�X�`�� �@������
static ID3D11ShaderResourceView* g_sloping_block_right_upper_Texture = NULL;//�e�N�X�`�� �@�E��
static ID3D11ShaderResourceView* g_sloping_block_left_upper_Texture = NULL;//�e�N�X�`�� �@����






sloping_block::sloping_block(b2Vec2 position, b2Vec2 size, SlopingBlockAspect aspect)
{

	//�������Z�b�g���Ă���
	SetBlockAspect(aspect);

	//�`��悤�̃T�C�Y�̐ݒ�
	SetSlopingBlockSize(size);




	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������

	//�T�C�Y��ݒ肷��
	b2Vec2 sloping_block_size;
	sloping_block_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	sloping_block_size.y = size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef sloping_block_body;//�Ζʂ̃u���b�N
	sloping_block_body.type = b2_staticBody;
	sloping_block_body.position.Set(position.x, position.y);
	sloping_block_body.fixedRotation = false;

	b2Body* m_sloping_block_body = world->CreateBody(&sloping_block_body);

	SetObjectSlopingBlockBody(m_sloping_block_body);



	b2PolygonShape sloping_block_shape;
	// �O�p�`�̃V�F�C�v���`
	b2PolygonShape triangleShape;
	b2Vec2 vertices[3] = {b2Vec2(0.0f,0.0f)};

	switch (aspect)
	{
	case right_down:
		vertices[0].Set(+sloping_block_size.x / 2, -sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x / 2, +sloping_block_size.y / 2); 
		vertices[2].Set(sloping_block_size.x / 2, sloping_block_size.y / 2);   
		break;
	case right_upper:
		vertices[0].Set(+sloping_block_size.x / 2, -sloping_block_size.y / 2);   
		vertices[1].Set(sloping_block_size.x / 2, sloping_block_size.y / 2);  
		vertices[2].Set(-sloping_block_size.x / 2, -sloping_block_size.y / 2);  
		break;
	case left_down:
		vertices[0].Set(-sloping_block_size.x / 2, sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x/2, -sloping_block_size.y/2); 
		vertices[2].Set(sloping_block_size.x, sloping_block_size.y); 
		break;
	case left_upper:
		vertices[0].Set(-sloping_block_size.x / 2, -sloping_block_size.y / 2); 
		vertices[1].Set(sloping_block_size.x / 2, -sloping_block_size.y / 2);
		vertices[2].Set(-sloping_block_size.x / 2, sloping_block_size.y / 2);
		break;
	default:
		break;
	}
	
	triangleShape.Set(vertices, 3); // ���_���w�肵�ĎO�p�`��ݒ�

	b2FixtureDef sloping_block_fixture;

	sloping_block_fixture.shape = &triangleShape;
	sloping_block_fixture.density = 3.0f;
	sloping_block_fixture.friction = 0.5f;//���C
	sloping_block_fixture.restitution = 0.0f;//�����W��
	sloping_block_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* object_sloping_block_fixture = m_sloping_block_body->CreateFixture(&sloping_block_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_sloping_block_data = new ObjectData{ collider_ground };//��U�n�ʔ���
	object_sloping_block_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_sloping_block_data);


	int ID = object_sloping_block_data->GenerateID();//ID���擾
	object_sloping_block_data->id = ID;//�t�B�N�X�`���ɓo�^
	SetID(ID);//�N���X�ɓo�^

};

sloping_block::~sloping_block()
{
}


void sloping_block::Initialize()
{
	//�e�N�X�`���̏�����
	g_sloping_block_left_down_Texture=	InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_left_down.png");//����
	g_sloping_block_left_upper_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_left_upper.png");//����
	g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_right_down.png");//�E��
	g_sloping_block_right_upper_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_Sloping_block_right_upper.png");//�E��

}

void sloping_block::Update()
{
}

void sloping_block::Draw()
{
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	///�������璲�����Ă�

	b2Vec2 sloping_block_pos = GetObjectSlopingBlockBody()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((sloping_block_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((sloping_block_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	switch (GetBlockAspect())//�������R���X�g���N�^�ŃZ�b�g���Ă�̂ł�����󂯎���ĕ`��
	{
	case right_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_down_Texture);
		break;
	case right_upper:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_upper_Texture);
		break;
	case left_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_down_Texture);
		break;
	case left_upper:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_upper_Texture);
		break;
	default:
		break;
	}

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectSlopingBlockBody()->GetAngle(),
		{ GetSlopingBlockSize().x * scale,GetSlopingBlockSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);
}

void sloping_block::Finalize()
{
}