#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include"include/box2d/box2d.h"
#include"scene.h"

class StageSelectScene :public Scene {

public:
    void Initialize() override;

    void Update() override;

    void Draw()override;

    void Finalize() override;


    

private:
    b2World* m_world;

};

void DestroyWorld(b2World*& world);

#endif // !STAGE_SELECT_H