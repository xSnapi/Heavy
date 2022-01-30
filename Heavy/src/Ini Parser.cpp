#include <hvpch.h>
#include "Ini Parser.hpp"

#include "Heavy Debug.hpp"

#include <algorithm>
#include <sstream>

using namespace hv;

IniParser::IniParser() {
	
}

IniParser::IniParser(const std::string& path) {
	Open(path);
}

IniParser::~IniParser() {
	
}

bool IniParser::Open(const std::string& path) {
	m_path = path;

	m_file.open(m_path, std::ios::in);

	m_open = CheckFile();

	if (m_open)
		Load();

	return m_open;
}

bool IniParser::Close() {
	if (m_open)
		m_file.close();

	return m_open;
}

bool IniParser::Save() {
	if (!m_open)
		return false;

	m_file.close();
	m_file.open(m_path, std::ios::out);

	for (std::list<std::string>::iterator it = m_SectionOrder.begin(); it != m_SectionOrder.end(); it++) {
		if (it != m_SectionOrder.begin())
			m_file << '\n';

		m_file << "[" << *it << "]" << '\n';

		for (auto& p : m_ParametrOrder[*it])
			m_file << p << "=" << m_Sections[*it][p] << '\n';
			
	}

	return m_open;
}

void IniParser::Clear() {
	m_SectionOrder.clear();
	m_Sections.clear();
}

void IniParser::Set(const std::string& str, const std::string& section, const std::string& parametr) {
	SetParametr(section, parametr, str);
}

void IniParser::Set(const int& var, const std::string& section, const std::string& parametr) {
	SetParametr(section, parametr, std::to_string(var));
}

void IniParser::Set(const float& var, const std::string& section, const std::string& parametr) {
	SetParametr(section, parametr, std::to_string(var));
}

void IniParser::Set(const sf::Vector2f& vec, const std::string& section, const std::string& parametr) {
	std::stringstream ss;

	ss << vec.x << ',' << vec.y;

	SetParametr(section, parametr, ss.str());
}

void IniParser::Set(const sf::Vector3f& vec, const std::string& section, const std::string& parametr) {
	std::stringstream ss;

	ss << vec.x << ',' << vec.y << ',' << vec.z;

	SetParametr(section, parametr, ss.str());
}

void IniParser::Set(const sf::Color& color, const std::string& section, const std::string& parametr) {
	std::stringstream ss;

	ss << (unsigned)color.r << ',' << (unsigned)color.g << ',' << (unsigned)color.b << ',' << (unsigned)color.a;

	SetParametr(section, parametr, ss.str());
}

std::string IniParser::GetStr(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	return (!str ? "" : *str);
}

int IniParser::GetInt(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	return (!str ? 0 : atoi(str->c_str()));
}

float IniParser::GetFloat(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	return (!str ? 0.0f : (float)atof(str->c_str()));
}

sf::Vector2f IniParser::GetVec2(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	if(!str)
		return { };

	sf::Vector2f vec;

	size_t pos = str->find(",");

	vec.x = (float)atof(str->substr(0, pos).c_str());
	vec.y = (float)atof(str->substr(pos + 1).c_str());

	return vec;
}

sf::Vector3f IniParser::GetVec3(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	if (!str)
		return { };

	float values[3] = { 0.0f };

	size_t last = 0;
	for (int i = 0; i < 2; i++) {
		size_t pos = str->find(",", last + 1);

		if (pos != std::string::npos) {
			values[i] = (float)atof(str->substr(last, pos - last).c_str());
			last = pos + 1;
		}
	}

	values[2] = (float)atof(str->substr(last).c_str());

	return sf::Vector3f(values[0], values[1], values[2]);
}

sf::Color IniParser::GetColor(const std::string& section, const std::string& parametr) const {
	const std::string* str = GetParametr(section, parametr);

	if (!str)
		return { };

	sf::Uint8 channels[4] = { 0 };

	size_t last = 0;
	for (int i = 0; i < 3; i++) {
		size_t pos = str->find(",", last + 1);

		if (pos != std::string::npos) {
			channels[i] = atoi(str->substr(last, pos - last).c_str());
			last = pos + 1;
		}
	}

	channels[3] = atoi(str->substr(last).c_str());

	return sf::Color(channels[0], channels[1], channels[2], channels[3]);
}

const std::string* IniParser::GetParametr(const std::string& section, const std::string& parametr) const {
	if (m_Sections.find(section) == m_Sections.end()) {
		Debug::Log(Color::Yellow, "[Section {", section, "} in file \"", m_path, "\" doesn't exist]\n");
		return nullptr;
	}

	if (m_Sections.at(section).find(parametr) == m_Sections.at(section).end()) {
		Debug::Log(Color::Yellow, "[Parametr {", parametr, "} in section {", section, "} doesn't exist \"", m_path, "\"]\n");
		return nullptr;
	}

	return &m_Sections.at(section).at(parametr);
}

void IniParser::SetParametr(const std::string& section, const std::string& parametr, const std::string& parsedParametr) {
	if (m_Sections.find(section) == m_Sections.end()) {
		m_SectionOrder.push_back(section);
		m_ParametrOrder[section].push_back(parametr);
	}

	m_Sections[section][parametr] = parsedParametr;
}

bool IniParser::CheckFile() const {
	bool good = m_file.good() && (m_path.find(".ini") != std::string::npos);

	if (!good)
		Debug::Log(Color::Red, "[Failed to load \"", m_path, "\"]\n");

	return good;
}

void IniParser::Load() {
	std::vector<std::string> Lines;

	Lines.resize(16);

	size_t i = 0;

	while (std::getline(m_file, Lines[i])) {
		if (i >= Lines.size() - 1)
			Lines.resize(Lines.size() * 2);

		i++;
	}

	Parse(Lines);
}

void IniParser::Parse(std::vector<std::string>& Lines) {
	std::string lastHeader = "";

	auto ThrowError = [this](const std::string& message, size_t i) {
		Debug::Log(Color::Red, "[", message, i, " in \"", m_path, "\"]\n");
		m_open = false;
	};

	auto ParseValue = [path = m_path](std::string value)->std::pair<std::string, std::string> {
		value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

		size_t pos = value.find("=");

		std::string definition = value.substr(0, pos);
		std::string stringValue = value.substr(pos + 1);

		if (pos == std::string::npos || pos == value.size() - 1) {
			Debug::Log(Color::Yellow, "[Failed to find value for definition: ", definition, " in file ", "\"", path, "\"]\n");

			stringValue.clear();
		}

		return { definition, stringValue };
	};

	for (size_t i = 0; i < Lines.size(); i++) {
		auto& line = Lines[i];

		if (line.empty() || line == "\n" /*|| line.find("#") != std::string::npos*/)
			continue;

		if (lastHeader.empty() && line.find("[") == std::string::npos)
			ThrowError("Failed to parse file! Syntax error at: ", i + 1);

		if (size_t pos0 = line.find("[") != std::string::npos) {
			size_t pos1 = line.find("]");

			if (pos1 == std::string::npos) {
				ThrowError("Failed to parse file! Syntax error at: ", i + 1);
				break;
			}

			lastHeader = line.substr(pos0, pos1 - 1);

			if (m_Sections.find(lastHeader) != m_Sections.end()) {
				ThrowError("Failed to parse file! Duplicated header definition at: ", i + 1);
				break;
			}

			m_Sections[lastHeader];
			m_SectionOrder.push_back(lastHeader);
			continue;
		}

		auto value = ParseValue(line);

		m_ParametrOrder[lastHeader].push_back(value.first);

		m_Sections[lastHeader][value.first] = value.second;
	}
}
