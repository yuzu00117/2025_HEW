//----------------------------------------------------------------------------------------------------
// #name　UI_Block.cpp
// #description センサーに接触したらプレイヤーの右上にUIを表示する
// #make 2025/02/09
// #update 2025/02/09
// #comment 追加・修正予定  UIの種類をがんがん足したい
//         
//          
//----------------------------------------------------------------------------------------------------

#include"UI_Block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"anchor_point.h"
#include"player.h"



static ID3D11ShaderResourceView* g_Texture = NULL;//センサーのテクスチャ
static ID3D11ShaderResourceView* g_UI_texture = NULL;//UIのテクスチャ
static ID3D11ShaderResourceView* g_arrow_Texture = NULL;//矢印のテクスチャ


static ID3D11ShaderResourceView* g_move_signboard = NULL;//看板　動き
static ID3D11ShaderResourceView* g_jump_signboard = NULL;//看板　ジャンプ
static ID3D11ShaderResourceView* g_anachor_signboard = NULL;//看板　アンカー
static ID3D11ShaderResourceView* g_soul_signboard = NULL;//看板　ソウル


static ID3D11ShaderResourceView* g_arrow_right_signboard = NULL;//看板　右
static ID3D11ShaderResourceView* g_arrow_right_down_signboard = NULL;//看板　右下
static ID3D11ShaderResourceView* g_arrow_left_signboard = NULL;//看板 左
static ID3D11ShaderResourceView* g_arrow_up_signboard = NULL;//看板 上

static ID3D11ShaderResourceView* g_boss_signboard = NULL;//看板　ボス
static ID3D11ShaderResourceView* g_wood_signboard = NULL;//看板　木
static ID3D11ShaderResourceView* g_rock_signboard = NULL;//看板　岩
static ID3D11ShaderResourceView* g_fall_signboard = NULL;//看板　落ちる岩


//センサーに触れたらUIを表示する
UI_block::UI_block(b2Vec2 Position, b2Vec2 block_size, b2Vec2 Sensor_size, b2Vec2 Sensor_Position, Ui_Block_Type type, float texture_angle)
{

	//サイズをセット
	SetSize(block_size);
	SetSensorSize(Sensor_size);

	b2Vec2 size;

	size.x = Sensor_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = Sensor_size.y / BOX2D_SCALE_MANAGEMENT;






	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();





	b2BodyDef body;//ボディ
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;

	b2Body* m_block_body = world->CreateBody(&body);

	SetBody(m_block_body);


	//フィクスチャを付ける
	b2PolygonShape block_shape;
	
	block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef block_fixture;

	block_fixture.shape = &block_shape;
	block_fixture.density = 3.0f;
	block_fixture.friction = 0.5f;//摩擦
	block_fixture.restitution = 0.0f;//反発係数
	block_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);

	// カスタムデータを作成して設定


	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_UI_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;

	SetID(ID);

	
	m_ui_type = type;
	angle = texture_angle * M_PI / 180.0;

};

UI_block::~UI_block()
{
	Finalize();
}


void UI_block::Initialize()
{
	if (g_Texture == NULL)
	{
		g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//センサーのテクスチャ
		g_arrow_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_arrow.png");//ボディのテクスチャ
		g_UI_texture = InitTexture(L"asset\\texture\\sample_texture\\Sample_ui_a_bottom.png");


		g_move_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_move.png");
		g_jump_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_jump.png");
		g_anachor_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_anchor.png");
		g_soul_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_soul.png");

		//矢印ず
		g_arrow_left_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_arrow_left.png");
		g_arrow_right_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_arrow_right.png");
		g_arrow_right_down_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_arrow_right_down.png");
		g_arrow_up_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_arrow_up.png");


		g_boss_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_boss.png");
		g_wood_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_wood.png");
		g_rock_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_rock.png");
		g_fall_signboard = InitTexture(L"asset\\texture\\signboard_texture\\signboard_fall.png");
	
	
	}


}
void UI_block::Update()
{

	if (m_body != nullptr)
	{
		if (m_flag == true)
		{
			Player& player = Player::GetInstance();
			switch (m_ui_type)
			{
			case NULL_UI_TYPE:
				break;
			case ARROW:
				break;
			case BUTTON_A:
				break;
			case BUTTON_B:
				break;
			case VIDEO_BUTTON_A:
				break;
			case VIDEO_BUTTON_LEFT_STICK:
				break;
			case VIDEO_BUTTON_RIGHT_STICK:
				break;
			case VIDEO_BUTTON_ZR:
				break;
			case MOVE_SIGNBOARD:

				sheet_cnt += 0.3;
				if (90 <= sheet_cnt)
				{
					sheet_cnt = 0;
				}
				break;
			case JUMP_SIGNBOARD:

				sheet_cnt += 0.3;
				if (90 <= sheet_cnt)
				{
					sheet_cnt = 0;
				}

				break;
			case ANCHOR_SIGNBOARD:

				

				//ターゲット出来ている
				if (AnchorPoint::GetTargetAnchorPointBody() != player.GetOutSidePlayerBody())
				{
					sheet_cnt += 0.5;
					if (sheet_cnt < 90)
					{
						sheet_cnt = 90;
					}
					//ループ
					if (160 < sheet_cnt)
					{
						sheet_cnt = 115;
					}
					change_flag = true;
				}
				else if (change_flag == true)
				{
					sheet_cnt += 0.5;
					if (sheet_cnt < 90)
					{
						sheet_cnt = 90;
					}
					//ループ
					if (160 < sheet_cnt)
					{
						sheet_cnt = 115;
					}
				}
				else
				{
					sheet_cnt += 0.5;
					//ループ
					if (84 < sheet_cnt)
					{
						sheet_cnt = 0;
					}
				}

				break;

			case SOUL_SIGNBOARD:

				//ターゲット出来ている
				if (AnchorPoint::GetTargetAnchorPointBody() != player.GetOutSidePlayerBody())
				{
					sheet_cnt += 0.3;
					if (sheet_cnt < 50)
					{
						sheet_cnt = 50;
					}
					//ループ
					if (105 < sheet_cnt)
					{
						sheet_cnt = 80;
					}
					change_flag = true;
				}
				else if (change_flag == true)
				{
					sheet_cnt += 0.5;
					if (sheet_cnt < 50)
					{
						sheet_cnt = 50;
					}
					//ループ
					if (105 < sheet_cnt)
					{
						sheet_cnt = 80;
					}

				}
				else
				{
					sheet_cnt += 0.3;
					//ループ
					if (45 < sheet_cnt)
					{
						sheet_cnt = 0;
					}
				}
				break;

			default:
				break;
			}

			
			
		}

	}
}



void UI_block::Draw()
{

	///ここから調整してね

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 Pos = GetBody()->GetPosition();




	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	//draw
	//DrawSprite(
	//	{ draw_x,
	//	  draw_y },
	//	GetBody()->GetAngle(),
	//	{ GetSensorSize().x * scale,GetSensorSize().y * scale }
	//);

	


	
		Player& player = Player::GetInstance();
		b2Vec2 player_Pos = player.GetOutSidePlayerBody()->GetPosition();



		
		// プレイヤー位置を考慮してスクロール補正を加える
		//取得したbodyのポジションに対してBox2dスケールの補正を加える
		float UI_draw_x = ((player_Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float UI_draw_y = ((player_Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		switch (m_ui_type)
		{
		case NULL_UI_TYPE:
			break;
		case ARROW:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_Texture);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;
		case ARROW_LEFT_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_left_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case ARROW_RIGHT_DOWN_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_right_down_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case ARROW_RIGHT_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_right_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case ARROW_UP_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_up_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;


		case BOSS_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case WOOD_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_wood_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case ROCK_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_rock_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case FALL_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_fall_signboard);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;

		case BUTTON_A:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_UI_texture);
			DrawSprite(
				{ UI_draw_x + 50,
				  UI_draw_x - 50 },
				GetBody()->GetAngle(),
				{ 100,100 }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			);
			break;
		case BUTTON_B:
			break;

		case VIDEO_BUTTON_A:
		case VIDEO_BUTTON_LEFT_STICK:
		case VIDEO_BUTTON_RIGHT_STICK:
		case VIDEO_BUTTON_ZR:
		/*	m_video.Draw({draw_x,draw_y}, m_size.y);*/
			break;
		default:
			break;

		case MOVE_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_move_signboard);

			DrawSplittingSprite(
				{ draw_x ,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale,GetSize().y * scale },
				15, 6, sheet_cnt,1.0f
				);

			break;

		case JUMP_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_jump_signboard);

			DrawSplittingSprite(
				{ draw_x ,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale,GetSize().y * scale },
				15, 6, sheet_cnt, 1.0f
			);

			break;

		case ANCHOR_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_anachor_signboard);

			DrawSplittingSprite(
				{ draw_x ,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale,GetSize().y * scale },
				28, 6, sheet_cnt, 1.0f
			);

			break;

		case SOUL_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_signboard);

			DrawSplittingSprite(
				{ draw_x ,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale,GetSize().y * scale },
				17, 7, sheet_cnt, 1.0f
			);

			break;

		}

	
	



}

void UI_block::Finalize()
{
	if (!m_body) return;

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

	// `b2Body` を削除
	Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
	m_body = nullptr;

	//画像の解放

	if (g_Texture) UnInitTexture(g_Texture);
	if (g_UI_texture) UnInitTexture(g_UI_texture);
	if (g_arrow_Texture) UnInitTexture(g_arrow_Texture);
	if (g_soul_signboard) UnInitTexture(g_soul_signboard);
	

	if (g_arrow_right_signboard) UnInitTexture(g_arrow_right_signboard);
	if (g_arrow_right_down_signboard) UnInitTexture(g_arrow_right_down_signboard);
	if (g_arrow_left_signboard) UnInitTexture(g_arrow_left_signboard);
	if (g_arrow_up_signboard) UnInitTexture(g_arrow_up_signboard);

	if (g_boss_signboard) UnInitTexture(g_boss_signboard);
	if (g_wood_signboard) UnInitTexture(g_wood_signboard);
	if (g_rock_signboard) UnInitTexture(g_rock_signboard);
	if (g_fall_signboard) UnInitTexture(g_fall_signboard);



	

}




