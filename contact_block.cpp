// #name�@contact_block.cpp
// #description �ڐG������Ȃɂ��̃A�N�V�������s���@���ǉ����Ă���̂͐G�ꂽ�玀�S���鏈��
// #make 2025/02/06
// #update 2025/02/06
// #comment �ǉ��E�C���\��  �ڐG�Ɋւ��ĂȂɂ��ǉ�������̂ł����Enum�^�ɏ���������
//         
//          
//----------------------------------------------------------------------------------------------------

#include"contact_block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"player.h"
#include"scene.h"


static ID3D11ShaderResourceView* g_Texture = NULL;//�Z���T�[��

//�ڐG�������ɗl�X�ȏo�������N����
contact_block::contact_block(b2Vec2 Position, b2Vec2 block_size, Contact_Block_Type type,b2Vec2 num)
{
	//�T�C�Y���Z�b�g
	SetSize(block_size);

	b2Vec2 size;

	size.x = block_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = block_size.y / BOX2D_SCALE_MANAGEMENT;






	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();





	b2BodyDef body;//�؂̊��̕���
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;

	b2Body* m_teleport_block_body = world->CreateBody(&body);

	SetBody(m_teleport_block_body);


	//�t�B�N�X�`����t����
	b2PolygonShape teleport_block_shape;

	teleport_block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef teleport_block_fixture;

	teleport_block_fixture.shape = &teleport_block_shape;
	teleport_block_fixture.density = 3.0f;
	teleport_block_fixture.friction = 0.5f;//���C
	teleport_block_fixture.restitution = 0.0f;//�����W��
	teleport_block_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	teleport_block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_teleport_fixture = m_body->CreateFixture(&teleport_block_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_objectData = std::make_unique<ObjectData>(collider_contact_block);
	object_teleport_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;

	SetID(ID);


	m_contact_type = type;


};

contact_block::~contact_block()
{
	Finalize();
}


void contact_block::Initialize()
{
	//�A���J�[�̕d�̕����i���{��j
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");
	

}

void contact_block::Update()
{
	if (m_body != nullptr)
	{
		if (m_flag==true)
		{
			//�t���O�����Z�b�g
			m_flag = false;
			Player& player = Player::GetInstance();
			SceneManager& sceneManager = SceneManager::GetInstance();
			switch (m_contact_type)
			{
			case NULL_TYPE:
				break;
			case DEAD_BLOCK_TYPE:

				//�v���C���[����������
				player.Player_Damaged(-1000, 0, nullptr);

				break;
			case GO_BOSS_STAGE:
				sceneManager.SetStageName(STAGE_BOSS);
				sceneManager.Set_Chenge_Scene_flag(true);
				break;
			case GO_STAGE_ISEKI:
				sceneManager.SetStageName(STAGE_ISEKI);
				sceneManager.Set_Chenge_Scene_flag(true);
				break;
			case GO_STAGE_SELECT:
				sceneManager.SetStageName(STAGE_SELECT);
				sceneManager.Set_Chenge_Scene_flag(true);
				break;
			case GO_BOSS_MOVIE:
				sceneManager.ChangeScene(SCENE_BOSS_MOVIE);
				sceneManager.Set_Chenge_Scene_flag(true);
				break;
			default:
				break;
			}


			
		}

	}
}



void contact_block::Draw()
{

	///�������璲�����Ă�

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 Pos = GetBody()->GetPosition();




	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	////draw
	//DrawSprite(
	//	{ draw_x,
	//	  draw_y },
	//	GetBody()->GetAngle(),
	//	{ GetSize().x * scale,GetSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	//);




}

void contact_block::Finalize()
{
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

	//�摜�̉��

	if (g_Texture) UnInitTexture(g_Texture);


}