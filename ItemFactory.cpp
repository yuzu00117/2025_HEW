#include"ItemFactory.h"
#include"texture.h"
#include"sprite.h"
#include"collider_type.h"


ItemFactory::ItemFactory(b2Vec2 position, b2Vec2 body_size, float rotate, ITEM type, wchar_t* texture_name, bool shape_polygon, float Alpha)
	:m_type(type), m_Texture(InitTexture(texture_name)), m_Alpha(Alpha)
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

    SetSize(body_size);//�v���C���[�\�������邽�߂ɃZ�b�g����


    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;


    //�l�p�`�̏ꍇ
   //-----------------------------------------------------------------------------
    if (shape_polygon)
    {
        b2PolygonShape shape;
        shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

        fixture.shape = &shape;
        fixture.density = 1.0f;//���x
        fixture.friction = 0.3f;//���C
        fixture.restitution = 0.1f;//�����W��
        fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
    }
    //�~�̏ꍇ
//-----------------------------------------------------------------------------
    else
    {
        b2CircleShape shape;
        shape.m_radius = size.x * 0.5f;

        fixture.shape = &shape;
        fixture.density = 1.0f;//���x
        fixture.friction = 0.3f;//���C
        fixture.restitution = 0.1f;//�����W��
        fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
    }



    b2Fixture* p_fixture = m_body->CreateFixture(&fixture);

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

}


ItemFactory::~ItemFactory()
{
	if (m_Texture)
	{
		UnInitTexture(m_Texture);
	}
}

void    ItemFactory::Draw()
{
    // �R���C�_�[�ƈʒu���̕␳�����邽��
    float scale = SCREEN_SCALE;


    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    // �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j
    b2Vec2 position;
    position.x = m_body->GetPosition().x;
    position.y = m_body->GetPosition().y;

    //�`��
    DrawSprite(
        { position.x,
          position.y },
        m_body->GetAngle(),
        { GetSize().x * scale,GetSize().y * scale },
        m_Alpha
    );

}

