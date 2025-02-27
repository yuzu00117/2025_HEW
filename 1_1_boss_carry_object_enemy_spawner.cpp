//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy_spawner.cpp
// #description�@�{�X�̏����v�J�v�J���Ă�G�l�~�[�I�u�W�F�N�g�������Ăė��Ƃ���G�l�~�[�̃X�|�i�[
// #make 2025/02/01�@�i��`��
// #update 2025/02/01
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------



#include"texture.h"
#include"sprite.h"
#include"1_1_boss_carry_object_enemy_spawner.h"
#include"1-1_boss_field_block.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"object_manager.h"



static ID3D11ShaderResourceView* g_Texture = NULL;//�n�ʂ̃e�N�X�`��
boss_carry_object_spawner::boss_carry_object_spawner(b2Vec2 position, b2Vec2 Size, Boss_Room_Level level,bool left)
{
    
    SetSize(Size);

    //�傫����␳

    b2Vec2 size;
    size.x = Size.x / BOX2D_SCALE_MANAGEMENT;
    size.y = Size.y / BOX2D_SCALE_MANAGEMENT;

    // ���[���h�̃C���X�^���X���擾
    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    // �X�|�i�[�̃{�f�B���쐬
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(position.x, position.y);
    body.fixedRotation = false;

    m_body = world->CreateBody(&body);

    // �`��̒�`
    b2PolygonShape shape;
    shape.SetAsBox(size.x / (2.0f * BOX2D_SCALE_MANAGEMENT), size.y / (2.0f * BOX2D_SCALE_MANAGEMENT));

    // �t�B�N�X�`���̐ݒ�
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;
    fixture.friction = 0.01f;
    fixture.restitution = 0.3f;
    fixture.isSensor = true;
    

    m_body->CreateFixture(&fixture);

    // �e��f�[�^�̕ۑ�
    BossRoomLevel = level;

    left_flag = left;
  
}

boss_carry_object_spawner::~boss_carry_object_spawner()
{
    Finalize();
}

void boss_carry_object_spawner::Initialize()
{
    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//�O���E���h�̃e�N�X�`��
    }
}

void boss_carry_object_spawner::Update()
{
    spawnTimer++;

    // 10�b���ƂɃG�l�~�[�𐶐�
    if (spawnTimer >= spawnIntervalFrames)
    {
        SpawnEnemy();
        spawnTimer = 0;
    }

 

    enemyList.erase(
        std::remove_if(enemyList.begin(), enemyList.end(), [](boss_carry_object_enemy* enemy) {
            if (enemy->IsDestroyed()) {
                delete enemy;  // ���������
                return true;
            }
            return false;
            }),
        enemyList.end()
    );
    enemyList.shrink_to_fit();

}

void boss_carry_object_spawner::SpawnEnemy()
{
    Boss_1_1& boss = Boss_1_1::GetInstance();
    if (BossRoomLevel == boss.GetBossFieldLevel())
    {
        ObjectManager& objectManager = ObjectManager::GetInstance();
        objectManager.AddBossCarryObjectEnemy(m_body->GetPosition(), b2Vec2(3.0f, 3.0f), left_flag, 1.0, b2Vec2(4.0f, 4.0f), 1, 2);
    }
}

void boss_carry_object_spawner::Draw()
{
 


    if (m_body != nullptr)
    {
        // �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
        float scale = SCREEN_SCALE;

        // �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;

        b2Vec2 Pos = GetBody()->GetPosition();


        float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


        //draw
        DrawSprite(
            { draw_x,
              draw_y },
            GetBody()->GetAngle(),
            { GetSize().x * scale ,GetSize().y * scale }
        );
    }
}



void boss_carry_object_spawner::Finalize()
{
    for (auto& enemy : enemyList)
    {
        delete enemy;
        enemy = nullptr;  // �|�C���^��null�ɂ���
    }
    enemyList.clear();
    enemyList.shrink_to_fit();

    if (g_Texture) {
        UnInitTexture(g_Texture);
    }
}