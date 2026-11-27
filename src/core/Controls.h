#pragma once
#include <unordered_map>
#include <functional>


class Controls{
    public:
        using Action = std::function<void()>;
        void bind(int key, Action action);
        void onKey(int key, int glfwAction);
        bool isDown(int key) const;
    private:
        std::unordered_map<int, Action> actions; 
        std::unordered_map<int, bool> down;
};