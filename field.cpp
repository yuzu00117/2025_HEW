//-----------------------------------------------------------------------------------------------------
// #name field.cpp
// #description �t�B�[���h
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �EField�̐ݒ�����Ă�  �Ăяo���̎d���Ƃ��Ă��X�R�[�v�������Z�q�g���Ă���� �iField::Draw)
//           
//----------------------------------------------------------------------------------------------------

#include"tool.h"
#include"include/box2d/box2d.h"
#include"field.h"
#include"player_position.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"


// 2�����z��̐ÓI�����o�̏�����
Field*** Field::m_p_field_array = nullptr;
int Field::m_field_height = 0;
int Field::m_field_width = 0;

//�O���[�o���ϐ��@�e�N�X�`���̓��ꕨ
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;



/**
 * @brief  �t�B�[���h�̃R���X�g���N�^
 * @param position  �o�^������W 
 * @param body_size �{�f�B�̑傫���A�֐����ő傫���̒���������
 * @param angle     �p�x��ݒ�
 * @param bFixed    
 * @param is_sensor 
 * @param texture 
 */
Field::Field(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor,FieldTexture texture)
{
	b2BodyDef body;
	body.type = bFixed?b2_staticBody:b2_dynamicBody;	//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x,position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	m_body = world->CreateBody(&body);//Body�����[���h�ɌŒ�


	SetSize(body_size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��

	

	b2Vec2 size;
	size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;

	

	b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f );//�����蔻���o�^����4�_�@*0.5����̂�
	
	b2FixtureDef fixture;
	fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture.density = 1.0f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	m_body->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

}

Field::~Field()
{
}

void Field::Initialize(int field_width, int field_height)
{
	//�e�N�X�`���̏�����
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

	//Draw�̕��Ŏg�����߃����o�ϐ����ɑ�����Ƃ�
	m_field_height = field_height;
	m_field_width  = field_width;

	//2�����z��̃������m��
	m_p_field_array = new Field * *[field_height]; // �c�����̔z����m��

	for (int y = 0; y < field_height; ++y) {
		m_p_field_array[y] = new Field * [field_width]; // �������̔z����e�s���ƂɊm��

		for (int x = 0; x < field_width; ++x) {
			m_p_field_array[y][x] = nullptr; // �e�v�f�� nullptr �ŏ�����
		}
	}


	//�}�b�v��o�^����񎟌��z��
	int field_map[20][90] =
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	};

	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (field_map[y][x] == 1) {
				//Size�� BOX2D_SCALE_MANAGEMENT�Ŋ����Ă�e���Ł@���W�̓o�^�ʒu������
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 2) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 3) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, false, true, ground_texture);
			}
			if (field_map[y][x] == 4) {
				m_p_field_array[y][x] = new Field(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
		}
	}
}



void Field::Update()
{

}

void Field::Draw()
{

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;



	for (int y = 0; y < m_field_height; ++y) {
		for (int x = 0; x < m_field_width; ++x) {
			if (m_p_field_array[y][x] != nullptr) {
				b2Vec2 position;
				position.x = m_p_field_array[y][x]->GetFieldBody()->GetPosition().x ;
				position.y = m_p_field_array[y][x]->GetFieldBody()->GetPosition().y ;

				// �v���C���[�ʒu���l�����ăX�N���[���␳��������
				//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
				float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		
				//�\��e�N�X�`�����w��
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
				
			
				//draw
				DrawSprite(
					{ draw_x,
					  draw_y },
					m_p_field_array[y][x]->GetFieldBody()->GetAngle(),
					{ m_p_field_array[y][x]->GetSize().x * scale , m_p_field_array[y][x]->GetSize().y * scale }
				);
			}
		}
	}
}

void Field::Finalize()
{
	// 2�����z��̃��������
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {

				delete m_p_field_array[y][x];
				m_p_field_array[y][x] = nullptr;
			}
		}
		delete[] m_p_field_array[y];
	}
	delete[] m_p_field_array;
	m_p_field_array = nullptr;
}