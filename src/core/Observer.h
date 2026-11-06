#pragma once

template <typename Source>

class Observer{
    public:
        virtual ~Observer() = default;
        virtual void update(const Source& source) = 0;
};