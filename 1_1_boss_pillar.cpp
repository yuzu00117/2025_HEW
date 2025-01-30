//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_pillar.cpp
// #description �{�X��ŗ��p���钌�̃I�u�W�F�N�g
// #make 2025/01/25		�i��`��
// #update 2025/01/25
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------



#include"1_1_boss_pillar.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//�t�B�[���h�̃e�N�X�`��

boss_pillar::boss_pillar(b2Vec2 position, b2Vec2 size, int splitting_x,int splitting_y,Boss_Room_Level level)
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
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//�`�̒�`
	b2PolygonShape shape;
	shape.SetAsBox(body_size.x * 0.5, body_size.y * 0.5);


	//-----------------------------------------------------
	//	fixture�����
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.01f;
	fixture.restitution = 0.0f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = Boss_pillar;
	SetID(ID);


	//�T�C�Y�̕␳�������
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = body_size.x;
	anchorpoint_size.y =0.5/ BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//�؂̊��̕���
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		position.x,
		position.y + (body_size.y / 2) + (anchorpoint_size.y / 2));
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
	anchorpoint_fixture.filter = createFilterExclude("object_filter", {"Boss_filter","MiniGolem_filter","Shockwave_filter", "object_filter" });

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	
	
	object_anchorpoint_data->id = ID;
	

	object_anchorpoint_data->object_name = Boss_pillar;
	object_anchorpoint_data->need_anchor_level = 1;

	b2Vec2 need_power;

	need_power.x = (GetSize().x * GetSize().y);//�P�͕K�v�ɉ����ĕύX����
	need_power.y = 10.0f;//�c�ɕK�v�ȗ͂͂Ȃ�


	object_anchorpoint_data->add_force = need_power;

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -body_size.y * 0.5f); // �؂̏�[
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // �A���J�[�|�C���g�̉��[
	jointDef.collideConnected = true;					  //�W���C���g�������̓��m�̐ڐG������

	world->CreateJoint(&jointDef);						  //���[���h�ɃW���C���g��ǉ�

	//����������ۑ�
	Splitting_x = splitting_x;
	Splitting_y = splitting_y;


	boss_room_level = level;

	//�{�f�B�̗̈�����O�Ɋm�ۂ��Ă���
	boss_pillar_body_Splitting.reserve(Splitting_x* Splitting_y);



	
	isUse = true;
}

boss_pillar::~boss_pillar()
{
}



void boss_pillar::Initialize()
{
	if (g_Texture == NULL) {
		g_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_gyaser.png");//���̃e�N�X�`��
	}
}

void boss_pillar::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss_room_level < boss.GetBossFieldLevel() && (Splitting_end == false))
		{
			Splitting_Destroy_Flag = true;
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

		if (m_body != nullptr)
		{
			float angle = m_body->GetAngle();  // ���W�A���Ŏ擾
			float angleDeg = angle * 57.2958f; // �x���@�ɕϊ�

			// 85���ȏ�275���ȉ��𔻒�
			if (angleDeg >= 85.0f && angleDeg <= 275.0f)
			{
				//�p�x�������Ȃ������
				Splitting_Destroy_Flag = true;
			}
		}
	

		Destroy_Splitting();
	}
}

void boss_pillar::Destroy_Splitting()
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


			//�A���J�[�|�C���g�̃{�f�B������

			world->DestroyBody(GetObjectAnchorPointBody());

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

					fragment->SetLinearVelocity(b2Vec2(vec.x*2,vec.y*2));
					fragment->SetAngularVelocity(angle_vec);

					// ������̌`��ƃt�B�N�X�`����ݒ�
					b2PolygonShape fragmentShape;
					fragmentShape.SetAsBox(size.x / (2.0f * Splitting_x), size.y / (2.0f * Splitting_y));

					b2FixtureDef fragmentFixture;
					fragmentFixture.shape = &fragmentShape;
					fragmentFixture.density = 1.0f; // �{�f�B�̖��x��ݒ�B���x���傫���قǃ{�f�B�̎��ʂ��d���Ȃ�B
					fragmentFixture.friction = 0.5f; // ���C�W����ݒ�B�ڐG�ʂ̊���₷���𐧌䂵�A�������l�قǊ���₷���B
					fragmentFixture.restitution = 0.0f; // �����W����ݒ�B0�͔������Ȃ��i�Փˎ��ɃG�l���M�[�������j�A1�͊��S�ɒe�ށB
					fragmentFixture.filter = createFilterExclude("ground_filter", {"Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter" });

					b2Fixture*fixture=fragment->CreateFixture(&fragmentFixture);

					// �J�X�^���f�[�^���쐬���Đݒ�
					ObjectData* object_anchorpoint_data = new ObjectData{ collider_ground };
					fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

					
					// �����_���ȕ����ɔ�юU��悤�ɑ��x��ݒ�
					fragment->SetLinearVelocity(GetRandomVelocity(5.0f)); // 5.0f �͊���x�i�����j
				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}
	

		
		
	}

}


void boss_pillar::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
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

void boss_pillar::Pulling_pillar(b2Vec2 pulling_power)
{
	b2Body* body = GetObjectAnchorPointBody();
	//�v���C���[���ɓ|��
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x)//�v���C���[������
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);

}

void boss_pillar::Draw()
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

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale }
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

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


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

void boss_pillar::Finalize()
{
	UnInitTexture(g_Texture);
}