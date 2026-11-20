constexpr int MAX_DIR = 4, MAX_POINT = 4, MAX_SPOT = 4;

struct DirLightStd140 {                      // 64 B
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

struct PointLightStd140 {                    // 80 B
    alignas(16) glm::vec3 position; float constant;
    float linear; float quadratic; float _p0[2];
    alignas(16) glm::vec3 ambient;  float _p1;
    alignas(16) glm::vec3 diffuse;  float _p2;
    alignas(16) glm::vec3 specular; float _p3;
};

struct SpotLightStd140 {                     // 112 B
    alignas(16) glm::vec3 position;  float _p0;
    alignas(16) glm::vec3 direction; float constant;
    float linear; float quadratic; float _p1[2];
    alignas(16) glm::vec3 ambient;   float _p2;
    alignas(16) glm::vec3 diffuse;   float _p3;
    alignas(16) glm::vec3 specular;  float cutOff;
    float outerCutOff; float _p4[3];
};

struct LightsBlock {
    int numDir = 0, numPoint = 0, numSpot = 0; int _pad;
    DirLightStd140   dir[MAX_DIR];
    PointLightStd140 point[MAX_POINT];
    SpotLightStd140  spot[MAX_SPOT];
};