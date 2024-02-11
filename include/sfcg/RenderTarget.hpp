#ifndef SFCG_RENDER_TARGET_HPP
#define SFCG_RENDER_TARGET_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <sfcg/OpenGL.hpp>
#include <sfcg/VertexBuffer.hpp>
#include <sfcg/RenderStates.hpp>

namespace sfcg
{
    class Drawable;

    namespace ShaderUniformLocations
    {
        const static char *FillColor = "sfcg_fillColor";
        const static char *OutlineColor = "sfcg_outlineColor";
        const static char *ModelMatrix = "sfcg_modelViewMatrix";
        const static char *ProjectionMatrix = "sfcg_projectionMatrix";
    };

    class RenderTarget
    {
    public:
        RenderTarget();

        void clear(sf::Color color = sf::Color::Black);

        void draw(const Drawable &drawable);
        void draw(const sfcg::VertexBuffer &vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates &states = RenderStates::Default);

        const sf::View &getDefaultView() const;
        const sf::View &getView() const;
        void setView(const sf::View &view);

    private:
        void drawPrimitives(sf::PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount);

        GLuint m_vao;

        sf::View m_view;
        sf::View m_defaultView;
    };
}; // namespace sfcg

#endif