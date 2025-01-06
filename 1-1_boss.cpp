#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include"main.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"

// �g�p����e�N�X�`���t�@�C�����i�[
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//�����ȃS�[�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//�Ռ��g�̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//�Ռ��g�̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//���ߍU���̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//���ߍU���̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���P
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���Q


//-------------------------------------------------------------------------------------------
//�f�o�b�N�p�̉摜
static ID3D11ShaderResourceView* g_debug_color = NULL;//�f�o�b�N�p


Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize)
{
	g_mini_boss_Texture =					InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//�~�j�S�[�����̃C���N���[�h
	g_boss_shock_wave_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�P
	g_boss_shock_wave_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�Q
	g_boss_charge_attack_sheet1_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//���ߍU���̃V�[�g�P
	g_boss_charge_attack_sheet2_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//���ߍU���̃V�[�g�Q
	g_mini_boss_create_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//�~�j�S�[�����̐����̃{�X���P
	g_mini_boss_create_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//�~�j�S�[�����̐����̃{�X���Q
	


	//�f�o�b�N�p
	g_debug_color= InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//�~�j�S�[�����̐����̃{�X���Q


	//�{�f�B�̃T�C�Y���Z�b�g
	SetBossSize(bodysize);

	b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true;//��]���Œ�ɂ���
	body.userData.pointer = (uintptr_t)this;
	

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//���[���h�ɓo�^
	b2Body *m_boss_body=world->CreateBody(&body);

	SetBossBody(m_boss_body);
	

	b2PolygonShape body_shape;
	body_shape.SetAsBox(size.x * 0.5, size.y * 0.5f);

	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.1f;//���C
	body_fixture.restitution = 0.1f;//�����W��
	body_fixture.density = 1.0f;
	body_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����


	b2Fixture* m_body_fixture = m_body->CreateFixture(&body_fixture);




}

void Boss_1_1::Update()
{
	if (m_body != nullptr)
	{
		sheet_cnt++;
	}
}

void Boss_1_1::Draw()
{
	if (m_body !=nullptr)
	{
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet1_Texture);

		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y- PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



		float pos_y = PlayerPosition::GetPlayerPosition().y;
		
		//DrawDividedSprite(XMFLOAT2(draw_x, draw_y), 0.0f,XMFLOAT2(GetBossSize().x, GetBossSize().y), 7, 7, 1, 1.0);
	
		DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSize().x*scale,GetBossSize().y*scale),7, 7, sheet_cnt, 1.0);



		debugDraw();
	}
}
void Boss_1_1::debugDraw()
{


	///------------------------------------------------------------------------
	//�{�f�B�̂����蔻���\��

	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_color);

	// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
	b2Vec2 boss_pos = GetBossBody()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	float pos_y = PlayerPosition::GetPlayerPosition().y;
	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSize().x * scale, GetBossSize().y * scale));
}

void Boss_1_1::Finalize()
{

}