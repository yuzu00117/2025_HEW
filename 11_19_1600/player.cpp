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

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_player_Texture;



//�v���[���[�̃|�C���^�[��NULL��
Player *player=nullptr;

Player::Player(b2Vec2 position, b2Vec2 body_size) :m_body(nullptr)
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

   
    SetSize(body_size);//�\�������邽�߂ɃZ�b�g����

    b2Vec2 size;
    size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;



    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5 , size.y * 0.5f );

    

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.05f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

    m_body->CreateFixture(&fixture);
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

    if (player == nullptr)
    {
        //�v���C���[�̃C���X�^���X���������I
        player = new Player(b2Vec2(10.0f, 0.0f), b2Vec2(1.0f, 1.0f));
    }

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
    m_body ->ApplyForceToCenter(b2Vec2(state.leftStickX / 800, 0.0), true);


    //�W�����v�`�F�b�N
    if (m_can_jump && (Keyboard_IsKeyDown(KK_UP) || (state.buttonA)))
    {

        m_body->ApplyLinearImpulseToCenter(m_jump_force, true);

    }



    //�v���C���[�|�W�V����CPP�̊֐��Ƀf�[�^���Z�b�g
    PlayerPostion::SetPlayerPostion(m_body->GetPosition());
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