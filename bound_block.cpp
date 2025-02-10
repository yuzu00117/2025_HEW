//-----------------------------------------------------------------------------------------------------
// #name bound_block.cpp
// #description ボス戦で設置するバウンドするオブジェクト
// #make 2025/02/06　永野義也
// #update 2025/02/06
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------

#include"bound_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"player.h"

static ID3D11ShaderResourceView* g_bound_block_texture1_open = NULL;//バウンドブロックのテクスチャ茶色
static ID3D11ShaderResourceView* g_bound_block_texture1_close = NULL;//バウンドブロックのテクスチャ茶色

static ID3D11ShaderResourceView* g_bound_block_texture2_open = NULL;//バウンドブロックのテクスチャ灰色
static ID3D11ShaderResourceView* g_bound_block_texture2_close = NULL;//バウンドブロックのテクスチャ灰色

boss_bound_block::boss_bound_block(b2Vec2 position, b2Vec2 size, b2Vec2 vec, Boss_Room_Level level,int texture_type)
{
	SetSize(size);//描画用のサイズを保存

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//サイズを調整する
	b2Vec2 body_size;
	body_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	body_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//ボディを作成する
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//形の定義
	b2PolygonShape shape;
	shape.SetAsBox(body_size.x * 0.5, body_size.y * 0.5);


	//-----------------------------------------------------
	//	fixtureを作る
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.01f;
	fixture.restitution = 0.3f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("ground_filter", {});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// 新しいフィルターを作成


	//カスタムデータを作成
	ObjectData* object_data = new ObjectData{ collider_bound_block };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = NULL_object;
	SetID(ID);


	//ジャンプ量を渡す
	vectol = vec;

	BossRoomLevel = level;

	Texture_type = texture_type;


}

boss_bound_block::~boss_bound_block()
{
}



void boss_bound_block::Initialize()
{
	if (g_bound_block_texture1_open == NULL) {
		g_bound_block_texture1_open = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_open_type1.png");//バウンドブロックのテクスチャ
		g_bound_block_texture1_close = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_close_type1.png");//バウンドブロックのテクスチャ
		g_bound_block_texture2_open = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_open_type2.png");//バウンドブロックのテクスチャ
		g_bound_block_texture2_close = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_close_type2.png");//バウンドブロックのテクスチャ
	}
}

void boss_bound_block::Update()
{
	Boss_1_1& boss = Boss_1_1::GetInstance();
	if (boss.GetBossFieldLevel() > BossRoomLevel && break_flag == false)
	{
		m_body->SetType(b2_dynamicBody);

		// フィルターを変更
		b2Fixture* fixture = m_body->GetFixtureList();

		// フィクスチャが存在しない場合は早期リターン
		if (!fixture) {
			return;
		}

		// 新しいフィルターを作成
		b2Filter newFilter = createFilterExclude("Boss_field_filter", { "ground_filter","object_filter" });
		fixture->SetFilterData(newFilter);

		break_flag = true;

		int x = GetRandomInt(1, 10);


		int minus = GetRandomInt(0, 1);

		if (minus == 1)
		{
			minus = -1;
		}
		else
		{
			minus = 1;
		}

		m_body->ApplyLinearImpulseToCenter(b2Vec2(x / 5 * minus, 0.0f), true);

	}

	if (break_flag == true)
	{
		body_delete_cnt++;
	}

	//３秒間たったらボディをけす
	if (body_delete_cnt > 180)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		if (m_body == nullptr)
		{
			world->DestroyBody(m_body);
		}

	}
	Player_jump();



}

void boss_bound_block::Draw()
{


	if (m_body != nullptr)
	{
		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;
		if (Change_Texture_Size_Frame == 0)
		{
		

			b2Vec2 Pos = GetBody()->GetPosition();


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			switch (Texture_type)
			{
			case 0:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture1_close);
				break;
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture2_close);
			default:
				break;
			}
		


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale }
			);
		}
		else
		{
		

			b2Vec2 Pos = GetBody()->GetPosition();

			


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;
			switch (Texture_type)
			{
			case 0:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture1_open);
				break;
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture2_open);
			default:
				break;
			}
			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale *2}
			);

			Change_Texture_Size_Frame++;

			if (30 < Change_Texture_Size_Frame)
			{
				Change_Texture_Size_Frame = 0;
			}
		}
	}

}
void boss_bound_block::Player_jump()
{
	if (jump_flag == true)
	{
		Player &player = Player::GetInstance();//シングルトン

		b2Body *player_body=player.GetOutSidePlayerBody();

		player_body->ApplyLinearImpulseToCenter(vectol, true);


		//フラグをリセット
		jump_flag = false;


		//チェンジテクスチャ
		Change_Texture_Size_Frame = 1;
	}
}



void boss_bound_block::Finalize()
{
	if (g_bound_block_texture1_open != NULL)
	{
		UnInitTexture(g_bound_block_texture1_open);
		g_bound_block_texture1_open = NULL;
		UnInitTexture(g_bound_block_texture2_open);
		g_bound_block_texture2_open = NULL;

		UnInitTexture(g_bound_block_texture1_close);
		g_bound_block_texture1_close = NULL;
		UnInitTexture(g_bound_block_texture2_close);
		g_bound_block_texture2_close = NULL;
	}
}