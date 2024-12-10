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
Ground::Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
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



	//��̖� (ground)
	//-----------------------------------------------------------------------
	b2Vec2 upper_size;
	upper_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	upper_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape_upper;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape_upper.SetAsBox(upper_size.x * 0.35f, upper_size.y * 0.49f);//�����蔻���o�^����4�_�@*0.5����̂�

	b2FixtureDef fixture_upper;
	fixture_upper.shape = &shape_upper;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture_upper.density = 1.0f;    //���x
	fixture_upper.friction = 0.5f;  //���C
	fixture_upper.restitution = 0.0f;//�����W��
	fixture_upper.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* ground_upper_fixture = GetFieldBody()->CreateFixture(&fixture_upper);//Body���Ƀt�B�N�X�`����o�^����

	// �J�X�^���f�[�^���쐬���Đݒ�
	// �n�ʂɒl��o�^
	// �n�ʂɃ��[�U�[�f�[�^��o�^
	ObjectData* ground_upper_data = new ObjectData{ collider_ground };
	ground_upper_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(ground_upper_data);




	//���E�̖� (wall)
	//------------------------------------------------------------------

	b2Vec2 side_size;
	side_size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	side_size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


	b2PolygonShape	shape_side;
	shape_side.SetAsBox(side_size.x * 0.5f, side_size.y * 0.45f);

	b2FixtureDef fixture_side;
	fixture_side.shape = &shape_side;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture_side.density = 0.0f;    //���x
	fixture_side.friction = 0.0f;  //���C
	fixture_side.restitution = 0.0f;//�����W��
	fixture_side.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* ground_side_fixture = GetFieldBody()->CreateFixture(&fixture_side);//Body���Ƀt�B�N�X�`����o�^����

	// �J�X�^���f�[�^���쐬���Đݒ�
	// �n�ʂɒl��o�^
	// �n�ʂɃ��[�U�[�f�[�^��o�^
	ObjectData* ground_side_data = new ObjectData{ collider_wall };
	ground_side_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(ground_side_data);
}

Ground::~Ground()
{
}