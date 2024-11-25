#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include"include/box2d/box2d.h"
#include"collider_type.h"
#include"anchor_point.h"
#include"anchor.h"




class MyContactListener : public b2ContactListener {
private:

public:
    b2Vec2 contactPoint;
   

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
        if (!fixtureA || !fixtureB) return; // NULL�`�F�b�N
       


       
        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
        if (!objectA || !objectB)return;//NULL�`�F�b�N

    

        // �v���[���[�ƒn�ʂ��Փ˂������𔻒�
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // �Փˏ����i�v���[���[�ƒn�ʂ��ڐG�������j
            
            int i = 0;
            // �K�v�ȏ����������ɋL�q


        }


        //�v���C���[�ɕt�����Ă���Z���T�[�ƃA���J�[�|�C���g���G�ꂽ�ꍇ
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_player_sensor))
        {
            //�Ԃ������I�u�W�F�N�g���ǂ������A���J�[�|�C���g���킩��Ȃ��̂Ŕ�r����

            b2Body* anchor_point_body;//�ꎞ�I�ȓ��ꕨ ���̒��ɃA���J�[�|�C���g�̃{�f�B�������

            if (objectA->collider_type == collider_anchor_point)
            {
                //objectA���A���J�[�|�C���g������
                //�̂�fixtureA���t�B�N�X�`�������
                anchor_point_body=fixtureA->GetBody();
            }
            else
            {
                //objectB���A���J�[�|�C���g������
                //�̂�fixtureB���t�B�N�X�`�������
                anchor_point_body = fixtureB->GetBody();
            }
            //�ǂ��炪�A���J�[�|�C���g�����������̂ŃA���J�[�|�C���g��ێ����Ă����z��ɂ����
            AnchorPoint::InsideSensor(anchor_point_body);
           
        }
        //�v���C���[�ɕt�����Ă���Z���T�[�ƃA���J�[�|�C���g���G�ꂽ�ꍇ
        if ((objectA->collider_type == collider_anchor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_anchor))
        {


            Anchor::SetAnchorState(Connected_state);//�v���C���[�A�b�v�f�[�g�̒��̃X�C�b�`���̈ڍs�悤 �ڑ���ԂɈڍs

            // �ڐG�ʒu���擾
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            contactPoint = worldManifold.points[0];
        }

     
    }

    // �ՓˏI����
    void EndContact(b2Contact* contact) override {
        // �Փ˂����t�B�N�X�`�����擾
        // �Փ˂����t�B�N�X�`�����擾
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        if (!fixtureA || !fixtureB) return; // NULL�`�F�b�N


        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
        if (!objectA || !objectB)return;//NULL�`�F�b�N

        // �v���[���[�ƒn�ʂ��Փ˂������𔻒�
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // �Փˏ����i�v���[���[�ƒn�ʂ��ڐG�������j


            // �K�v�ȏ����������ɋL�q
        }



        //�v���C���[�ɕt�����Ă���Z���T�[�ƃA���J�[�|�C���g���G��I������@�i�Z���T�[�͈̔͊O�ɂł��j
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_player_sensor))
        {
            //�Ԃ������I�u�W�F�N�g���ǂ������A���J�[�|�C���g���킩��Ȃ��̂Ŕ�r����

            b2Body* anchor_point_body;//�ꎞ�I�ȓ��ꕨ ���̒��ɃA���J�[�|�C���g�̃{�f�B�������

            if (objectA->collider_type == collider_anchor_point)
            {
                //objectA���A���J�[�|�C���g������
                //�̂�fixtureA���t�B�N�X�`�������
                anchor_point_body = fixtureA->GetBody();
            }
            else
            {
                //objectB���A���J�[�|�C���g������
                //�̂�fixtureB���t�B�N�X�`�������
                anchor_point_body = fixtureB->GetBody();
            }
            //�ǂ��炪�A���J�[�|�C���g�����������̂ŃA���J�[�|�C���g��ێ����Ă����z��ɂ����
            AnchorPoint::OutsideSensor(anchor_point_body);

        }
    }

};


#endif // !CONTACTLIST_H