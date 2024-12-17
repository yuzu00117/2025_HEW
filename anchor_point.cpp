//-----------------------------------------------------------------------------------------------------
// #name anchor_point.cpp
// #description �A���J�[�|�C���g���Ǘ����Ă���
// #make 2024/11/22�@�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�A���J�[�|�C���g�𑼂̃I�u�W�F�N�g�ƃW���C���g������
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

#define MAX_ANCHOR_POINT_IN_SENSOR (10)//�Z���T�[���ɑ��݂ł���ő�̃A���J�[�|�C���g�̐�


b2Body* g_anchor_point_body[10];//�A���J�[�|�C���g�̃{�f�B��ݒ�@�O���[�o���ϐ�

b2Body* g_select_anchor_point_body;//�^�[�Q�b�g�ƂȂ�A���J�[�|�C���g�̃{�f�B



//�Z���T�[�̉摜
ID3D11ShaderResourceView* g_anchor_point_target_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev1_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev2_Texture = NULL;
ID3D11ShaderResourceView* g_anchor_point_target_lev3_Texture = NULL;





/**
 * @brief  �t�B�[���h�̃R���X�g���N�^
 * @param position  �o�^������W
 * @param body_size �{�f�B�̑傫���A�֐����ő傫���̒���������
 * @param angle     �p�x��ݒ�
 * @param bFixed
 * @param is_sensor
 * @param texture
 */
AnchorPoint::AnchorPoint(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
{

	//�e�N�X�`���̎��ʗp���Z�b�g
	SetFieldTexture(texture);

	b2BodyDef body;
	body.type = bFixed ? b2_staticBody : b2_dynamicBody;//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�true
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = false;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	SetFieldBody(world->CreateBody(&body));//Body�����[���h�ɌŒ�

	SetSize(body_size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//�����蔻���o�^����4�_�@*0.5����̂�

	b2FixtureDef fixture;
	fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture.density = 1.0f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* ground_fixture = GetFieldBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

	// �J�X�^���f�[�^���쐬���Đݒ�
	// �n�ʂɒl��o�^
	// �n�ʂɃ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_anchor_point };
	ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

AnchorPoint::~AnchorPoint()
{
}


void AnchorPoint::InsideSensor(b2Body* new_anchor_point_body)
{
	//�A���J�[�|�C���g�̃{�f�B���擾������z��ɓ���Ƃ�

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR ; i++)
	{
		if (g_anchor_point_body[i] == nullptr)//�z����`�F�b�N����NULL�������炻���ɑ��
		{
			g_anchor_point_body[i] = new_anchor_point_body;//null�̔z��ɑ��
			return;//����I�������֐��I��
		}
	}
}



void AnchorPoint::OutsideSensor(b2Body* delete_anchor_point_body)
{
	//�Z���T�[����ł��A���J�[�|�C���g�̃{�f�B��z�񂩂�폜����

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR; i++)
	{
		if (g_anchor_point_body[i] == delete_anchor_point_body)//��r���ē����{�f�B�������炻�̃{�f�B�̔z�������
		{
			g_anchor_point_body[i] = nullptr;//��v�����{�f�B�̔z���NULL��

			//�I�����Ă����A���J�[�|�C���g���Z���T�[�O�ɂł�
			if (delete_anchor_point_body == g_select_anchor_point_body)
			{
				//�A���J�[�����������A���J�[�|�C���g���Ԃ����āA�Z���T�[�O�ɂł�ƁA
				//���W�X�V�ŃA���J�[�ƃA���J�[�|�C���g�ƃv���C���[����̉������o�O�������@�����΂���
				//���}���u�Ƃ��āA�A���J�[�|�C���g���W���C���g���ĂȂ��Ƃ��ɔ�������悤�ɂ���
				if (Anchor::GetAnchorCreateJointFlag() != true)
				{
					if (Anchor::GetAnchorState() == Nonexistent_state)//�A���J�[��ł��Ȃ���ړ����Ă鎞�ɃZ���T�[�O�ɂłĂ鎞��NULL���ł�����
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

	//�I�����Ă���A���J�[�|�C���g���Ȃ�������Aplayer��Body����_�ɂ���
	if (g_select_anchor_point_body == nullptr)
	{
		//�A���J�[�����������A���J�[�|�C���g���Ԃ����āA�Z���T�[�O�ɂł�ƁA
		//���W�X�V�ŃA���J�[�ƃA���J�[�|�C���g�ƃv���C���[����̉������o�O�������@�����΂���
		//���}���u�Ƃ��āA�A���J�[�|�C���g���W���C���g���ĂȂ��Ƃ��ɔ�������悤�ɂ���
		if (Anchor::GetAnchorCreateJointFlag() != true)
		{
			Player& player = Player::GetInstance();//�Q�b�g�C���X�^���X
			g_select_anchor_point_body = player.GetOutSidePlayerBody();
		}
	}

	

}

void AnchorPoint::Draw()
{

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	//�Z���T�[�Ȃ��̂��ׂĂ�AP�Ƀ}�[�N������
	//---------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 10; i++)
	{
		if (g_anchor_point_body[i]!=nullptr)
		{
			b2Vec2 position;
			position.x = g_anchor_point_body[i]->GetPosition().x;
			position.y = g_anchor_point_body[i]->GetPosition().y;

			// �v���C���[�ʒu���l�����ăX�N���[���␳��������
			//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
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
				{50 ,50}///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
			);
		}
	}


	if (g_select_anchor_point_body != nullptr) 
	{
		b2Vec2 position;
 		position.x = g_select_anchor_point_body->GetPosition().x;
		position.y = g_select_anchor_point_body->GetPosition().y;

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_point_target_Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			0.0f,
			{ 70 ,70 }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
		);
	}


	//-------------------------------------------------------------------------------------------------------------------------
}
void AnchorPoint::Finalize()
{
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	
	world->DestroyBody(g_select_anchor_point_body);


	//�e�N�X�`���̉��
	UnInitTexture(g_anchor_point_target_lev1_Texture);
	UnInitTexture(g_anchor_point_target_lev2_Texture);
	UnInitTexture(g_anchor_point_target_lev3_Texture);
	UnInitTexture(g_anchor_point_target_Texture);
}

void AnchorPoint::SelectAnchorPoint(float stick_x, float stick_y)
{

	//stick�̍��W���ɂ���
	//��  ���̕���	���̕���
	//X��	�E	    ��
	//Y��	��	    ��

	//�t��Box2d�ł́@Y���͉��ɍs���΁{�ɂȂ�@���̂��ߍ����Ystick��Y���Ƀ}�C�i�X������

	// �X�e�B�b�N�����̃x�N�g��
	if (g_select_anchor_point_body == nullptr) return;//null��������Ԃ�



	// �X�e�B�b�N�����̃x�N�g��
	b2Vec2 stick = { stick_x, -stick_y }; // Box2D �� Y �������ɑΉ�
	stick.Normalize(); // ���K�����ĒP�ʃx�N�g���ɂ���

	b2Vec2 currentPosition = g_select_anchor_point_body->GetPosition();//��_�̒l�������

	b2Body* closestBody = nullptr;//���܌��݈�Ԋp�x���ˋ߂��A���J�[�|�C���g
	float closestDistanceSquared = 100000; // �n���ł��l�������l�Ƃ��Đݒ�@�������炢����
	float Allowable_Angle_MAX = cos(70.0f * M_PI / 180.0f); // 30�x�ȓ��ł���Δ��苖�e���Ă��@�����͒��g�ς�����ϐ��ύX�\�����Ɗɂ����Ă�����

	for (int i = 0; i < MAX_ANCHOR_POINT_IN_SENSOR; i++) {
		if (g_anchor_point_body[i] == nullptr) continue;//NULL��������o��

		b2Fixture* fixtureA = g_anchor_point_body[i]->GetFixtureList();
		auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);

		if (objectA->need_anchor_level > AnchorSpirit::GetAnchorLevel())continue;

		// �e�A���J�[�|�C���g�ւ̃x�N�g��
		b2Vec2 targetPosition = g_anchor_point_body[i]->GetPosition();//�z�������`�F�b�N���Ă���

		b2Vec2 toTarget = targetPosition - currentPosition;//�x�N�g���̎Z�o������ ����b2vec2�^��

		float distanceSquared = toTarget.LengthSquared();//��������l��Ԃ��āA�v�Z�̂�萳�m�ɂ��Ă���

		if (distanceSquared == 0) continue; // �����ʒu�͖�������

		// ���ς��v�Z���Ċp�x���`�F�b�N
		toTarget.Normalize();//Box2d�ł���x�N�g���𐳋K������֐�
		float dotProduct = b2Dot(stick, toTarget);//���ς̌v�Z���@b2Dot��box2d�̊֐�����

		if (dotProduct >= Allowable_Angle_MAX) {
			// �X�e�B�b�N�����ɋ߂�
			if (distanceSquared < closestDistanceSquared) {
				closestDistanceSquared = distanceSquared;//��r�Ɏg���l���X�V
				closestBody = g_anchor_point_body[i];//���̒������ԋ߂����ɍX�V�����Ƃ��`�ɂȂ�
			}
		}
	}

	// �ł��߂��A���J�[�|�C���g��I��
	if (closestBody != nullptr) {
		g_select_anchor_point_body = closestBody;//��_��ύX
	}
}




b2Body* AnchorPoint::GetTargetAnchorPointBody()
{
	return g_select_anchor_point_body;
}


