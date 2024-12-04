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



void Anchor::CreateAnchorBody(b2Vec2 anchor_size)
{
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

	// 負の角度を正の範囲に調整（0°〜360°）
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
	g_anchor_instance->GetAnchorBody()->SetLinearVelocity(b2Vec2_zero);//とんできたアンカーのベロシティをゼロにする
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



void Anchor::DrawChain() {

	float scale = SCREEN_SCALE;



}




