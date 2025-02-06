//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy.cpp
// #description�@�{�X�̏����v�J�v�J���Ă�G�l�~�[�I�u�W�F�N�g�������Ăė��Ƃ���
// #make 2025/02/01�@�i��`��
// #update 2025/02/01
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#include"1_1_boss_carry_object_enemy.h"
#include "world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"sprite.h"
#include"create_filter.h"

static ID3D11ShaderResourceView* g_Enemy_Texture = NULL;//�G�l�~�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Object_Texture = NULL;//�G�l�~�[�̃e�N�X�`��


boss_carry_object_enemy::boss_carry_object_enemy(b2Vec2 position,b2Vec2 Enemy_size,bool left, float Enemy_speed,b2Vec2 Object_size, int Object_type,int anchor_level)
{

	if (g_Enemy_Texture == NULL)
	{
		g_Enemy_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating .png");
		g_Object_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform.png");//�I�u�W�F�N�g�̃e�N�X�`��
	}
	// ���[���h�̃C���X�^���X
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	left_flag = left;
	enemy_speed= Enemy_speed;

	object_type = Object_type;

	//��������o�^���Ă���
	Splitting_x = Object_size.x;
	Splitting_y = Object_size.y;

	//�T�C�Y��o�^
	SetEnemySize(Enemy_size);
	SetObjectSize(Object_size);

	// �T�C�Y�̕␳
	b2Vec2 object_size;
	b2Vec2 enemy_size;

	object_size.x = Object_size.x / BOX2D_SCALE_MANAGEMENT;
	object_size.y = Object_size.y / BOX2D_SCALE_MANAGEMENT;

	enemy_size.x = Enemy_size.x / BOX2D_SCALE_MANAGEMENT;
	enemy_size.y = Enemy_size.y / BOX2D_SCALE_MANAGEMENT;

	b2Vec2 pos = position; 

	// === EnemyBody �̍쐬 ===
	b2BodyDef enemyBodyDef;
	enemyBodyDef.type = b2_dynamicBody;
	enemyBodyDef.position.Set(pos.x, pos.y);
	enemyBodyDef.gravityScale = (0.0f);
	b2Body* enemyBody = world->CreateBody(&enemyBodyDef);
	SetEnemyBody(enemyBody);

	b2PolygonShape enemyShape;
	enemyShape.SetAsBox(enemy_size.x * 0.5f, enemy_size.y * 0.5f);

	b2FixtureDef enemyFixtureDef;
	enemyFixtureDef.shape = &enemyShape;
	enemyFixtureDef.density = 1.0f;
	enemyFixtureDef.friction = 0.3f;
	enemyFixtureDef.isSensor = true;
	b2Fixture* enemy_fixture = enemyBody->CreateFixture(&enemyFixtureDef);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* enemy_data = new ObjectData{ collider_object_carry_enemy };
	enemy_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemy_data);

	int ID = enemy_data->GenerateID();
	SetID(ID);

	enemy_data->id = GetID();



	// === ObjectBody �̍쐬 ===
	b2BodyDef objectBodyDef;
	objectBodyDef.type = b2_dynamicBody;
	objectBodyDef.position.Set(pos.x, pos.y + (object_size.y / 2) + (enemy_size.y / 2));
	objectBodyDef.gravityScale = (0.0f);
	b2Body* objectBody = world->CreateBody(&objectBodyDef);
	SetObjectBody(objectBody);

	b2PolygonShape objectShape;
	objectShape.SetAsBox(object_size.x * 0.5f, object_size.y * 0.5f);

	b2FixtureDef objectFixtureDef;
	objectFixtureDef.shape = &objectShape;
	objectFixtureDef.density = 1.0f;
	objectFixtureDef.friction = 0.3f;
	objectFixtureDef.isSensor = true;

	b2Fixture* object_fixture = objectBody->CreateFixture(&objectFixtureDef);
	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_data = new ObjectData{ collider_anchor_point };
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	object_data->need_anchor_level = anchor_level;
	object_data->id = GetID();
	object_data->object_name = Boss_Carry_Object_Enemy;





	// === �W���C���g�̍쐬 (Weld Joint) ===
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = enemyBody;
	weldJointDef.bodyB = objectBody;
	weldJointDef.localAnchorA.Set(0.0f, enemy_size.y * 0.5f); // �G�l�~�[�̉�
	weldJointDef.localAnchorB.Set(0.0f, -object_size.y * 0.5f); // �I�u�W�F�N�g�̏�
	weldJointDef.collideConnected = false;  // �W���C���g�������̓��m�̏Փ˂�h��
	world->CreateJoint(&weldJointDef);

	

	isUse = true;
}


void boss_carry_object_enemy::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//�j��̃t���O���I���ɂȂ��Ă���
	{
		

		if (object_body != nullptr && Splitting_end == false)
		{
			//���[���h�̃C���X�^���X�������Ă���
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//�j�󂳂ꂽposition���擾


			//���ʂ̃{�f�B������
			b2Vec2 Destroy_position = object_body->GetPosition();
			float angle = object_body->GetAngle(); // ���̃{�f�B�̊p�x���擾
			b2Vec2 vec = object_body->GetLinearVelocity();
			float angle_vec = object_body->GetAngularVelocity();

			world->DestroyBody(object_body);
			object_body = nullptr;

			SetObjectBody(nullptr);


			//�A���J�[�|�C���g�̃{�f�B������

		

			b2Vec2 size;
			size.x = GetObjectSize().x / BOX2D_SCALE_MANAGEMENT;
			size.y = GetObjectSize().y / BOX2D_SCALE_MANAGEMENT;



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
					boss_Object_body_Splitting.push_back(fragment);

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


					// �����_���ȕ����ɔ�юU��悤�ɑ��x��ݒ�
					fragment->SetLinearVelocity(GetRandomVelocity(5.0f)); // 5.0f �͊���x�i�����j

				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}




	}

}

boss_carry_object_enemy::~boss_carry_object_enemy()
{

}

void boss_carry_object_enemy::Initialize()
{
	
}

void boss_carry_object_enemy::Update()
{
	if (isUse)
	{
		if (enemy_body != nullptr && object_body != nullptr)
		{
			// �ړ�����������i��: ���A�E: ���j
			float moveSpeed = left_flag ? -fabs(enemy_speed) : fabs(enemy_speed);


			// �G�l�~�[���ړ�
			enemy_body->SetLinearVelocity(b2Vec2(moveSpeed, enemy_body->GetLinearVelocity().y));

			// �I�u�W�F�N�g���G�l�~�[�̈ʒu�ɒǏ]������
			b2Vec2 enemyPos = enemy_body->GetPosition();
			b2Vec2 objectPos = object_body->GetPosition();

			object_body->SetTransform(b2Vec2(enemyPos.x, objectPos.y), 0);
		}

		if (Anchor_Hit_flag == true)
		{
			AnchorHit();
			Anchor_Hit_flag = false;
		}

		if (Splitting_end == true)
		{
			Destroy_Cnt++;
		}
		if (180 < Destroy_Cnt)//�����������Ɣj�󂳂��t���O
		{
			DestroySplittedBodies(boss_Object_body_Splitting);
			isUse = false;
		}

		// 30�b�i1800�t���[���j�o�ߌ�Ƀ{�f�B�폜
		lifetime++;
		if (lifetime > 1800)
		{
			Destroy_Body();
		}
	}

	Destroy_Splitting();

}

void boss_carry_object_enemy::AnchorHit()
{
	
		b2Body* enemyBody = GetEnemyBody();
		b2Body* objectBody =GetObjectBody();


		// ���[���h�̃C���X�^���X�擾
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2JointEdge* jointEdge = enemyBody->GetJointList();
		while (jointEdge)
		{
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next;
			world->DestroyJoint(joint);
		}

		// --- �����̃t�B�N�X�`�����폜 ---
		for (b2Fixture* f = enemyBody->GetFixtureList(); f;)
		{
			b2Fixture* next = f->GetNext();
			enemyBody->DestroyFixture(f);
			f = next;
		}

		for (b2Fixture* f = objectBody->GetFixtureList(); f;)
		{
			b2Fixture* next = f->GetNext();
			objectBody->DestroyFixture(f);
			f = next;
		}

		// --- �V�����t�B�N�X�`�����쐬 ---
		b2PolygonShape newEnemyShape;
		newEnemyShape.SetAsBox(GetEnemySize().x/ BOX2D_SCALE_MANAGEMENT * 0.5f, GetEnemySize().y/ BOX2D_SCALE_MANAGEMENT * 0.5f);

		b2FixtureDef newEnemyFixtureDef;
		newEnemyFixtureDef.shape = &newEnemyShape;
		newEnemyFixtureDef.density = 1.0; // �ύX��
		newEnemyFixtureDef.friction = 0.5f;
		newEnemyFixtureDef.isSensor = false;

		b2Fixture* newEnemyFixture = enemyBody->CreateFixture(&newEnemyFixtureDef);

		// --- �J�X�^���f�[�^�̍X�V ---
		ObjectData* enemyData = new ObjectData{ collider_object };
		newEnemyFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemyData);
		enemyData->id = GetID();
		enemyData->object_name = Boss_Carry_Object_Enemy;

		// --- ObjectBody�̍Đݒ� ---
		b2PolygonShape newObjectShape;
		newObjectShape.SetAsBox(GetObjectSize().x / BOX2D_SCALE_MANAGEMENT * 0.5f, GetObjectSize().y / BOX2D_SCALE_MANAGEMENT * 0.5f);

		b2FixtureDef newObjectFixtureDef;
		newObjectFixtureDef.shape = &newObjectShape;
		newObjectFixtureDef.density = 1.0f; // �ύX��
		newObjectFixtureDef.friction = 0.4f;
		newObjectFixtureDef.isSensor = false;

		b2Fixture* newObjectFixture = objectBody->CreateFixture(&newObjectFixtureDef);

		// --- �J�X�^���f�[�^�̍X�V ---
		ObjectData* objectData = new ObjectData{ collider_object };
		objectData-= GetID();
		objectData->object_name = Boss_Carry_Object_Enemy;
		newObjectFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectData);
		objectData->id = GetID();

		// === �O�����B�e�B�X�P�[���̕ύX ===
		enemyBody->SetGravityScale(0.0f);  // �ύX��̒l�i�f�t�H���g1.0�j
		objectBody->SetGravityScale(1.0f); // �ύX��̒l�i�f�t�H���g1.0�j

	
		
	

}



void boss_carry_object_enemy::Draw()
{
	if (isUse)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;
		//�G�l�~�[�̃{�f�B�̕`�揈��
		if (enemy_body != nullptr)
		{


			b2Vec2 Pos = GetEnemyBody()->GetPosition();


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Enemy_Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetEnemyBody()->GetAngle(),
				{ GetEnemySize().x * scale ,GetEnemySize().y * scale }
			);
		}

		//�I�u�W�F�N�g�̕`�揈��
		if (GetObjectBody() != nullptr)
		{


			b2Vec2 ObjectPos = GetObjectBody()->GetPosition();


			float object_draw_x = ((ObjectPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float object_draw_y = ((ObjectPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);


			//draw
			DrawSprite(
				{ object_draw_x,
				  object_draw_y },
				GetObjectBody()->GetAngle(),
				{ GetObjectSize().x * scale ,GetObjectSize().y * scale }
			);
		}

		//�I�u�W�F�N�g�̕`��I����

			//������̕`��
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_Object_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_Object_body_Splitting[i]->GetAngle(),
					{ GetObjectSize().x / Splitting_x * scale,GetObjectSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}
}

void boss_carry_object_enemy::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
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




void boss_carry_object_enemy::Finalize()
{
	if (g_Enemy_Texture != NULL)
	{
		UnInitTexture(g_Enemy_Texture);
		UnInitTexture(g_Object_Texture);

		g_Enemy_Texture = NULL;
		g_Object_Texture = NULL;
	}
}

void boss_carry_object_enemy::Destroy_Body()
{
	if (enemy_body == nullptr && object_body == nullptr) return;

	// ���[���h�̃C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// �{�f�B�̍폜
	if (enemy_body)
	{
		world->DestroyBody(enemy_body);
		enemy_body = nullptr;
	}
	if (object_body)
	{
		world->DestroyBody(object_body);
		object_body = nullptr;
	}

	// ������
	isUse = false;
}