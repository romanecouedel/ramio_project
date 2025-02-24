#include "Enemy.h"


class Koopa : public Enemy {
    protected:
        bool isShell = false;
        float shellTimer = 0.f;
        const float shellDuration = 2.f;
    
    public:
        Koopa();
        void update(float deltaTime) override;
        void move_ia();
    
    };
    