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
#include"create_filter.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//�؂̃e�N�X�`���P
static ID3D11ShaderResourceView* g_Wood_Texture1 = NULL;//�؂̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_Wood_Texture2 = NULL;//�؂̂̃e�N�X�`���R
static ID3D11ShaderResourceView* g_Stump_Texture = NULL;//�؂̐؂芔�̃e�N�X�`��


int ObjectData::current_id = 0;

wood::wood(b2Vec2 Position, b2Vec2 Wood_size, b2Vec2 AnchorPoint_size,int need_level)
{
	b2Vec2 Stump_size;
	Stump_size.x = Wood_size.x * 1.5f;
	Stump_size.y = Wood_size.y * 0.2f;

	SetWoodSize(Wood_size);
	SetStumpSize(Stump_size);
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
	Wood_body.position.Set(Position.x, Position.y - (Stump_size.y / BOX2D_SCALE_MANAGEMENT) * 0.5f);
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
	wood_fixture.filter = createFilterExclude("object_filter",{});


	b2Fixture* object_wood_fixture = m_Wood_body->CreateFixture(&wood_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_wood_data = new ObjectData{ collider_object };//��U�ǔ���
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
		Position.x,
		Position.y + (wood_size.y / 2) + (anchorpoint_size.y / 2));
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
	anchorpoint_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_wood_data->object_name = Object_Wood;
	object_anchorpoint_data->object_name = Object_Wood;


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
	//---------------------------------------------------------------------------//
	//3�߂̃{�f�B�@�؂̉��ɐ؂芔������

	//�T�C�Y�̕␳�������
	b2Vec2 stump_size;
	stump_size.x = Stump_size.x / BOX2D_SCALE_MANAGEMENT;
	stump_size.y = Stump_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef stump_body;//�؂̊��̕���
	stump_body.type = b2_dynamicBody;
	stump_body.position.Set(Position.x, Position.y + (wood_size.y * 0.5f) - (stump_size.y * 0.5f));
	stump_body.fixedRotation = false;

	auto m_stump_body = world->CreateBody(&stump_body);
	SetObjectStumpBody(m_stump_body);

	b2PolygonShape stump_shape;
	b2Vec2 vertices[4];
	vertices[0].Set(-stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[1].Set(stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[2].Set(-stump_size.x * 0.25f, stump_size.y * 0.2f);
	vertices[3].Set(stump_size.x * 0.25f, stump_size.y * 0.2f);
	stump_shape.Set(vertices, 4);	//�Z���T�[�̃��[�J���ʒu��ύX


	b2FixtureDef stump_fixture;

	stump_fixture.shape = &stump_shape;
	stump_fixture.density = 20.0f;
	stump_fixture.friction = 0.5f;//���C
	stump_fixture.restitution = 0.0f;//�����W��
	stump_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����


	b2Fixture* object_stump_fixture = GetObjectStumpBody()->CreateFixture(&stump_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_stump_data = new ObjectData{ collider_object };//��U�ǔ���
	object_stump_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_stump_data);

	object_stump_data->object_name = Object_Wood;




	//ObjecrData�́@ID�ݒ�@��@���̐ݒ�
	//------------------------------------------------------------------------------------------------------------------------------------------
	int ID=object_anchorpoint_data->GenerateID();
	object_wood_data->id = ID;
	object_anchorpoint_data->id = ID;
	SetID(ID);

	//�؂�|�������ɕK�v�ɂȂ�Force �Ƃ肠�����T�C�Y�Ɉˑ��ł���
	b2Vec2 need_power;

	need_power.x = ((GetWoodSize().x * GetWoodSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1;//�P�͕K�v�ɉ����ĕύX����
	need_power.y = 10.0f;//�c�ɕK�v�ȗ͂͂Ȃ�
	

	object_anchorpoint_data->add_force = need_power;


	//�A���J�[���x���̐ݒ�
	object_anchorpoint_data->need_anchor_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//�W���C���g����

	b2WeldJointDef jointDef2;
	jointDef2.bodyA = m_Wood_body;
	jointDef2.bodyB = m_stump_body;
	jointDef2.localAnchorA.Set(0.0f, wood_size.y * 0.5f + 0.15f); // �؂̉��[	(0.15�͔�����)
	jointDef2.localAnchorB.Set(0.0f, stump_size.y * 0.5f - 0.1f); // �؂芔�̏�[	(0.1�͔�����)
	jointDef2.collideConnected = false;					  //�W���C���g�������̓��m�̐ڐG������

	auto joint = world->CreateJoint(&jointDef2);						  //���[���h�ɃW���C���g��ǉ�
	SetWoodStumpJoint(joint);	//�؂������������炱��joint��������悤�ɕۑ����Ă���
	//-------------------------------------------------------------------------------------------
	//�؂�|���ׂɕK�v�ȋ���

};

wood::~wood()
{
}


void wood::Initialize()
{
	
	if (g_Wood_Texture == NULL) {
		g_Wood_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood.png");
		g_Wood_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
		g_Wood_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
		g_Stump_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood_stump.png");
	}
}

void wood::Update()
{
	//�؂芔�Ɩ{�̂̃W���C���g�������t���O���I���ɂȂ��Ă�ꍇ
	if (m_destory_joint)
	{
		b2Joint* joint = GetWoodStumpJoint();
		if (joint != nullptr)
		{
			//���[���h�̃C���X�^���X�������Ă���
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			world->DestroyJoint(joint);		//	���[���h����W���C���g������
			SetWoodStumpJoint(nullptr);		//	�������ێ����Ă�W���C���g�̏�������
		}
		m_destory_joint = false;	//�t���O���I�t�ɂ���
	}


	//list���󂶂�Ȃ��ꍇ�̂݉����o�����𔻒f
	if (!object_collided_when_falling.empty())
	{
		//���list�̒��g���������߂ɃC�e���[�^��ۑ����邽�߂�list
		std::list<std::list<ObjectCollided_WhenFalling*>::iterator> IteratorList;
		for (auto a : object_collided_when_falling)
		{
			a->count_down_to_play_sound--; //�J�E���g�_�E��
			//�J�E���g�_�E����0�̏ꍇ����炷
			if (a->count_down_to_play_sound == 0)
			{
				app_atomex_start(m_sound_FalledDown);	//���炷
				auto id = std::find(object_collided_when_falling.begin(), object_collided_when_falling.end(), a);	//���̃I�u�W�F�̃C�e���[�^���擾
				IteratorList.push_back(id);	//��ŏ������߂ɃC�e���[�^���X�g�ɓ���Ă���
			}
		}

		//�C�e���[�^���X�g���󂶂�Ȃ��ꍇ
		if (!IteratorList.empty())
		{
			for (auto a : IteratorList)
			{
				object_collided_when_falling.erase(a);	//�Ԃ������I�u�W�F�N�g�̃��X�g�ɑΉ��̃C�e���[�^������
			}
			IteratorList.clear();
			object_collided_when_falling.clear();
		}
	}
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
	SetIfPulling(true);
}



void wood::Add_CollidedObjectWhenFalling_List(b2Vec2 position)
{
	//����list����Ȃ�
	if (object_collided_when_falling.empty())
	{
		goto add;	//list�ɒǉ�
	}
	//list���󂶂�Ȃ�
	for (auto a : object_collided_when_falling)
	{
		//�������Ԃ������I�u�W�F�N�g���O�ɂԂ������I�u�W�F�N�g�̂����W���߂�����߂�
		if ((unsigned)(position.y - a->position.y) < 0.5f)
		{
			a->count_down_to_play_sound = 1;	//���o���J�E���g�_�E�������Z�b�g
			return;
		}
	}
	
	add:
		ObjectCollided_WhenFalling object;
		object.position = position;
		object.count_down_to_play_sound = 1;
		object_collided_when_falling.push_back(&object);

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
	
	//�؂芔��`��
//--------------------------------------------------------------------------------------------------
// �v���C���[�ʒu���l�����ăX�N���[���␳��������
//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������

	b2Vec2 Stump_pos = GetObjectStumpBody()->GetPosition();

	float draw_x = ((Stump_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Stump_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Stump_Texture);

	//�`��
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectStumpBody()->GetAngle(),
		{ GetStumpSize().x * scale,GetStumpSize().y * scale }
	);


	//�{�̂�`��
//--------------------------------------------------------------------------------------------------
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
	draw_x = ((textureCenter.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	draw_y = ((textureCenter.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


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

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetObjectWoodBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(Wood_body);
	}

	if (GetObjectAnchorPointBody() != nullptr)
	{
		world->DestroyBody(AnchorPoint_body);
	}

	//�e�N�X�`���̉��
	UnInitTexture(g_Wood_Texture);
	UnInitTexture(g_Wood_Texture1);
	UnInitTexture(g_Wood_Texture2);
}