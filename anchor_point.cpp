//-----------------------------------------------------------------------------------------------------
// #name anchor_point.cpp
// #description アンカーポイントを管理している
// #make 2024/11/22　　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・アンカーポイントを他のオブジェクトとジョイントしたい
//           
//----------------------------------------------------------------------------------------------------
#include"anchor_point.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"contactlist.h"
#include"sprite.h"
#include"texture.h"
#include"player.h"
#include"player_position.h"
#include"anchor_spirit.h"

#define MAX_ANCHOR_POINT_IN_SENSOR (10)//センサー内に存在できる最大のアンカーポイントの数


b2Body* g_anchor_point_body[10];//アンカーポイントのボディを設定　グローバル変数

b2Body* g_select_anchor_point_body;//ターゲットとなるアンカーポイントのボディ



//センサーの画像
ID3D11ShaderResourceView* g_anchor_point_target_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev1_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev2_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev3_Texture = NULL;





/**
 * @brief  フィールドのコンストラクタ
 * @param position  登録する座標
 * @param body_size ボディの大きさ、関数中で大きさの調整をする
 * @param angle     角度を設定
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
AnchorPoint::AnchorPoint(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
{

	//テクスチャの識別用をセット
	SetFieldTexture(texture);

	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;//静的なオブジェクトにするならtrue
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = false;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetFieldBody(world->CreateBody(&body));//Bodyをワールドに固定

	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 1.0f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* ground_fixture = GetFieldBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 地面に値を登録
	// 地面にユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_anchor_point };
	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

AnchorPoint::~AnchorPoint()
{
}


void AnchorPoint::InsideSensor(b2Body* new_anchor_point_body)
{
	//アンカーポイントのボディを取得したら配列に入れとく

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR ; i++)
	{
		if (g_anchor_point_body[i] == nullptr)//配列をチェックしてNULLだったらそこに代入
		{
			g_anchor_point_body[i] = new_anchor_point_body;//nullの配列に代入
			return;//入れ終わったら関数終了
		}
	}
}



void AnchorPoint::OutsideSensor(b2Body* delete_anchor_point_body)
{
	//センサーからでたアンカーポイントのボディを配列から削除する

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR; i++)
	{
		if (g_anchor_point_body[i] == delete_anchor_point_body)//比較して同じボディだったらそのボディの配列を消す
		{
			g_anchor_point_body[i] = nullptr;//一致したボディの配列をNULLに

			//選択していたアンカーポイントがセンサー外にでた
			if (delete_anchor_point_body == g_select_anchor_point_body)
			{
				//アンカーが当たったアンカーポイントがぶつかって、センサー外にでると、
				//座標更新でアンカーとアンカーポイントとプレイヤーが一体化したバグが発生　おい笑える
				//応急処置として、アンカーポイントがジョイントしてないときに発動するようにした
				if (Anchor::GetAnchorCreateJointFlag() != true)
				{
					if (Anchor::GetAnchorState() == Nonexistent_state)//アンカーを打ちながら移動してる時にセンサー外にでてる時にNULLがでたから
					{
						g_select_anchor_point_body = nullptr;
					}
				}
			}
			return;
		}
	}
}

void AnchorPoint::Initialize()
{
	g_anchor_point_target_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_purple.png");
	g_anchor_point_target_lev1_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_anchorpoint_lev1.png");
	g_anchor_point_target_lev2_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_anchorpoint_lev2.png");
	g_anchor_point_target_lev3_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_anchorpoint_lev3.png");
}

void AnchorPoint::Update()
{

	//選択しているアンカーポイントがなかったら、playerのBodyを基準点にする
	if (g_select_anchor_point_body == nullptr)
	{
		//アンカーが当たったアンカーポイントがぶつかって、センサー外にでると、
		//座標更新でアンカーとアンカーポイントとプレイヤーが一体化したバグが発生　おい笑える
		//応急処置として、アンカーポイントがジョイントしてないときに発動するようにした
		if (Anchor::GetAnchorCreateJointFlag() != true)
		{
			Player& player = Player::GetInstance();//ゲットインスタンス
			g_select_anchor_point_body = player.GetOutSidePlayerBody();
		}
	}

	

}

void AnchorPoint::Draw()
{

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	//センサーないのすべてのAPにマークをつける
	//---------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 10; i++)
	{
		if (g_anchor_point_body[i]!=nullptr)
		{
			b2Vec2 position;
			position.x = g_anchor_point_body[i]->GetPosition().x;
			position.y = g_anchor_point_body[i]->GetPosition().y;

			// プレイヤー位置を考慮してスクロール補正を加える
			//取得したbodyのポジションに対してBox2dスケールの補正を加える
			float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		
			

			b2Fixture*fixtureA= g_anchor_point_body[i]->GetFixtureList();
			auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);

			switch (objectA->need_anchor_level)
			{
			case 0:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_Texture);
				break;
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_lev1_Texture);
				break;
			case 2:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_lev2_Texture);
				break;
			case 3:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_lev3_Texture);
				break;
			default:
				break;
			}


		
			
			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				g_anchor_point_body[i]->GetAngle(),
				{50 ,50}///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			);
		}
	}


	if (g_select_anchor_point_body != nullptr) 
	{
		b2Vec2 position;
 		position.x = g_select_anchor_point_body->GetPosition().x;
		position.y = g_select_anchor_point_body->GetPosition().y;

		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			0.0f,
			{ 70 ,70 }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
		);
	}


	//-------------------------------------------------------------------------------------------------------------------------
}
void AnchorPoint::Finalize()
{
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	
	world->DestroyBody(g_select_anchor_point_body);


	//テクスチャの解放
	UnInitTexture(g_anchor_point_target_lev1_Texture);
	UnInitTexture(g_anchor_point_target_lev2_Texture);
	UnInitTexture(g_anchor_point_target_lev3_Texture);
	UnInitTexture(g_anchor_point_target_Texture);
}

void AnchorPoint::SelectAnchorPoint(float stick_x, float stick_y)
{

	//stickの座標軸について
	//軸  正の方向	負の方向
	//X軸	右	    左
	//Y軸	上	    下

	//逆にBox2dでは　Y軸は下に行けば＋になる　このため今回はYstickのY軸にマイナスをつける

	// スティック方向のベクトル
	if (g_select_anchor_point_body == nullptr) return;//nullだったら返す



	// スティック方向のベクトル
	b2Vec2 stick = { stick_x, -stick_y }; // Box2D の Y 軸方向に対応
	stick.Normalize(); // 正規化して単位ベクトルにする

	b2Vec2 currentPosition = g_select_anchor_point_body->GetPosition();//基準点の値をいれる

	b2Body* closestBody = nullptr;//いま現在一番角度がね近いアンカーポイント
	float closestDistanceSquared = 100000; // 馬鹿でか値を初期値として設定　そしたらいける
	float Allowable_Angle_MAX = cos(70.0f * M_PI / 180.0f); // 30度以内であれば判定許容してやる　ここは中身変えたら変数変更可能もっと緩くしてもいい

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR; i++) {
		if (g_anchor_point_body[i] == nullptr) continue;//NULLだったら出る

		b2Fixture* fixtureA = g_anchor_point_body[i]->GetFixtureList();
		auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);

		if (objectA->need_anchor_level > AnchorSpirit::GetAnchorLevel())continue;

		// 各アンカーポイントへのベクトル
		b2Vec2 targetPosition = g_anchor_point_body[i]->GetPosition();//配列を一つずつチェックしていく

		b2Vec2 toTarget = targetPosition - currentPosition;//ベクトルの算出をする 両方b2vec2型ね

		float distanceSquared = toTarget.LengthSquared();//二条した値を返して、計算のより正確にしている

		if (distanceSquared == 0) continue; // 同じ位置は無視する

		// 内積を計算して角度をチェック
		toTarget.Normalize();//Box2dであるベクトルを正規化する関数
		float dotProduct = b2Dot(stick, toTarget);//内積の計算式　b2Dotはbox2dの関数だよ

		if (dotProduct >= Allowable_Angle_MAX) {
			// スティック方向に近い
			if (distanceSquared < closestDistanceSquared) {
				closestDistanceSquared = distanceSquared;//比較に使う値を更新
				closestBody = g_anchor_point_body[i];//候補の中から一番近い物に更新したとう形になる
			}
		}
	}

	// 最も近いアンカーポイントを選択
	if (closestBody != nullptr) {
		g_select_anchor_point_body = closestBody;//基準点を変更
	}
}




b2Body* AnchorPoint::GetTargetAnchorPointBody()
{
	return g_select_anchor_point_body;
}


