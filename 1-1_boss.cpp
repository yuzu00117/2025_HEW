#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include"main.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"

// 使用するテクスチャファイルを格納
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//小さなゴーレムのテクスチャ
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//衝撃波のテクスチャ１
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//衝撃波のテクスチャ２
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//溜め攻撃のテクスチャ１
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//ため攻撃のテクスチャ２
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//小さなゴーレムを生成する際のボス側１
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//小さなゴーレムを生成する際のボス側２


//-------------------------------------------------------------------------------------------
//デバック用の画像
static ID3D11ShaderResourceView* g_debug_color = NULL;//デバック用


Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize)
{
	g_mini_boss_Texture =					InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//ミニゴーレムのインクルード
	g_boss_shock_wave_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//衝撃波攻撃のインクルードシート１
	g_boss_shock_wave_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//衝撃波攻撃のインクルードシート２
	g_boss_charge_attack_sheet1_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//ため攻撃のシート１
	g_boss_charge_attack_sheet2_Texture =	InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//ため攻撃のシート２
	g_mini_boss_create_sheet1_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//ミニゴーレムの生成のボス側１
	g_mini_boss_create_sheet2_Texture =		InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//ミニゴーレムの生成のボス側２
	


	//デバック用
	g_debug_color= InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//ミニゴーレムの生成のボス側２


	//ボディのサイズをセット
	SetBossSize(bodysize);

	b2Vec2 size; //サイズのスケールを調整
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true;//回転を固定にする
	body.userData.pointer = (uintptr_t)this;
	

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//ワールドに登録
	b2Body *m_boss_body=world->CreateBody(&body);

	SetBossBody(m_boss_body);
	

	b2PolygonShape body_shape;
	body_shape.SetAsBox(size.x * 0.5, size.y * 0.5f);

	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.1f;//摩擦
	body_fixture.restitution = 0.1f;//反発係数
	body_fixture.density = 1.0f;
	body_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える


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

		// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet1_Texture);

		// コライダーの位置の取得（プレイヤーの位置）
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
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
	//ボディのあたり判定を表示

	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_color);

	// コライダーの位置の取得（プレイヤーの位置）
	b2Vec2 boss_pos = GetBossBody()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	float pos_y = PlayerPosition::GetPlayerPosition().y;
	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSize().x * scale, GetBossSize().y * scale));
}

void Boss_1_1::Finalize()
{

}