#include <hvpch.h>
#include "Text Array.hpp"

using namespace hv;

TextArray::TextArray() {

}

TextArray::~TextArray() {

}

void TextArray::SetFont(const sf::Font& font) {
	m_font = &font;
}

void TextArray::SetCharacterSize(uint32_t characterSize) {
	m_characterSize = characterSize;
}

void TextArray::AddText(const std::string& name, const std::string& str, sf::Vector2f position) {
	HV_DEBUG_ASSERT(m_font); // Font wasn't initialized

	m_Texts[name] = Text(str, *m_font, position);
	m_Texts[name].setCharacterSize(m_characterSize);
}

void TextArray::AddText(const std::string& name, const Text& text) {
	HV_DEBUG_ASSERT(m_font); // Font wasn't initialized

	m_Texts[name] = text;
}

Text& TextArray::GetText(const std::string& name) {
	return m_Texts.find(name) == m_Texts.end() ? *(Text*)nullptr : m_Texts[name];
}

void TextArray::Clear() {
	m_Texts.clear();
}

void TextArray::DeleteText(const std::string& name) {
	m_Texts.erase(name);
}

std::pair<const std::string, hv::Text>* TextArray::begin() {
	return &*(m_Texts.begin());
}

std::pair<const std::string, hv::Text>* TextArray::end() {
	return &*(m_Texts.begin());
}

Text& TextArray::operator[](const std::string& name) {
	return GetText(name);
}

void TextArray::DisplayImGuiInfo() {
	for (auto& t : m_Texts)
		t.second.DisplayImGuiInfo(t.first);
}

void TextArray::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& t : m_Texts)
		target.draw(t.second, states);
}