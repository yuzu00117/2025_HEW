//-----------------------------------------------------------------------------------------------------
// #name sprite.cpp
// #description スプライト
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・とりあえず動く！
//          ・申し訳ないが中身は意味わからん　動くぞ！こいつ！　　なにがしたいかはわかる
//          ・アニメーションとかは今のところないのでつくるならここかなーって感じ　予定は未定
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "keyboard.h"

//マクロ定義
#define NUM_VERTEX 4
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//プロトタイプ宣言
void SetVertexSprite(void);

//グローバル変数
static ID3D11Buffer* g_VertexBuffer = NULL;

struct VERTEX_T {
	XMFLOAT2  pos;   //位置ベクトル
	XMFLOAT2  vel;	 //速度ベクトル
	XMFLOAT4  color; //頂点カラー
	XMFLOAT2  texcoord;
};

static VERTEX_T g_Vertex[NUM_VERTEX];

//初期化処理
void InitSprite(void)
{
	ID3D11Device* pDevice = GetDevice();

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//初期化
	g_Vertex[0].pos.x = -0.5f;
	g_Vertex[0].pos.y = -0.5f;
	g_Vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[0].texcoord = XMFLOAT2(0.0f, 0.0f);

	g_Vertex[1].pos.x = 0.5f;
	g_Vertex[1].pos.y = -0.5f;
	g_Vertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[1].texcoord = XMFLOAT2(1.0f, 0.0f);

	g_Vertex[2].pos.x = -0.5f;
	g_Vertex[2].pos.y = 0.5f;
	g_Vertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[2].texcoord = XMFLOAT2(0.0f, 1.0f);

	g_Vertex[3].pos.x = 0.5f;
	g_Vertex[3].pos.y = 0.5f;
	g_Vertex[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[3].texcoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ更新
	SetVertexSprite();
}

//終了処理
void UninitSprite(void)
{
	//頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//描画処理
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale)
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プロジェクションマトリクス設定
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//移動・回転マトリクス設定
	XMMATRIX world, rot, trans, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定（半年後に現れる）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	g_Vertex[0].texcoord = XMFLOAT2(0.f, 0.f);
	g_Vertex[1].texcoord = XMFLOAT2(1.f, 0.f);
	g_Vertex[2].texcoord = XMFLOAT2(0.f, 1.f);
	g_Vertex[3].texcoord = XMFLOAT2(1.f, 1.f);

	SetVertexSprite();

	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

void DrawSpriteAnim(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern)
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プロジェクションマトリクス設定
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//移動・回転マトリクス設定
	XMMATRIX world, rot, trans, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定（半年後に現れる）
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//アニメーション
	int x, y;
	x = AnimPattern % AnimCols;
	y = AnimPattern / AnimCols;

	g_Vertex[0].texcoord = XMFLOAT2(1.f / AnimCols * x, 1.f / AnimRows * y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / AnimCols * (x + 1), 1.f / AnimRows * y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / AnimCols * x, 1.f / AnimRows * (y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / AnimCols * (x + 1), 1.f / AnimRows * (y + 1));

	SetVertexSprite();

	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}



//頂点データ設定
void SetVertexSprite(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点バッファのデータを更新する
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertex[i].Position = XMFLOAT3(g_Vertex[i].pos.x, g_Vertex[i].pos.y, 0.0f);
		vertex[i].Diffuse = g_Vertex[i].color;
		vertex[i].TexCoord = g_Vertex[i].texcoord;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
};