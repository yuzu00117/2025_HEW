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


/**
 * @brief  �t�B�[���h�̃R���X�g���N�^
 * @param position  �o�^������W
 * @param body_size �{�f�B�̑傫���A�֐����ő傫���̒���������
 * @param angle     �p�x��ݒ�
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture,bool object_sensor)
{
	//�e�N�X�`�����Z�b�g
	SetFieldTexture(texture);


	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;	//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	SetFieldBody(world->CreateBody(&body));//Body�����[���h�ɌŒ�


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
	b2Fixture* ground_fixture = GetFieldBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

	


	// �J�X�^���f�[�^���쐬���Đݒ�
	// �v���C���[�ɒl��o�^
	// �v���[���[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* playerdata = new ObjectData{ collider_ground };




	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
}

Ground::~Ground()
{
}