#ifndef SFCG_RENDER_STATES_HPP
#define SFCG_RENDER_STATES_HPP

#include <SFML/Graphics/Transform.hpp>

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
        }

        sf::Transform transform;
        GLuint vao;
        Shader *shader;

        static const RenderStates Default;
    };
};

#endif