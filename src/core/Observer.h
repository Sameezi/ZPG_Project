#pragma once

class Camera;

class Observer{
    public:
        virtual ~Observer() = default;
        virtual void update(Camera& source) const = 0;
};