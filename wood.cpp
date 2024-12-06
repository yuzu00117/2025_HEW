//-----------------------------------------------------------------------------------------------------
// #name wood.cpp
// #description �؂̃I�u�W�F�N�g��CPP
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------

#include"wood.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Wood_Texture1 = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Wood_Texture2 = NULL;//�A���J�[�̃e�N�X�`��


int ObjectData::current_id = 0;

wood::wood(b2Vec2 Postion, b2Vec2 Wood_size, b2Vec2 AnchorPoint_size,bool left)
{

	SetWoodSize(Wood_size);
	SetAnchorPointSize(AnchorPoint_size);



	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������

	//�T�C�Y��ݒ肷��
	b2Vec2 wood_size;
	wood_size.x = Wood_size.x / BOX2D_SCALE_MANAGEMENT;
	wood_size.y = Wood_size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Wood_body;//�؂̊��̕���
	Wood_body.type = b2_dynamicBody;
	Wood_body.position.Set(Postion.x, Postion.y);
	Wood_body.fixedRotation = false;

	b2Body* m_Wood_body = world->CreateBody(&Wood_body);

	SetObjectWoodBody(m_Wood_body);



	b2PolygonShape Wood_shape;
	Wood_shape.SetAsBox(wood_size.x * 0.5, wood_size.y * 0.5);

	b2FixtureDef wood_fixture;

	wood_fixture.shape = &Wood_shape;
	wood_fixture.density = 3.0f;
	wood_fixture.friction = 0.5f;//���C
	wood_fixture.restitution = 0.0f;//�����W��
	wood_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* object_wood_fixture = m_Wood_body->CreateFixture(&wood_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_wood_data = new ObjectData{ collider_wall };//��U�ǔ���
	object_wood_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_wood_data);

	//---------------------------------------------------------------------------//
	//2�߂̃{�f�B�@�؂̏�̃A���J�[�|�C���g������

	//�T�C�Y�̕␳�������
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = AnchorPoint_size.x / BOX2D_SCALE_MANAGEMENT;
	anchorpoint_size.y = AnchorPoint_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//�؂̊��̕���
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		Postion.x,
		Postion.y + (wood_size.y / 2) + (anchorpoint_size.y / 2));
	anchorpoint_body.fixedRotation = false;

	b2Body* m_AnchorPoint_body = world->CreateBody(&anchorpoint_body);

	SetObjectAnchorPointBody(m_AnchorPoint_body);



	b2PolygonShape anchorpoint_shape;
	anchorpoint_shape.SetAsBox(anchorpoint_size.x * 0.5, anchorpoint_size.y * 0.5);

	b2FixtureDef anchorpoint_fixture;

	anchorpoint_fixture.shape = &anchorpoint_shape;
	anchorpoint_fixture.density = 1.0f;
	anchorpoint_fixture.friction = 0.05f;//���C
	anchorpoint_fixture.restitution = 0.0f;//�����W��
	anchorpoint_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_anchorpoint_data->object_name = Object_Wood;


	int ID=object_anchorpoint_data->GenerateID();
	object_anchorpoint_data->id = ID;
	SetID(ID);

	//�؂�|�������ɕK�v�ɂȂ�Force �Ƃ肠�����T�C�Y�Ɉˑ��ł���
	b2Vec2 need_power;

	need_power.x = ((GetWoodSize().x * GetWoodSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1;//�P�͕K�v�ɉ����ĕύX����
	need_power.y = 10.0f;//�c�ɕK�v�ȗ͂͂Ȃ�
	

	object_anchorpoint_data->add_force = need_power;
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//�W���C���g����

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Wood_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -wood_size.y * 0.5f); // �؂̏�[
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // �A���J�[�|�C���g�̉��[
	jointDef.collideConnected = false;					  //�W���C���g�������̓��m�̐ڐG������

	world->CreateJoint(&jointDef);						  //���[���h�ɃW���C���g��ǉ�

	//-------------------------------------------------------------------------------------------
	//�؂�|���ׂɕK�v�ȋ���

};

wood::~wood()
{
}


void wood::Initialize()
{
	//�A���J�[�̕d�̕����i���{��j
	g_Wood_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_wood.png");
	g_Wood_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
	g_Wood_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

}

void wood::Update()
{


}

void wood::Pulling_wood(b2Vec2 pulling_power)
{
	b2Body*body=GetObjectAnchorPointBody();
	//�v���C���[���ɓ|��
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x)//�v���C���[������
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);
}

void wood::Draw()
{

	///�������璲�����Ă�




	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	b2Vec2 Wood_pos = GetObjectWoodBody()->GetPosition();
	b2Vec2 AnchorPoint_pos = GetObjectAnchorPointBody()->GetPosition();


	b2Vec2 Wood_size = GetWoodSize();
	b2Vec2 AnchorPoint_size = GetAnchorPointSize();
	// �؂̒��S�����[�J�����W����v�Z
	float woodLocalCenterX =0.0f;
	float woodLocalCenterY = 0.0f;

	// �A���J�[�|�C���g�̒��S�����[�J�����W����v�Z
	float anchorLocalCenterX = 0.0f;
	float anchorLocalCenterY = 0.0f;

	// ��]�p�x�i���W�A���j
	float angle = GetObjectWoodBody()->GetAngle();

	// ��]�s���K�p���ă��[���h���W���v�Z
	b2Vec2 woodWorldCenter(
		Wood_pos.x + woodLocalCenterX * cos(angle) - woodLocalCenterY * sin(angle),
		Wood_pos.y + woodLocalCenterX * sin(angle) + woodLocalCenterY * cos(angle)
	);

	b2Vec2 anchorWorldCenter(
		AnchorPoint_pos.x + anchorLocalCenterX * cos(angle) - anchorLocalCenterY * sin(angle),
		AnchorPoint_pos.y + anchorLocalCenterX * sin(angle) + anchorLocalCenterY * cos(angle)
	);

	// �؂ƃA���J�[�|�C���g�̒��S�ʒu�����d���ςŌv�Z
	float totalHeight = Wood_size.y + AnchorPoint_size.y;
	float centerX = (woodWorldCenter.x * Wood_size.y + anchorWorldCenter.x * AnchorPoint_size.y) / totalHeight;
	float centerY = (woodWorldCenter.y * Wood_size.y + anchorWorldCenter.y * AnchorPoint_size.y) / totalHeight;

	// ���S�_
	b2Vec2 textureCenter(centerX, centerY);



	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((textureCenter.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((textureCenter.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Wood_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetWoodSize().x * scale,totalHeight * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);




}

void wood::Finalize()
{

}