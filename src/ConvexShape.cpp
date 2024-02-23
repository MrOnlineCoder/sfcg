#include <sfcg/ConvexShape.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>

namespace sfcg
{
    ConvexShape::ConvexShape(std::size_t pointCount)
    {
        m_vertexBuffer = new VertexBuffer();
        setPointCount(pointCount);
    }

    void ConvexShape::setPointCount(std::size_t count)
    {
        m_pointCount = count;

        m_points.resize(count);

        m_vertexBuffer->setPrimitiveType(sf::PrimitiveType::TriangleFan);
        m_vertexBuffer->create(m_pointCount);
    }

    std::size_t ConvexShape::getPointCount() const
    {
        return m_pointCount;
    }

    void ConvexShape::setPoint(std::size_t index, const sf::Vector2f &point)
    {
        if (index < m_pointCount)
        {
            m_points[index] = sf::Vertex(point, m_fillColor);
            m_vertexBuffer->update(&m_points[index], 1, index);
        }
    }

    sf::Vector2f ConvexShape::getPoint(std::size_t index) const
    {
        return m_points[index].position;
    }

    ConvexShape::~ConvexShape()
    {
        delete m_vertexBuffer;
    }
}