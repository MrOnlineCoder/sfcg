#ifndef SFCG_RENDER_STATES_HPP
#define SFCG_RENDER_STATES_HPP

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Texture.hpp>
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
        }

        sf::Transform transform;
        GLuint vao;
        Shader *shader;
        sf::Transform scaleTransform;
        const sf::Texture *texture;

        static const RenderStates Default;
    };
};

#endif