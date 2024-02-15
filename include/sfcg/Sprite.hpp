#ifndef SFCG_SPRITE_HPP
#define SFCG_SPRITE_HPP

#include <sfcg/Drawable.hpp>
#include <sfcg/VertexBuffer.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{
    class Texture;
}

namespace sfcg
{
    class Sprite : public Drawable, public sf::Transformable
    {
    public:
        Sprite();

        explicit Sprite(const sf::Texture &texture);

        Sprite(const sf::Texture &texture, const sf::IntRect &rectangle);

        void setTexture(const sf::Texture &texture, bool resetRect = false);

        void setTextureRect(const sf::IntRect &rectangle);

        const sf::Texture *getTexture() const;

        const sf::IntRect &getTextureRect() const;

        const sf::Color &getColor() const;

        sf::FloatRect getLocalBounds() const;

        sf::FloatRect getGlobalBounds() const;

    private:
        virtual void draw(RenderTarget &target, RenderStates states) const;

        void update();

        sf::Color m_color;

        VertexBuffer m_vertexBuffer;
        const sf::Texture *m_texture;
        sf::IntRect m_textureRect;
        sf::Vertex m_vertices[4];
    };

} // namespace sf

#endif
