#pragma once

#include <string>
#include <unordered_map>
#include <memory>

template <typename T>
class ResourceManager {
public:
    template <typename... Args>
    T& load(const std::string& name, Args&&... args) {
        auto [it, inserted] = resources.try_emplace(name, nullptr);
        if (inserted)
            it->second = std::make_unique<T>(std::forward<Args>(args)...);
        return *it->second;
    }

    T* get(const std::string& name) {
        auto it = resources.find(name);
        return it == resources.end() ? nullptr : it->second.get();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> resources;
};