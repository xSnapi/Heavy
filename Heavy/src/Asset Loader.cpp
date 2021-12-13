#include <hvpch.h>
#include "Asset Loader.hpp"

#include "Asset Manager.hpp"

namespace hv {
    AssetLoader& AssetLoader::Get() { return s_instance; }

    void AssetLoader::Sync() {
        // Waiting for all async tasks to return
        while (!m_Results.empty()) {
            m_Results.top().get();
            m_Results.pop();
        }
    }

    void AssetLoader::AddResource(std::string name, std::string path, AssetType type) {
        // Phrasing name and type to struct with pointer to equivalent function
        Resource res = GetResource(name, type);

        m_Results.push(std::async(
            std::launch::async,
            res.Fun,
            res.Raw,
            path
        ));
    }

    void AssetLoader::AddResource(const std::string& name, const std::string& fragment, const std::string& vertex, AssetType type) {
        switch (type) {
        case AssetType::Shader:

            m_Results.push(std::async(
                std::launch::async,
                LoadShader,
                &hv::ShaderLibrary::Get().Raw()[name.c_str()],
                fragment,
                vertex
            ));

            break;

        case AssetType::MemoryShader:

            m_Results.push(std::async(
                std::launch::async,
                LoadShaderFromMemory,
                &hv::ShaderLibrary::Get().Raw()[name.c_str()],
                fragment,
                vertex
            ));

            break;
        default:
            HV_ASSERT(false);
        }
    }

    AssetLoader::AssetLoader() {

    }

    AssetLoader::~AssetLoader() {

    }

    AssetLoader::Resource AssetLoader::GetResource(const std::string& name, const AssetType& type) {
        switch (type) {
        case AssetType::Texture:
            return { LoadTexture, &hv::TextureLibrary::Get().Raw()[name.c_str()] };
            break;
        case AssetType::Font:
            return { LoadFont,  &hv::FontLibrary::Get().Raw()[name.c_str()] };
            break;
        case AssetType::Sound:
            return { LoadSound, &hv::SoundLibrary::Get().Sound(name.c_str()) };
            break;
        case AssetType::Music:
            return { LoadMusic, &hv::SoundLibrary::Get().Music(name.c_str()) };
            break;
        }

        return { };
    }

    void AssetLoader::LoadTexture(void* res, std::string path) { ((sf::Texture*)res)->loadFromFile(path); }

    void AssetLoader::LoadFont(void* res, std::string path) { ((sf::Font*)res)->loadFromFile(path); }

    void AssetLoader::LoadMusic(void* res, std::string path) { ((sf::Music*)res)->openFromFile(path); }

    void AssetLoader::LoadSound(void* res, std::string path) { ((sf::SoundBuffer*)res)->loadFromFile(path); }

    void AssetLoader::LoadShader(sf::Shader* res, std::string fragment, std::string vertex) {
        res->loadFromFile(
            vertex,
            fragment
        );
    }

    void AssetLoader::LoadShaderFromMemory(sf::Shader* res, std::string fragment, std::string vertex) {
        res->loadFromMemory(
            vertex,
            fragment
        );
    }

    AssetLoader AssetLoader::s_instance;
}