#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include"include/box2d/box2d.h"
#include"collider_type.h"




class MyContactListener : public b2ContactListener {
public:
   


    // �V���O���g���̃C���X�^���X���擾����
    static MyContactListener& GetInstance() {
        static MyContactListener instance;
        return instance;
    }




    // �Փ˂����u��
    void BeginContact(b2Contact* contact) override {


        // �Փ˂����t�B�N�X�`�����擾
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);

        // �v���[���[�ƒn�ʂ��Փ˂������𔻒�
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // �Փˏ����i�v���[���[�ƒn�ʂ��ڐG�������j
            
            int i = 0;
            // �K�v�ȏ����������ɋL�q
        }
        
     
    }

    // �ՓˏI����
    void EndContact(b2Contact* contact) override {
        // �Փ˂����t�B�N�X�`�����擾
        // �Փ˂����t�B�N�X�`�����擾
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);

        // �v���[���[�ƒn�ʂ��Փ˂������𔻒�
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // �Փˏ����i�v���[���[�ƒn�ʂ��ڐG�������j

            int i = 0;

            // �K�v�ȏ����������ɋL�q
        }
    }

};


#endif // !CONTACTLIST_H