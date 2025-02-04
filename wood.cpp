//-----------------------------------------------------------------------------------------------------
// #name wood.cpp
// #description 木のオブジェクトのCPP
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------

#include"wood.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//木のテクスチャ１
static ID3D11ShaderResourceView* g_Wood_Texture1 = NULL;//木のテクスチャ２
static ID3D11ShaderResourceView* g_Wood_Texture2 = NULL;//木ののテクスチャ３
static ID3D11ShaderResourceView* g_Stump_Texture = NULL;//木の切り株のテクスチャ



static ID3D11ShaderResourceView* g_leaf_Texture1 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture2 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture3 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture4 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture5 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture6 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture7 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture8 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture9 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture10 = NULL;//葉っぱのテクスチャ
static ID3D11ShaderResourceView* g_leaf_Texture11 = NULL;//葉っぱのテクスチャ

int ObjectData::current_id = 0;

wood::wood(b2Vec2 Position, b2Vec2 Wood_size, b2Vec2 AnchorPoint_size,int need_level)
{
	b2Vec2 Stump_size;
	Stump_size.x = Wood_size.x * 1.8f;
	Stump_size.y = Wood_size.y * 0.2f;

	SetWoodSize(Wood_size);
	SetStumpSize(Stump_size);
	SetAnchorPointSize(AnchorPoint_size);



	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 wood_size;
	wood_size.x = Wood_size.x / BOX2D_SCALE_MANAGEMENT;
	wood_size.y = Wood_size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Wood_body;//木の幹の部分
	Wood_body.type = b2_dynamicBody;
	Wood_body.position.Set(Position.x, Position.y - (Stump_size.y / BOX2D_SCALE_MANAGEMENT) * 0.5f);
	Wood_body.fixedRotation = false;

	b2Body* m_Wood_body = world->CreateBody(&Wood_body);

	SetObjectWoodBody(m_Wood_body);



	b2PolygonShape Wood_shape;
	Wood_shape.SetAsBox(wood_size.x * 0.5, wood_size.y * 0.5);

	b2FixtureDef wood_fixture;

	wood_fixture.shape = &Wood_shape;
	wood_fixture.density = 3.0f;
	wood_fixture.friction = 0.5f;//摩擦
	wood_fixture.restitution = 0.0f;//反発係数
	wood_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	wood_fixture.filter = createFilterExclude("object_filter",{});


	b2Fixture* object_wood_fixture = m_Wood_body->CreateFixture(&wood_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_wood_data = new ObjectData{ collider_object };//一旦壁判定
	object_wood_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_wood_data);

	//---------------------------------------------------------------------------//
	//2つめのボディ　木の上のアンカーポイントをつくる

	//サイズの補正をいれる
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = AnchorPoint_size.x / BOX2D_SCALE_MANAGEMENT;
	anchorpoint_size.y = AnchorPoint_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//木の幹の部分
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		Position.x,
		Position.y + (wood_size.y / 2) + (anchorpoint_size.y / 2));
	anchorpoint_body.fixedRotation = false;

	b2Body* m_AnchorPoint_body = world->CreateBody(&anchorpoint_body);

	SetObjectAnchorPointBody(m_AnchorPoint_body);



	b2PolygonShape anchorpoint_shape;
	anchorpoint_shape.SetAsBox(anchorpoint_size.x * 0.5, anchorpoint_size.y * 0.5);

	b2FixtureDef anchorpoint_fixture;

	anchorpoint_fixture.shape = &anchorpoint_shape;
	anchorpoint_fixture.density = 1.0f;
	anchorpoint_fixture.friction = 0.05f;//摩擦
	anchorpoint_fixture.restitution = 0.0f;//反発係数
	anchorpoint_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	anchorpoint_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_wood_data->object_name = Object_Wood;
	object_wood_data->need_anchor_level = need_level;
	object_anchorpoint_data->object_name = Object_Wood;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Wood_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -wood_size.y * 0.5f); // 木の上端
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

	//-------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------//
	//3つめのボディ　木の下に切り株をつくる

	//サイズの補正をいれる
	b2Vec2 stump_size;
	stump_size.x = Stump_size.x / BOX2D_SCALE_MANAGEMENT;
	stump_size.y = Stump_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef stump_body;//木の幹の部分
	stump_body.type = b2_dynamicBody;
	stump_body.position.Set(Position.x, Position.y + (wood_size.y * 0.5f) - (stump_size.y * 0.5f));
	stump_body.fixedRotation = false;

	auto m_stump_body = world->CreateBody(&stump_body);
	SetObjectStumpBody(m_stump_body);

	b2PolygonShape stump_shape;
	b2Vec2 vertices[4];
	vertices[0].Set(-stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[1].Set(stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[2].Set(-stump_size.x * 0.25f, stump_size.y * 0.2f);
	vertices[3].Set(stump_size.x * 0.25f, stump_size.y * 0.2f);
	stump_shape.Set(vertices, 4);	//センサーのローカル位置を変更


	b2FixtureDef stump_fixture;

	stump_fixture.shape = &stump_shape;
	stump_fixture.density = 20.0f;
	stump_fixture.friction = 0.5f;//摩擦
	stump_fixture.restitution = 0.0f;//反発係数
	stump_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える


	b2Fixture* object_stump_fixture = GetObjectStumpBody()->CreateFixture(&stump_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_stump_data = new ObjectData{ collider_object };//一旦壁判定
	object_stump_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_stump_data);

	object_stump_data->object_name = Object_Wood;




	//ObjecrDataの　ID設定　や　他の設定
	//------------------------------------------------------------------------------------------------------------------------------------------
	int ID=object_anchorpoint_data->GenerateID();
	object_wood_data->id = ID;
	object_anchorpoint_data->id = ID;
	SetID(ID);

	//木を倒しす時に必要になるForce とりあえずサイズに依存でつくる
	b2Vec2 need_power;

	need_power.x = ((GetWoodSize().x * GetWoodSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 3;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない
	

	object_anchorpoint_data->add_force = need_power;


	//アンカーレベルの設定
	object_anchorpoint_data->need_anchor_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef2;
	jointDef2.bodyA = m_Wood_body;
	jointDef2.bodyB = m_stump_body;
	jointDef2.localAnchorA.Set(0.0f, wood_size.y * 0.61f ); // 木の下端	(0.61は微調整して出た値)
	jointDef2.localAnchorB.Set(0.0f, stump_size.y * 0.3f); // 切り株の上端	(0.3は微調整して出た値)
	jointDef2.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	auto joint = world->CreateJoint(&jointDef2);						  //ワールドにジョイントを追加
	SetWoodStumpJoint(joint);	//木を引っ張ったらこのjointを消せるように保存しておく



	//-------------------------------------------------------------------------------------------
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//ここからボディを１１こ作って
	leaf_size = GetWoodSize();


	leaf_bodies.reserve(NUM_BODIES);//事前に容量を確保


	// ボディの定義
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // 動的ボディ
	bodyDef.gravityScale = (0.4);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(leaf_size.x/100, leaf_size.y/100); // 1x1 の四角形

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = false;
	fixtureDef.filter = createFilterExclude("texture_body_filter", {"texture_body_filter","object_filter","ground_filter","enemy_filter","Player_filter" });


	// 11個のボディを作成してワールドに追加
	for (int i = 0; i < NUM_BODIES; i++) {
		bodyDef.position.Set(Position.x,Position.y + (wood_size.y / 2)); // 適当な位置に配置
		b2Body* body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		leaf_bodies.push_back(body); // ベクターに追加

		b2WeldJointDef leafjointDef;
		leafjointDef.bodyA = body;
		leafjointDef.bodyB = m_Wood_body;
		leafjointDef.localAnchorA.Set(0.0f, 0.0f);
		leafjointDef.localAnchorB.Set(0.0f, -wood_size.y*0.3);
		leafjointDef.collideConnected = true;					  //ジョイントした物体同士の接触を消す

		world->CreateJoint(&leafjointDef);						  //ワールドにジョイントを追加

	}

};

wood::~wood()
{
}


void wood::Initialize()
{
	
	if (g_Wood_Texture == NULL) {
		g_Wood_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood.png");
		g_Wood_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
		g_Wood_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
		g_Stump_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood_stump.png");


		g_leaf_Texture1 = InitTexture(L"asset\\texture\\wood_texture\\three_happa1.png");
		g_leaf_Texture2 = InitTexture(L"asset\\texture\\wood_texture\\three_happa2.png");
		g_leaf_Texture3 = InitTexture(L"asset\\texture\\wood_texture\\three_happa3.png");
		g_leaf_Texture4 = InitTexture(L"asset\\texture\\wood_texture\\three_happa4.png");
		g_leaf_Texture5 = InitTexture(L"asset\\texture\\wood_texture\\three_happa5.png");
		g_leaf_Texture6 = InitTexture(L"asset\\texture\\wood_texture\\three_happa6.png");
		g_leaf_Texture7 = InitTexture(L"asset\\texture\\wood_texture\\three_happa7.png");
		g_leaf_Texture8 = InitTexture(L"asset\\texture\\wood_texture\\three_happa_tama8.png");
		g_leaf_Texture9 = InitTexture(L"asset\\texture\\wood_texture\\three_happa9.png");
		g_leaf_Texture10 = InitTexture(L"asset\\texture\\wood_texture\\three_happa_tama10.png");
		g_leaf_Texture11 = InitTexture(L"asset\\texture\\wood_texture\\three_happa11.png");
	}
}

void wood::Update()
{

		

		// 切り株と本体のジョイントを消すフラグがオンになってる場合
		if (m_destory_joint)
		{
			b2Joint* joint = GetWoodStumpJoint();
			if (joint != nullptr)
			{
				// ワールドのインスタンスを持ってくる
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();
				world->DestroyJoint(joint); // ワールドからジョイントを消す
				SetWoodStumpJoint(nullptr); // 自分が保持してるジョイントの情報を消す
			}
			m_destory_joint = false; // フラグをオフにする

		
		}

		if (leaf_drop_flag)
		{
			// 10秒後に葉っぱのボディを削除
			if (leafDeleteCountdown > now_delete_leaf_countDown) {
				now_delete_leaf_countDown++;

				if (now_delete_leaf_countDown % 60==0)
				{
					// 葉っぱの物理計算を有効化 & ランダムな力を加える
					for (b2Body* body : leaf_bodies) {
						body->SetAwake(true);
						body->SetAngularVelocity(0.0f);
						body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));

						// ランダムな力を適用（葉っぱを散らす）
						float forceX = ((rand() % 20) - 10) * 0.01f;
						float forceY = (rand() % 10) * -0.01f;
						body->ApplyForceToCenter(b2Vec2(forceX, forceY), true);

						
						
					}
				}
			}
			else {
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();

				for (b2Body* body : leaf_bodies) {
					world->DestroyBody(body);
				}
				leaf_bodies.clear();
				leaf_drop_flag = false;
			}
		}

	//ゲーム開始直後木が地面まで落ちる時音鳴らさないためのカウントダウン
	if (start_stop_sound_count > 0) {
		start_stop_sound_count--;
		return;
	}

	b2Vec2 velocity = Wood_body->GetLinearVelocity();
	velocity.Normalize();

	//落ちている状態、かつ、ぶつかって跳ね上がった時
	if ((m_state == Wood_Pulling || m_state == Wood_Falling) && (velocity.y > -0.1f && velocity.y < 0.1f))
	{
		float	angle = Wood_body->GetAngle();
		float	rotated = angle - angle_when_pulling_start;
		//引っ張り始めた時や前回音鳴らした直後の回転角度と今の回転角度の差が0.5から-0.5の間なら（ある一定回転した）
		if (rotated > 0.5f || rotated < -0.5f)
		{
			SetState(Wood_HitObject);	//音鳴らす
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			// `m_Wood_body` に関連するすべてのジョイントを削除
			b2JointEdge* jointEdge = GetObjectWoodBody()->GetJointList();
			while (jointEdge) {
				b2Joint* joint = jointEdge->joint;
				jointEdge = jointEdge->next; // 次のジョイントを先に取得

				// アンカーポイントのジョイントは削除しない
				if (joint->GetBodyA() == GetObjectAnchorPointBody() || joint->GetBodyB() == GetObjectAnchorPointBody()) {
					continue;
				}

				world->DestroyJoint(joint); // ジョイントを削除
			}

			// 葉っぱの物理計算を有効化
			for (b2Body* body : leaf_bodies) {
				body->SetAwake(true);
			}

			// 葉っぱの物理計算を有効化 & ランダムな力を加える
			for (b2Body* body : leaf_bodies) {
				body->SetAwake(true);
				body->SetAngularVelocity(0.0f);
				body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

				// ランダムな力を適用（葉っぱを散らす）
				float forceX = ((rand() % 20) - 10) * 0.01f;
				float forceY = (rand() % 10) * -0.01f;
				body->ApplyForceToCenter(b2Vec2(forceX, forceY), true);

				// 角速度もランダムにする
				float angularImpulse = ((rand() % 20) - 10) * 0.000005f;
				body->ApplyAngularImpulse(angularImpulse, true);
			}

			leaf_drop_flag = true;
		}
	}
	//さっきまで落ちていて、今は静止している
	if (m_state == Wood_Falling && velocity == b2Vec2{0.0f,0.0f})
	{
		SetState(Wood_Idle);	//通常状態にセット
	}
	//通常状態で、今静止していない
	if (m_state == Wood_Idle && (velocity.y < -0.999f || velocity.y > 0.999f) )
	{
		SetState(Wood_Falling);	//落ちている状態にセット
	}

}

void wood::Pulling_wood(b2Vec2 pulling_power)
{
	b2Body*body=GetObjectAnchorPointBody();
	//プレイヤー側に倒す
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x)//プレイヤーが左側
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);
	SetIfPulling(true);
}


void wood::SetState(Wood_State state)
{
	m_state = state;
	switch (m_state)
	{
	case Wood_Idle:
		break;
	case Wood_Pulling:
		m_destory_joint = true;	//切り株と本体のジョイントを消すためにフラグをオンにする
		angle_when_pulling_start = Wood_body->GetAngle();
		break; 
	case Wood_Falling:
		break;
	case Wood_HitObject:
		app_atomex_start(m_sound_FalledDown);	//音鳴らす
		SetState(Wood_Idle);
		angle_when_pulling_start = Wood_body->GetAngle();
		break;
	}
}

void wood::Draw()
{

	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;
	
	//切り株を描く
//--------------------------------------------------------------------------------------------------
// プレイヤー位置を考慮してスクロール補正を加える
//取得したbodyのポジションに対してBox2dスケールの補正を加える

	b2Vec2 Stump_pos = GetObjectStumpBody()->GetPosition();

	float draw_x = ((Stump_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Stump_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Stump_Texture);

	//描画
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectStumpBody()->GetAngle(),
		{ GetStumpSize().x * scale,GetStumpSize().y * scale }
	);


	//本体を描く
//--------------------------------------------------------------------------------------------------
	b2Vec2 Wood_pos = GetObjectWoodBody()->GetPosition();
	b2Vec2 AnchorPoint_pos = GetObjectAnchorPointBody()->GetPosition();


	b2Vec2 Wood_size = GetWoodSize();
	b2Vec2 AnchorPoint_size = GetAnchorPointSize();
	// 木の中心をローカル座標から計算
	float woodLocalCenterX =0.0f;
	float woodLocalCenterY = 0.0f;

	// アンカーポイントの中心をローカル座標から計算
	float anchorLocalCenterX = 0.0f;
	float anchorLocalCenterY = 0.0f;

	// 回転角度（ラジアン）
	float angle = GetObjectWoodBody()->GetAngle();

	// 回転行列を適用してワールド座標を計算
	b2Vec2 woodWorldCenter(
		Wood_pos.x + woodLocalCenterX * cos(angle) - woodLocalCenterY * sin(angle),
		Wood_pos.y + woodLocalCenterX * sin(angle) + woodLocalCenterY * cos(angle)
	);

	b2Vec2 anchorWorldCenter(
		AnchorPoint_pos.x + anchorLocalCenterX * cos(angle) - anchorLocalCenterY * sin(angle),
		AnchorPoint_pos.y + anchorLocalCenterX * sin(angle) + anchorLocalCenterY * cos(angle)
	);

	// 木とアンカーポイントの中心位置を加重平均で計算
	float totalHeight = Wood_size.y + AnchorPoint_size.y;
	float centerX = (woodWorldCenter.x * Wood_size.y + anchorWorldCenter.x * AnchorPoint_size.y) / totalHeight;
	float centerY = (woodWorldCenter.y * Wood_size.y + anchorWorldCenter.y * AnchorPoint_size.y) / totalHeight;

	// 中心点
	b2Vec2 textureCenter(centerX, centerY);



	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	draw_x = ((textureCenter.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	draw_y = ((textureCenter.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Wood_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetWoodSize().x * scale,totalHeight * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);


	for (size_t i = 0; i < leaf_bodies.size(); i++) {
		b2Vec2 position = leaf_bodies[i]->GetPosition();
		float angle = leaf_bodies[i]->GetAngle(); // 修正: 各葉っぱの角度を取得

		draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		// テクスチャを設定
		ID3D11ShaderResourceView* texture = nullptr;
		switch (i) {
		case 0: texture = g_leaf_Texture1; break;
		case 1: texture = g_leaf_Texture2; break;
		case 2: texture = g_leaf_Texture3; break;
		case 3: texture = g_leaf_Texture4; break;
		case 4: texture = g_leaf_Texture5; break;
		case 5: texture = g_leaf_Texture6; break;
		case 6: texture = g_leaf_Texture7; break;
		case 7: texture = g_leaf_Texture8; break;
		case 8: texture = g_leaf_Texture9; break;
		case 9: texture = g_leaf_Texture10; break;
		case 10: texture = g_leaf_Texture11; break;
		default: break;
		}

		if (texture) {
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
		}

		// 修正: 各葉っぱの角度を使用
		DrawSprite(
			{ draw_x, draw_y },
			angle, // 各葉っぱの角度を適用
			{ GetLeafSize().x * scale * 5, GetLeafSize().y * scale }
		);
	}
}

void wood::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetObjectWoodBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(Wood_body);
	}

	if (GetObjectAnchorPointBody() != nullptr)
	{
		world->DestroyBody(AnchorPoint_body);
	}

	//テクスチャの解放
	UnInitTexture(g_Wood_Texture);
	UnInitTexture(g_Wood_Texture1);
	UnInitTexture(g_Wood_Texture2);
}