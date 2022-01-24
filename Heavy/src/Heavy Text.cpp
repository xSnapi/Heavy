#include <hvpch.h>
#include "Heavy Text.hpp"

#include "Heavy ImGui.hpp"
#include "ImGui/imgui_stdlib.h"

namespace hv {
	Text::Text() {

	}

	Text::Text(const std::string& str, const sf::Font& font) {
		setFont(font);
		setString(str);
	}

	Text::Text(const std::string& str, const sf::Font& font, sf::Vector2f position) {
		setFont(font);
		setString(str);
		setPosition(position);
	}

	Text::~Text() {

	}

	void Text::setFont(const sf::Font& font) {
		m_text::setFont(font);
		setSmooth(m_smooth);
		CorrectAlignment();
	}

	void Text::setFont(const sf::Font& font, uint32_t characterSize) {
		m_text::setFont(font);
		m_text::setCharacterSize(characterSize);
		setSmooth(m_smooth);
		CorrectAlignment();
	}

	void Text::setSmooth(bool var) {
		if (m_text::getFont()) {
			const_cast<sf::Texture&>(m_text::getFont()->getTexture(m_text::getCharacterSize())).setSmooth(var);
			m_smooth = var;
		}
	}

	void Text::setCharacterSize(uint32_t characterSize) {
		m_text::setCharacterSize(characterSize);
		CorrectAlignment();
		setSmooth(m_smooth);
	}

	void Text::setString(const std::string& str) {
		m_text::setString(str);
		CorrectAlignment();
	}

	sf::Vector2f Text::getSize() const {
		return sf::Vector2f(m_text::getGlobalBounds().width, m_text::getGlobalBounds().height);
	}

	void Text::DisplayImGuiInfo(const std::string& tabName) {
		if (ImGui::CollapsingHeader(tabName.c_str())) {
			float pos[2]
			{
				m_text::getPosition().x,
				m_text::getPosition().y,
			};

			if (ImGui::DragFloat2("Position", pos, 0.5f))
				m_text::setPosition(pos[0], pos[1]);

			std::string str = m_text::getString();

			if (ImGui::InputText("String", &str))
				m_text::setString(str);

			for(int i = 0; i < 5; i++)
				ImGui::Spacing();

			float color[3]
			{
				(float)m_text::getFillColor().r / 255.0f,
				(float)m_text::getFillColor().g / 255.0f,
				(float)m_text::getFillColor().b / 255.0f,
			};

			if (ImGui::Button("Correct Aligment"))
				CorrectAlignment();

			if (ImGui::ColorPicker3(("#" + tabName).c_str(), color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_RGB))
				m_text::setFillColor(sf::Color(uint8_t(color[0] * 255.0f), uint8_t(color[1] * 255.0f), uint8_t(color[2] * 255.0f), 255));
		}
	}

	void Text::CorrectAlignment() {
		m_text::setOrigin(sf::Vector2f((float)(uint32_t(m_text::getGlobalBounds().width) / 2),
			(float)(uint32_t(m_text::getGlobalBounds().height + m_text::getCharacterSize() / 2) / 2)));
	}
}