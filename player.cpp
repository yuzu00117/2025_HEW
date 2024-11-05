//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description プレイヤー
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・現状、Game.cppないで初期化というかプレイヤーのnewをしている
// 
//          
//----------------------------------------------------------------------------------------------------

#include"player.h"
#include"tool.h"
#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"collider_type.h"



Player::Player(XMFLOAT2 position, XMFLOAT2 size, float angle, bool bFixed)
	: SquareCollider(position, size, angle, bFixed)
{
	// プレイヤー専用の初期設定
	SquareCollider::SetColliderType(collider_player);




	if (!bFixed) {
		b2FixtureDef fixture;
		fixture.shape = SquareCollider::GetColliderBody()->GetFixtureList()->GetShape();
		fixture.density = 1.0f;
		fixture.friction = 0.5f;  // プレイヤーに特化した摩擦
		fixture.restitution = 0.1f; // 反発係数の追加

		SquareCollider::GetColliderBody()->CreateFixture(&fixture);
	}
}

Player::~Player()
{

}

void Player::Init() {
	// 初期化処理
	
	//プレイヤー
	
	
}

void Player::Update() {
	// プレイヤーの更新処理



}


void Player::Draw()
{
	
		//コライダーと位置情報の補正をするため
		float scale = 80.f;

		//現実で換算するとい　16m*9にする　つまり画面の真ん中は x.8 y.4.5


		//コライダーの位置の取得
		XMFLOAT2 postion;
		postion.x = SquareCollider::GetColliderBody()->GetPosition().x;
		postion.y = SquareCollider::GetColliderBody()->GetPosition().y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &m_p_Texture);

		//サイズをコンストラクの時点で/100してるから調整のため　＊１００
		DrawSprite({ postion.x * scale,postion.y * scale }, SquareCollider::GetColliderBody()->GetAngle(), { SquareCollider::Get_m_size().x * 100 * scale, SquareCollider::Get_m_size().y * 100 * scale });




		
}

void Player::Uninit() {
	// リソースの解放処理
}