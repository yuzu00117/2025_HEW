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

//グローバル変数
static ID3D11ShaderResourceView* g_Anchor_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Anchor_Chain_Texture = NULL;//アンカーの鎖のテクスチャ

//アンカーの一端のプレイヤーのボディをもっとく
b2Body* Player_body;

//アンカーの目標のアンカーポイント
b2Body* Target_anchor_point_body;



//アンカーの鎖の部分のボディの入れ物
#define MAX_CHAIN_NUM (40)//チェーンの最大個数
b2Body* anchor_chain[MAX_CHAIN_NUM];



//アンカークラスのインスタンス
Anchor*g_anchor_instance;


Anchor::Anchor()
{
	anchor_create_joint_flag = false;
}

Anchor::~Anchor()
{
	DestroyAnchorBody();
}


void Anchor::Initialize()
{
	//テクスチャの初期化

	//アンカーの錨の部分（日本語）
	g_Anchor_Texture=InitTexture(L"asset\\texture\\sample_texture\\img_anchor.png");

	//アンカーの鎖
	g_Anchor_Chain_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
}

void Anchor::CreateAnchor(b2Vec2 anchor_size)
{
	if (g_anchor_instance == nullptr)
	{
		g_anchor_instance = new Anchor();//NULLだったらアンカーを作って上げる
	}
	g_anchor_instance->CreateAnchorBody(anchor_size);//アンカーのボディをつくる
}

void Anchor::DeleteAnchor()
{
	if (g_anchor_instance != nullptr) {
		g_anchor_instance->DestroyAnchorBody();
	
		//配列自体もデリート
		delete g_anchor_instance;
		//NULLもしとく
		g_anchor_instance = nullptr;

	}
	
}
void Anchor::ToggleAnchor()
{
	if (g_anchor_instance != nullptr && g_anchor_instance->GetAnchorBody() != nullptr) {
		DeleteAnchor();
	}
	else {
		CreateAnchor(b2Vec2(2.0f,2.0f));
	}
}


void Anchor::CreateAnchorBody(b2Vec2 anchor_size)
{
	//アンカーの錨の部分を作ってあげちゃう
	b2Body* player_body = Player::GetOutSidePlayerBody();			//プレイヤーのBody情報を取得
	b2Body* target_AP_body = AnchorPoint::GetTargetAnchorPointBody();//ターゲットとしたアンカーポイントのボディ情報を取得


	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(player_body->GetPosition().x + 0.5, player_body->GetPosition().y - 0.5);

	//投げられた角度にあわせてアンカーの角度の設定の必要があるっぴよ

	b2Vec2 Temporary_angle;//ベクトル使って飛んでいく角度 変数名は一時的なっていみ
	Temporary_angle.x = target_AP_body->GetPosition().x - player_body->GetPosition().x;//xの座標の管理
	Temporary_angle.y = target_AP_body->GetPosition().y - player_body->GetPosition().y;//yの座標の管理

	//ラジアン角を算出
	float anchor_angle = atan2(Temporary_angle.y, Temporary_angle.x);

	// ラジアンから度数へ変換
	anchor_angle = anchor_angle * 180.0f / M_PI;

	// 270度を補正 画像が下向きだったから
	anchor_angle += 270.0f;

	// 負の角度を正の範囲に調整（0°～360°）
	if (anchor_angle < 0) {
		anchor_angle += 360.0f;
	}

	//度数をラジアンに変換して
	//Box2dのラジアンで管理してる
	body.angle = anchor_angle * M_PI / 180.0f;


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
	// プレイヤーに値を登録
	// プレーヤーにユーザーデータを登録
	ObjectData* anchordata = new ObjectData{ collider_anchor };
	anchor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(anchordata);


}

void Anchor::DestroyAnchorBody()
{
	if (m_body != nullptr) {
		if (m_body != nullptr) {
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			world->DestroyBody(m_body);
			m_body = nullptr;
			m_isAnchorCreated = false;
		}
	}
}

void Anchor::Update()
{

	
	if(Anchor::GetAnchorState()==Connected_state)//くっついている状態になったら
	{
		g_anchor_instance->GetAnchorBody()->SetLinearVelocity(b2Vec2_zero);

		Anchor::CreateRotateJoint();

		Anchor::SetAnchorState(Pulling_state);
	}
}

void Anchor::Draw()
{

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	if (g_anchor_instance == nullptr)
	{
		return;
	}

	b2Body *anchor = g_anchor_instance->GetAnchorBody();

	if (anchor!= nullptr)
	{
		b2Vec2 position;
		position.x = anchor->GetPosition().x;
		position.y = anchor->GetPosition().y;

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			g_anchor_instance->GetAnchorBody()->GetAngle(),
			{ g_anchor_instance->GetSize().x*scale,g_anchor_instance->GetSize().y*scale}///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
		);


		
	}

	DrawChain();
	
}


void Anchor::Finalize()
{

}





void Anchor::ThrowAnchorToAP()
{
	if (g_anchor_instance == nullptr)
	{
		return;//NULLチェック
	}

	//今のアンカーがある座標の取得
	b2Vec2 anchor_pos=g_anchor_instance->GetAnchorBody()->GetPosition();

	//対象となるAPの座標を取得する
	b2Vec2 to_pos = AnchorPoint::GetTargetAnchorPointBody()->GetPosition();

	b2Vec2 velocity = to_pos - anchor_pos;
	velocity.Normalize(); // 単位ベクトル化して方向を決定
	velocity *= 20; // 投擲速度を設定	

	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(velocity);//ここで力を加えてる

}
	
void Anchor::CreateRotateJoint()
{
	MyContactListener& contact_listener = MyContactListener::GetInstance();

	if (g_anchor_instance == nullptr || g_anchor_instance->GetAnchorBody() == nullptr) {
		return; // アンカーが存在しない場合は何もしない
	}

	b2Body* anchorBody = g_anchor_instance->GetAnchorBody();
	b2Body* targetBody = AnchorPoint::GetTargetAnchorPointBody();

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
}

void Anchor::PullingAnchor(void)
{
	//プレイヤーとアンカーの座標を取得する

	b2Vec2 player_postion = PlayerPosition::GetPlayerPosition();
	b2Vec2 anchor_postion = g_anchor_instance->GetAnchorBody()->GetPosition();

	b2Vec2 velocity = player_postion - anchor_postion;
	velocity.Normalize(); // 単位ベクトル化して方向を決定
	velocity *= 3; // 投擲速度を設定	

	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(velocity);

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



/**
 * @brief 
 * @param chain_size 
 * @param max_chain_number 
 */
void Anchor::CreateChain(b2Vec2 chain_size, int max_chain_number) {
	if (max_chain_number > MAX_CHAIN_NUM) max_chain_number = MAX_CHAIN_NUM; // 鎖のセグメント数の最大値を設定

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	g_anchor_instance->SetChainSize(chain_size);//chianのサイズをセットしておく

	// スケールの調整
	chain_size.x = chain_size.x / BOX2D_SCALE_MANAGEMENT;
	chain_size.y = chain_size.y / BOX2D_SCALE_MANAGEMENT;

	// プレイヤーとアンカーの位置を取得
	b2Vec2 playerPosition = Player::GetOutSidePlayerBody()->GetPosition();//プレイヤーの座標の取得
	b2Vec2 anchorPosition = g_anchor_instance->GetAnchorBody()->GetPosition();//アンカーの座標の取得

	// プレイヤーとアンカーの距離を計算
	float distance = (playerPosition - anchorPosition).Length();//原点からの距離を求める計算式　ここではプレイヤーのとアンカーの距離を求めている

	// chain数を距離に基づいて計算
	int numSegments = static_cast<int>(distance / chain_size.y);//求めた距離をアンカー当たりの一つ分のサイズで割っている
	if (numSegments > max_chain_number) numSegments = max_chain_number; // 最大Chain数に制限
	if (numSegments < 1) numSegments = 1; // 最小Chain数は1

	// chainの開始位置を計算
	b2Vec2 direction = (playerPosition - anchorPosition);//2点間の距離の算出
	direction.Normalize();//距離のの正規化
	b2Vec2 startPosition = anchorPosition + b2Vec2(direction.x * (chain_size.y * 0.5f), direction.y * (chain_size.y * 0.5f));//初期位置の出力　アンカー側だね

	// 初期ボディをアンカーに設定
	b2Body* previousBody = g_anchor_instance->GetAnchorBody();//アンカーのボディの取得

	for (int i = 0; i < numSegments; ++i) {
		// chainのボディ定義
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;//動的ですよと

		// 各chainの位置を計算
		b2Vec2 segmentPosition = startPosition + b2Vec2(direction.x * (chain_size.y * i), direction.y * (chain_size.y * i));//さっきの開始地点の距離の算出を繰り返しているイメージだよ
		bodyDef.position = segmentPosition;
		bodyDef.fixedRotation = false;//回転軸は固定にすると大変なことなる　具体的にはアンカーを起点にしてプレイヤーが上昇していく

		// chainのボディを作成
		b2Body* chainBody = world->CreateBody(&bodyDef);//ワールドにボディを登録していく

		// chainの形状とフィクスチャを設定
		b2PolygonShape shape;
		shape.SetAsBox(chain_size.x * 0.5f, chain_size.y * 0.5f);//サイズのセット

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 2.0f;//ここの密度を上げるとバグりずらいけどチェーン　重くなっちゃうから要相談
		fixtureDef.friction = 0.1f;//反発係数　ここは低い方がいいかもねー
		fixtureDef.isSensor = false;//センサーオンにすると地面に埋まりだして草だぜ

		

		b2Fixture* anchor_chain_fixture =chainBody->CreateFixture(&fixtureDef);//あたり判定

		ObjectData* anchor_chain_data = new ObjectData{ collider_anchor_chain };
		anchor_chain_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(anchor_chain_data);

		// 鎖の配列に格納
		if (i < MAX_CHAIN_NUM) anchor_chain[i] = chainBody;//配列にボディ入れておくよ

		// 前のセグメントと現在のセグメントをジョイントで接続
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = previousBody;//一個前のチェーンのボディ
		jointDef.bodyB = chainBody;	  //今のチェーンのボディ
		jointDef.localAnchorA.Set(0.0f, -chain_size.y * 0.5f);//前のチェーンの軸
		jointDef.localAnchorB.Set(0.0f, chain_size.y * 0.5f); //後のチェーンの軸
		jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

		world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

		// 次のセグメントを準備
		previousBody = chainBody;
	}

	// 最後のchainとプレイヤーを接続
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = previousBody;//チェーンの最後尾のBody
	jointDef.bodyB = Player::GetOutSidePlayerBody();
	jointDef.localAnchorA.Set(0.0f, -chain_size.y * 0.5f);//チェーン側のジョイントの軸
	jointDef.localAnchorB.Set(0.0f, 0.0f);				  //プレイヤーの中心の回転ジョイントの中心に設定
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);                        //ワールドにジョイントを追加
}


void Anchor::DeleteChain() {
	// Box2Dワールドの取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// 作成された鎖の（ボディ）を削除
	for (int i = 0; i < MAX_CHAIN_NUM; ++i) { // 最大40chain
		if (anchor_chain[i]) { // 有効なポインタか確認
			// ボディに関連付けられたすべてのジョイントを削除
			b2JointEdge* jointEdge = anchor_chain[i]->GetJointList();
			while (jointEdge) {
				b2Joint* joint = jointEdge->joint;
				jointEdge = jointEdge->next; // 次のジョイントを保持
				world->DestroyJoint(joint); // ジョイント削除
			}
			// ボディを削除
			world->DestroyBody(anchor_chain[i]);
			anchor_chain[i] = nullptr; // ポインタを無効化
		}
	}

	// 最後にプレイヤーとの接続ジョイントを削除
	b2Body* playerBody = Player::GetOutSidePlayerBody();
	if (playerBody) {
		b2JointEdge* jointEdge = playerBody->GetJointList();
		while (jointEdge) {
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next; // 次のジョイントを保持
			world->DestroyJoint(joint); // ジョイント削除
		}
	}
}

void Anchor::DrawChain() {

	float scale = SCREEN_SCALE;

	for (int i = 0; i < MAX_CHAIN_NUM; ++i) {
		b2Body* chainBody = anchor_chain[i];
		if (chainBody == nullptr) continue;

		b2Vec2 position = chainBody->GetPosition();

		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + SCREEN_CENTER_X;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + SCREEN_CENTER_Y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Chain_Texture);

		// 描画
		DrawSprite(
			{ draw_x, draw_y },
			chainBody->GetAngle(),
			{ g_anchor_instance->GetChainSize().x * scale ,  g_anchor_instance->GetChainSize().y * scale }
		);
	}
}