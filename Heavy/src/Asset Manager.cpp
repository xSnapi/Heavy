#include <hvpch.h>
#include "Asset Manager.hpp"

namespace hv {
	//// Shader Lib ////
	void ShaderLibrary::Load(std::string name, const char* path) { HV_ASSERT(false); }

	void ShaderLibrary::Load(std::string name, const char* fragmentPath, const char* vertexPath) {
		i_Resources[name].loadFromFile(
			vertexPath,
			fragmentPath
		);
	}

	void ShaderLibrary::LoadFromMemory(std::string name, const char* fragment, const char* vertex) {
		i_Resources[name].loadFromMemory(
			vertex,
			fragment
		);
	}

	//// Sound Lib ////
	void SoundLibrary::Update(const float& soundVol, const float& musicVol) {
		for (auto& s : m_Sounds)
			s.second.setVolume(soundVol);

		for (auto& m : m_Musics)
			m.second.setVolume(musicVol);
	}

	void SoundLibrary::ClearSounds() {
		for (auto& b : m_Buffers)
			delete b;

		m_Buffers.clear();
	}

	void SoundLibrary::ClearMusics() { m_Musics.clear(); }

	// TODO: Check if async SOUND load creates thread error

	// Sound and music volumes are set to 0 by default
	void SoundLibrary::LoadSound(std::string name, const char* path) {
		// We need SoundBuffer as a pointer so it dosen't change it reference if vector resized
		m_Buffers.push_back(new sf::SoundBuffer());
		m_Buffers.back()->loadFromFile(path);

		m_Sounds[name].setBuffer(*m_Buffers.back());
		m_Sounds[name].setVolume(0.0f);
	}

	void SoundLibrary::LoadMusic(std::string name, const char* path) {
		m_Musics[name].openFromFile(path);
		m_Musics[name].setVolume(0.0f);
	}

	sf::Sound& SoundLibrary::Sound(const char* name) { return m_Sounds[name]; }

	sf::Music& SoundLibrary::Music(const char* name) { return m_Musics[name]; }

	SoundLibrary::~SoundLibrary() {
		for (auto& b : m_Buffers)
			delete b;
	}

	ShaderLibrary	ShaderLibrary::s_instance;
	TextureLibrary  TextureLibrary::s_instance;
	FontLibrary		FontLibrary::s_instance;
	SoundLibrary	SoundLibrary::s_instance;

}