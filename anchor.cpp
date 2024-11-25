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

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Anchor_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Anchor_Chain_Texture = NULL;//�A���J�[�̍��̃e�N�X�`��

//�A���J�[�̈�[�̃v���C���[�̃{�f�B�������Ƃ�
b2Body* Player_body;

//�A���J�[�̖ڕW�̃A���J�[�|�C���g
b2Body* Target_anchor_point_body;



//�A���J�[�̍��̕����̃{�f�B�̓��ꕨ
b2Body* anchor_chain[20];

//�A���J�[�N���X�̃C���X�^���X
Anchor*g_anchor_instance;


Anchor::Anchor()
{
	anchor_create_joint_flag = false;
}

Anchor::~Anchor()
{
	DestroyAnchorBody();
}


void Anchor::Initialize()
{
	//�e�N�X�`���̏�����

	//�A���J�[�̕d�̕����i���{��j
	g_Anchor_Texture=InitTexture(L"asset\\texture\\sample_texture\\img_anchor.png");

	//�A���J�[�̍�
	g_Anchor_Chain_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
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
void Anchor::ToggleAnchor()
{
	if (g_anchor_instance != nullptr && g_anchor_instance->GetAnchorBody() != nullptr) {
		DeleteAnchor();
	}
	else {
		CreateAnchor(b2Vec2(2.0f,2.0f));
	}
}


void Anchor::CreateAnchorBody(b2Vec2 anchor_size)
{
	//�A���J�[�̕d�̕���������Ă������Ⴄ
	b2Body* player_body = Player::GetOutSidePlayerBody();			//�v���C���[��Body�����擾
	b2Body* target_AP_body = AnchorPoint::GetTargetAnchorPointBody();//�^�[�Q�b�g�Ƃ����A���J�[�|�C���g�̃{�f�B�����擾


	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(player_body->GetPosition().x + 0.5, player_body->GetPosition().y - 0.5);
	body.angle = 0.0f;
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




	b2Fixture* player_fixture = m_body->CreateFixture(&fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	// �v���C���[�ɒl��o�^
	// �v���[���[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* playerdata = new ObjectData{ collider_anchor };
	player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);


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
		float draw_x = ((position.x - PlayerPostion::GetPlayerPostion().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPostion::GetPlayerPostion().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			g_anchor_instance->GetAnchorBody()->GetAngle(),
			{ g_anchor_instance->GetSize().x*scale,g_anchor_instance->GetSize().y*scale}///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
		);
	}
	
}


void Anchor::Finalize()
{

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

void Anchor::PullingAnchor(void)
{
	//�v���C���[�ƃA���J�[�̍��W���擾����

	b2Vec2 player_postion = PlayerPostion::GetPlayerPostion();
	b2Vec2 anchor_postion = g_anchor_instance->GetAnchorBody()->GetPosition();

	b2Vec2 velocity = player_postion - anchor_postion;
	velocity.Normalize(); // �P�ʃx�N�g�������ĕ���������
	velocity *= 3; // �������x��ݒ�	

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
