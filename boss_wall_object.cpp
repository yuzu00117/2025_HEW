//-----------------------------------------------------------------------------------------------------
// #name boss_wall_object.h
// #description �{�X��Ŏg���ǂ̃I�u�W�F�N�g
// #make 2025/02/13		�i��`��
// #update 2025/02/13
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------




#include"boss_wall_object.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"sound.h"
#include"anchor_spirit.h"
#include"1-1_boss.h"


Boss_Wall_Objcet::Boss_Wall_Objcet(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,ID3D11ShaderResourceView* g_Texture,bool left)
{
	SetSize(size);//�`��p�̃T�C�Y��ۑ�

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//�T�C�Y�𒲐�����
	b2Vec2 body_size;
	body_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	body_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//�{�f�B���쐬����
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;
	body.gravityScale=(0.0);

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//�`�̒�`
	b2CircleShape shape;
	shape.m_radius = body_size.y * 0.4; // ���a��K�؂ɐݒ�


	//-----------------------------------------------------
	//	fixture�����
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.5f;
	fixture.restitution = 0.2f;
	fixture.isSensor = true;
	fixture.filter = createFilterExclude("object_filter", {"Player_filter"});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = Boss_Wall;
	SetID(ID);

	Texture=g_Texture;
	isUse = true;

	left_flag =left;
}

Boss_Wall_Objcet::~Boss_Wall_Objcet()
{
}



void Boss_Wall_Objcet::Initialize()
{

}

void Boss_Wall_Objcet::Update()
{
	if (isUse)
	{
		
		if (m_body != nullptr)
		{


			if (AnchorSpirit::GetAnchorLevel() == 3)
			{
				//�A���J�[�|�C���g�����
				CreateAnchorPoint();
			}
			else
			{
				//�A���J�[�|�C���g���폜����
				DeleteAnchorPoint();
			}

			if (Splitting_end == true)
			{
				Destroy_Cnt++;
			}

			if (180 < Destroy_Cnt)//�����������Ɣj�󂳂��t���O
			{
				DestroySplittedBodies(boss_pillar_body_Splitting);
				isUse = false;
			}



			if (GetrPullingFlag() == true)
			{
				ChangeBody();
				SetPullingFlag(false);
			}

			if (move_flag == true)
			{
				
				WallPullling();
				pulling_cnt++;
			}

			if (120 < pulling_cnt)
			{
				Splitting_Destroy_Flag = true;
			}

			Destroy_Splitting();
		}
		
	}
}


void Boss_Wall_Objcet::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//�j��̃t���O���I���ɂȂ��Ă���
	{
		if (m_body != nullptr && Splitting_end == false)
		{
			//���[���h�̃C���X�^���X�������Ă���
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//�j�󂳂ꂽposition���擾


			//���ʂ̃{�f�B������
			b2Vec2 Destroy_position = m_body->GetPosition();
			float angle = m_body->GetAngle(); // ���̃{�f�B�̊p�x���擾
			b2Vec2 vec = m_body->GetLinearVelocity();
			float angle_vec = m_body->GetAngularVelocity();

			world->DestroyBody(m_body);
			m_body = nullptr;

			SetBody(nullptr);

			//�T�E���h���Đ�
			app_atomex_start(Object_Pillar_Break_Sound);

			//�A���J�[�|�C���g�̃{�f�B������

			if (GetObjectAnchorPointBody() != nullptr)
			{
				world->DestroyBody(GetObjectAnchorPointBody());
			}

			b2Vec2 size;
			size.x = m_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = m_size.y / BOX2D_SCALE_MANAGEMENT;



			// ������̃{�f�B��z�u
			for (int y = 0; y < Splitting_y; y++)
			{
				for (int x = 0; x < Splitting_x; x++)
				{
					// ������̃{�f�B�̃��[�J�����W���v�Z
					float localX = ((x - (Splitting_x - 1) / 2.0f) * size.x / Splitting_x);
					float localY = ((y - (Splitting_y - 1) / 2.0f) * size.y / Splitting_y);

					// ���̊p�x���l�����ă��[���h���W�ɕϊ�
					float rotatedX = localX * cos(angle) - localY * sin(angle);
					float rotatedY = localX * sin(angle) + localY * cos(angle);

					b2Vec2 fragmentPosition(
						Destroy_position.x + rotatedX,
						Destroy_position.y + rotatedY
					);

					// ������̃{�f�B���쐬
					b2BodyDef fragmentDef;
					fragmentDef.type = b2_dynamicBody;
					fragmentDef.position = fragmentPosition;
					fragmentDef.angle = angle; // ���̃{�f�B�̊p�x�������p��

					b2Body* fragment = world->CreateBody(&fragmentDef);
					boss_pillar_body_Splitting.push_back(fragment);

					fragment->SetLinearVelocity(b2Vec2(vec.x * 2, vec.y * 2));
					fragment->SetAngularVelocity(angle_vec);

					// ������̌`��ƃt�B�N�X�`����ݒ�
					b2PolygonShape fragmentShape;
					fragmentShape.SetAsBox(size.x / (2.0f * Splitting_x), size.y / (2.0f * Splitting_y));

					b2FixtureDef fragmentFixture;
					fragmentFixture.shape = &fragmentShape;
					fragmentFixture.density = 1.0f; // �{�f�B�̖��x��ݒ�B���x���傫���قǃ{�f�B�̎��ʂ��d���Ȃ�B
					fragmentFixture.friction = 0.5f; // ���C�W����ݒ�B�ڐG�ʂ̊���₷���𐧌䂵�A�������l�قǊ���₷���B
					fragmentFixture.restitution = 0.0f; // �����W����ݒ�B0�͔������Ȃ��i�Փˎ��ɃG�l���M�[�������j�A1�͊��S�ɒe�ށB
					fragmentFixture.filter = createFilterExclude("ground_filter", { "Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter","ground_filter" });

					b2Fixture* fixture = fragment->CreateFixture(&fragmentFixture);

					// �J�X�^���f�[�^���쐬���Đݒ�
					ObjectData* object_anchorpoint_data = new ObjectData{ collider_ground };
					fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


					// �����x�̓[���ɐݒ�i�K�v�ɉ����đ��x��ǉ��\�j
					fragment->SetLinearVelocity(b2Vec2(0.0f, 0.0f));


					// �����_���ȕ����ɔ�юU��悤�ɑ��x��ݒ�
					fragment->SetLinearVelocity(GetRandomVelocity(5.0f)); // 5.0f �͊���x�i�����j

				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}

	}

}

void Boss_Wall_Objcet::CreateAnchorPoint()
{

	if (GetObjectAnchorPointBody() == nullptr)
	{
		b2Vec2 size = b2Vec2(1.0f, 1.0f);

	

		b2Vec2 anchorpoint_size;

		anchorpoint_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
		anchorpoint_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

		float left = -1;
		if (left_flag == false)
		{
			left = 1;
		}

		b2Vec2 position = m_body->GetPosition();

		b2BodyDef anchor_point_body;
		anchor_point_body.type = b2_dynamicBody;																   // ���I�ȃI�u�W�F�N�g�ɂ���
		anchor_point_body.position.Set(GetBody()->GetPosition().x+(GetSize().x/BOX2D_SCALE_MANAGEMENT/2)*left, GetBody()->GetPosition().y); // �|�W�V������ݒ�
		anchor_point_body.angle = 0;																			   // �p�x�̏�����
		anchor_point_body.userData.pointer = (uintptr_t)this;													   // userData�̃|�C���^��ݒ�
		anchor_point_body.fixedRotation = true;																	   // ��]���Œ肷��

		Box2dWorld& box2d_world = Box2dWorld::GetInstance(); // ���[���h�̃C���X�^���X���擾����
		b2World* world = box2d_world.GetBox2dWorldPointer(); // ���[���h�̃|�C���^���擾����

		b2Body* m_anchor_point_body = world->CreateBody(&anchor_point_body); // Body�����[���h�ɒǉ�

		SetObjectAnchorPointBody(m_anchor_point_body);

		b2PolygonShape shape;												  // shape�ɂ͎l�p�`��ݒ肷��
		shape.SetAsBox(anchorpoint_size.x * 0.5f, anchorpoint_size.y * 0.5f); // �l�p�`��ݒ肷��

		b2FixtureDef fixture;
		fixture.shape = &shape;		// �V�F�C�v���t�B�N�X�`���ɐݒ肷��
		fixture.density = 1.0f;		// ���x
		fixture.friction = 0.00f;	// ���C
		fixture.restitution = 0.0f; // �����W��
		fixture.isSensor = false;	// �Z���T�[���ǂ���

		b2Fixture* ground_fixture = m_anchor_point_body->CreateFixture(&fixture); // Body�Ƀt�B�N�X�`����ݒ肷��

		// �J�X�^���f�[�^���쐬���Đݒ�
		// ���O�ɒl��ݒ�
		// ���O�Ƀ��[�U�[�f�[�^��ݒ�
		ObjectData* data = new ObjectData{ collider_anchor_point };
		ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

		data->need_anchor_level = 3;
		data->id = GetID();

		data->object_name = Boss_Wall;

		// �v���C���[�ƃW���C���g����
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;			   // �ǂ̖{��
		jointDef.bodyB = GetObjectAnchorPointBody(); // �{�X�̃A���J�[�|�C���g�̃{�f�B

		// �ǂ���
		jointDef.localAnchorA.Set(GetSize().x/BOX2D_SCALE_MANAGEMENT*0.5*left, 0.0f);
		// �A���J�[�|�C���g��
		jointDef.localAnchorB.Set(0.0f, 0.0f);

		jointDef.collideConnected = true; // �W���C���g���m�̏Փ˂𖳌��ɂ���

		world->CreateJoint(&jointDef); // ���[���h�ɃW���C���g��ǉ�
	}
}

void Boss_Wall_Objcet::DeleteAnchorPoint()
{
	// �A���J�[�|�C���g�{�f�B���폜
	if (GetObjectAnchorPointBody() != nullptr)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance(); // ���[���h�̃C���X�^���X���擾����
		b2World* world = box2d_world.GetBox2dWorldPointer(); // ���[���h�̃|�C���^���擾����

		world->DestroyBody(GetObjectAnchorPointBody());

		// null�ɐݒ�
		SetObjectAnchorPointBody(nullptr);
	}
}


void Boss_Wall_Objcet::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	for (b2Body*& body : bodyList) {
		if (body != nullptr) {
			world->DestroyBody(body);
			body = nullptr; // �|�C���^�𖳌���
		}
	}

}


void Boss_Wall_Objcet::ChangeBody()
{
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	m_body->SetType(b2_dynamicBody);



	move_flag = true;
}


void Boss_Wall_Objcet::WallPullling()
{
	if (m_body != nullptr)
	{
		int minus = 1;

		if (left_flag)
		{
			minus = -1;
		}
	
		m_body->SetLinearVelocity(b2Vec2(50.0f * minus, 0.0f));
		
	}
}

void Boss_Wall_Objcet::Draw()
{
	if (isUse)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;


		if (m_body != nullptr)
		{

			b2Vec2 Pos = GetBody()->GetPosition();


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				0.0f,
				{ (GetSize().x+6.f) * scale ,GetSize().y * scale*1.2f }
			);
		}


		//������̕`��
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_pillar_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_pillar_body_Splitting[i]->GetAngle(),
					{ GetSize().x / Splitting_x * scale,GetSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}

}

void Boss_Wall_Objcet::Finalize()
{
	if (Texture) {
		UnInitTexture(Texture);
	}
}