//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description �Q�[��
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
//           
//----------------------------------------------------------------------------------------------------

#include"tool.h"
#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"collider_type.h"
#include"feild.h"


// 2�����z��̐ÓI�����o�̏�����
Field*** Field::m_p_field_array = nullptr;
int Field::m_field_height = 0;
int Field::m_field_width = 0;

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

Field::Field(XMFLOAT2 position, XMFLOAT2 size, float angle, bool bFixed,bool is_sensor,bool in_piece)
	: SquareCollider(position, size, angle, bFixed)
{
	// field��p�̏����ݒ�
	SquareCollider::SetColliderType(collider_ground);


	if (!bFixed) {
		b2FixtureDef fixture;
		fixture.shape = SquareCollider::GetColliderBody()->GetFixtureList()->GetShape();
		fixture.density = 1.0f;
		fixture.friction = 0.5f;  // �v���C���[�ɓ����������C
		fixture.restitution = 0.1f; // �����W���̒ǉ�

		

		SquareCollider::GetColliderBody()->CreateFixture(&fixture);
	}
}

Field::~Field()
{
}

 void Field::Init(int field_width, int field_height)
{
	//�e�N�X�`���̏�����
	 g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

	 if (g_Texture == nullptr) {
		 
		 //�e�X�g�p�@���ꂤ�������Ȃ����ǂ�����
		 int i = 0;
	 }
	m_field_width = field_width;
	m_field_height = field_height;

	//2�����z��̃������m��

	m_p_field_array = new Field * *[field_height]; // �c�����̔z����m��

	for (int y = 0; y < field_height; ++y) {
		m_p_field_array[y] = new Field * [field_width]; // �������̔z����e�s���ƂɊm��

		for (int x = 0; x < field_width; ++x) {
			m_p_field_array[y][x] = nullptr; // �e�v�f�� nullptr �ŏ�����
		}
	}

	int field_map[20][30] =
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	};

	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (field_map[y][x] == 1) {
				m_p_field_array[y][x] = new Field(XMFLOAT2(1 * x, 1 * y), XMFLOAT2(1.0f, 1.0f), 0.0f, true, true, true);
			}
		}
	}
	
}

void Field::Update()
{

}

void Field::Draw()
{

	//�R���C�_�[�ƈʒu���̕␳�����邽��
	float scale = 80.f;

	//�����Ŋ��Z����Ƃ��@16m*9�ɂ���@�܂��ʂ̐^�񒆂� x.8 y.4.5

	for (int y = 0; y < m_field_height; ++y) {
		for (int x = 0; x < m_field_width; ++x) {
			if (m_p_field_array[y][x] != nullptr) {
				XMFLOAT2 position;
				position.x = m_p_field_array[y][x]->GetColliderBody()->GetPosition().x;
				position.y = m_p_field_array[y][x]->GetColliderBody()->GetPosition().y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
				DrawSprite(
					{ position.x * scale, position.y * scale },
					m_p_field_array[y][x]->GetColliderBody()->GetAngle(),
					{ m_p_field_array[y][x]->Get_m_size().x * 100 * scale, m_p_field_array[y][x]->Get_m_size().y * 100 * scale }
				);
			}
		}
	}


}

void Field::Uninit()
{
	// 2�����z��̃��������
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {

				delete m_p_field_array [y][x];
				m_p_field_array [y][x] = nullptr;
			}
		}
		delete[] m_p_field_array [y];
	}
	delete[] m_p_field_array;
	m_p_field_array= nullptr;
}