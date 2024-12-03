//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description �v���C���[
// #make 2024/11/22�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�R���X�g���N�^��body��fixture�����GetInstance�ŏ����l�����
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
#include"anchor_point.h"
#include"anchor.h"

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_player_Texture=NULL;


//�Z���T�[�̉摜
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;


bool    Player::m_can_jump = false;

b2Body* player_body;


int g_anchor_pulling_number = 0;

Player::Player(b2Vec2 position, b2Vec2 body_size,b2Vec2 sensor_size) :m_body(nullptr)
{

    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x , position.y);
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
    size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;


    //�Z���T�[�̐ݒ�p��
    b2Vec2 size_sensor;//�������܂�
    size_sensor.x=sensor_size.x / BOX2D_SCALE_MANAGEMENT;
    size_sensor.y=sensor_size.y / BOX2D_SCALE_MANAGEMENT;


    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5 , size.y * 0.5f );

    

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.05f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

  
    
   
    b2Fixture* player_fixture =m_body->CreateFixture(&fixture);

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* playerdata = new ObjectData{collider_player};
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

    g_player_sensor_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");


    

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
    m_body ->ApplyForceToCenter(b2Vec2(state.leftStickX / m_speed, 0.0), true);


    //�W�����v�`�F�b�N
    if (m_can_jump &&( (Keyboard_IsKeyDown(KK_UP) || (state.buttonA))))
    {

        m_body->ApplyLinearImpulseToCenter(m_jump_force, true);

    }

 //�A���J�[�̏���
//----------------------------------------------------------------------------------------------------------------------------------------------------

    if ((Keyboard_IsKeyDown(KK_T) || (state.buttonB))&&Anchor::GetAnchorState()==Nonexistent_state)//�������݂��Ȃ���ԂŃ{�^�����͂ňڍs����
    {
        Anchor::SetAnchorState(Create_state);//�쐬��ԂɈڍs
    }

    switch (Anchor::GetAnchorState())
    {
    case Nonexistent_state://�����Ȃ����
        //��������̈ڍs�͏�̃{�^���ŊǗ�
        break;
    case Create_state:
        Anchor::CreateAnchor(b2Vec2(2.0f, 2.0f));//�����̈����ŃA���J�[�̑傫���̒������ł����[
        Anchor::SetAnchorState(Throwing_state);//�A���J�[�̏�Ԃ𓊂���Ă����ԂɈڍs
        break;
    case Throwing_state://�d�����ł�����
        Anchor::ThrowAnchorToAP();//�A���J�[���^�[�Q�b�g�Ƃ����A���J�[�|�C���g�Ɍ������ē�����֐�

        
        //�����̓R���^�N�g���X�g�Ȃ��̐ڐG���肩��ڐG��Ԃւƈڍs
        break;
    case Connected_state://���̂�����������ԁ@�W���C���g�̍쐬

        Anchor::CreateChain(b2Vec2(0.3f, 1.0f),40);

        Anchor::CreateRotateJoint();//��]�W���C���g���쐬
        Anchor::SetAnchorState(Pulling_state);//���������ԂɈڍs
        break;

    case Pulling_state://���������Ă�����
        //Anchor::PullingAnchor();//�Ԃ������A���J�[����������
        //�����̔���̎d���ǂ����悤���ȁH�H
        //�Ă΂ꂽ�񐔂ł��邩�ˁ@�Ƃ肠����2�b��

        if (g_anchor_pulling_number > 1200)
        {
            Anchor::SetAnchorState(Deleting_state);//��Ԃ��A���J�[���폜�����ԂɈڍs

            g_anchor_pulling_number = 0;//�l�����Z�b�g
        }
        g_anchor_pulling_number++;

        if ((state.buttonY)||(Keyboard_IsKeyDown(KK_G)))
        {
            g_anchor_pulling_number = 0;//�l�����Z�b�g
            Anchor::SetAnchorState(Deleting_state);//��Ԃ��A���J�[���폜�����ԂɈڍs
        }
        

        break;

    case Deleting_state://�폜���Ă�����
        Anchor::DeleteAnchor();//�A���J�[���폜
        Anchor::DeleteChain();
        Anchor::SetAnchorState(Nonexistent_state);

        break;

    default:
        break;
    }

 //---------------------------------------------------------------------------------------------------------------------------------------------------------------------  

    //�X�e�B�b�N�̒l���󂯎���Đ��K������
    float stick_x= state.rightStickX / 32768.0f;
    float stick_y= state.rightStickY / 32768.0f;

    //keybord�ł̃A���J�[�|�C���g�̐ݒ� X��
    if (Keyboard_IsKeyDown(KK_A))
    {
        stick_x = -1.0f;
    }
    if (Keyboard_IsKeyDown(KK_D))
    {
        stick_x = 1.0f;
    }

    //keybord�ł̃A���J�[�|�C���g�̐ݒ�@�x��
    if (Keyboard_IsKeyDown(KK_W))
    {
        stick_y = -1.0f;
    }
    if (Keyboard_IsKeyDown(KK_S))
    {
        stick_y = 1.0f;
    }
   


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


//�{�f�B���O������擾���邽�߂ɍ�����֐�
b2Body* Player::GetOutSidePlayerBody()
{
    return player_body;
}