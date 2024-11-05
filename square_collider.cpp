//-----------------------------------------------------------------------------------------------------
// #name square_collider.cpp
// #description 四角のあたり判定
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ここで四角のあたり判定のコンストラクタしてる
// 　　　　 ・ここでほとんどの処理を管理してる
//          ・引数足らない気がするもっといろいろつけれるけどデフォルトはこれとして、これの継承さきで作ろうかなって思ってる（反発係数とかね）
// 　　　　 ・とあいえ結構な情報入っているので、いや分けろよって言われたら　うんってなる　あんまり細分化してもやりずらくねって思ったけど　かえるなら別にいいよ
//          ・スケール周りの処理はもうちょい上手くできる気もするわ
//          
//----------------------------------------------------------------------------------------------------

#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"
#include"box2d_world.h"




//コンストラクション
SquareCollider::SquareCollider(XMFLOAT2 p, XMFLOAT2 s, float angle, bool bFixed)
{
	//インスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();

	//ワールドに登録
	this->m_p_world = box2d_world.GetBox2dWorldPointer();

	//引数からサイズを代入
	//この際に演算の負荷を下げるために　サイズを/100する
	//描画のときに＊１００して調整する
	m_p_size.x = s.x/100;
	m_p_size.y = s.y/100;

	////コライダーのタイプをセット  初期値として地面をセット
	m_p_collider_type = collider_ground;

	////テクスチャをセットしちゃう！
	//SetColliderTexture(m_p_collider_type);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef dBody;
	dBody.type = bFixed ? b2_staticBody : b2_dynamicBody;
	dBody.position.Set(p.x, p.y);
	dBody.angle = angle;
	dBody.userData.pointer = (uintptr_t)this;

	
	

	m_p_body = box2d_world.GetBox2dWorldPointer()->CreateBody(&dBody);
	
	
	// Define another box shape for our dynamic body.
	b2PolygonShape shape;
	shape.SetAsBox(s.x * 0.5f, s.y * 0.5f);

	if (!bFixed) {
		// Define the dynamic body fixture.
		b2FixtureDef fixture;
		fixture.shape = &shape;

		// Set the box density to be non-zero, so it will be dynamic.
		fixture.density = 1.0f;


		//これをオンにするとセンサーになる

		/*fixture.isSensor = true;*/


		// Override the default friction.
		fixture.friction = 0.3f;

		// Add the shape to the body.
		m_p_body->CreateFixture(&fixture);
	}
	else {

		b2FixtureDef fixture;

		m_p_body->CreateFixture(&shape, 0);
	}

	
}
//void SquareCollider::SetColliderTexture(ColliderType collider_type)
//{
//	//コライダーのタイプによってテクスチャを変更　　あくまで試し、引数にテクスチャわたして　情報持たせた方が管理しやすいきがする
//	switch (collider_type)
//	{
//	case collider_enemy:
//		m_p_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
//		break;
//	case collider_player:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_bule.png");
//		break;
//	case collider_anchor:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
//		break;
//	case collider_ground:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
//		break;
//	default:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_black.png");
//		break;
//	}
//}







