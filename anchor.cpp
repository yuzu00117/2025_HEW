//-----------------------------------------------------------------------------------------------------
// #name anchor.cpp
// #description �A���J�[
// #make 2024/11/19�@�@�i��`��
// #update 2024/11/24
// #comment �ǉ��E�C���\��
//          �E�A���J�[�����ɂ��Ă͑��v���Ǝv���@������������Ȃ�ǉ�
//			�EChain�ɂ��Ă͕`��̖ʂŕύX���K�v ��̓I�ɂ�
//			  ���܃A���J�[���A���J�[�|�C���g�ɐڐG���Ă���ꊇ�Ń`�F�[���������ƍ폜����Ă���֌W�ŕ`��ʂ����������̂ł����̕ύX
// 
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"anchor.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player.h"
#include"anchor_point.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"contactlist.h"
#include"create_filter.h"
#include"anchor_spirit.h"

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev1 = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev2 = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev3 = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Anchor_Chain_Texture = NULL;//�A���J�[�̍��̃e�N�X�`��

//�A���J�[�̈�[�̃v���C���[�̃{�f�B�������Ƃ�
b2Body* Player_body;

//�A���J�[�̖ڕW�̃A���J�[�|�C���g
b2Body* Target_anchor_point_body;



//�A���J�[�̍��̕����̃{�f�B�̓��ꕨ
#define MAX_CHAIN_NUM (40)//�`�F�[���̍ő��
b2Body* anchor_chain[MAX_CHAIN_NUM];



//�A���J�[�N���X�̃C���X�^���X
Anchor*g_anchor_instance;


Anchor::Anchor()
{
	anchor_create_joint_flag = false;
}

Anchor::~Anchor()
{
	
}


void Anchor::Initialize()
{
	//�e�N�X�`���̏�����

	//�A���J�[�̕d�̕����i���{��j
	g_Anchor_Texture_Lev1 =InitTexture(L"asset\\texture\\anchor_point\\anchor_point_lev1.png");
	g_Anchor_Texture_Lev2 = InitTexture(L"asset\\texture\\anchor_point\\anchor_point_lev2.png");
	g_Anchor_Texture_Lev3 = InitTexture(L"asset\\texture\\anchor_point\\anchor_point_lev3.png");


	//�A���J�[�̍�
	g_Anchor_Chain_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_chain.png");
}

void Anchor::CreateAnchor(b2Vec2 anchor_size)
{
	if (g_anchor_instance == nullptr)
	{
		g_anchor_instance = new Anchor();//NULL��������A���J�[������ďグ��
	}
	g_anchor_instance->CreateAnchorBody(anchor_size);//�A���J�[�̃{�f�B������
}

void Anchor::DeleteAnchor()
{
	if (g_anchor_instance != nullptr) {
		g_anchor_instance->DestroyAnchorBody();
	
		//�z�񎩑̂��f���[�g
		delete g_anchor_instance;
		//NULL�����Ƃ�
		g_anchor_instance = nullptr;

	}
	
}



void Anchor::CreateAnchorBody(b2Vec2 anchor_size)
{
	//�A���J�[�̕d�̕���������Ă������Ⴄ
	b2Body* player_body = Player::GetOutSidePlayerBody();			//�v���C���[��Body�����擾
	b2Body* target_AP_body = AnchorPoint::GetTargetAnchorPointBody();//�^�[�Q�b�g�Ƃ����A���J�[�|�C���g�̃{�f�B�����擾


	// �v���C���[�ƃ^�[�Q�b�g�A���J�[�|�C���g�̌��݂̈ʒu���擾
	b2Vec2 player_position = player_body->GetPosition(); // �v���C���[�̈ʒu
	b2Vec2 target_position = target_AP_body->GetPosition(); // �^�[�Q�b�g�A���J�[�|�C���g�̈ʒu

	// �v���C���[����^�[�Q�b�g�A���J�[�|�C���g�ւ̕����x�N�g�����v�Z
	b2Vec2 direction = target_position - player_position;

	// �x�N�g���̐��K���i�����݂̂�ێ��j
	float length = direction.Length();
	if (length > 0.0f) {
		direction *= (1.0f / length); // ���K��
	}

	// ���ł��������ɃI�t�Z�b�g��K�p
	float offset_distance = 0.5f; // �d�𐶐����鋗���i�v���C���[����̋����j
	b2Vec2 anchor_position = player_position + b2Vec2(direction.x * offset_distance, direction.y * offset_distance);

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position=anchor_position;

	//������ꂽ�p�x�ɂ��킹�ăA���J�[�̊p�x�̐ݒ�̕K�v��������҂�

	b2Vec2 Temporary_angle;//�x�N�g���g���Ĕ��ł����p�x �ϐ����͈ꎞ�I�Ȃ��Ă���
	Temporary_angle.x = target_AP_body->GetPosition().x - player_body->GetPosition().x;//x�̍��W�̊Ǘ�
	Temporary_angle.y = target_AP_body->GetPosition().y - player_body->GetPosition().y;//y�̍��W�̊Ǘ�

	//���W�A���p���Z�o
	float anchor_angle = atan2(Temporary_angle.y, Temporary_angle.x);

	// ���W�A������x���֕ϊ�
	anchor_angle = anchor_angle * 180.0f / M_PI;

	// 270�x��␳ �摜������������������
	anchor_angle += 270.0f;

	// ���̊p�x�𐳂͈̔͂ɒ����i0���`360���j
	if (anchor_angle < 0) {
		anchor_angle += 360.0f;
	}

	//�x�������W�A���ɕϊ�����
	//Box2d�̃��W�A���ŊǗ����Ă�
	body.angle = anchor_angle * M_PI / 180.0f;


	body.fixedRotation = true;//��]���Œ�ɂ���
	body.userData.pointer = (uintptr_t)this;


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();



	m_body = world->CreateBody(&body);

	player_body = m_body;//�v���C���[�̃{�f�B���Z�b�g



	SetSize(anchor_size);//�v���C���[�\�������邽�߂ɃZ�b�g����



	b2Vec2 size;
	size.x = anchor_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = anchor_size.y / BOX2D_SCALE_MANAGEMENT;


	b2PolygonShape shape;
	shape.SetAsBox(size.x * 0.5, size.y * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.1f;//���x
	fixture.friction = 0.05f;//���C
	fixture.restitution = 0.1f;//�����W��
	fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����




	b2Fixture* anchor_fixture = m_body->CreateFixture(&fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	// �v���C���[�ɒl��o�^
	// �v���[���[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* anchordata = new ObjectData{ collider_anchor };
	anchor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(anchordata);


}

void Anchor::DestroyAnchorBody()
{
	if (m_body != nullptr) {
		if (m_body != nullptr) {
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			world->DestroyBody(m_body);
			m_body = nullptr;
			m_isAnchorCreated = false;
		}
	}
}

void Anchor::Update()
{

	
	if(Anchor::GetAnchorState()==Connected_state)//�������Ă����ԂɂȂ�����
	{
		g_anchor_instance->GetAnchorBody()->SetLinearVelocity(b2Vec2_zero);

		Anchor::CreateRotateJoint();

		Anchor::SetAnchorState(Pulling_state);
	}
}

void Anchor::Draw()
{

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	if (g_anchor_instance == nullptr)
	{
		return;
	}

	b2Body *anchor = g_anchor_instance->GetAnchorBody();

	if (anchor!= nullptr)
	{
		b2Vec2 position;
		position.x = anchor->GetPosition().x;
		position.y = anchor->GetPosition().y;

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		float anchor_size_scale = 0;

		switch (AnchorSpirit::GetAnchorLevel())
		{
		case 1:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Texture_Lev1);
			anchor_size_scale = 1;
			break;
		case 2:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Texture_Lev2);
			anchor_size_scale = 1.5;
			break;
		case 3:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Texture_Lev3);
			anchor_size_scale = 2;
			break;
		default:
			break;
		}

		

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			g_anchor_instance->GetAnchorBody()->GetAngle(),
			{ g_anchor_instance->GetSize().x*scale*anchor_size_scale,g_anchor_instance->GetSize().y*scale* anchor_size_scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
		);


		
	}

	DrawChain();//�`�F�[���̕`�揈��
	DrawNormalAttack();//�ʏ�U���̕`��
}


void Anchor::Finalize()
{
	//�A���J�[�̏I��������ǉ������

	 // �ÓI�t���O���Ԃ̃��Z�b�g
	anchor_create_joint_flag = false;
	now_anchor_state = Nonexistent_state;



	if(g_anchor_instance!=nullptr)
	g_anchor_instance->DestroyAnchorBody();//�A���J�[�̃{�f�B�����

	UnInitTexture(g_Anchor_Chain_Texture);//�`�F�[���̃e�N�X�`���̉��
	UnInitTexture(g_Anchor_Texture_Lev1);	  //�A���J�[�̃e�N�X�`���̉��
	UnInitTexture(g_Anchor_Texture_Lev2);	  //�A���J�[�̃e�N�X�`���̉��
	UnInitTexture(g_Anchor_Texture_Lev3);	  //�A���J�[�̃e�N�X�`���̉��
	
}





void Anchor::ThrowAnchorToAP()
{
	if (g_anchor_instance == nullptr)
	{
		return;//NULL�`�F�b�N
	}

	//���̃A���J�[��������W�̎擾
	b2Vec2 anchor_pos=g_anchor_instance->GetAnchorBody()->GetPosition();

	//�ΏۂƂȂ�AP�̍��W���擾����
	b2Vec2 to_pos = AnchorPoint::GetTargetAnchorPointBody()->GetPosition();

	b2Vec2 velocity = to_pos - anchor_pos;
	velocity.Normalize(); // �P�ʃx�N�g�������ĕ���������
	velocity *= 20; // �������x��ݒ�	

	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(velocity);//�����ŗ͂������Ă�

}
	
void Anchor::CreateRotateJoint()
{
	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(b2Vec2_zero);//�Ƃ�ł����A���J�[�̃x���V�e�B���[���ɂ���
	MyContactListener& contact_listener = MyContactListener::GetInstance();

	if (g_anchor_instance == nullptr || g_anchor_instance->GetAnchorBody() == nullptr) {
		return; // �A���J�[�����݂��Ȃ��ꍇ�͉������Ȃ�
	}

	b2Body* anchorBody = g_anchor_instance->GetAnchorBody();
	b2Body* targetBody = AnchorPoint::GetTargetAnchorPointBody();

	if (anchorBody == nullptr || targetBody == nullptr) {
		return; // �^�[�Q�b�g�����݂��Ȃ��ꍇ�͉������Ȃ�
	}

	// ��]�W���C���g���`
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = anchorBody;
	jointDef.bodyB = targetBody;

	// �W���C���g�̃A���J�[�_��ݒ� (��: �A���J�[�̈ʒu�ɍ��킹��)
	b2Vec2 localAnchorA = anchorBody->GetLocalPoint(contact_listener.contactPoint);
	b2Vec2 localAnchorB = targetBody->GetLocalPoint(contact_listener.contactPoint);

	jointDef.collideConnected = true; // �W���C���g�Őڑ����ꂽ�{�f�B�Ԃ̏Փ˂𖳌���

	// �W���C���g�𐶐�
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->CreateJoint(&jointDef);
}

/**
 * @brief �W���C���g���폜����
 * @param 
 */
void Anchor::DeleteRotateJoint(void)
{
	if (g_anchor_instance == nullptr || g_anchor_instance->GetAnchorBody() == nullptr) {
		return; // �A���J�[�����݂��Ȃ��ꍇ�͉������Ȃ�
	}

	b2Body* anchorBody = g_anchor_instance->GetAnchorBody();

	// ���ׂẴW���C���g�𒲂ׂ�
	for (b2JointEdge* jointEdge = anchorBody->GetJointList(); jointEdge != nullptr; jointEdge = jointEdge->next) {
		b2Joint* joint = jointEdge->joint;

		// �W���C���g����]�W���C���g���m�F
		if (joint->GetType() == e_revoluteJoint) {
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			// �W���C���g���폜
			world->DestroyJoint(joint);
			break; // 1�폜�����烋�[�v�I��
		}
	}
}



void Anchor::PullingAnchor(void)
{
	//�v���C���[�ƃA���J�[�̍��W���擾����

	b2Vec2 player_postion = PlayerPosition::GetPlayerPosition();
	b2Vec2 anchor_postion = g_anchor_instance->GetAnchorBody()->GetPosition();

	b2Vec2 velocity = player_postion - anchor_postion;
	velocity.Normalize(); // �P�ʃx�N�g�������ĕ���������
	velocity *= 6; // �������x��ݒ�	

	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(velocity);

}



// �ÓI�����o�ϐ��̒�`
bool Anchor::anchor_create_joint_flag = false;

void Anchor::SetAnchorCreateJointFlag(bool flag) {
	anchor_create_joint_flag = flag;
}

bool Anchor::GetAnchorCreateJointFlag() {
	return anchor_create_joint_flag;
}


//�ÓI�����o�̕ϐ��̒�`
AnchorState Anchor::now_anchor_state = Nonexistent_state;

void Anchor::SetAnchorState(AnchorState state)
{
	now_anchor_state = state;
}

AnchorState Anchor::GetAnchorState()
{
	return now_anchor_state;
}



void Anchor::DrawChain() 
{


	// �X�P�[���ݒ�
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu
	b2Vec2 screen_center(SCREEN_CENTER_X, SCREEN_CENTER_Y);

	// �v���C���[�ƃA���J�[�̈ʒu���擾
	b2Body* anchor = g_anchor_instance->GetAnchorBody();

	if (anchor == nullptr)
	{
		return;
	}

	b2Vec2 anchor_position = anchor->GetPosition();
	b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

	

	

	// �������v�Z
	float distance = b2Distance(anchor_position, player_position);

	// �`�F�[���̕`��Ԋu�ƃ`�F�[����
	const float chain_interval = 0.2f; // �`�F�[���Ԋu�iBox2D�X�P�[���Łj
	int chain_count = static_cast<int>(distance / chain_interval);

	// �`�F�[���̊p�x���v�Z�i�v���C���[����A���J�[�ւ̊p�x�j
	float angle = atan2(anchor_position.y - player_position.y, anchor_position.x - player_position.x);

	// �`�F�[���T�C�Y�ݒ� (X����������)
	b2Vec2 chain_size(0.2f, 0.05); // X�������`�F�[��

	// �`�F�[���`��
	for (int i = 1; i <= chain_count; ++i)
	{
		// ���`��Ԃňʒu���v�Z
		float t = static_cast<float>(i) / chain_count; // 0.0�`1.0�͈̔�
		b2Vec2 interpolated_position = player_position + t * (anchor_position - player_position);

		// �X�N���[�����W�ɕϊ�
		float draw_x = ((interpolated_position.x - player_position.x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((interpolated_position.y - player_position.y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		// �`�F�[���`��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture); // �`�F�[���p�e�N�X�`��
		DrawSprite(
			{ draw_x, draw_y },
			angle, // �v���C���[�ƃA���J�[�̊p�x
			{ chain_size.x * BOX2D_SCALE_MANAGEMENT * scale, chain_size.y * BOX2D_SCALE_MANAGEMENT * scale }
		);
	}



}


void Anchor::CreateNormalAttack(b2Vec2 anchor_size, bool right)
{

	if (g_anchor_instance == nullptr)
	{
		g_anchor_instance = new Anchor();//NULL��������A���J�[������ďグ��
	}
	g_anchor_instance->CreateNormalAttackAnchorBody(anchor_size, right);	
}
	
void Anchor::DeleteNormalAttackAnchor()
{
	g_anchor_instance->DeleteNormalAttackAnchorBody();
}


void Anchor::CreateNormalAttackAnchorBody(b2Vec2 size,bool right)
{
	//�A���J�[�̕d�̕���������Ă������Ⴄ
	b2Body* player_body = Player::GetOutSidePlayerBody();			//�v���C���[��Body�����擾

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//�T�C�Y�̕␳������
	b2Vec2 anchor_size;
	anchor_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	anchor_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;

	body.type = b2_dynamicBody;


	if (right)//�E���ǂ���
	{
		//�v���C���[�̃T�C�Y�̏������Ă��ĂȂ�����@�v���C���[�̃T�C�Y�ɕύX�������Ƃ����邢
		body.position.Set(player_body->GetPosition().x + (1/2 / BOX2D_SCALE_MANAGEMENT) + (anchor_size.x / 2), player_body->GetPosition().y);//�v���C���[�̉E���ɐ���
	}
	else
	{
		body.position.Set(player_body->GetPosition().x - (1/2 / BOX2D_SCALE_MANAGEMENT) - (anchor_size.x / 2), player_body->GetPosition().y);//�v���C���[�̍����ɐ���
	}
	body.fixedRotation = false;//��]����

	b2Body* m_body = world->CreateBody(&body);

	m_normal_attack_body = m_body;



	////--------------------------------------------------------------------------------------------------
	//�ʏ�U���̃t�B�N�X�`��
	b2FixtureDef fixture;

	// �N���X���� b2Shape �������o�[�Ƃ��ĕێ�����ꍇ�̗�
	b2PolygonShape shape; // �N���X�̃����o�[�ϐ��Ƃ��ĕێ�
	shape.SetAsBox(anchor_size.x * 0.5, anchor_size.y * 0.5);

	fixture.shape = &shape;//�`��ݒ�
	fixture.density = 0.1f;//���x
	fixture.friction = 0.0f;//���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = true;//�Z���T�[���ǂ���


	b2Fixture* m_anchorpoint_fixture = m_body->CreateFixture(&fixture);


	ObjectData* object_anchorpoint_data = new ObjectData{ collider_normal_attack_anchor };
	m_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	

	//�v���C���[�ƃW���C���g����
	b2WeldJointDef jointDef;
	jointDef.bodyA = Player::GetOutSidePlayerBody();//�v���C���[�̃{�f�B
	jointDef.bodyB = g_anchor_instance->GetNormalAttackAnchorBody();//�ʏ�U���̃A���J�[�̃{�f�B

	if (right)//�E���ǂ���
	{
		//�v���C���[��
		jointDef.localAnchorA.Set(((1 / BOX2D_SCALE_MANAGEMENT) * 0.5), 0.0f);
		//�ʏ�U����
		jointDef.localAnchorB.Set((-anchor_size.x * 0.5), 0.0f);
	}
	else//����
	{
		//�v���C���[��
		jointDef.localAnchorA.Set(((-1/ BOX2D_SCALE_MANAGEMENT) * 0.5), 0.0f);
		//�ʏ�U����
		jointDef.localAnchorB.Set((anchor_size.x * 0.5), 0.0f);
	}
	jointDef.collideConnected = false;//�W���C���g�������̓��m�̐ڐG������

	world->CreateJoint(&jointDef); //���[���h�ɃW���C���g��ǉ�
}

void Anchor::UpdateNormalAttack()
{

}

void Anchor::DrawNormalAttack()
{
	//// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	//float scale = SCREEN_SCALE;

	//// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	//b2Vec2 screen_center;
	//screen_center.x = SCREEN_CENTER_X;
	//screen_center.y = SCREEN_CENTER_Y;


	//if (g_anchor_instance == nullptr)
	//{
	//	return;
	//}

	//b2Body* anchor = g_anchor_instance->GetNormalAttackAnchorBody();

	//if (anchor != nullptr)
	//{
	//	b2Vec2 position;
	//	position.x = anchor->GetPosition().x;
	//	position.y = anchor->GetPosition().y;

	//	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	//	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	//	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture);

	//	//draw
	//	DrawSprite(
	//		{ draw_x,
	//		  draw_y },
	//		0.0	,
	//		{ 2 * scale, 2 * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	//	);

	//}
}

void Anchor::DeleteNormalAttackAnchorBody()
{

	// ���[���h�̃C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// �A���J�[�̃W���C���g���폜
	if (m_normal_attack_body != nullptr)
	{
		// �A���J�[�Ɋ֘A�t����ꂽ���ׂẴW���C���g���擾���폜
		for (b2JointEdge* jointEdge = m_normal_attack_body->GetJointList(); jointEdge != nullptr; )
		{
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next; // ���̃W���C���g�G�b�W��ۑ�

			// �W���C���g�����[���h����폜
			world->DestroyJoint(joint);
		}

		// �A���J�[�̃{�f�B���폜
		world->DestroyBody(m_normal_attack_body);
		m_normal_attack_body = nullptr;
	}

}