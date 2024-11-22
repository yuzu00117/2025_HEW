//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description �v���C���[
// #make 2024/11/19
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �E�R���X�g���N�^��body��fixture�����Initialize�ł���
//          �E
//           
//----------------------------------------------------------------------------------------------------




#include"include/box2d/box2d.h"
#include"player.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"collider_type.h"
#include"tool.h"
#include"anchor_point.h"

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_player_Texture;

//�Z���T�[�̉摜
ID3D11ShaderResourceView* g_player_sensor_Texture = NULL;


//�v���[���[�̃|�C���^�[��NULL��
Player* player = nullptr;

b2Body* player_body;

Player::Player(b2Vec2 position, b2Vec2 body_size, b2Vec2 sensor_size) :m_body(nullptr)
{

    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x, position.y);
    body.angle = 0.0f;
    body.fixedRotation = true;//��]���Œ�ɂ���
    body.userData.pointer = (uintptr_t)this;




    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();



    m_body = world->CreateBody(&body);

    player_body = m_body;//�v���C���[�̃{�f�B���Z�b�g


    SetSize(body_size);//�v���C���[�\�������邽�߂ɃZ�b�g����
    SetSensorSize(sensor_size);//�Z���T�[�\�������邽�߂ɃZ�b�g




    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    //�Z���T�[�̐ݒ�p��
    b2Vec2 size_sensor;//�������܂�
    size_sensor.x = sensor_size.x / BOX2D_SCALE_MANAGEMENT;
    size_sensor.y = sensor_size.y / BOX2D_SCALE_MANAGEMENT;


    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5, size.y * 0.5f);



    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.05f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����




    b2Fixture* player_fixture = m_body->CreateFixture(&fixture);

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* playerdata = new ObjectData{ collider_player };
    player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);


    //--------------------------------------------------------------------------------------------------

    //�v���C���[�̃Z���T�[��V��������

    b2PolygonShape shape_sensor;
    shape_sensor.SetAsBox(size_sensor.x * 0.5, size_sensor.y * 0.5);



    b2FixtureDef fixture_sensor;
    fixture_sensor.shape = &shape_sensor;
    fixture_sensor.density = 0.0f;//���x
    fixture_sensor.friction = 0.0f;//���C
    fixture_sensor.restitution = 0.0f;//�����W��
    fixture_sensor.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

    b2Fixture* player_sensor_fixture = m_body->CreateFixture(&fixture_sensor);


    // �J�X�^���f�[�^���쐬���Đݒ�
   // �v���C���[�ɒl��o�^
   // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* player_sensor_data = new ObjectData{ collider_player_sensor };
    player_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(player_sensor_data);

    //---------------------------------------------------------------------------------------------------
}

Player::~Player()
{
    ////�e�N�X�`���̍폜
    //UnInitTexture(g_player_Texture);
}

void Player::Initialize()
{
  
    //�e�N�X�`���̃��[�h
    g_player_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");

    g_player_sensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");

 

}

void Player::Update()
{
    // �v���C���[�̍X�V����


    //�R���g���[���[�̓��͂̎󂯎��
    ControllerState state = GetControllerInput();

    //�E�ړ�
    if (Keyboard_IsKeyDown(KK_RIGHT))
    {
        m_body->ApplyForceToCenter(b2Vec2(0.1, 0.0), true);
    }

    //���ړ�
    if (Keyboard_IsKeyDown(KK_LEFT))
    {
        m_body->ApplyForceToCenter(b2Vec2(-0.1, 0.0), true);
    }


    //�R���g���[���[�ł̎󂯎�� ���ړ�
    m_body->ApplyForceToCenter(b2Vec2(state.leftStickX / 20000, 0.0), true);


    //�W�����v�`�F�b�N
    if (m_can_jump && (Keyboard_IsKeyDown(KK_UP) || (state.buttonA)))
    {

        m_body->ApplyLinearImpulseToCenter(m_jump_force, true);

    }


    //�X�e�B�b�N�̒l���󂯎���Đ��K������
    float stick_x = state.rightStickX / 32768.0f;
    float stick_y = state.rightStickY / 32768.0f;

    //��Βl�ɕύX���� �f�b�g�]�[���̐R���Ɏg�����߁@tool.cpp�ɍ����
    //�f�b�g�]�[�������� x,y�̒l�𑫂��Ĉ��ȏ�o������@�Ăяo��
    if (0.98 < ReturnAbsoluteValue(stick_x) + ReturnAbsoluteValue(stick_y))
    {
        AnchorPoint::SelectAnchorPoint(stick_x, stick_y);
    }

    //�v���C���[�|�W�V����CPP�̊֐��Ƀf�[�^���Z�b�g
    PlayerPosition::SetPlayerPosition(m_body->GetPosition());
}

void Player::Draw()
{

    // �R���C�_�[�ƈʒu���̕␳�����邽��
    float scale = SCREEN_SCALE;

    // �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;

    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_Texture);

    // �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
    b2Vec2 player_position;
    player_position.x = m_body->GetPosition().x;
    player_position.y = m_body->GetPosition().y;


    //�v���C���[�͉�ʒ����ɌŒ�A�܂��␳�l�͂�����₯��
    //�ق���player�ȊO�̕��̂̓v���C���[��Position��������
   // �`��ʒu�𒲐����āA�v���C���[���X�N���[�������ɕ\�������悤�ɂ���
    DrawSprite(
        { screen_center.x,
          screen_center.y },
        m_body->GetAngle(),
        {GetSize().x* scale,GetSize().y * scale }
    );


    //----------------------------------------------------------------------------------------
 //�Z���T�[�`��


 // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_sensor_Texture);

    DrawSprite(
        { screen_center.x,
          screen_center.y },
        m_body->GetAngle(),
        { GetSensorSize().x * scale,GetSensorSize().y * scale }
    );
    float size_sensor = GetSensorSize().x * scale;
    float size = GetSize().x * scale;

}

void Player::Finalize()
{
    if (m_body) {
        // �{�f�B���폜
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;
    }
    if (g_player_Texture != nullptr)
    {
        UnInitTexture(g_player_Texture);
    }
}