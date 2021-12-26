#include <hvpch.h>
#include "Light World.hpp"

#include "Camera.hpp"
#include "Heavy Utilities.hpp"

#include "Input.hpp"

namespace hv {
	LightWorld LightWorld::s_instance;

	LightWorld::LightWorld() {
		m_Lights.reserve(2048);

		m_Vertices.reserve(2048);

		m_Edges.reserve(2048);

		m_RayPoints.reserve(2048);

		m_ShadowVertexArray.reserve(2048);
	}

	LightWorld::~LightWorld() {

	}

	void LightWorld::Init(sf::Vector2u size) {
		{
			m_lightMask  = new sf::RenderTexture();
			m_maskBuffer = new sf::RenderTexture();

		}

		{
			m_shadowMask   = new sf::RenderTexture();
			m_shadowMask->setSmooth(true);

			m_shadowBuffer = new sf::RenderTexture();
			m_shadowBuffer->setSmooth(true);
		}

		{
			m_blurShader = new sf::Shader();
			m_blurShader->loadFromMemory(ShaderSource::BlurVertex, ShaderSource::BlurFragment);
		}

		{
			m_shadowShader = new sf::Shader();
			m_shadowShader->loadFromMemory(ShaderSource::ShadowVertex, ShaderSource::ShadowFragment);
		}

		SetShadowSoftness(m_shadowSoftness);

		{
			m_maskShader = new sf::Shader();
			m_maskShader->loadFromMemory(ShaderSource::MaskVertex, ShaderSource::MaskFragment);
			SetLightLevel(m_lightLevel);
		}

		{
			m_spotShader = new sf::Shader();
			m_spotShader->loadFromMemory(ShaderSource::SpotVertex, ShaderSource::SpotFragment);
		}

		{
			m_mergeShader = new sf::Shader();
			m_mergeShader->loadFromMemory(ShaderSource::ShadowMergeVertex, ShaderSource::ShadowMergeFragment);
		}

		{
			m_lightMergeShader = new sf::Shader();
			m_lightMergeShader->loadFromMemory(ShaderSource::LightShadowMergeVertex, ShaderSource::LightShadowMergeFragment);
		}

		Resize(size);
	}

	void LightWorld::Update() {

	}

	LightWorld& LightWorld::Get() {
		return s_instance;
	}

	void LightWorld::Resize(sf::Vector2u size) {
		if (m_lightMask) {
			m_shadowBuffer->create(size.x, size.y);
			m_shadowMask  ->create(size.x, size.y);

			m_shadowBuffer->clear(sf::Color::White);
			m_shadowBuffer->display();

			m_shadowMask->clear(sf::Color::White);
			m_shadowMask->display();

			////////////////////////////////////////////

			m_lightMask ->create(size.x, size.y);
			m_maskBuffer->create(size.x, size.y);

			m_lightMask->clear(sf::Color::White);
			m_lightMask->display();

			m_maskBuffer->clear(sf::Color::White);
			m_maskBuffer->display();
		}

		if (m_spotShader)
			m_spotShader->setUniform("u_resolution", (sf::Vector2f)size);

		if (m_blurShader)
			m_blurShader->setUniform("u_resolution", (sf::Vector2f)size);

		{

			if (!m_OldShadowBox.empty()) {
				for (auto& b : m_OldShadowBox) {
					m_Edges.erase(b);
					delete b;
				}
			}

			m_OldShadowBox.push_back(new Edge({ sf::Vector2f(0.0f,			0.0f),			sf::Vector2f((float)size.x, 0.0f)			}));
			m_OldShadowBox.push_back(new Edge({ sf::Vector2f((float)size.x, 0.0f),			sf::Vector2f((float)size.x, (float)size.y)  }));
			m_OldShadowBox.push_back(new Edge({ sf::Vector2f((float)size.x, (float)size.y), sf::Vector2f(0.0f,			(float)size.y)  }));
			m_OldShadowBox.push_back(new Edge({ sf::Vector2f(0.0f,          (float)size.y), sf::Vector2f(0.0f,			0.0f)			}));

			for (auto& b : m_OldShadowBox)
				m_Edges.insert(b);
		}
	}

	void LightWorld::SmoothenShadow(bool smoothen) {
		m_shadowBuffer->setSmooth(smoothen);
		m_shadowMask  ->setSmooth(smoothen);
	}

	void LightWorld::SetShadowSoftness(float softness) {
		m_shadowSoftness = softness;

		m_shadowShader->setUniform("u_scalingFactor", m_shadowSoftness);
		m_blurShader  ->setUniform("u_scalingFactor", m_shadowSoftness);
	}

	void LightWorld::SetShadowEnabled(bool enabled) {
		m_shadowsEnabled = enabled;
	}

	void LightWorld::SetLightLevel(float level) {
		m_lightLevel = level;

		if (m_maskShader)
			m_maskShader->setUniform("u_level", m_lightLevel < 0.0f ? 0.0f : m_lightLevel);
	}

	void LightWorld::SetLightEnabled(bool enabled) {
		m_lightEnabled = enabled;
	}

	float LightWorld::GetShadowSoftness() const {
		return m_shadowSoftness;
	}

	bool LightWorld::ShadowsEnabled() const {
		return m_shadowsEnabled;
	}

	float LightWorld::GetLightLevel() const {
		return m_lightLevel;
	}

	bool LightWorld::LightEnabled() const {
		return m_lightEnabled;
	}

	void LightWorld::RenderLights(Renderer& renderer) {
		if (m_changed) {
			//m_mergeShader->setUniform("u_reverse", 0.0f);

			m_lightMask->clear();
			m_maskBuffer->clear(sf::Color::White);

			m_shadowMask->clear(sf::Color::White);

			//TODO: zoptymalizowaæ u¿ywaj¹c algorytmu sprawdzaj¹cego czy cia³o jest w poprawnym miejscu
			for (auto& l : m_Lights) {
				if (!l->Drawable /*tutaj warunek ze jest poza boxem*/)
					continue;

				m_spotShader->setUniform("u_attenuation", l->Attenuation <= 0.0f ? 0.0005f : l->Attenuation);
				m_spotShader->setUniform("u_lightPower", l->LightPower);
				m_spotShader->setUniform("u_position", l->Position);
				m_spotShader->setUniform("u_radius", l->Radius);
				m_spotShader->setUniform("u_color", sf::Vector3f(ColorToNormalizedVec3(l->Color)));

				m_maskSprite.setTexture(m_maskBuffer->getTexture());
				m_lightMask->draw(m_maskSprite, m_spotShader);

				m_maskSprite.setTexture(m_lightMask->getTexture());

				m_maskBuffer->draw(m_maskSprite);
				m_maskBuffer->display();

				if (m_shadowsEnabled) {
					CreateShadowMask(l->Position);

					m_shadowMaskSprite.setTexture(m_shadowMask->getTexture());
					m_mergeShader->setUniform("u_last", m_shadowBuffer->getTexture());

					m_shadowMask->draw(m_shadowMaskSprite, m_mergeShader);
					m_shadowMask->display();
				}
			}

			m_lightMask->display();

		}

		
		if (m_shadowsEnabled) {
			m_lightMergeShader->setUniform("u_last", m_shadowMask->getTexture());

			m_lightMask->draw(m_maskSprite, m_lightMergeShader);
			m_lightMask->display();
		}

		if (m_shadowsEnabled && m_changed) {
			m_shadowBuffer->draw(m_shadowMaskSprite, m_blurShader);
			m_shadowBuffer->display();

			m_shadowMaskSprite.setTexture(m_shadowBuffer->getTexture());

			m_shadowMask->draw(m_shadowMaskSprite, m_shadowShader);
			m_shadowMask->display();
		}

		m_maskShader->setUniform("u_light", m_lightMask->getTexture());

		//renderer.draw(sf::Sprite(m_shadowMask->getTexture()));
		renderer.draw(renderer.GetFrame(), m_maskShader);

		m_changed = false;
	}

	void LightWorld::DestroyLight(Light* light) {
		m_Lights.erase(light);

		delete light;
	}

	Light* LightWorld::CreateLight(LightType type) {
		Light* buffer = new Light(type);

		m_Lights.emplace(buffer);

		return buffer;
	}

	void LightWorld::DestroyEdge(Edge* edge) {
		m_Edges.erase(edge);

		delete edge;
	}

	Edge* LightWorld::CreateEdge(sf::Vector2f start, sf::Vector2f end) {
		Edge* buffer = new Edge({ start, end });

		m_Edges.emplace(buffer);

		return buffer;
	}

	void LightWorld::CreateShadowMask(sf::Vector2f pos) {
		m_Vertices.clear();
		m_RayPoints.clear();

		// to nam wyrzuci punkty które siê powtarzaj¹ 
		for (auto& e : m_Edges) {
			m_Vertices.insert(e->Start);
			m_Vertices.insert(e->End);
		}

		for (auto& v : m_Vertices) {
			sf::Vector2f rd = v - pos;

			float baseAngle = atan2f(rd.y, rd.x);

			float ang = 0.0f;

			for (uint32_t j = 0; j < 3; j++) {
				if (j == 0)	ang = baseAngle - 0.0005f;
				if (j == 1)	ang = baseAngle;
				if (j == 2)	ang = baseAngle + 0.0005f;

				rd.x = 1000.0f * cosf(ang);
				rd.y = 1000.0f * sinf(ang);

				sf::Vector2f minP;
				float		 minT1  = 9999.0f;
				float		 minAng = 0.0f;
				bool		 valid  = false;

				for (auto& e : m_Edges) {
					sf::Vector2f sd = e->End - e->Start;

					float t2 = (rd.x * (e->Start.y - pos.y) + (rd.y * (pos.x - e->Start.x))) / (sd.x * rd.y - sd.y * rd.x);

					float t1 = (e->Start.x + sd.x * t2 - pos.x) / rd.x;

					if (t1 > 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
						if (t1 < minT1) {
							minT1 = t1;
							minP = pos + rd * t1;
							minAng = atan2f(minP.y - pos.y, minP.x - pos.x);

							valid = true;
						}
					}
				}

				if (valid)
					m_RayPoints.emplace_back(sf::Vector2f(minP.x, minP.y), minAng);
			}
		}

		std::sort(m_RayPoints.begin(), m_RayPoints.end(), [&](const Point& a, const Point& b) { return a.Angle < b.Angle; });

		m_ShadowVertexArray.clear();

		// Center of triangle fan
		m_ShadowVertexArray.emplace_back(pos);

		for (uint32_t i = 0; i < m_RayPoints.size(); i++)
			m_ShadowVertexArray.emplace_back(m_RayPoints[i].Position);

		if (!m_RayPoints.empty())
			m_ShadowVertexArray.emplace_back(m_RayPoints[0].Position);

		m_shadowBuffer->clear();
		m_shadowBuffer->draw(m_ShadowVertexArray.data(), m_ShadowVertexArray.size(), sf::TriangleFan);
		m_shadowBuffer->display();
	}

	const std::string LightWorld::ShaderSource::MaskVertex	 = R"(
		varying vec2 i_tex;

		void main()
		{
			i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		};
)";

	const std::string LightWorld::ShaderSource::MaskFragment = R"(
		#version 330 core

		precision mediump float;

		layout(location = 0) out vec4 o_color;

		in vec2 i_tex;

		uniform sampler2D texture;
		uniform sampler2D u_light;
		uniform float u_level;
		
		void main() {
			vec4 pixel = texture2D(texture, i_tex);
		
			vec4 lightPixel = texture2D(u_light, i_tex);
			
			vec4 light = vec4(1.0 - lightPixel.x , 1.0 - lightPixel.y, 1.0 - lightPixel.z, 1.0);		
			
			light += u_level;
			
			light = clamp(light, 0.0, 1.0);
			
			o_color = pixel * light;
		}
)";

	const std::string LightWorld::ShaderSource::SpotVertex   = R"(
		varying vec2 i_tex;
		
		void main()
		{
			i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		}
)";

	const std::string LightWorld::ShaderSource::SpotFragment = R"(
		#version 330
		
		precision mediump float;
		
		layout(location = 0) out vec4 o_color;
		
		in vec2 i_tex;
			
		uniform sampler2D texture;
		
		uniform vec2  u_resolution;
		uniform vec2  u_position;
		uniform vec3  u_color;
		uniform float u_radius;
		uniform float u_attenuation;
		uniform float u_lightPower;
		
		vec3 Circle(vec2 position, vec3 color, float radius) {
			float circleShape = length(position);
			float circle = smoothstep(radius, radius + u_attenuation, 1.0 - circleShape);
			return color * circle;
		}
		
		void main() {
			vec2 uv = gl_FragCoord.xy / u_resolution.xy;
			float aspect = u_resolution.x / u_resolution.y;
			uv.x *= aspect;
			uv.y = 1.0 - uv.y;
		
			vec2  pos = u_position / u_resolution;
			float rad = 1.0 - (u_radius   / u_resolution.x);
			
			vec4 pixel = texture2D(texture, i_tex);
			o_color = pixel * vec4(1.0 - Circle(uv - pos, u_color, rad) * u_lightPower, 1.0);
		}
)";

	const std::string LightWorld::ShaderSource::BlurVertex	   = R"(
varying vec2 i_tex;

uniform float u_scalingFactor;

void main()
{
	vec4 pos    = gl_ModelViewProjectionMatrix * gl_Vertex;
	i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = vec4(pos.x * u_scalingFactor, pos.y * u_scalingFactor, 0, 1);
};
)";

	const std::string LightWorld::ShaderSource::BlurFragment   = R"(
#version 330

precision mediump float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D texture;
uniform vec2 u_resolution;

const float PI2 = 6.28318530718;
    
const float directions  = 16.0;
const float quality     = 5.0;
const float size        = 9.0;

void main() {   
    vec2 radius = size / u_resolution;

    vec4 pixel = texture2D(texture, i_tex);
    
    for(float d = 0.0; d < PI2; d += PI2 / directions) {
		for(float i = 1.0 / quality; i <= 1.0; i += 1.0 / quality) {
			pixel += texture2D(texture, i_tex + vec2(cos(d), sin(d)) * radius * i);		
        }
    }
    
    pixel  /= quality * directions - 15.0;
    o_color =  pixel;
}
)";

	const std::string LightWorld::ShaderSource::ShadowVertex   = R"(
varying vec2 i_tex;

uniform float u_scalingFactor;

void main() {
	vec4 pos    = gl_ModelViewProjectionMatrix * gl_Vertex;
	i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = vec4(pos.x / u_scalingFactor, pos.y / u_scalingFactor, 0, 1);
}
)";

	const std::string LightWorld::ShaderSource::ShadowFragment = R"(
#version 330

precision mediump float;

layout(location = 0) out vec4 o_color;

uniform sampler2D texture;

in vec2 i_tex;

void main() {
	o_color = texture2D(texture, i_tex);
}
)";

	const std::string LightWorld::ShaderSource::ShadowMergeVertex   = R"(
varying vec2 i_tex;

void main() {
	i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
)";

	const std::string LightWorld::ShaderSource::ShadowMergeFragment = R"(
#version 330

precision mediump float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D texture;
uniform sampler2D u_last;

uniform float u_strength;

void main() {
	vec4 pixel		= texture2D(texture, i_tex);
	vec4 lastPixel  = texture2D(u_last,  i_tex);
    o_color = pixel * lastPixel;
}
)";

	const std::string LightWorld::ShaderSource::LightShadowMergeVertex = R"(
varying vec2 i_tex;

void main() {
	i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
)";

	const std::string LightWorld::ShaderSource::LightShadowMergeFragment = R"(
#version 330

precision mediump float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D texture;
uniform sampler2D u_last;

uniform float u_strength;

void main() {
	vec4 pixel		= texture2D(texture, i_tex);
	vec4 lastPixel  = texture2D(u_last,  i_tex);
    o_color = pixel + vec4(1.0 - lastPixel.x, 1.0 - lastPixel.y, 1.0 - lastPixel.z, 1.0);
}
)";

}
