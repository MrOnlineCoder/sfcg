#ifndef SCFG_CONVEX_SHAPE_HPP
#define SCFG_CONVEX_SHAPE_HPP

#include <sfcg/Shape.hpp>

#include <SFML/Graphics/VertexBuffer.hpp>

#include <vector>

namespace sfcg
{
    class ConvexShape : public Shape
    {
    public:
        explicit ConvexShape(std::size_t pointCount = 0);

        void setSize(const sf::Vector2f &size);
        const sf::Vector2f &getSize() const;

        virtual std::size_t getPointCount() const;
        virtual sf::Vector2f getPoint(std::size_t index) const;
        void setPointCount(std::size_t count);
        void setPoint(std::size_t index, const sf::Vector2f &point);

        ~ConvexShape();

    private:
        std::size_t m_pointCount;

        std::vector<sf::Vertex> m_points;
    };
};

#endif