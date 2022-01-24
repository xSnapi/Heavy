#include <hvpch.h>
#include "Animator.hpp"

#include "Heavy Debug.hpp"
#include "Heavy Times.hpp"
#include "Heavy ImGui.hpp"

namespace hv {
	Animation::Animation() {

	}

	Animation::Animation(const Animation& animation) {

	}

	Animation::~Animation() {
		delete[] Frames;
	}

	void Animation::CopyAnimation(const Animation& other) {
		//TODO: check if working

		*this = other;

		if (FrameCount == 0)
			return;

		Frames = new sf::IntRect[other.FrameCount];

		std::copy_n(other.Frames, FrameCount, Frames);
	}

	void Animation::Create
	(
		uint32_t frameCountX, 
		uint32_t frameCountY, 
		SpriteSheet mode,
		uint32_t offset,
		sf::Sprite* sprite, 
		sf::Texture* texture
	) 
	{
		if (sprite)	
			Sprite = sprite;

		if (Sprite && texture)
			Sprite->setTexture(*texture);

		if (FrameWidth == 0 || FrameHeight == 0) {
			Debug::Log(Color::Red, "[Frame width or height is '0']\n");
			return;
		}

		delete[] Frames;
		Frames = new sf::IntRect[frameCountX * frameCountY];

		bool rd = mode == SpriteSheet::RightDown;

		if (rd)
			frameCountY += offset;
		else
			frameCountX += offset;

		for (uint32_t y = rd ? offset : 0; y < frameCountY; y++) {
			for (uint32_t x = rd ? 0 : offset; x < frameCountX; x++) {
				if(rd)
					Frames[FrameCount] = sf::IntRect(x * FrameWidth, y * FrameHeight, FrameWidth, FrameHeight);
				else
					Frames[FrameCount] = sf::IntRect(x * FrameWidth, y * FrameHeight, FrameWidth, FrameHeight);

				FrameCount++;
			}
		}

		ApplyFrame(Frames[Reversed ? FrameCount - 1 : 0]);
	}

	void Animation::ApplyFrame(const sf::IntRect& frame) {
		if (!Sprite) {
			Debug::Log(Color::Red, "[No sprite bind to animation]\n");
			return;
		}

		Sprite->setTextureRect(frame);
	}

	void Animation::Restart(bool frame) {
		Playable		= true;
		m_elapsedTime	= 0.0f;

		if (frame) {
			CurrentFrame = Reversed ? FrameCount - 1 : 0;
			ApplyFrame(Frames[CurrentFrame]);
		}
	}

	void Animation::DisplayImGuiInfo(const std::string& tabName) {
		if(ImGui::CollapsingHeader(tabName.c_str())) {
			ImGui::Text("Size: ");
			ImGui::Indent(25.0f);
			ImGui::Text("Width %d\nHeight %d", FrameWidth, FrameHeight);
			ImGui::Indent(-25.0f);

			ImGui::DragFloat("Speed", &Speed, 0.001f, 0.0f, 9999.9f);
			ImGui::Checkbox("Playable", &Playable);
			ImGui::Checkbox("Play Once", &PlayOnce);
			ImGui::Checkbox("Reversed", &Reversed);

			if (Sprite)
				ImGui::Image(*Sprite, { 128, 128 });

			ImGui::SliderInt("Frame", &CurrentFrame, 0, FrameCount - 1);

			if (ImGui::Button("Apply Frame"))
				ApplyFrame(Frames[CurrentFrame]);

			if (ImGui::Button("Restart"))
				Restart(true);
		}
	}

	Animation* Animation::operator=(const Animation& animation) {

		return nullptr;
	}
}

namespace hv {
	Animator::Animator() {

	}

	Animator::Animator(float defaultSpeed) {

	}

	Animator::~Animator() {

	}

	void Animator::Play(Animation& animation) {
		if (!Check(animation))
			return;

		if (animation.Reversed)
			UpdateReverse(animation);
		else
			Update(animation);
	}

	void Animator::Update(Animation& animation) {
		animation.m_elapsedTime += dt;
		while (animation.m_elapsedTime >= m_animationSpeed) {
			animation.m_elapsedTime -= m_animationSpeed;

			if (++animation.CurrentFrame > (int32_t)animation.FrameCount - 1) {
				if (animation.OnEnd)
					animation.OnEnd();

				if (animation.PlayOnce)
					animation.Playable = false;

				animation.CurrentFrame = 0;
			}
			animation.Sprite->setTextureRect(animation.Frames[animation.CurrentFrame]);
		}
	}

	void Animator::UpdateReverse(Animation& animation) {
		animation.m_elapsedTime += dt;
		while (animation.m_elapsedTime >= m_animationSpeed) {
			animation.m_elapsedTime -= m_animationSpeed;

			if (--animation.CurrentFrame < 0) {
				if (animation.OnEnd)
					animation.OnEnd();

				if (animation.PlayOnce)
					animation.Playable = false;

				animation.CurrentFrame = animation.FrameCount - 1;
			}
			animation.Sprite->setTextureRect(animation.Frames[animation.CurrentFrame]);
		}
	}

	bool Animator::Check(const Animation& animation) {
		if (!animation.Playable)
			return false;

		float as = animation.Speed <= 0.0f ? DefaultSpeed : animation.Speed;
			  as = DefaultSpeed > 0.0f ? DefaultSpeed : as;

		if (as <= 0.0f) {
			Debug::Log(Color::Red, "[Animation speed wasn't correct]\n");
			return false;
		}

		m_animationSpeed = as;
		return true;
	}
}