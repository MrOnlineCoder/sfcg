#include <sfcg/CircleShape.hpp>

#include <SFML/Graphics/Vertex.hpp>

#include <sfcg/GeometryCache.hpp>

#include <SFML/Graphics/Glsl.hpp>

#include <sfcg/RenderTarget.hpp>
#include <sfcg/RenderStates.hpp>
#include <sfcg/VertexBuffer.hpp>

namespace sfcg
{
    CircleShape::CircleShape(float radius)
    {
        setRadius(radius);
    }

    std::size_t CircleShape::getPointCount() const
    {
        return 30;
    }

    void CircleShape::setRadius(float radius)
    {
        m_radius = radius;
        update();
    }

    void CircleShape::update()
    {
        float diameter = m_radius * 2;
        m_sizeScale = sf::Vector2f(diameter, diameter);
        m_insideBounds = sf::FloatRect(0, 0, diameter, diameter);
        m_bounds = m_insideBounds;

        m_vertexBuffer = const_cast<VertexBuffer *>(&GeometryCache::getInstance().getUnitCircleVertexBuffer(30));
        m_shapeVao = GeometryCache::getInstance().getUnitCircleVao(30);
    }

    sf::Vector2f CircleShape::getPoint(std::size_t index) const
    {
        switch (index)
        {
        case 0:
            return sf::Vector2f(0, 0); // top-left
        case 1:
            return sf::Vector2f(1, 0); // top-right
        case 2:
            return sf::Vector2f(1, 1); // bottom-right
        case 3:
            return sf::Vector2f(0, 1); // bottom-left
        case 4:
            return sf::Vector2f(0, 0); // top-left
        default:
            return sf::Vector2f(0, 0);
        }
    }
}