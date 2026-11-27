#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

template <typename T>
class ResourceManager {
public:
    template <typename... Args>
    T& load(const std::string& name, Args&&... args) {
        auto it = index.find(name);
        if (it != index.end())
            return *items[it->second];

        items.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        names.push_back(name);
        index.emplace(name, items.size() - 1);
        return *items.back();
    }

    T* get(const std::string& name) {
        auto it = index.find(name);
        return it == index.end() ? nullptr : items[it->second].get();
    }

    T& at(std::size_t i)             { return *items.at(i); }
    const T& at(std::size_t i) const { return *items.at(i); }

    const std::string& nameAt(std::size_t i) const { return names.at(i); }

    bool contains(const std::string& name) const { return index.count(name) > 0; }
    std::size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }

    template <typename Func>
    void forEach(Func&& f) {
        for (std::size_t i = 0; i < items.size(); ++i)
            f(names[i], *items[i]);
    }

private:
    std::vector<std::unique_ptr<T>> items;              
    std::vector<std::string> names;                      
    std::unordered_map<std::string, std::size_t> index;  
};


struct Resources {
    ResourceManager<ShaderProgram> shaders;
    ResourceManager<Texture>       textures;
    ResourceManager<Cubemap>       cubemaps;
    ResourceManager<Model>         models;
	ResourceManager<InstancedModel> imodels;
};