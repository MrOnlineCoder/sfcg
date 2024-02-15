#include <sfcg/CircleShape.hpp>

#include <SFML/Graphics/Vertex.hpp>

#include <sfcg/GeometryCache.hpp>

#include <SFML/Graphics/Glsl.hpp>

#include <sfcg/RenderTarget.hpp>
#include <sfcg/RenderStates.hpp>
#include <sfcg/VertexBuffer.hpp>

namespace sfcg
{
    CircleShape::CircleShape(float radius, int pointCount)
        : m_radius(radius), m_pointCount(pointCount)
    {
        update();
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

        m_vertexBuffer = const_cast<VertexBuffer *>(&GeometryCache::getInstance().getUnitCircleVertexBuffer(m_pointCount));
        m_shapeVao = GeometryCache::getInstance().getUnitCircleVao(m_pointCount);
    }

    void CircleShape::setPointCount(int newCount)
    {
        m_pointCount = newCount;
        update();
    }

    sf::Vector2f CircleShape::getPoint(std::size_t index) const
    {
        static const float pi = 3.141592654f;

        float angle = static_cast<float>(index) * 2.f * pi / static_cast<float>(m_pointCount) - pi / 2.f;
        float x = std::cos(angle) * m_radius;
        float y = std::sin(angle) * m_radius;

        return sf::Vector2f(m_radius + x, m_radius + y);
    }
}