//-----------------------------------------------------------------------------------------------------
// #name ground.cpp
// #description �n�ʂ�����
// #make 2024/11/16
// #update 2024/12/03
// #comment �ǉ��E�C���\��
//          �EFiled�̃N���X���p�����č쐬�@
//			�E�t�B�N�X�`���𕡐��ݒ肵�ĕǂ̔���ƒn�ʂ̔����ݒ肵����
//           
//----------------------------------------------------------------------------------------------------
#include"ground.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"contactlist.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"


/**
 * @brief  �t�B�[���h�̃R���X�g���N�^
 * @param position  �o�^������W
 * @param body_size �{�f�B�̑傫���A�֐����ő傫���̒���������
 * @param angle     �p�x��ݒ�
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, ID3D11ShaderResourceView* texture, bool object_sensor)
{
	//�e�N�X�`�����Z�b�g
	g_texture=texture;


	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;	//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	m_body = world->CreateBody(&body);

	SetBody(m_body);//Body�����[���h�ɌŒ�


	SetSize(body_size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��



	//�ӂ���������[
	//-----------------------------------------------------------------------
	b2Vec2 upper_size;
	upper_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	upper_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;
	shape.SetAsBox(upper_size.x * 0.5, upper_size.y * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;//���x
	fixture.friction = 0.3f;//���C
	fixture.restitution = 0.1f;//�����W��
	fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	if (object_sensor) {
		fixture.filter = createFilterExclude("one-way_platform_filter", { "object_filter" });
	}
	else
	{
		fixture.filter = createFilterExclude("ground_filter", {});
	}
	b2Fixture* ground_fixture = GetBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

	


	
	// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_objectData = std::make_unique<ObjectData>(collider_ground);
	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());


}
Ground::~Ground()
{
	Finalize();
}

void Ground::Finalize() {
	if (!m_body) return;

	for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		if (!fixture) continue;

		// UserData �擾
	

		// �����ȃ|�C���^�Ȃ�X�L�b�v
		if (!fixture->GetUserData().pointer) {
			continue;
		}

		


		// ObjectData ���폜��
		fixture->GetUserData().pointer = 0;  // �|�C���^�̃N���A
	}

	// `b2Body` ���폜
	Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
	m_body = nullptr;
}


void Ground::Initialize()
{

}

void Ground::Update()
{

}

void Ground::Draw()
{
	if (m_body != nullptr)
	{
	

		// �R���C�_�[�ƈʒu���̕␳�����邽��
		float scale = SCREEN_SCALE;

		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;


		// �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j
		b2Vec2 position;
		position.x = m_body->GetPosition().x;
		position.y = m_body->GetPosition().y;


		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_texture);
		//�`��
		DrawSprite(
			{ draw_x,
			  draw_y },
			m_body->GetAngle(),
			{ GetSize().x * scale ,GetSize().y * scale  },
			3
		);

		
	}
}

