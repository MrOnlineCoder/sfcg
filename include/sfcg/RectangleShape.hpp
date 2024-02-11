#ifndef SCFG_RECTANGLE_SHAPE_HPP
#define SCFG_RECTANGLE_SHAPE_HPP

#include <sfcg/Shape.hpp>

#include <SFML/Graphics/VertexBuffer.hpp>

namespace sfcg
{
    class RectangleShape : public Shape
    {
    public:
        explicit RectangleShape(const sf::Vector2f &size = sf::Vector2f(0, 0));

        void setSize(const sf::Vector2f &size);
        const sf::Vector2f &getSize() const;

        virtual std::size_t getPointCount() const;
        virtual sf::Vector2f getPoint(std::size_t index) const;

        // virtual void draw(RenderTarget &target, RenderStates states) const;

    protected:
        virtual void update();

    private:
        sf::Vector2f m_size;
    };
};

#endif