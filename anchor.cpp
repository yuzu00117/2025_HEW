//-----------------------------------------------------------------------------------------------------
// #name anchor.cpp
// #description アンカー
// #make 2024/11/19　　永野義也
// #update 2024/11/24
// #comment 追加・修正予定
//          ・アンカー部分については大丈夫だと思う　横投げがいるなら追加
//			・Chainについては描画の面で変更が必要 具体的には
//			  いまアンカーがアンカーポイントに接触してから一括でチェーンが生成と削除されている関係で描画面がおかしいのでそこの変更
// 
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"anchor.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player.h"
#include"anchor_point.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"contactlist.h"
#include"create_filter.h"
#include"anchor_spirit.h"
#include"sound.h"

//グローバル変数
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev1 = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev2 = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Anchor_Texture_Lev3 = NULL;//アンカーのテクスチャ

static ID3D11ShaderResourceView* g_Anchor_Chain_Texture_Lv1 = NULL;//アンカーの鎖のテクスチャ
static ID3D11ShaderResourceView* g_Anchor_Chain_Texture_Lv2 = NULL;//アンカーの鎖のテクスチャ
static ID3D11ShaderResourceView* g_Anchor_Chain_Texture_Lv3 = NULL;//アンカーの鎖のテクスチャ

static ID3D11ShaderResourceView* g_Anchor_Hit_Effect_Texture = NULL;//アンカーのヒット時のエフェクトのテクスチャ


static ID3D11ShaderResourceView* g_Anchor_Hit_Effect_Level1_Texture = NULL;//アンカーがオブジェクトにヒットした時のレベル別のエフェクト
static ID3D11ShaderResourceView* g_Anchor_Hit_Effect_Level2_Texture = NULL;//アンカーがオブジェクトにヒットした時のレベル別のエフェクト
static ID3D11ShaderResourceView* g_Anchor_Hit_Effect_Level3_Texture = NULL;//アンカーがオブジェクトにヒットした時のレベル別のエフェクト


static ID3D11ShaderResourceView* g_Anchor_Effect_S1 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_S2 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_S3 = NULL;//アンカーのエフェクト

static ID3D11ShaderResourceView* g_Anchor_Effect_M1 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_M2 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_M3 = NULL;//アンカーのエフェクト


static ID3D11ShaderResourceView* g_Anchor_Effect_L1 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_L2 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_L3 = NULL;//アンカーのエフェクト













Anchor Anchor::g_anchor_instance;  // グローバルなインスタンス

Anchor::Anchor()
{
	anchor_create_joint_flag = false;
}

Anchor::~Anchor()
{
	
}


void Anchor::Initialize()
{
	//テクスチャの初期化

	//アンカーの錨の部分（日本語）
	g_Anchor_Texture_Lev1 =InitTexture(L"asset\\texture\\anchor_point\\Anchor_Lv1.png");
	g_Anchor_Texture_Lev2 = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Lv2.png");
	g_Anchor_Texture_Lev3 = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Lv3.png");
	assert(g_Anchor_Texture_Lev3);


	//アンカーの鎖
	g_Anchor_Chain_Texture_Lv1 = InitTexture(L"asset\\texture\\anchor_point\\chain_Blue.png");
	g_Anchor_Chain_Texture_Lv2 = InitTexture(L"asset\\texture\\anchor_point\\chain_Yellow.png");
	g_Anchor_Chain_Texture_Lv3 = InitTexture(L"asset\\texture\\anchor_point\\chain_Red.png");



	//通常攻撃のエフェクト
	g_Anchor_Hit_Effect_Texture=InitTexture(L"asset\\texture\\anchor_point\\Anchor_Hit_Effect.png");

	//レベル別の
	g_Anchor_Hit_Effect_Level1_Texture = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Hit_Effect_Level1.png");
	g_Anchor_Hit_Effect_Level2_Texture = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Hit_Effect_Level2.png");
	g_Anchor_Hit_Effect_Level3_Texture = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Hit_Effect_Level3.png");



	g_Anchor_Effect_S1 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL01_blue_2x4.png");
	g_Anchor_Effect_S2 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL02_blue_2x5.png");
	g_Anchor_Effect_S3 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL03_blue2x5.png");


	g_Anchor_Effect_M1 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL01_yellow_2x4.png");
	g_Anchor_Effect_M2 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL02_yellow_2x5.png");
	g_Anchor_Effect_M3 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL03_yellow_2x5.png");

	g_Anchor_Effect_L1 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL01_red_2x4.png");
	g_Anchor_Effect_L2 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL02_red_2x5.png");
	g_Anchor_Effect_L3 = InitTexture(L"asset\\texture\\anchor_point\\EFF_AncL03_red_2x5.png");

}

void Anchor::CreateAnchor(b2Vec2 anchor_size)
{
	g_anchor_instance.CreateAnchorBody(anchor_size);//アンカーのボディをつくる
}

void Anchor::DeleteAnchor()
{

		g_anchor_instance.DestroyAnchorBody();

	
}



void Anchor::CreateAnchorBody(b2Vec2 anchor_size)
{
	//アンカーを削除にする
	if (AnchorPoint::GetTargetAnchorPointBody() == nullptr) { Anchor::SetAnchorState(Deleting_state); return; }

	//アンカーの錨の部分を作ってあげちゃう
	b2Body* player_body = Player::GetOutSidePlayerBody();			//プレイヤーのBody情報を取得
	b2Body* target_AP_body = AnchorPoint::GetTargetAnchorPointBody();//ターゲットとしたアンカーポイントのボディ情報を取得


	// プレイヤーとターゲットアンカーポイントの現在の位置を取得
	b2Vec2 player_position = player_body->GetPosition(); // プレイヤーの位置
	b2Vec2 target_position = target_AP_body->GetPosition(); // ターゲットアンカーポイントの位置

	// プレイヤーからターゲットアンカーポイントへの方向ベクトルを計算
	b2Vec2 direction = target_position - player_position;

	// ベクトルの正規化（方向のみを保持）
	float length = direction.Length();
	if (length > 0.0f) {
		direction *= (1.0f / length); // 正規化
	}

	// 飛んでいく方向にオフセットを適用
	float offset_distance = 0.5f; // 錨を生成する距離（プレイヤーからの距離）
	b2Vec2 anchor_position = player_position + b2Vec2(direction.x * offset_distance, direction.y * offset_distance);

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position=anchor_position;
	body.angle = 0;

	//投げられた角度にあわせてアンカーの角度の設定の必要があるっぴよ

	




	body.fixedRotation = true;//回転を固定にする
	body.userData.pointer = (uintptr_t)this;


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();



	m_body = world->CreateBody(&body);

	player_body = m_body;//プレイヤーのボディをセット



	SetSize(anchor_size);//プレイヤー表示をするためにセットする



	b2Vec2 size;
	size.x = anchor_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = anchor_size.y / BOX2D_SCALE_MANAGEMENT;


	b2PolygonShape shape;
	shape.SetAsBox(size.x * 0.5, size.y * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.1f;//密度
	fixture.friction = 0.05f;//摩擦
	fixture.restitution = 0.1f;//反発係数
	fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える




	b2Fixture* anchor_fixture = m_body->CreateFixture(&fixture);



	// カスタムデータを作成して設定
	// ユニークポインターを使って ObjectData を作成
	m_anchor_objectData = std::make_unique<ObjectData>(collider_anchor);
	anchor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_anchor_objectData.get());

	SetChainEffect();
}

void Anchor::DestroyAnchorBody()
{
	if (m_body != nullptr) {
		


		for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
			if (!fixture) continue;

			// UserData 取得


			// 無効なポインタならスキップ
			if (!fixture->GetUserData().pointer) {
				continue;
			}




			// ObjectData を削除す
			fixture->GetUserData().pointer = 0;  // ポインタのクリア
		}
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			world->DestroyBody(m_body);
			m_body = nullptr;
			m_isAnchorCreated = false;

		
	}
}

void Anchor::Update()
{

	
	
}

void Anchor::Draw()
{

	if (g_anchor_instance.GetAnchorBody() != nullptr)
	{


		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;




		b2Body* anchor = g_anchor_instance.GetAnchorBody();

		if (anchor != nullptr)
		{
			b2Vec2 position;
			position.x = anchor->GetPosition().x;
			position.y = anchor->GetPosition().y;

			// プレイヤー位置を考慮してスクロール補正を加える
			//取得したbodyのポジションに対してBox2dスケールの補正を加える
			float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			float anchor_size_scale = 0;


			
			if (g_anchor_instance.GetAnchorBody() == nullptr) {
				return;
			}

			// アンカーレベルによるテクスチャ選択
			ID3D11ShaderResourceView* texture = nullptr;
			switch (AnchorSpirit::GetAnchorLevel()) {
			case 1:
				texture = g_Anchor_Texture_Lev1;
				anchor_size_scale = 1.3f;
				break;
			case 2:
				texture = g_Anchor_Texture_Lev2;
				anchor_size_scale = 2.0f;
				break;
			case 3:
				texture = g_Anchor_Texture_Lev3;
				anchor_size_scale = 3.0f;
				break;
			default:
				break;
			}

			if (texture == nullptr) {
				OutputDebugStringA("Error: Selected anchor texture is nullptr\n");
				return;
			}

		


			b2Body* target_AP_body = AnchorPoint::GetTargetAnchorPointBody();

			b2Vec2 Temporary_angle;//ベクトル使って飛んでいく角度 変数名は一時的なっていみ

			if (Anchor::GetAnchorState() == Throwing_state)
			{
				Temporary_angle.x = target_AP_body->GetPosition().x - anchor->GetPosition().x;//xの座標の管理
				Temporary_angle.y = target_AP_body->GetPosition().y - anchor->GetPosition().y;//yの座標の管理
			}
			else
			{
				Player& player = Player::GetInstance();
				Temporary_angle.x = anchor->GetPosition().x - player.GetOutSidePlayerBody()->GetPosition().x;//xの座標の管理
				Temporary_angle.y = anchor->GetPosition().y - player.GetOutSidePlayerBody()->GetPosition().y;//yの座標の管理
			}



			//ラジアン角を算出
			float anchor_angle = atan2(Temporary_angle.y, Temporary_angle.x);

			// ラジアンから度数へ変換
			anchor_angle = anchor_angle * 180.0f / M_PI;

			// 270度を補正 画像が下向きだったから
			anchor_angle += 40.0f;

			// 負の角度を正の範囲に調整（0°〜360°）
			if (anchor_angle < 0) {
				anchor_angle += 360.0f;
			}

			//度数をラジアンに変換して
			//Box2dのラジアンで管理してる
			float angle = anchor_angle * M_PI / 180.0f;


			//シェーダーリソースビュー
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ g_anchor_instance.GetSize().x * scale * anchor_size_scale,g_anchor_instance.GetSize().y * scale * anchor_size_scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			);



		}

		DrawChain();//チェーンの描画処理
		DrawNormalAttack();//通常攻撃の描写
		DrawAnchorHitEffect();//ヒットエフェクト
	}
}


void Anchor::Finalize()
{
	//アンカーの終了処理を追加するよ

	 // 静的フラグや状態のリセット
	anchor_create_joint_flag = false;
	now_anchor_state = Nonexistent_state;



	
	g_anchor_instance.DestroyAnchorBody();//アンカーのボディを解放


	if (g_Anchor_Texture_Lev1) UnInitTexture(g_Anchor_Texture_Lev1);
	if (g_Anchor_Texture_Lev2) UnInitTexture(g_Anchor_Texture_Lev2);
	if (g_Anchor_Texture_Lev3) UnInitTexture(g_Anchor_Texture_Lev3);

	if (g_Anchor_Chain_Texture_Lv1) UnInitTexture(g_Anchor_Chain_Texture_Lv1);
	if (g_Anchor_Chain_Texture_Lv2) UnInitTexture(g_Anchor_Chain_Texture_Lv2);
	if (g_Anchor_Chain_Texture_Lv3) UnInitTexture(g_Anchor_Chain_Texture_Lv3);

	if (g_Anchor_Hit_Effect_Texture) UnInitTexture(g_Anchor_Hit_Effect_Texture);

	if (g_Anchor_Hit_Effect_Level1_Texture) UnInitTexture(g_Anchor_Hit_Effect_Level1_Texture);
	if (g_Anchor_Hit_Effect_Level2_Texture) UnInitTexture(g_Anchor_Hit_Effect_Level2_Texture);
	if (g_Anchor_Hit_Effect_Level3_Texture) UnInitTexture(g_Anchor_Hit_Effect_Level3_Texture);

	if (g_Anchor_Effect_S1) UnInitTexture(g_Anchor_Effect_S1);
	if (g_Anchor_Effect_S2) UnInitTexture(g_Anchor_Effect_S2);
	if (g_Anchor_Effect_S3) UnInitTexture(g_Anchor_Effect_S3);

	if (g_Anchor_Effect_M1) UnInitTexture(g_Anchor_Effect_M1);
	if (g_Anchor_Effect_M2) UnInitTexture(g_Anchor_Effect_M2);
	if (g_Anchor_Effect_M3) UnInitTexture(g_Anchor_Effect_M3);

	if (g_Anchor_Effect_L1) UnInitTexture(g_Anchor_Effect_L1);
	if (g_Anchor_Effect_L2) UnInitTexture(g_Anchor_Effect_L2);
	if (g_Anchor_Effect_L3) UnInitTexture(g_Anchor_Effect_L3);

	
}





void Anchor::ThrowAnchorToAP(float speed_up)
{


	//今のアンカーがある座標の取得
	b2Vec2 anchor_pos=g_anchor_instance.GetAnchorBody()->GetPosition();


	b2Body* body=AnchorPoint::GetTargetAnchorPointBody();

	
	if (AnchorPoint::GetTargetAnchorPointBody() != nullptr)
	{
		b2Vec2 to_pos = AnchorPoint::GetTargetAnchorPointBody()->GetPosition();

		// 値が異常かチェック
		if (std::abs(to_pos.x) > 1e6 || std::abs(to_pos.y) > 1e6) {
			// 異常値の場合、エラーログを出力するか処理をスキップ
			std::cerr << "Error: to_pos has invalid values. x=" << to_pos.x << ", y=" << to_pos.y << std::endl;
			SetAnchorState(Pulling_state);
			return; // 処理を中断
		}
		b2Vec2 velocity = to_pos - anchor_pos;
		velocity.Normalize(); // 単位ベクトル化して方向を決定
		velocity *= (20 * speed_up); // 投擲速度を設定	

		g_anchor_instance.GetAnchorBody()->SetLinearVelocity(velocity);//ここで力を加えてる
	}

}
	
void Anchor::CreateRotateJoint()
{
	g_anchor_instance.GetAnchorBody()->SetLinearVelocity(b2Vec2_zero);//とんできたアンカーのベロシティをゼロにする
	MyContactListener& contact_listener = MyContactListener::GetInstance();

	if (g_anchor_instance.GetAnchorBody() == nullptr) {
		return; // アンカーが存在しない場合は何もしない
	}

	b2Body* anchorBody = g_anchor_instance.GetAnchorBody();
	b2Body* targetBody = AnchorPoint::GetTargetAnchorPointBody();


	//くっついたアンカーポイントをフィクスチャを変更する

	// まず現在のフィクスチャのサイズを取得する
	if (targetBody != nullptr)
	{
		b2Fixture* fixture = targetBody->GetFixtureList();
		if (fixture != nullptr) {
			bool sensor_on_off = fixture->IsSensor();
			float m_density = fixture->GetDensity();
			float m_friction = fixture->GetFriction();
			float m_restitution = fixture->GetRestitution();

			// 形状を取得
			b2Shape* baseShape = fixture->GetShape();
			if (baseShape == nullptr) {
				return;
			}

			// 新しい形状用ポインタ
			b2Shape* newShape = nullptr;

			// 形状タイプの判定
			if (baseShape->GetType() == b2Shape::e_polygon) {
				// ポリゴン形状の場合
				b2PolygonShape* originalShape = static_cast<b2PolygonShape*>(baseShape);
				if (originalShape == nullptr) {
					return;
				}

				b2PolygonShape* newPolygonShape = new b2PolygonShape();
				b2Vec2 vertices[b2_maxPolygonVertices];
				int vertexCount = min(originalShape->m_count, b2_maxPolygonVertices);

				for (int i = 0; i < vertexCount; ++i) {
					vertices[i] = originalShape->m_vertices[i];
				}

				newPolygonShape->Set(vertices, vertexCount);
				newShape = newPolygonShape;

			}
			else if (baseShape->GetType() == b2Shape::e_circle) {
				// サークル形状の場合
				b2CircleShape* originalShape = static_cast<b2CircleShape*>(baseShape);
				if (originalShape == nullptr) {
					return;
				}

				b2CircleShape* newCircleShape = new b2CircleShape();
				newCircleShape->m_radius = originalShape->m_radius;
				newCircleShape->m_p = originalShape->m_p; // 位置もコピー
				newShape = newCircleShape;
			}
			else {
				// 他の形状は対応しない
				return;
			}

			// 既存のフィクスチャを削除
			targetBody->DestroyFixture(fixture);

			// 新しいフィクスチャを作成
			b2FixtureDef fixtureDef;
			fixtureDef.shape = newShape;
			fixtureDef.density = m_density;
			fixtureDef.friction = m_friction;
			fixtureDef.restitution = m_restitution;
			fixtureDef.isSensor = sensor_on_off;

			b2Fixture* anchor_fixture = targetBody->CreateFixture(&fixtureDef);

			// カスタムデータを作成して設定
			ObjectData* anchordata = new ObjectData{ collider_rotate_joint };
			anchor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(anchordata);

			// 動的に確保した形状を削除
			delete newShape;
		}
	}

		if (anchorBody == nullptr || targetBody == nullptr) {
			return; // ターゲットが存在しない場合は何もしない
		}

		// 回転ジョイントを定義
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = anchorBody;
		jointDef.bodyB = targetBody;

		// ジョイントのアンカー点を設定 (例: アンカーの位置に合わせる)
		b2Vec2 localAnchorA = anchorBody->GetLocalPoint(contact_listener.contactPoint);
		b2Vec2 localAnchorB = targetBody->GetLocalPoint(contact_listener.contactPoint);

		jointDef.collideConnected = true; // ジョイントで接続されたボディ間の衝突を無効化

		// ジョイントを生成
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->CreateJoint(&jointDef);

		//エフェクトスタート
		g_anchor_instance.anchor_hit_effect_flag = true;
	
}

/**
 * @brief ジョイントを削除する
 * @param 
 */
void Anchor::DeleteRotateJoint(void)
{
	if (g_anchor_instance.GetAnchorBody() == nullptr) {
		return; // アンカーが存在しない場合は何もしない
	}

	b2Body* anchorBody = g_anchor_instance.GetAnchorBody();

	// すべてのジョイントを調べる
	for (b2JointEdge* jointEdge = anchorBody->GetJointList(); jointEdge != nullptr; jointEdge = jointEdge->next) {
		b2Joint* joint = jointEdge->joint;

		// ジョイントが回転ジョイントか確認
		if (joint->GetType() == e_revoluteJoint) {
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			// ジョイントを削除
			world->DestroyJoint(joint);
			break; // 1つ削除したらループ終了
		}
	}
}





void Anchor::PullingAnchor(void)
{
	//プレイヤーとアンカーの座標を取得する

	b2Vec2 player_postion = PlayerPosition::GetPlayerPosition();
	b2Vec2 anchor_postion = g_anchor_instance.GetAnchorBody()->GetPosition();

	b2Vec2 velocity = player_postion - anchor_postion;
	velocity.Normalize(); // 単位ベクトル化して方向を決定
	velocity *= 6; // 投擲速度を設定	

	g_anchor_instance.GetAnchorBody()->SetLinearVelocity(velocity);

}



// 静的メンバ変数の定義
bool Anchor::anchor_create_joint_flag = false;

void Anchor::SetAnchorCreateJointFlag(bool flag) {
	anchor_create_joint_flag = flag;
}

bool Anchor::GetAnchorCreateJointFlag() {
	return anchor_create_joint_flag;
}


//静的メンバの変数の定義
AnchorState Anchor::now_anchor_state = Nonexistent_state;

void Anchor::SetAnchorState(AnchorState state)
{
	now_anchor_state = state;
}

AnchorState Anchor::GetAnchorState()
{
	return now_anchor_state;
}

void Anchor::SetChainEffect()
{
	int rand = GetRandomInt(1, 3);;

	int anchorlevel = AnchorSpirit::GetAnchorLevel();

	//０を代入
	for (int i = 0; i < MAX_CHAIN_NUM; i++)
	{
		g_anchor_instance.Anchor_effect_sheet[i] = 0;
	}


	switch (anchorlevel)
	{
		//アンカーレベル１
	case 1:
		g_anchor_instance.chain_effect_scale = 10.0f;
		switch (rand)
		{
		case 1:
			g_anchor_instance.Anchor_Effect_Type = 1;
			g_anchor_instance.Max_Anchor_effect_sheet = 8;

			break;
		case 2:
			g_anchor_instance.Anchor_Effect_Type = 2;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;
		case 3:
			g_anchor_instance.Anchor_Effect_Type = 3;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;


		default:
			break;
		}
		break;
		//アンカーレベル2
	case 2:
		g_anchor_instance.chain_effect_scale = 10.0f;
		switch (rand)
		{
		case 1:
			g_anchor_instance.Anchor_Effect_Type = 4;
			g_anchor_instance.Max_Anchor_effect_sheet = 8;

			break;
		case 2:
			g_anchor_instance.Anchor_Effect_Type = 5;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;
		case 3:
			g_anchor_instance.Anchor_Effect_Type = 6;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;


		default:
			break;
		}
		break;
		//アンカーレベル3
	case 3:
		g_anchor_instance.chain_effect_scale = 8.0f;
		switch (rand)
		{
		case 1:
			g_anchor_instance.Anchor_Effect_Type = 7;
			g_anchor_instance.Max_Anchor_effect_sheet = 8;

			break;
		case 2:
			g_anchor_instance.Anchor_Effect_Type = 8;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;
		case 3:
			g_anchor_instance.Anchor_Effect_Type = 9;
			g_anchor_instance.Max_Anchor_effect_sheet = 10;
			break;


		default:
			break;
		}
		break;

	default:
		break;
	}
}

void Anchor::DrawChain() 
{


	// スケール設定
	float scale = SCREEN_SCALE;

	// スクリーン中央位置
	b2Vec2 screen_center(SCREEN_CENTER_X, SCREEN_CENTER_Y);

	// プレイヤーとアンカーの位置を取得
	b2Body* anchor = g_anchor_instance.GetAnchorBody();

	if (anchor == nullptr)
	{
		return;
	}

	b2Vec2 anchor_position = anchor->GetPosition();
	b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

	

	// 距離を計算
	float distance = b2Distance(anchor_position, player_position);

	// チェーンの描画間隔とチェーン数
	const float chain_interval = 0.2f; // チェーン間隔（Box2Dスケールで）
	int chain_count = static_cast<int>(distance / chain_interval);

	// チェーンの角度を計算（プレイヤーからアンカーへの角度）
	float angle = atan2(anchor_position.y - player_position.y, anchor_position.x - player_position.x);

	// チェーンサイズ設定 (X方向が長い)
	b2Vec2 chain_size(0.2f, 0.1); // Xが長いチェーン

	// チェーン描画
	for (int i = 1; i <= chain_count-2; ++i)
	{
		// 線形補間で位置を計算
		float t = static_cast<float>(i) / chain_count; // 0.0〜1.0の範囲
		b2Vec2 interpolated_position = player_position + t * (anchor_position - player_position);

		// スクリーン座標に変換
		float draw_x = ((interpolated_position.x - player_position.x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((interpolated_position.y - player_position.y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		switch (AnchorSpirit::GetAnchorLevel())
		{
		case 1:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture_Lv1); // チェーン用テクスチャ
			break;
		case 2:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture_Lv2); // チェーン用テクスチャ
			break;
		case 3:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture_Lv3); // チェーン用テクスチャ
			break;
		default:
			break;
		}
		// チェーン描画
		
		DrawSprite(
			{ draw_x, draw_y },
			angle, // プレイヤーとアンカーの角度
			{ chain_size.x * BOX2D_SCALE_MANAGEMENT * scale, chain_size.y * BOX2D_SCALE_MANAGEMENT * scale }
		);
		
		


		

			int player_scale_x = g_anchor_instance.chain_effect_scale*2.0f;
			int player_scale_y = g_anchor_instance.chain_effect_scale*2.0f;

			float chain_alpha=1.0f;

			bool m_direction=true;
		

		

			if (i % 2 == 0)
			{

				if (g_anchor_instance.Anchor_effect_sheet[i] < g_anchor_instance.Max_Anchor_effect_sheet)
				{

					switch (g_anchor_instance.Anchor_Effect_Type)
					{
					case 1:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S1);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x , chain_size.x * scale * player_scale_y },
							4, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 2:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S2);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 3:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S3);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 4:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M1);

						DrawDividedSpritePlayer(
							{ draw_x,
							 draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							4, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 5:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M2);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 6:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M3);

						DrawDividedSpritePlayer(
							{ draw_x,
							 draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.y * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 7:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L1);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							4, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					case 8:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L2);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x ,chain_size.x * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;

					case 9:
						// シェーダリソースを設定
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L3);

						DrawDividedSpritePlayer(
							{ draw_x,
							  draw_y },
							angle,
							{ chain_size.x * scale * player_scale_x, chain_size.x * scale * player_scale_y },
							5, 2, g_anchor_instance.Anchor_effect_sheet[i] / 4, chain_alpha, m_direction
						);
						break;
					default:
						break;
					}


					g_anchor_instance.Anchor_effect_sheet[i] += 0.3;
				}
			}
		
	}



}

void Anchor::DrawAnchorHitEffect(void)
{

	//描画の表示が
	if (g_anchor_instance.anchor_hit_effect_flag == true)
	{
		// スケール設定
		float scale = SCREEN_SCALE;
		// スクリーン中央位置
		b2Vec2 screen_center(SCREEN_CENTER_X, SCREEN_CENTER_Y);

		b2Body* anchor = g_anchor_instance.GetAnchorBody();

		//アンカーの選択
		if (g_anchor_instance.now_anchor_hit_effect_sheet_cnt == 0)
		{
			switch (AnchorSpirit::GetAnchorLevel())
			{
			case 1:
				g_anchor_instance.anchor_hit_effect_type = 1;
				g_anchor_instance.max_anchor_hit_effect_sheet_cnt = 6;
				break;
			case 2:
				g_anchor_instance.anchor_hit_effect_type = 2;
				g_anchor_instance.max_anchor_hit_effect_sheet_cnt = 9;
				break;
			case 3:
				g_anchor_instance.anchor_hit_effect_type = 3;
				g_anchor_instance.max_anchor_hit_effect_sheet_cnt = 16;
				break;
			default:
				break;
			};
		}


		if (anchor != nullptr)
		{
		
			// 角度と距離に基づいてオフセットを計算
			float angle = anchor->GetAngle();  // ラジアン角
			float distance = 50.0f;  // ずらす距離（適宜調整）

			// 極座標変換によるオフセット
			float offset_x = cos(angle) * distance;
			float offset_y = sin(angle) * distance;

			b2Vec2 position;
			position.x = anchor->GetPosition().x;
			position.y = anchor->GetPosition().y;

			// プレイヤー位置を考慮してスクロール補正を加える
			float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x + offset_x;
			float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y + offset_y;

		

			switch (g_anchor_instance.anchor_hit_effect_type)
			{
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Hit_Effect_Level1_Texture);

				DrawSplittingSprite(
					{ draw_x,
					draw_y },
					anchor->GetAngle(),
					{ g_anchor_instance.GetSize().x * scale*1.5f  ,g_anchor_instance.GetSize().y * scale *1.5f },
					3, 2,
					g_anchor_instance.now_anchor_hit_effect_sheet_cnt / 2,
					1.0f
				);
				break;
			case 2:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Hit_Effect_Level2_Texture);

				DrawSplittingSprite(
					{ draw_x ,
					draw_y  },
					anchor->GetAngle(),
					{ g_anchor_instance.GetSize().x * scale *2.5f  ,g_anchor_instance.GetSize().y * scale * 2.5f },
					3, 3,
					g_anchor_instance.now_anchor_hit_effect_sheet_cnt / 2,
					1.0f
				);
				break;
			case 3:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Hit_Effect_Level3_Texture);

				DrawSplittingSprite(
					{ draw_x ,
					draw_y },
					anchor->GetAngle(),
					{ g_anchor_instance.GetSize().x * scale * 3  ,g_anchor_instance.GetSize().y * scale * 3 },
					4, 4,
					g_anchor_instance.now_anchor_hit_effect_sheet_cnt / 2,
					1.0f
				);
				break;

			default:
				break;
			}
			
		}

		g_anchor_instance.now_anchor_hit_effect_sheet_cnt++;

		if (g_anchor_instance.max_anchor_hit_effect_sheet_cnt <g_anchor_instance.now_anchor_hit_effect_sheet_cnt)
		{
			g_anchor_instance.anchor_hit_effect_flag = false;
			g_anchor_instance.now_anchor_hit_effect_sheet_cnt = 0;
			g_anchor_instance.anchor_create_joint_flag = 0;
		}

	}
}


void Anchor::CreateNormalAttack(b2Vec2 anchor_size, bool right)
{


	g_anchor_instance.CreateNormalAttackAnchorBody(anchor_size, right);	
}
	
void Anchor::DeleteNormalAttackAnchor()
{
	g_anchor_instance.DeleteNormalAttackAnchorBody();
}


void Anchor::CreateNormalAttackAnchorBody(b2Vec2 size,bool right)
{
	//アンカーの錨の部分を作ってあげちゃう
	b2Body* player_body = Player::GetOutSidePlayerBody();			//プレイヤーのBody情報を取得

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//サイズの補正をする
	b2Vec2 anchor_size;
	anchor_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	anchor_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;

	body.type = b2_dynamicBody;
	body.gravityScale = (0.0f);


	if (right)//右かどうか
	{
		//プレイヤーのサイズの情報を貰ってきてないから　プレイヤーのサイズに変更あったときだるい
		body.position.Set(player_body->GetPosition().x + (1 / BOX2D_SCALE_MANAGEMENT) + (anchor_size.x / 2), player_body->GetPosition().y);//プレイヤーの右側に生成
	}
	else
	{
		body.position.Set(player_body->GetPosition().x - (1 / BOX2D_SCALE_MANAGEMENT) - (anchor_size.x / 2), player_body->GetPosition().y);//プレイヤーの左側に生成
	}
	body.fixedRotation = false;//回転する

	b2Body* m_body = world->CreateBody(&body);

	m_normal_attack_body = m_body;



	////--------------------------------------------------------------------------------------------------
	//通常攻撃のフィクスチャ
	b2FixtureDef fixture;

	// クラス内に b2Shape をメンバーとして保持する場合の例
	b2PolygonShape shape; // クラスのメンバー変数として保持
	shape.SetAsBox(anchor_size.x * 0.5, anchor_size.y * 0.5);

	fixture.shape = &shape;//形を設定
	fixture.density = 0.1f;//密度
	fixture.friction = 0.0f;//摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = true;//センサーかどうか


	b2Fixture* m_anchorpoint_fixture = m_body->CreateFixture(&fixture);


	ObjectData* object_anchorpoint_data = new ObjectData{ collider_normal_attack_anchor };
	m_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	

	////プレイヤーとジョイントする
	//b2WeldJointDef jointDef;
	//jointDef.bodyA = Player::GetOutSidePlayerBody();//プレイヤーのボディ
	//jointDef.bodyB = g_anchor_instance->GetNormalAttackAnchorBody();//通常攻撃のアンカーのボディ

	//if (right)//右かどうか
	//{
	//	//プレイヤー側
	//	jointDef.localAnchorA.Set(((1 / BOX2D_SCALE_MANAGEMENT) * 0.5), 0.0f);
	//	//通常攻撃側
	//	jointDef.localAnchorB.Set((-anchor_size.x * 0.5), 0.0f);
	//}
	//else//左側
	//{
	//	//プレイヤー側
	//	jointDef.localAnchorA.Set(((-1/ BOX2D_SCALE_MANAGEMENT) * 0.5), 0.0f);
	//	//通常攻撃側
	//	jointDef.localAnchorB.Set((anchor_size.x * 0.5), 0.0f);
	//}
	//jointDef.collideConnected = false;//ジョイントした物体同士の接触を消す

	//world->CreateJoint(&jointDef); //ワールドにジョイントを追加

	//エフェクトスタート
	g_anchor_instance.anchor_hit_effect_flag = true;
}

void Anchor::UpdateNormalAttack()
{

}

void Anchor::DrawNormalAttack()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;



	b2Body* anchor = g_anchor_instance.GetNormalAttackAnchorBody();

	if (anchor != nullptr)
	{
		b2Vec2 position;
		position.x = anchor->GetPosition().x;
		position.y = anchor->GetPosition().y;

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Hit_Effect_Texture);

		//draw
		DrawSplittingSprite(
			{ draw_x,
			  draw_y },
			0.0	,
			{ 6 * scale, 6 * scale },///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			4,2,g_anchor_instance.anchor_nomal_attack_effect,1.0f
		);
		g_anchor_instance.anchor_nomal_attack_effect +=0.4;
	}
	else
	{
		g_anchor_instance.anchor_nomal_attack_effect = 0;
	}
}

void Anchor::DeleteNormalAttackAnchorBody()
{

	// ワールドのインスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// アンカーのジョイントを削除
	if (m_normal_attack_body != nullptr)
	{
		// アンカーに関連付けられたすべてのジョイントを取得し削除
		for (b2JointEdge* jointEdge = m_normal_attack_body->GetJointList(); jointEdge != nullptr; )
		{
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next; // 次のジョイントエッジを保存

			// ジョイントをワールドから削除
			world->DestroyJoint(joint);
		}

		// アンカーのボディを削除
		world->DestroyBody(m_normal_attack_body);
		m_normal_attack_body = nullptr;
	}

}