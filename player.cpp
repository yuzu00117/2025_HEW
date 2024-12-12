#include"player.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include"player_position.h"
#include"collider_type.h"
#include"anchor_point.h"
#include"anchor.h"
#include"anchor_spirit.h"
#include"create_filter.h"

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_player_Texture=NULL;


//�Z���T�[�̉摜
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;

//static�����o�[�ϐ��̏�����
bool    Player::m_is_jumping = false;
bool    Player::m_jump_pressed = false;
int     Player::m_direction = 1;



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


    //�v���C���[�̏�̉~�̃R���C�_�[
//-------------------------------------------
    b2CircleShape circle_upper;
    circle_upper.m_p.Set(position.x, position.y);//��̕��̉~
    circle_upper.m_radius = body_size.x / BOX2D_SCALE_MANAGEMENT * 0.5f;

    b2FixtureDef fixture_circle_upper;
    fixture_circle_upper.shape = &circle_upper;
    fixture_circle_upper.density = 1.3f;
    fixture_circle_upper.friction = 3.0f;//���C
    fixture_circle_upper.restitution = 0.0f;//�����W��
    fixture_circle_upper.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
    fixture_circle_upper.filter = createFilterExclude("Player_filter",{});
  
 


    //�v���C���[�̉��̉~�̃R���C�_�[
    //-------------------------------------------
    b2CircleShape circle_bottom;
    circle_bottom.m_p.Set(position.x, position.y + 0.1f);//���̕��̉~
    circle_bottom.m_radius = body_size.x / BOX2D_SCALE_MANAGEMENT * 0.5f;

    b2FixtureDef fixture_circle_bottom;
    fixture_circle_bottom.shape = &circle_bottom;
    fixture_circle_bottom.density = 1.3f;
    fixture_circle_bottom.friction = 3.0f;//���C
    fixture_circle_bottom.restitution = 0.0f;//�����W��
    fixture_circle_bottom.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
    fixture_circle_bottom.filter = createFilterExclude("Player_filter", {});
   
  
    
    //----------------------------------------------------

    //fixture��body�ɓo�^
    b2Fixture* upper_circle_fixture = m_body->CreateFixture(&fixture_circle_upper);
    b2Fixture* bottom_circle_fixture = m_body->CreateFixture(&fixture_circle_bottom);

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* playerdata = new ObjectData{ collider_player };
    //player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
    upper_circle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
    bottom_circle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);

  
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

    //���ړ�
   //----------------------------------------------------------------------------------------------------------------------------------------------------

       //�X�e�B�b�N�̒l���󂯎���Đ��K������
    float left_stick_x = state.leftStickX / 40000.0f;
    float left_stick_y = state.leftStickY / 40000.0f;
    b2Vec2 vel = m_body->GetLinearVelocity();
    b2Vec2 max_velocity = { 1.8f , 1.2f };
    b2Vec2 player_position = { PlayerPosition::GetPlayerPosition().x,PlayerPosition::GetPlayerPosition().y };
    b2Vec2 player_point = m_body->GetWorldPoint(player_position);

    //��Βl�ɕύX���� �f�b�g�]�[���̐R���Ɏg�����߁@tool.cpp�ɍ����
    //�f�b�g�]�[�������� x,y�̒l�𑫂��Ĉ��ȏ�o������@�Ăяo��
    if (0.5f < ReturnAbsoluteValue(left_stick_x) || Keyboard_IsKeyDown(KK_RIGHT) || Keyboard_IsKeyDown(KK_LEFT))
    {
        // ���̃X�e�B�b�N�l���A���K�����ĒP�ʃx�N�g���ɂ���
        b2Vec2 stick = { left_stick_x,  left_stick_y };
        stick.Normalize();

        //��Ԃɂ���ăX�s�[�h�����������
       //----------------------------------------------------------
        float adjust_speed = 0.0f;
        //�W�����v���Ă��鎞�̓X�s�[�h�͔���������
        if (GetIsJumping())
        {
            adjust_speed = -(GetSpeed() / 2);
        }
        //----------------------------------------------------------
        //�E�ړ�
        if ((vel.x < max_velocity.x) && ((stick.x > 0) || (Keyboard_IsKeyDown(KK_RIGHT))))
        {
            m_body->ApplyLinearImpulse({ GetSpeed() + adjust_speed , 0.0f }, player_point, true);
            m_direction = 1;
        }
        //���ړ�
        if ((vel.x > -max_velocity.x) && ((stick.x < 0) || (Keyboard_IsKeyDown(KK_LEFT))))
        {
            m_body->ApplyLinearImpulse({ -(GetSpeed()) + adjust_speed , 0.0f }, player_point, true);
            m_direction = -1;
        }

    }


    //�W�����v����
//----------------------------------------------------------------------------------------------------------------------------------------------------

    //�W�����v�`�F�b�N
    if (!m_is_jumping && !m_jump_pressed && ((Keyboard_IsKeyDown(KK_UP) || (state.buttonA))))
    {
        if (vel.y < max_velocity.y)
        {
            m_body->ApplyLinearImpulse(m_jump_force, { 0.0f,1.0f }, true);
        }
        // m_body->ApplyLinearImpulseToCenter(m_jump_force, true);
        m_is_jumping = true;
    }
    m_jump_pressed = (Keyboard_IsKeyDown(KK_UP) || (state.buttonA));



    //�A���J�[�̃��x�����蓮�ŕς����邵���@�����łł͂���
    if (Keyboard_IsKeyDown(KK_O) || (state.dpadUp))
    {
        AnchorSpirit::EditAnchorSpiritValue(50); //���Z
    }

    if (Keyboard_IsKeyDown(KK_P) || (state.dpadDown))
    {
        AnchorSpirit::EditAnchorSpiritValue(-50); //���Z
    }



    //----------------------------------------------------------------------------------------------------------------------------------------------------

    //�v���C���[�|�W�V����CPP�̊֐��Ƀf�[�^���Z�b�g
    PlayerPosition::SetPlayerPosition(m_body->GetPosition());


 //�A���J�[�̏���
//----------------------------------------------------------------------------------------------------------------------------------------------------


    if ((Keyboard_IsKeyDown(KK_T) || (state.rightTrigger)) && Anchor::GetAnchorState() == Nonexistent_state)//�������݂��Ȃ���ԂŃ{�^�����͂ňڍs����
    {
        if(AnchorPoint::GetTargetAnchorPointBody()->GetPosition()!=m_body->GetPosition())//���݃v���C���[��W���Ƃ��Ă��Ȃ��ꍇ�ł̂����
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

        Anchor::CreateRotateJoint();//��]�W���C���g���쐬
        AnchorSpirit::EditAnchorSpiritValue(-25);//�A���J�[���g������Q�[�W�𕥂�
        Anchor::SetAnchorState(Pulling_state);//���������ԂɈڍs
        break;

    case Pulling_state://���������Ă�����

        //�Ă΂ꂽ�񐔂ł��邩�ˁ@�Ƃ肠����2�b��
        if (g_anchor_pulling_number > 100)
        {
            Anchor::DeleteRotateJoint();
            Anchor::PullingAnchor();
        }


        if ((state.rightTrigger) || (Keyboard_IsKeyDown(KK_G)))
        {
            g_anchor_pulling_number = 200;
        }

        g_anchor_pulling_number++;//�A���J�[����������

        break;

    case Deleting_state://�폜���Ă�����
        g_anchor_pulling_number = 0;
        Anchor::DeleteAnchor();//�A���J�[���폜

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
        stick_y = +1.0f;
    }
    if (Keyboard_IsKeyDown(KK_S))
    {
        stick_y = -1.0f;
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