//-----------------------------------------------------------------------------------------------------
// #name sloping_block.cpp
// #description 傾斜のついたブロックのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//
//----------------------------------------------------------------------------------------------------
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"sloping_block.h"
#include"scene.h"
#include"FixtureSizeCalculate.h"
#include"player.h"



//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_sloping_block_right_down_Texture = NULL;//テクスチャ 右下
static ID3D11ShaderResourceView* g_sloping_block_left_down_Texture = NULL;//テクスチャ  左した

static ID3D11ShaderResourceView* g_sloping_block_right_up_Texture = NULL;//テクスチャ 右上
static ID3D11ShaderResourceView* g_sloping_block_left_up_Texture = NULL;//テクスチャ  左上



b2Fixture* g_fixture = nullptr;



sloping_block::sloping_block(b2Vec2 position, b2Vec2 size, SlopingBlockAspect aspect)
{

	//向きをセットしておく
	SetBlockAspect(aspect);

	//描画ようのサイズの設定
	SetSlopingBlockSize(size);




	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 sloping_block_size;
	sloping_block_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	sloping_block_size.y = size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef sloping_block_body;//斜面のブロック
	sloping_block_body.type = b2_staticBody;
	sloping_block_body.position.Set(position.x, position.y);
	sloping_block_body.fixedRotation = false;

	b2Body* m_sloping_block_body = world->CreateBody(&sloping_block_body);

	SetObjectSlopingBlockBody(m_sloping_block_body);



	// 三角形のシェイプを定義
	b2PolygonShape triangleShape;
	b2Vec2 vertices[3] = {b2Vec2(0.0f,0.0f)};

	switch (aspect)
	{
	case right_down:
		vertices[0].Set(+sloping_block_size.x / 2, -sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x / 2, +sloping_block_size.y / 2); 
		vertices[2].Set(sloping_block_size.x / 2, sloping_block_size.y / 2);   
		break;
	case left_down:
		vertices[0].Set(-sloping_block_size.x / 2, sloping_block_size.y / 2); 
		vertices[1].Set(-sloping_block_size.x/2, -sloping_block_size.y/2); 
		vertices[2].Set(sloping_block_size.x, sloping_block_size.y); 
		break;
	default:
		break;
	}
	
	triangleShape.Set(vertices, 3); // 頂点を指定して三角形を設定

	b2FixtureDef sloping_block_fixture;

	sloping_block_fixture.shape = &triangleShape;
	sloping_block_fixture.density = 3.0f;
	sloping_block_fixture.friction = 0.5f;//摩擦
	sloping_block_fixture.restitution = 0.0f;//反発係数
	sloping_block_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

	b2Fixture* object_sloping_block_fixture = m_sloping_block_body->CreateFixture(&sloping_block_fixture);
	g_fixture = object_sloping_block_fixture;

	// カスタムデータを作成して設定

	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_ground);
	object_sloping_block_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	m_objectData->object_name = Object_sloping_block;



	int ID = m_objectData->GenerateID();//IDを取得
	m_objectData->id = ID;//フィクスチャに登録
	SetID(ID);//クラスに登録

};

sloping_block::~sloping_block()
{
	Finalize();
}


void sloping_block::Initialize()
{
	//テクスチャの初期化
	if (g_sloping_block_left_down_Texture == NULL)
	{

		SceneManager& sceneManager = SceneManager::GetInstance();
	
		//ステージによって代入する
		switch (sceneManager.GetStageName())
		{
		case STAGE_TUTORIAL:
			g_sloping_block_left_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_Downhill_02.png");//右側のテクスチャ
			g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_slope_02.png");//右側のテクスチャ
			g_sloping_block_left_up_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_Downhill_02.png");//右側のテクスチャ
			g_sloping_block_right_up_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_slope_02.png");//右側のテクスチャ
			break;
		case STAGE_1_1:
			g_sloping_block_left_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_Downhill_02.png");//右側のテクスチャ
			g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_slope_02.png");//右側のテクスチャ
			g_sloping_block_left_up_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_Downhill_02.png");//右側のテクスチャ
			g_sloping_block_right_up_Texture = InitTexture(L"asset\\texture\\stage_block\\1-1_block_slope_02.png");//右側のテクスチャ
			break;
		case STAGE_ISEKI:
			g_sloping_block_left_down_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_blokc_left.png");//右側のテクスチャ
			g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_block_right.png");//右側のテクスチャ
			g_sloping_block_left_up_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_blokc_left_up.png");//右側のテクスチャ
			g_sloping_block_right_up_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_block_right_up.png");//右側のテクスチャ
			break;
		case STAGE_BOSS:
			g_sloping_block_left_down_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_blokc_left.png");//右側のテクスチャ
			g_sloping_block_right_down_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_block_right.png");//右側のテクスチャ
			g_sloping_block_left_up_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_blokc_left_up.png");//右側のテクスチャ
			g_sloping_block_right_up_Texture = InitTexture(L"asset\\texture\\stage_block\\iseki_sloping_block_right_up.png");//右側のテクスチャ
			break;

		default:
			break;
		}	

	}
}

void sloping_block::Update()
{
	//プレイヤーがと衝突しているならプレイヤーが登れるように力を加える
	if (m_player_collided)
	{

		Player& player = Player::GetInstance();

		//プレイヤーが歩いていないのなら力を加えない
		if (player.GetState() != player_walk_state)
		{
			return;
		}

		//頂点データをfixtureから取得
		b2Vec2 vertex[3];
		b2Vec2 body_position = SlopingBlock_body->GetPosition();
		for (int i = 0; i < 3; i++)
		{
			vertex[i] = GetPolygonFixtureVertex(g_fixture->GetShape(), i);
			//頂点データをbox2D座標（directXのワールド座標一歩手前）に変換
			vertex[i].x += body_position.x;
			vertex[i].y += body_position.y;
		}

		//斜面のベクトル計算（掛ける8.0は調整値）
		b2Vec2	vector;
		vector.x = (vertex[0].x - vertex[2].x)*8.0f;
		vector.y = (vertex[0].y - vertex[2].y)*8.0f;

		//プレイヤーのボディを取得しておく
		auto player_body = player.GetOutSidePlayerBody();
		//力を加える時最大のベロシティを設定
		b2Vec2 max_velocity = { 2.0f,-1.0f };

		//ブロックの向きによってプレイヤーの向きが変わった時の処理が違う
		switch (GetBlockAspect())
		{
		case right_down:
			//プレイヤーが「右」向いている時上る
			if (player.GetDirection() == 1)
			{
				//プレイヤーに力を加える
				player_body->ApplyForceToCenter(vector, true);
				auto player_velocity = player_body->GetLinearVelocity();
				//ベロシティの最大値を超えないようにするための処理
				if (player_velocity.x > max_velocity.x)
				{
					player_body->SetLinearVelocity({ max_velocity.x, player_velocity.y });
					player_velocity.x = max_velocity.x;
				}
				//ベロシティの最大値を超えないようにするための処理
				if (player_velocity.y < max_velocity.y)
				{
					player_body->SetLinearVelocity({ player_velocity.x, max_velocity.y });
					player_velocity.y = max_velocity.y;
				}
			}
			//プレイヤーが「左」向いている時下る
			else
			{
				//プレイヤーが斜面に沿って下れるように力を加える
				player.GetOutSidePlayerBody()->ApplyLinearImpulseToCenter({ 0.0f,0.2f }, true);
			}
			break;
		case left_down:
			//プレイヤーが「左」向いている時上る
			if (player.GetDirection() == 0)
			{
				//プレイヤーに力を加える
				player.GetOutSidePlayerBody()->ApplyForceToCenter(vector, true);
				auto player_velocity = player_body->GetLinearVelocity();
				//ベロシティの最大値を超えないようにするための処理
				if (player_velocity.x > max_velocity.x)
				{
					player_body->SetLinearVelocity({ max_velocity.x, player_velocity.y });
					player_velocity.x = max_velocity.x;
				}
				//ベロシティの最大値を超えないようにするための処理
				if (player_velocity.y < max_velocity.y)
				{
					player_body->SetLinearVelocity({ player_velocity.x, max_velocity.y });
					player_velocity.y = max_velocity.y;
				}


			}
			//プレイヤーが「右」向いている時下る
			else
			{
				//プレイヤーが斜面に沿って下れるように力を加える
				player.GetOutSidePlayerBody()->ApplyLinearImpulseToCenter({ 0.0f,0.2f }, true);
			}
			break;
		default:
			break;
		}

	}
}



void sloping_block::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	///ここから調整してね

	b2Vec2 sloping_block_pos = GetObjectSlopingBlockBody()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((sloping_block_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((sloping_block_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	switch (GetBlockAspect())//向きをコンストラクタでセットしてるのでそれを受け取って描画
	{
	case right_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_down_Texture);
		break;

	case left_down:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_down_Texture);
		break;

	case right_up:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_right_up_Texture);
		break;

	case left_up:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_sloping_block_left_up_Texture);
		break;
	default:
		break;
	}

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectSlopingBlockBody()->GetAngle(),
		{ GetSlopingBlockSize().x * scale,GetSlopingBlockSize().y * scale },///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
		3.0f
	);
}

void sloping_block::Finalize()
{

	if (!GetObjectSlopingBlockBody()) return;

	for (b2Fixture* fixture = GetObjectSlopingBlockBody()->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		if (!fixture) continue;

		// UserData 取得


		// 無効なポインタならスキップ
		if (!fixture->GetUserData().pointer) {
			continue;
		}




		// ObjectData を削除す
		fixture->GetUserData().pointer = 0;  // ポインタのクリア
	}

	// `b2Body` を削除
	Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(GetObjectSlopingBlockBody());
	SlopingBlock_body = nullptr;

	
	if (g_sloping_block_right_down_Texture) UnInitTexture(g_sloping_block_right_down_Texture);
	if (g_sloping_block_left_down_Texture) UnInitTexture(g_sloping_block_left_down_Texture);

	if (g_sloping_block_right_up_Texture) UnInitTexture(g_sloping_block_right_up_Texture);
	if (g_sloping_block_left_up_Texture) UnInitTexture(g_sloping_block_left_up_Texture);



}

void sloping_block::SetPlayerCollided(bool flag)
{
	m_player_collided = flag;
}
