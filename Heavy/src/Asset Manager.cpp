#include <hvpch.h>
#include "Asset Manager.hpp"

using namespace hv;

ShaderLibrary	ShaderLibrary::s_instance;
TextureLibrary  TextureLibrary::s_instance;
FontLibrary		FontLibrary::s_instance;
SoundLibrary	SoundLibrary::s_instance;

//// Shader Lib ////
void ShaderLibrary::Load(std::string name, const char* path) { HV_ASSERT(false); }

void ShaderLibrary::Load(std::string name, const char* fragmentPath, const char* vertexPath) {
	HV_DEBUG
	(
		if (!Exists(name, i_Resources)) return;
	);

	i_Resources[name].loadFromFile(
		vertexPath,
		fragmentPath
	);
}

void ShaderLibrary::LoadFromMemory(std::string name, const char* fragment, const char* vertex) {
	HV_DEBUG
	(
		if (!Exists(name, i_Resources)) return;
	);

	i_Resources[name].loadFromMemory(
		vertex,
		fragment
	);
}

//// Sound Lib ////
void SoundLibrary::Update(const float& soundVol, const float& musicVol) {
	for (auto& s : m_Sounds)
		s.second.Sound.setVolume(soundVol);

	for (auto& m : m_Musics)
		m.second.setVolume(musicVol);
}

void SoundLibrary::ClearSounds() { m_Sounds.clear(); }

void SoundLibrary::ClearMusics() { m_Musics.clear(); }

// Sound and music volumes are set to 0 by default
void SoundLibrary::LoadSound(std::string name, const char* path) {
	HV_DEBUG
	(
		if (!Exists(name, m_Sounds)) return;
	);

	m_Sounds[name].SoundBuffer.loadFromFile(path);

	m_Sounds[name].Sound.setBuffer(m_Sounds[name].SoundBuffer);
	m_Sounds[name].Sound.setVolume(100.0f);
}

// Sound and music volumes are set to 0 by default
void SoundLibrary::LoadMusic(std::string name, const char* path) {
	HV_DEBUG
	(
		if (!Exists(name, m_Musics)) return;
	);

	m_Musics[name].openFromFile(path);
	m_Musics[name].setVolume(100.0f);
}

SoundPack& SoundLibrary::Sound(const char* name) { return m_Sounds[name]; }

sf::Music& SoundLibrary::Music(const char* name) { return m_Musics[name]; }

std::unordered_map<std::string, sf::Music>& SoundLibrary::RawMusic() {
	return m_Musics;
}

std::unordered_map<std::string, SoundPack>& SoundLibrary::RawSound() {
	return m_Sounds;
}

SoundLibrary::~SoundLibrary() {
	ClearSounds();
}