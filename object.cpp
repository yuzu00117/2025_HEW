#include"object.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"field.h"
#include"collider_type.h"
#include"player_position.h"
#include"texture.h"
#include"sprite.h"
#include"object_map_type.h"


//�e�N�X�`���̓��ꕨ
static ID3D11ShaderResourceView* g_Object_Texture = NULL;//�n�ʂ̃e�N�X�`��

static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//�A���J�[�|�C���g�̃e�N�X�`��


Object *objects[MAX_OBJECT_NUMBER];



Object::Object(b2Vec2 position, b2Vec2 body_size,bool is_sensor,object_map_type type)
{

	SetSize(body_size);//�T�C�Y��������

	


	//�I�u�W�F�N�g�}�b�v�̓��ꕨ�ɃZ�b�g����
	for (int y = 0; y < OBJECT_MAX_HEIGHT; y++)
	{
		for (int x = 0; x < OBJECT_MAX_WIDTH; x++)
		{
			switch (type)
			{
			case type_wood:
				object_map[y][x] = object_map_name_Wood[y][x];
				break;
			case type_Rock:
				object_map[y][x] = object_map_name_Rock[y][x];
				break;
			default:
				break;
			}
		}
	}
	

	
	for (int y = 0;y< OBJECT_MAX_HEIGHT; y++)
	{
		for (int x = 0; x < OBJECT_MAX_WIDTH; x++)
		{
			if (object_map[y][x] != 0)//�{�f�B�����݂��邩�̃`�F�b�N
			{
				//���݂���Ȃ�{�f�B�����
				b2BodyDef body;
				body.type = b2_dynamicBody;


				//�^�񒆂𒆐S�Ƃ��������ߕ␳����
				body.position.Set(
					(position.x-4/BOX2D_SCALE_MANAGEMENT)+x/BOX2D_SCALE_MANAGEMENT,
					(position.y-4/BOX2D_SCALE_MANAGEMENT)+y/BOX2D_SCALE_MANAGEMENT
				);
				body.angle = 0.0;
				body.fixedRotation = false;//��]���Œ�ɂ���
				body.userData.pointer = (uintptr_t)this;

				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();

				b2Body *m_body=world->CreateBody(&body);

				SetObjectBody(m_body, x, y);//body�̏���o�^


				b2Vec2 size;
				size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
				size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


				b2PolygonShape shape;
				shape.SetAsBox(size.x * 0.5, size.y * 0.5f);

				b2FixtureDef fixture;
				fixture.shape = &shape;
				fixture.density = 10.0f;//���x
				fixture.friction = 0.05f;//���C
				fixture.restitution = 0.0f;//�����W��
				fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

				b2Fixture* object_fixture = m_body->CreateFixture(&fixture);


			
				ObjectData* objectdata;

				 switch (object_map[y][x])
				 {
				 case 1://�A���J�[�|�C���g
					 objectdata= new ObjectData{collider_anchor_point};//�����̃I�u�W�F�N�g�f�[�^��z��ɂ��Ă��邩���r���ł��Ă��Ȃ��@���������̍l����
					 object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectdata);

					 //�e�N�X�`�����Z�b�g
					 SetObjectTexture(anchorpoint_texture, x, y);
					 break;
				 case 2://�A���J�[�|�C���g����Ȃ����ǃI�u�W�F�N�g�ł͂�����
					 objectdata = new ObjectData{collider_object};
					 object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectdata);

					 //�e�N�X�`�����Z�b�g
					 SetObjectTexture(sample_blue, x, y);
					 break;

				 case 3:
					 break;

				 default:
					 break;
				 }


				 //-----------------------------------------------------------------------------------
				 //�n�ڃW���C���g���쐬

				 if (0 <= x - 1)//�I�u�W�F�N�g�̃}�b�v��̈�ԍ����ƌv�Z���Ȃ�
				 {
					 if (object_map[y][x - 1] != 0)//���쐬�����{�f�B�̍��ɃI�u�W�F�N�g�����邩�ǂ����̊m�F
					 {
						 //�I�u�W�F�N�g���������n�ڃW���C���g����
						 b2WeldJointDef jointDef;
						 jointDef.bodyA = GetObjectBody(x, y);
						 jointDef.bodyB = GetObjectBody(x - 1, y);
						 jointDef.localAnchorA.Set(-size.x*0.5, 0.0f);
						 jointDef.localAnchorB.Set(size.x*0.5, 0.0f);
						 jointDef.collideConnected = false;					  //�W���C���g�������̓��m�̐ڐG������

						 world->CreateJoint(&jointDef);						  //���[���h�ɃW���C���g��ǉ�
					 }
				 }

				 if (0 <= y - 1)
				 {
					 if (object_map[y - 1][x] != 0)
					 {
						 //�I�u�W�F�N�g���������n�ڃW���C���g����
						 b2WeldJointDef jointDef;
						 jointDef.bodyA = GetObjectBody(x, y);
						 jointDef.bodyB = GetObjectBody(x, y-1);
						 jointDef.localAnchorA.Set(0.0f, -size.y*0.5);
						 jointDef.localAnchorB.Set(0.0f, size.y*0.5);
						 jointDef.collideConnected = false;					  //�W���C���g�������̓��m�̐ڐG������

						 world->CreateJoint(&jointDef);
					 }
				 }
				 
			}
		}

	}

}

Object::~Object()
{

}


void Object::Initialize()
{
	//�摜�̃C���N���[�h
	g_Object_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//�O�����h�̃e�N�X�`��
	g_AnchorPoint_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//�A���J�[�|�C���g�̃e�N�X�`��



}

void Object::SetObject(int x, int y,b2Vec2 body_size, bool is_sensor, object_map_type type)
{
	for (int i = 0; i < MAX_OBJECT_NUMBER; i++)
	{
		if (objects[i] == nullptr)//�l��null��������@�����
		{
			objects[i]=new Object(b2Vec2( x/ BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), false, type);
			
			break;
		}
	}
}

void Object::Update()
{

}

void Object::Draw()
{
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	for (int i = 0; i < MAX_OBJECT_NUMBER; i++)//�I�u�W�F�N�g�ő吔������
	{
		if (objects[i] != nullptr) {//�I�u�W�F�N�g�����݂��邩�̊m�F
			for (int y = 0; y < OBJECT_MAX_HEIGHT; ++y) {
				for (int x = 0; x < OBJECT_MAX_WIDTH; ++x) {
					if (objects[i]->object_map[y][x]!=0) {
						b2Vec2 position;
						position.x = objects[i]->GetObjectBody(x,y)->GetPosition().x;
						position.y = objects[i]->GetObjectBody(x,y)->GetPosition().y;

						// �v���C���[�ʒu���l�����ăX�N���[���␳��������
						//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
						float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
						float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



						//�\��e�N�X�`�����w��
						switch (objects[i]->GetObjectTexture(x,y))
						{
						case anchorpoint_texture:
							GetDeviceContext()->PSSetShaderResources(0, 1, &g_AnchorPoint_Texture);
							break;
						case sample_blue:
							GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);
							break;
						default:
							break;
						}



						//draw
						DrawSprite(
							{ draw_x,
							  draw_y },
							objects[i]->GetObjectBody(x,y)->GetAngle(),
							{ objects[i]->GetSize().x * scale , objects[i]->GetSize().y * scale}
						);
					}
				}
			}
		}
	}
}

void Object::Finalize()
{

}