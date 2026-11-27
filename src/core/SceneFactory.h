#pragma once
#include "Scene.h"
#include "ResourceManager.hpp"

class SceneFactory{
    private:
        static float randomFloat(float lo, float hi);
    public:
        static Scene& space(Resources* resources);
        static Scene& forest(Resources* resources);
        static Scene& forestNight(Resources* resources);
        static Scene& classroom(Resources* resources);
        static Scene& lightingShowcase(Resources* resources);
        static Scene& triagle(Resources* resources);
};