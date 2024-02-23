#ifndef SFCG_SHAPE_HPP
#define SFCG_SHAPE_HPP

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sfcg/Drawable.hpp>

#include <sfcg/VertexBuffer.hpp>

namespace sfcg
{
    class Shape : public sf::Transformable, public Drawable
    {
    public:
        virtual ~Shape();

        void setFillColor(const sf::Color &color);
        void setOutlineColor(const sf::Color &color);
        void setOutlineThickness(float thickness);
        const sf::Color &getFillColor() const;
        const sf::Color &getOutlineColor() const;
        float getOutlineThickness() const;

        virtual std::size_t getPointCount() const = 0;
        virtual sf::Vector2f getPoint(std::size_t index) const = 0;
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;

        virtual void draw(RenderTarget &target, RenderStates states) const;

    protected:
        Shape();

        virtual void update();

        // const Texture *m_texture;
        // IntRect m_textureRect;
        sf::Color m_fillColor;
        sf::Color m_outlineColor;
        float m_outlineThickness;
        sf::FloatRect m_insideBounds;
        sf::FloatRect m_bounds;
        sf::Vector2f m_sizeScale;
        VertexBuffer *m_vertexBuffer;
        unsigned int m_shapeVao;
    };
}; // namespace sfcg

#endif