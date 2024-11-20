#include "polygon.h"
#include "keyboard.h"

//�}�N����`
#define NUM_VERTEX 4
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//�v���g�^�C�v�錾
void SetVertex(void);

//�O���[�o���ϐ�
static ID3D11Buffer*  g_VertexBuffer = NULL;
static ID3D11ShaderResourceView* g_Texture = NULL;
static float g_rot = 0.0f;
static XMFLOAT2 g_scale = {1.0f, 1.0f};
static XMFLOAT2 g_pos = {0.0f, 0.0f};
static int g_frame = 0;

static int g_pattern = 0;

static bool g_oldKeyStateSpace = false;

struct VERTEX_T {
	XMFLOAT2  pos;   //�ʒu�x�N�g��
	XMFLOAT2  vel;	 //���x�x�N�g��
	XMFLOAT4  color; //���_�J���[
	XMFLOAT2 texcoord;
};

static VERTEX_T g_Vertex[NUM_VERTEX];

//����������
void InitPolygon(void)
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

	//��ʉ����ꂽ��p�`
	/*for (int i = 0; i < NUM_VERTEX; i++) {
		g_Vertex[i].pos.x = RADIUS * sin(INNNER_RAD * i) + SCREEN_WIDTH * 0.5f;
	}*/









	//�l�p�`
	g_Vertex[0].pos.x = -50.0f;
	g_Vertex[0].pos.y = -100.0f;
	g_Vertex[0].color = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	g_Vertex[0].texcoord = XMFLOAT2(0.0f, 0.0f);

	g_Vertex[1].pos.x = 50.0f;
	g_Vertex[1].pos.y = -100.0f;
	g_Vertex[1].color = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	g_Vertex[1].texcoord = XMFLOAT2(1.0f, 0.0f);

	g_Vertex[2].pos.x = -50.0f;
	g_Vertex[2].pos.y = 100.0f;
	g_Vertex[2].color = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	g_Vertex[2].texcoord = XMFLOAT2(0.0f, 1.0f);

	g_Vertex[3].pos.x = 50.0f;
	g_Vertex[3].pos.y = 100.0f;
	g_Vertex[3].color = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	g_Vertex[3].texcoord = XMFLOAT2(1.0f, 1.0f);

	//���^(NUM_VERTEX��5)
	/*
	for (int i = 0; i < NUM_VERTEX; i++) {
		g_Vertex[i].pos.x = 200 * (float)(cos((0.8f * i - 0.1f) * M_PI)) + SCREEN_WIDTH * 0.5f;
		g_Vertex[i].pos.y = 200 * (float)(sin((0.8f * i - 0.1f) * M_PI)) + SCREEN_HEIGHT * 0.5f;
		g_Vertex[i].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	*/
	//�~(NUM_VERTEX��361)
	/*
	for (int i = 0; i < NUM_VERTEX; i++) {
		g_Vertex[i].pos.x = 300 * (float)(cos(M_PI * i / 180)) + SCREEN_WIDTH * 0.5f;
		g_Vertex[i].pos.y = 300 * (float)(sin(M_PI * i / 180)) + SCREEN_HEIGHT * 0.5f;
		g_Vertex[i].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	*/



	//���_�o�b�t�@�X�V
	SetVertex();

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\runningman000.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);
}

//�I������
void UninitPolygon(void)
{
	//���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	//�e�N�X�`���̉��
	if (g_Texture) {
		g_Texture->Release();
		g_Texture = NULL;
	}
}

//�X�V����
void UpdatePolygon(void) 
{

}

//�`�揈��
void DrawPolygon(void)
{
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�}�g���N�X�ݒ�
	//SetWorldViewProjection2D();//���W��2D�ϊ�

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
	scale = XMMatrixScaling(g_scale.x, g_scale.y, 0.0f);
	rot = XMMatrixRotationZ(g_rot);
	trans = XMMatrixTranslation(SCREEN_XCENTER + g_pos.x, SCREEN_YCENTER + g_pos.y, 0.0f);
	world = scale * rot * trans;
	SetWorldMatrix(world);

	//�v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�i���N��Ɍ����j
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	g_Vertex[0].texcoord = XMFLOAT2{ g_pattern * 0.125f, 0.0f };
	g_Vertex[1].texcoord = XMFLOAT2{ (g_pattern + 1) * 0.125f, 0.0f };
	g_Vertex[2].texcoord = XMFLOAT2{ g_pattern * 0.125f, 1.0f };
	g_Vertex[3].texcoord = XMFLOAT2{ (g_pattern + 1) * 0.125f, 1.0f };

	SetVertex();

	//�e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	//�|���S���`��
	GetDeviceContext()->Draw(NUM_VERTEX, 0);
}

//���_�f�[�^�ݒ�
void SetVertex(void)
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
}