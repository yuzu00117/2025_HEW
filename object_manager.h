//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description �I�u�W�F�N�g���Ǘ����邽�߂̃t�@�N�g���[�̃C���[�W�ɋ߂�
// #make 2024/11/22�@�i��`��
// #update 2024/12/03
// #comment �ǉ��E�C���\��
//          �E�I�u�W�F�N�g����邲�Ƃɐ������銴��
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER
#define OBJECT_MANAGER


#include"vector"
#include"wood.h"
#include <iostream>

class ObjectManager {
public:
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize) {//�؂�ǉ�����I�u�W�F�N�g
        woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize));
    }

    void InitializeAll() {
        for (auto& w : woodList) {
            w->Initialize();
        }
    }

    void UpdateAll() {
        for (auto& w : woodList) {
            w->Update();
        }
    }

    void DrawAll() {
        for (auto& w : woodList) {
            w->Draw();
        }
    }

    void FinalizeAll() {
        for (auto& w : woodList) {
            w->Finalize();
        }
        woodList.clear(); // ���I�z����N���A���ă��������
    }

private:
    std::vector<std::unique_ptr<wood>> woodList; // �؂̃��X�g
};

#endif // !OBJECT_MANAGER
