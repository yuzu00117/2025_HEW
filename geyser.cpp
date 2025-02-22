//-----------------------------------------------------------------------------------------------------
// #name geyser.cpp
// #description �Ԍ���̃I�u�W�F�N�g��CPP
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//----------------------------------------------------------------------------------------------------

#include"geyser.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"
#include"player.h"
#include"easing.h"
#include"sound.h"
#include"Xinput_controller.h"



//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�



static ID3D11ShaderResourceView* g_Geyser_Water_Texture = NULL;//�Ԍ���̐�
static ID3D11ShaderResourceView* g_Geyser_Start_Water_Texture = NULL;//�Ԍ���̏��߂̕��݂̂�

//�Ԍ���̃e�N�X�`���@�A���J�[�̑łO
static ID3D11ShaderResourceView* g_Geyser_Close_Texture = NULL;
static ID3D11ShaderResourceView* g_Geyser_Border_Texture_Lv1 = NULL;
static ID3D11ShaderResourceView* g_Geyser_Border_Texture_Lv2 = NULL;
static ID3D11ShaderResourceView* g_Geyser_Border_Texture_Lv3 = NULL;

//�Ԍ���̃e�N�X�`���@�A���J�[�̑ł�������
static ID3D11ShaderResourceView* g_Geyser_Open_Texture = NULL;



geyser::geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize, int splitting_x, int splitting_y, int level)
{
	//�{�f�B�͈�Ł@�t�B�N�X�`�����t����\���ɂ���
	SetGeyserSize(GeyserSize);					//�Ԍ���̃T�C�Y���Z�b�g
	SetRangeFlyWaterSize(RangeFlyWaterSize);	//�Ԍ���̂������o�����̔�Ԕ͈�


	SetFlag(false);



	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������ �Ԍ���

	b2Vec2 geyser_size;

	geyser_size.x = GeyserSize.x / BOX2D_SCALE_MANAGEMENT;
	geyser_size.y = GeyserSize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef geyser_body;//�Ԍ���{�̂̃{�f�B
	geyser_body.type = b2_staticBody;
	geyser_body.position.Set(GeyserPosition.x, GeyserPosition.y);
	geyser_body.fixedRotation = true;//��]�͂��Ȃ�

	b2Body* m_geyser_body = world->CreateBody(&geyser_body);

	SetGeyserBody(m_geyser_body);



	//�Ԍ��򎩑̂̃t�B�N�X�`��
	b2PolygonShape geyser_shape;
	geyser_shape.SetAsBox(
		geyser_size.x * 0.5f,  // ���͂��̂܂�
		geyser_size.y * 0.1f, // �����𔼕���
		b2Vec2(0, geyser_size.y * 0.4f), // ���S���������ֈړ�
		0.0f // �p�x
	);

	b2FixtureDef geyser_fixture;
	geyser_fixture.shape = &geyser_shape;
	geyser_fixture.density = 3.0f;
	geyser_fixture.restitution = 0.0f;//�����W��
	geyser_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	geyser_fixture.filter = createFilterExclude("ground_filter", {});


	b2Fixture* object_geyser_fixture = m_geyser_body->CreateFixture(&geyser_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* geyser_object_data = new ObjectData{ collider_object };
	object_geyser_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(geyser_object_data);

	int ID = geyser_object_data->GenerateID();

	SetID(ID);
	//�����o�����̃t�B�N�X�`����t����
	geyser_object_data->id = GetID();

	//�Ԍ���̐��̃T�C�Y�̃X�P�[���̒���
	b2Vec2 range_fly_water_size;
	range_fly_water_size.x = RangeFlyWaterSize.x / BOX2D_SCALE_MANAGEMENT;
	range_fly_water_size.y = RangeFlyWaterSize.y / BOX2D_SCALE_MANAGEMENT;

	//�Ԍ���̐��̃t�B�N�X�`��
	b2PolygonShape range_fly_water_shape;

	b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

	vertices[0].Set(-range_fly_water_size.x / 2, 0 );//����
	vertices[1].Set(range_fly_water_size.x / 2, 0);//�E��
	vertices[2].Set(range_fly_water_size.x / 2, -geyser_size.y / 2 - range_fly_water_size.y);//����
	vertices[3].Set(-range_fly_water_size.x / 2, -geyser_size.y / 2 - range_fly_water_size.y);//�E��

	range_fly_water_shape.Set(vertices, 4); 

	b2FixtureDef range_fly_water_fixture;
	range_fly_water_fixture.shape = &range_fly_water_shape;
	range_fly_water_fixture.density = 3.0f;
	range_fly_water_fixture.restitution = 0.0f;//�����W��
	range_fly_water_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	range_fly_water_fixture.filter = createFilterExclude("object_filter", {});


	b2Fixture* object_range_fly_water_fixture = m_geyser_body->CreateFixture(&range_fly_water_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* range_fly_water_object_data = new ObjectData{ collider_geyser_water };//�ݒ���Ԍ���̐���
	object_range_fly_water_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(range_fly_water_object_data);

	range_fly_water_object_data->object_name = Object_Geyser_Water;
	range_fly_water_object_data->id = GetID();




	b2BodyDef body;//�A���J�[�|�C���g
	body.type = b2_staticBody;
	body.position.Set(GeyserPosition.x, GeyserPosition.y);
	body.fixedRotation = true;

	b2Body* m_anchor_point_body = world->CreateBody(&body);

	SetAnchorPointBody(m_anchor_point_body);


	//�t�B�N�X�`����t����
	b2PolygonShape anchor_point_block_shape;

	anchor_point_block_shape.SetAsBox(
		geyser_size.x * 0.5f,  // ���͂��̂܂�
		geyser_size.y * 0.25f, // �����𔼕���
		b2Vec2(0, geyser_size.y * 0.25f), // ���S���������ֈړ�
		0.0f // �p�x
	);

	b2FixtureDef anachor_point_fixture;

	anachor_point_fixture.shape = &anchor_point_block_shape;
	anachor_point_fixture.density = 3.0f;
	anachor_point_fixture.friction = 0.5f;//���C
	anachor_point_fixture.restitution = 0.0f;//�����W��
	anachor_point_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	anachor_point_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_anchor_point_fixture = m_anchor_point_body->CreateFixture(&anachor_point_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_anchor_point_data = new ObjectData{collider_anchor_point };//��U�ǔ���
	object_anchor_point_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchor_point_data);

	object_anchor_point_data->id = GetID();
	object_anchor_point_data->need_anchor_level = level;
	object_anchor_point_data->object_name = Object_Geyser;

	//�A���J�[���x���������o�ϐ��ŕێ�
	m_need_level = level;

	b2WeldJointDef jointDef;
	jointDef.bodyA = GetGeyserBody();
	jointDef.bodyB = GetAnchorPointBody();

	
	jointDef.localAnchorA.Set(0.0f, 0.0f);

	jointDef.localAnchorB.Set(0.0f, 0.0f);

	jointDef.collideConnected = true;//

	world->CreateJoint(&jointDef); 







	Splitting_x = splitting_x;
	Splitting_y = splitting_y;

	isUse = true;
};

geyser::~geyser()
{
}


void geyser::Initialize()
{
	//�Ԍ������̃e�N�X�`���I�I�I�i���{��j
	if (g_Geyser_Open_Texture == NULL)
	{
	
		g_Geyser_Open_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\Geyser_Open_Texture.png");
		g_Geyser_Close_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\Geyser_Close_Texture.png");
		g_Geyser_Border_Texture_Lv1 = InitTexture(L"asset\\texture\\stage_1_1_object\\Geyser_Close_Texture_border_lv1_TEST.png");
		g_Geyser_Border_Texture_Lv2 = InitTexture(L"asset\\texture\\stage_1_1_object\\Geyser_Close_Texture_border_lv2_TEST.png");
		g_Geyser_Border_Texture_Lv3 = InitTexture(L"asset\\texture\\stage_1_1_object\\Geyser_Close_Texture_border_lv3_TEST.png");
		g_Geyser_Start_Water_Texture= InitTexture(L"asset\\texture\\stage_1_1_object\\geyser_start_water.png");
		g_Geyser_Water_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\geyser_loop_water.png");
	}
	

}

void geyser::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss_room_level < boss.GetBossFieldLevel() && (Splitting_end == false))
		{
			Splitting_Destroy_Flag = true;
		}

		
		if (geyser_body != nullptr)
		{
			JumpPlayer();
		}
		


		if (Splitting_end == true)
		{
			Destroy_Cnt++;
		}

		if (180 < Destroy_Cnt)//�����������Ɣj�󂳂��t���O
		{
			DestroySplittedBodies(boss_geyser_body_Splitting);
			isUse = false;
		}

		/*Destroy_Splitting();*/
	}
}





void geyser::JumpPlayer()
{


	if (GetFlag() == true)
	{
		if (open_gyeser_flag == true)
		{
			//��ɏグ�鏊
			easing_rate += 0.02;

			if (1.3 < easing_rate)
			{
				easing_rate = 1.3;
			}

			// �C�[�W���O�����������l��ۑ�����ϐ�
			double easingRate;
			easingRate = Ease::InCubic(easing_rate);


			Player::GetOutSidePlayerBody()->ApplyForceToCenter(b2Vec2(0.0f, -easingRate), true);
			//-------------------------------------------------------------------------------------------

			b2Vec2 vec = Player::GetOutSidePlayerBody()->GetLinearVelocity();


			Player::GetOutSidePlayerBody()->SetLinearVelocity(b2Vec2(vec.x / 2, vec.y));


			VibrationController::StartVibration(0.5f, 0.5f, 30);
		}
	}
	else
	{
		easing_rate = 0.8;
	}
}



void geyser::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//�j��̃t���O���I���ɂȂ��Ă���
	{
		if (geyser_body != nullptr && Splitting_end == false)
		{
			//���[���h�̃C���X�^���X�������Ă���
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//�j�󂳂ꂽposition���擾


			//���ʂ̃{�f�B������
			b2Vec2 Destroy_position = geyser_body->GetPosition();
			float angle = geyser_body->GetAngle(); // ���̃{�f�B�̊p�x���擾
			b2Vec2 vec = geyser_body->GetLinearVelocity();
			float angle_vec = geyser_body->GetAngularVelocity();

			world->DestroyBody(geyser_body);
			geyser_body = nullptr;

			SetGeyserBody(nullptr);


		

			b2Vec2 size;
			size.x = geyser_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = geyser_size.y / BOX2D_SCALE_MANAGEMENT;



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
					boss_geyser_body_Splitting.push_back(fragment);

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
					fragmentFixture.filter = createFilterExclude("ground_filter", { "Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter" });

					b2Fixture* fixture = fragment->CreateFixture(&fragmentFixture);

					// �J�X�^���f�[�^���쐬���Đݒ�
					ObjectData* object_anchorpoint_data = new ObjectData{ collider_ground };
					fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

					// �����x�̓[���ɐݒ�i�K�v�ɉ����đ��x��ǉ��\�j
					fragment->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}




	}

}


void geyser::DestroySplittedBodies(std::vector<b2Body*>& bodyList) 
{
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

void geyser::Destroy_Joint()
{
	// ���[���h�̃C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// �{�f�B�̃W���C���g���X�g���m�F
	b2Body* body = GetGeyserBody();
	if (body == nullptr) {
		return; // �{�f�B�������Ȃ牽�����Ȃ�
	}

	b2JointEdge* jointEdge = body->GetJointList();
	if (jointEdge == nullptr) {
		return; // �W���C���g�����݂��Ȃ��ꍇ�͉������Ȃ�
	}

	// �W���C���g��j��
	world->DestroyJoint(jointEdge->joint);
}

void geyser::PullingOnRock()
{
	float player_pos_x = PlayerPosition::GetPlayerPosition().x;
	float rock_pos_x = GetGeyserOnRockBody()->GetPosition().x;

	float add_force = GetGeyserOnRockSize().x * GetGeyserOnRockSize().y * 10;

	if (player_pos_x < rock_pos_x)
	{
		add_force = add_force * -1;
	}


	GetGeyserOnRockBody()->ApplyForceToCenter(b2Vec2(add_force, 0.0f), true);
}


void geyser::Draw()
{
	if (isUse)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;


		int geyser_Draw_y=0;
		///�������璲�����Ă�
		if (GetGeyserBody() != nullptr)
		{


			b2Vec2 geyser_pos = GetGeyserBody()->GetPosition();

			// �v���C���[�ʒu���l�����ăX�N���[���␳��������
			//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
			float draw_x = ((geyser_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((geyser_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


			if (open_gyeser_flag == true)
			{
				if (water_sheet_cnt < 150)
				{
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Start_Water_Texture);
				}
				else
				{
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Water_Texture);
				}
				
				//draw
				DrawSplittingSprite(
					{ draw_x,
					  draw_y - (GetGeyserSize().y * scale) - (GetRangeFlyWaterSize().y / 2 * scale)+40},
					GetGeyserBody()->GetAngle(),
					{ GetRangeFlyWaterSize().x * scale * 2,GetRangeFlyWaterSize().y * scale * 1.5f },///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
					10, 5, water_sheet_cnt / 3, 3.0f

				);
				water_sheet_cnt++;
				if (290 < water_sheet_cnt)
				{
					water_sheet_cnt = 151;
				}

				if (120 < water_sheet_cnt && water_sheet_cnt<150)
				{
					water_sheet_cnt = 150;
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Open_Texture);

				//draw
				DrawSplittingSprite(
					{ draw_x,
					  draw_y+ geyser_Draw_y },
					GetGeyserBody()->GetAngle(),
					{ GetGeyserSize().x * scale,GetGeyserSize().y * scale },///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
					15, 1, draw_cnt, 2.0f
				);
			}
			else
			{
				//���̕`��
				if (m_is_border)
				{
					switch (m_need_level)
					{
					case 1:
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Border_Texture_Lv1);
						break;
					case 2:
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Border_Texture_Lv2);
						break;
					case 3:
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Border_Texture_Lv3);
						break;
					default:
						break;
					}
					//draw
					DrawSplittingSprite(
						{ draw_x,
						  draw_y + geyser_Draw_y },
						GetGeyserBody()->GetAngle(),
						{ GetGeyserSize().x * scale * 1.5f,GetGeyserSize().y * scale * 1.5f },///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
						15, 1, draw_cnt, m_border_alpha
					);

					//���ߗ��ݒ�
					m_border_alpha -= 0.01;
					if (m_border_alpha <= m_border_alpha_min)
					{
						m_border_alpha = m_border_alpha_max;
					}
				}

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Open_Texture);

				//draw
				DrawSplittingSprite(
					{ draw_x,
					  draw_y + geyser_Draw_y },
					GetGeyserBody()->GetAngle(),
					{ GetGeyserSize().x * scale,GetGeyserSize().y * scale },///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
					15, 1, draw_cnt, 2.0f
				);
			}

			draw_cnt += 0.3;

			
			

		


			
		}

		//������̕`��
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_geyser_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Close_Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_geyser_body_Splitting[i]->GetAngle(),
					{ GetGeyserSize().x / Splitting_x /15* scale,GetGeyserSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}
}




void geyser::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetGeyserBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(geyser_body);
	}

	if (GetGeyserOnRockBody() != nullptr)
	{
		world->DestroyBody(geyser_on_rock_body);
	}
	//�摜�̉��

	if (g_Geyser_Close_Texture != NULL)
	{
		UnInitTexture(g_Geyser_Close_Texture);
		UnInitTexture(g_Geyser_Border_Texture_Lv1);
		UnInitTexture(g_Geyser_Border_Texture_Lv2);
		UnInitTexture(g_Geyser_Border_Texture_Lv3);
		UnInitTexture(g_Geyser_Open_Texture);
		UnInitTexture(g_Geyser_Water_Texture);
		UnInitTexture(g_Geyser_Start_Water_Texture);

		g_Geyser_Close_Texture = NULL;
		g_Geyser_Border_Texture_Lv1 = NULL;
		g_Geyser_Border_Texture_Lv2 = NULL;
		g_Geyser_Border_Texture_Lv3 = NULL;
		g_Geyser_Open_Texture = NULL;
		g_Geyser_Water_Texture = NULL;
		g_Geyser_Start_Water_Texture = NULL;

	}
}