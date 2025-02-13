//-----------------------------------------------------------------------------------------------------
// #name sprite.cpp
// #description �X�v���C�g
// #make 2024/11/02�@�@�i��`��
// #update 2024/12/13
// #comment �ǉ��E�C���\��
//          �E�Ƃ肠���������I
//          �E�\����Ȃ������g�͈Ӗ��킩���@�������I�����I�@�@�Ȃɂ����������͂킩��
//          �E�A�j���[�V�����Ƃ��͍��̂Ƃ���Ȃ��̂ł���Ȃ炱�����ȁ[���Ċ����@�\��͖���
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "keyboard.h"
#include"display.h"
#include"player_position.h"

//�}�N����`
#define NUM_VERTEX 4
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//�v���g�^�C�v�錾
void SetVertexSprite(void);

//�O���[�o���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;

struct VERTEX_T {
	XMFLOAT2  pos;   //�ʒu�x�N�g��
	XMFLOAT2  vel;	 //���x�x�N�g��
	XMFLOAT4  color; //���_�J���[
	XMFLOAT2  texcoord;
};

static VERTEX_T g_Vertex[NUM_VERTEX];

//����������
void InitSprite(void)
{
	ID3D11Device* pDevice = GetDevice();

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//������
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

	//���_�o�b�t�@�X�V
	SetVertexSprite();
}

//�I������
void UninitSprite(void)
{
	//���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}



//�`�揈��
void DrawBgSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha)
{


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale, box2d_scale, box2d_trans;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	//���������X�P�[��������ǉ����Ă���
	box2d_scale = XMMatrixScaling(display::GetDisplayScale(), display::GetDisplayScale(), 0.0f);

	//�����̒���
	box2d_trans = XMMatrixTranslation(display::GetDisplayWidth(), display::GetDisplayHeight(), 0.0f);



	world = scale * rot * trans * box2d_scale * box2d_trans;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	g_Vertex[0].texcoord = XMFLOAT2(0.f, 0.f);
	g_Vertex[1].texcoord = XMFLOAT2(1.f, 0.f);
	g_Vertex[2].texcoord = XMFLOAT2(0.f, 1.f);
	g_Vertex[3].texcoord = XMFLOAT2(1.f, 1.f);

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

//�`�揈��
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha)
{

	if (Position.x > SCREEN_WIDTH)
	{
		return;
	}

	if (Position.x < -SCREEN_WIDTH)
	{
		return;
	}



	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale,box2d_scale,box2d_trans;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	//���������X�P�[��������ǉ����Ă���
	box2d_scale = XMMatrixScaling(display::GetDisplayScale(), display::GetDisplayScale(), 0.0f);

	//�����̒���
	box2d_trans= XMMatrixTranslation(display::GetDisplayWidth(),display::GetDisplayHeight() , 0.0f);

	

	world = scale * rot * trans*box2d_scale*box2d_trans;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	g_Vertex[0].texcoord = XMFLOAT2(0.f, 0.f);
	g_Vertex[1].texcoord = XMFLOAT2(1.f, 0.f);
	g_Vertex[2].texcoord = XMFLOAT2(0.f, 1.f);
	g_Vertex[3].texcoord = XMFLOAT2(1.f, 1.f);

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

//�`�揈��
void DrawSpriteOld(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha)
{
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);


	world = scale * rot * trans ;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	g_Vertex[0].texcoord = XMFLOAT2(0.f, 0.f);
	g_Vertex[1].texcoord = XMFLOAT2(1.f, 0.f);
	g_Vertex[2].texcoord = XMFLOAT2(0.f, 1.f);
	g_Vertex[3].texcoord = XMFLOAT2(1.f, 1.f);

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

void DrawDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha)
{
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	//����
	int x, y;
	x = Pattern_ID % TotalCols;
	y = Pattern_ID / TotalCols;

	g_Vertex[0].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * (y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * (y + 1));

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


void DrawDividedSpritePlayer(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left)
{

	if (Position.x > SCREEN_WIDTH)
	{
		return;
	}

	if (Position.x < -SCREEN_WIDTH)
	{
		return;
	}

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	float flag = 1.0;
	if (left == false)
	{
		flag = -1.0f;
	}

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale, box2d_scale, box2d_trans;
	scale = XMMatrixScaling(Scale.x * flag, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	//���������X�P�[��������ǉ����Ă���
	box2d_scale = XMMatrixScaling(display::GetDisplayScale(), display::GetDisplayScale(), 0.0f);

	//�����̒���
	box2d_trans = XMMatrixTranslation(display::GetDisplayWidth(), display::GetDisplayHeight(), 0.0f);



	world = scale * rot * trans * box2d_scale * box2d_trans;

	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	//����
	int x, y;
	x = Pattern_ID % TotalCols;
	y = Pattern_ID / TotalCols;

	g_Vertex[0].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * (y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * (y + 1));

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}



void DrawSerialDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int StartPattern_ID, int EndPattern_ID, float Alpha)
{
	//EndPatter_ID��MAX�A�܂�TotalCols�Ɠ����ꍇ�A����end_x��0�ɂȂ����Ⴄ����A�����h�����߂̃C���N�������g����
	TotalCols++;

	//����
	int start_x, start_y;
	start_x = StartPattern_ID % TotalCols;
	start_y = StartPattern_ID / TotalCols;
	int end_x, end_y;
	end_x = EndPattern_ID % TotalCols;
	end_y = EndPattern_ID / TotalCols;

	if (end_y > start_y)
	{
		return;
	}

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);


	g_Vertex[0].texcoord = XMFLOAT2(1.f / TotalCols * start_x, 1.f / TotalRows * end_y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / TotalCols * (end_x + 1), 1.f / TotalRows * end_y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / TotalCols * start_x, 1.f / TotalRows * (end_y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / TotalCols * (end_x + 1), 1.f / TotalRows * (end_y + 1));

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


void DrawDividedSpriteBoss(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left)
{

	if (Position.x > SCREEN_WIDTH)
	{
		return;
	}

	if (Position.x < -SCREEN_WIDTH)
	{
		return;
	}
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	float flag = 1.0;
	if (left == false)
	{
		flag = -1.0f;
	}

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale, box2d_scale, box2d_trans;
	scale = XMMatrixScaling(Scale.x * flag, Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	//���������X�P�[��������ǉ����Ă���
	box2d_scale = XMMatrixScaling(display::GetDisplayScale(), display::GetDisplayScale(), 0.0f);

	//�����̒���
	box2d_trans = XMMatrixTranslation(display::GetDisplayWidth(), display::GetDisplayHeight(), 0.0f);



	world = scale * rot * trans * box2d_scale * box2d_trans;

	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	//����
	int x, y;
	x = Pattern_ID % TotalCols;
	y = Pattern_ID / TotalCols;

	g_Vertex[0].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * (y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * (y + 1));

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}


void DrawSplittingSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha)
{
	if (Position.x > SCREEN_WIDTH)
	{
		return;
	}

	if (Position.x < -SCREEN_WIDTH)
	{
		return;
	}
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//�r���[�}�g���N�X�ݒ�
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	

	//�ړ��E��]�}�g���N�X�ݒ�
	XMMATRIX world, rot, trans, scale, box2d_scale, box2d_trans;
	scale = XMMatrixScaling(Scale.x , Scale.y, 0.0f);
	rot = XMMatrixRotationZ(Rotation);
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	//���������X�P�[��������ǉ����Ă���
	box2d_scale = XMMatrixScaling(display::GetDisplayScale(), display::GetDisplayScale(), 0.0f);

	//�����̒���
	box2d_trans = XMMatrixTranslation(display::GetDisplayWidth(), display::GetDisplayHeight(), 0.0f);



	world = scale * rot * trans * box2d_scale * box2d_trans;

	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha);
	SetMaterial(material);

	//����
	int x, y;
	x = Pattern_ID % TotalCols;
	y = Pattern_ID / TotalCols;

	g_Vertex[0].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * y);
	g_Vertex[1].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * y);
	g_Vertex[2].texcoord = XMFLOAT2(1.f / TotalCols * x, 1.f / TotalRows * (y + 1));
	g_Vertex[3].texcoord = XMFLOAT2(1.f / TotalCols * (x + 1), 1.f / TotalRows * (y + 1));

	SetVertexSprite();

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}





//���_�f�[�^�ݒ�
void SetVertexSprite(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//���_�o�b�t�@�̃f�[�^���X�V����
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertex[i].Position = XMFLOAT3(g_Vertex[i].pos.x, g_Vertex[i].pos.y, 0.0f);
		vertex[i].Diffuse = g_Vertex[i].color;
		vertex[i].TexCoord = g_Vertex[i].texcoord;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
};