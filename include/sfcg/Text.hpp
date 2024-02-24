#ifndef SFCG_TEXT_HPP
#define SFCG_TEXT_HPP

#include <string>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <sfcg/Drawable.hpp>

#include <sfcg/RenderTarget.hpp>
#include <sfcg/RenderStates.hpp>
#include <sfcg/VertexBuffer.hpp>

namespace sfcg
{
    class Text : public sf::Transformable, public sfcg::Drawable
    {
    public:
        Text();
        Text(const sf::String &string, const sf::Font &font, unsigned int characterSize = 30);
        ~Text();

        void setString(const sf::String &string);
        void setFont(const sf::Font &font);
        void setCharacterSize(unsigned int size);
        void setFillColor(const sf::Color &color);
        void setOutlineColor(const sf::Color &color);
        void setLineSpacing(float spacingFactor);
        void setLetterSpacing(float spacingFactor);
        void setStyle(sf::Uint32 style);
        void setOutlineThickness(float thickness);

        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;

        const sf::String &getString() const;
        const sf::Font *getFont() const;
        unsigned int getCharacterSize() const;
        float getLineSpacing() const;
        float getLetterSpacing() const;
        sf::Uint32 getStyle() const;
        const sf::Color &getFillColor() const;
        const sf::Color &getOutlineColor() const;
        float getOutlineThickness() const;
        sf::Vector2f findCharacterPos(std::size_t index) const;

        virtual void draw(sfcg::RenderTarget &target, sfcg::RenderStates states) const;

    private:
        void update();

        sf::String m_string;
        const sf::Font *m_font;
        unsigned int m_characterSize;
        float m_lineSpacing;
        float m_letterSpacing;
        sf::Uint32 m_style;
        sf::Color m_fillColor;
        sf::Color m_outlineColor;
        float m_outlineThickness;
        sf::FloatRect m_bounds;

        VertexBuffer m_vbo;
    };
};

#endif