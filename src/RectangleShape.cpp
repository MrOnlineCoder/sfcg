#include <sfcg/RectangleShape.hpp>

#include <SFML/Graphics/Vertex.hpp>

#include <sfcg/GeometryCache.hpp>

#include <SFML/Graphics/Glsl.hpp>

#include <sfcg/RenderTarget.hpp>
#include <sfcg/RenderStates.hpp>
#include <sfcg/VertexBuffer.hpp>

namespace sfcg
{
    RectangleShape::RectangleShape(const sf::Vector2f &size)
    {
        setSize(size);
        m_vertexBuffer = const_cast<VertexBuffer *>(&GeometryCache::getInstance().getUnitRectangleVertexBuffer());
        m_shapeVao = GeometryCache::getInstance().getUnitRectangleVao();
    }

    std::size_t RectangleShape::getPointCount() const
    {
        return 5;
    }

    void RectangleShape::setSize(const sf::Vector2f &size)
    {
        m_size = size;
        update();
    }

    void RectangleShape::update()
    {
        m_sizeScale = sf::Vector2f(m_size.x, m_size.y);
        m_insideBounds = sf::FloatRect(0, 0, m_size.x, m_size.y);
        m_bounds = m_insideBounds;
    }

    sf::Vector2f RectangleShape::getPoint(std::size_t index) const
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