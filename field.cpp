//-----------------------------------------------------------------------------------------------------
// #name field.cpp
// #description csv��p���ă}�b�v�`�b�v���쐬���A�`�悷��t�@�C��
// #make 2024/11/04�@�i��`��
// #update 2024/12/01
// #comment �ǉ��E�C���\��
//          �EField�̐ݒ�����Ă�  �Ăяo���̎d���Ƃ��Ă��X�R�[�v�������Z�q�g���Ă���� �iField::Draw)
//			�E�}�b�v���Ǘ�������N���X�ŃO�����h�Ȃǂ��p�����Ă���
//			�E12/01 �t�B�[���h�̃}�b�v�`�b�v��csv��p�����`��ɕύX
//				�E�}�b�v�̕ύX��csv�t�@�C����ҏW���Ă�������
//----------------------------------------------------------------------------------------------------
#include"tool.h"
#include"include/box2d/box2d.h"
#include"field.h"
#include"player_position.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"collider_type.h"
#include"ground.h"
#include"anchor_point.h"



// 2�����z��̐ÓI�����o�̏�����
Field*** Field::m_p_field_array = nullptr;

// csv�t�@�C������ǂݍ��񂾃}�b�v�f�[�^�̈ꎞ�i�[�p
std::vector<std::vector<int>> Field::m_field_data;

// �t�B�[���h�̕��ƍ�����ێ�����ÓI�����o�ϐ���������
// �N���X�S�̂ŋ��L�����ϐ��Ƃ��Ďg�p���邽�߁A�����ŏ�����
int Field::m_field_width = 0;
int Field::m_field_height = 0;

// �g�p����e�N�X�`���t�@�C�����i�[
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//�n�ʂ̃e�N�X�`��
static ID3D11ShaderResourceView* g_AnchorPoint_Texture = NULL;//�A���J�[�|�C���g�̃e�N�X�`��



Field::Field()
{
}



Field::~Field()
{
}



//������
void Field::Initialize(int field_width, int field_height)
{
	
	//�e�N�X�`���̏�����
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//�O���E���h�̃e�N�X�`��
	g_AnchorPoint_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//�A���J�[�|�C���g�̃e�N�X�`��

	//AP�̃C�j�V�����C�Y
	AnchorPoint::Initialize();

	// csv����}�b�v�`�b�v��ǂݍ���
	Field::LoadCSV("asset/mapchip.csv");
	//�ǂݍ��񂾃f�[�^��field_map�Ɋi�[
	std::vector<std::vector<int>> field_map = m_field_data;

	//�}�b�v�Ɋ�Â���2�����z��̃������m��
	m_p_field_array = new Field * *[field_height]; // �c�����̔z����m��

	for (int y = 0; y < field_height; ++y) {
		m_p_field_array[y] = new Field * [field_width]; // �������̔z����e�s���ƂɊm��

		for (int x = 0; x < field_width; ++x) {
			m_p_field_array[y][x] = nullptr; // �e�v�f�� nullptr �ŏ�����
		}
	}

	// �}�b�v�̐��l�ɉ������I�u�W�F�N�g��`�悷�邽�߁A�Ή������I�u�W�F�N�g�̃C���X�^���X�𐶐�
	for (int y = 0; y < field_height; ++y) {
		for (int x = 0; x < field_width; ++x) {
			if (field_map[y][x] == 1) {
				//Size�� BOX2D_SCALE_MANAGEMENT�Ŋ����Ă�e���Ł@���W�̓o�^�ʒu������
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
			if (field_map[y][x] == 2) {
				m_p_field_array[y][x] = new AnchorPoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, anchor_point_texture);
			}
			if (field_map[y][x] == 3) {
				m_p_field_array[y][x] = new AnchorPoint(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 5.0f), 0.0f, false, true, anchor_point_texture);
			}
			if (field_map[y][x] == 4) {
				m_p_field_array[y][x] = new Ground(b2Vec2(x / BOX2D_SCALE_MANAGEMENT, y / BOX2D_SCALE_MANAGEMENT), b2Vec2(1.0f, 1.0f), 0.0f, true, true, ground_texture);
			}
		}
	}
}



void Field::Update()
{
	//�A���J�[�|�C���g�̍X�V
	AnchorPoint::Update();
}



void Field::Draw()
{
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	//m_p_field_array �̊e�ʒu�ɑΉ�����t�B�[���h�I�u�W�F�N�g��`�悵�܂��B
	for (int y = 0; y < m_field_height; ++y) {
		for (int x = 0; x < m_field_width; ++x) {
			if (m_p_field_array[y][x] != nullptr) {
				b2Vec2 position;
				position.x = m_p_field_array[y][x]->GetFieldBody()->GetPosition().x ;
				position.y = m_p_field_array[y][x]->GetFieldBody()->GetPosition().y ;

				// �v���C���[�ʒu���l�����ăX�N���[���␳��������
				//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
				float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y)*BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				//�\��e�N�X�`�����w��
				switch (m_p_field_array[y][x]->GetFieldTexture())
				{
				case anchor_point_texture:
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_AnchorPoint_Texture);
					break;
				case ground_texture:
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);
					break;
				default:
					break;
				}
				
				//draw
				DrawSprite(
					{ draw_x,
					  draw_y },
					m_p_field_array[y][x]->GetFieldBody()->GetAngle(),
					{ m_p_field_array[y][x]->GetSize().x * scale , m_p_field_array[y][x]->GetSize().y * scale }
				);
			}
		}
	}
	// �A���J�[�|�C���g��`��
	AnchorPoint::Draw();
}



void Field::Finalize()
{
	// 2�����z��̃��������
	for (int y = 0; y < m_field_height; ++y) {

		for (int x = 0; x < m_field_width; ++x) {

			if (m_p_field_array[y][x] != nullptr) {

				delete m_p_field_array[y][x];
				m_p_field_array[y][x] = nullptr;
			}
		}
		delete[] m_p_field_array[y];
	}
	delete[] m_p_field_array;
	m_p_field_array = nullptr;
}



//csv�t�@�C���̓ǂݍ��݂��s���֐�
//csv�t�@�C����ǂݍ���ŁA���̓��e�� m_field_data �Ɋi�[���܂��B
bool Field::LoadCSV(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open CSV file" << filename << std::endl;
		return false;
	}

	std::string line;
	m_field_data.clear(); //�ȑO�̃f�[�^���N���A

	//�t�@�C���̊e�s��ǂݍ���`
	while (std::getline(file, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string cell;

		//�e�Z����ǂݍ��݁A�J���}�ŋ�؂�ꂽ���l���擾
		while (std::getline(ss, cell, ','))
		{
			try
			{
				row.push_back(std::stoi(cell)); // ���l�ɕϊ����Ēǉ�
			}
			catch(const std::invalid_argument& e)
			{
				std::cerr << "Invalid data in CSV" << cell << std::endl;
				file.close();
				return false;
			}
		}
		m_field_data.push_back(row);
	}
	file.close();

	// �����ƕ���CSV�̃f�[�^����擾
    m_field_height = m_field_data.size();  // �s�����t�B�[���h�̍���
    m_field_width = (m_field_data.empty() ? 0 : m_field_data[0].size());  // �ŏ��̍s�̗񐔂��t�B�[���h�̕�
	return true;
}