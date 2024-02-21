#ifndef SFCG_RENDER_STATES_HPP
#define SFCG_RENDER_STATES_HPP

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <sfcg/Shader.hpp>

namespace sfcg
{
    class RenderStates
    {
    public:
        RenderStates()
        {
            transform = sf::Transform::Identity;
            shader = nullptr;
            vao = 0;
            texture = nullptr;
            blendMode = sf::BlendMode(
                sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add,
                sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add);
        }

        sf::Transform transform;
        GLuint vao;
        Shader *shader;
        sf::Transform scaleTransform;
        const sf::Texture *texture;
        sf::BlendMode blendMode;

        static const RenderStates Default;
    };
};

#endif