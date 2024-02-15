#ifndef SCFG_CIRCLE_SHAPE_HPP
#define SCFG_CIRCLE_SHAPE_HPP

#include <sfcg/Shape.hpp>

#include <SFML/Graphics/VertexBuffer.hpp>

namespace sfcg
{
    class CircleShape : public Shape
    {
    public:
        explicit CircleShape(float radius = 0.0f, int pointCount = 30);

        void setRadius(float newRadius);
        float getRadius() const;

        virtual std::size_t getPointCount() const;
        virtual sf::Vector2f getPoint(std::size_t index) const;

        void setPointCount(int newCount);

    protected:
        virtual void update();

    private:
        float m_radius;

        int m_pointCount;
    };
};

#endif