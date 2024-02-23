#include <sfcg/Text.hpp>

#include <vector>

#include <SFML/Graphics/Text.hpp>
#include <sfcg/GeometryCache.hpp>

namespace
{
    // Add an underline or strikethrough line to the vertex array
    void addLine(std::vector<sf::Vertex> &vertices, float lineLength, float lineTop, const sf::Color &color, float offset, float thickness, float outlineThickness = 0, sf::Vector2u textureSize = sf::Vector2u(1, 1))
    {
        float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
        float bottom = top + std::floor(thickness + 0.5f);

        sf::Vector2f whitePixelPosition = sf::Vector2f(
            1 / static_cast<float>(textureSize.x),
            1 / static_cast<float>(textureSize.y));

        vertices.push_back(sf::Vertex(sf::Vector2f(-outlineThickness, top - outlineThickness), color, whitePixelPosition));
        vertices.push_back(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, whitePixelPosition));
        vertices.push_back(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, whitePixelPosition));
        vertices.push_back(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, whitePixelPosition));
        vertices.push_back(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, whitePixelPosition));
        vertices.push_back(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, whitePixelPosition));
    }

    // Add a glyph quad to the vertex array
    void addGlyphQuad(std::vector<sf::Vertex> &vertices, sf::Vector2f position, const sf::Color &color, const sf::Glyph &glyph, float italicShear, sf::Vector2u fontTextureSize)
    {
        float padding = 1.0;

        float left = glyph.bounds.left - padding;
        float top = glyph.bounds.top - padding;
        float right = glyph.bounds.left + glyph.bounds.width + padding;
        float bottom = glyph.bounds.top + glyph.bounds.height + padding;

        float u1 = (static_cast<float>(glyph.textureRect.left) - padding) / fontTextureSize.x;
        float v1 = (static_cast<float>(glyph.textureRect.top) - padding) / fontTextureSize.y;
        float u2 = (static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding) / fontTextureSize.x;
        float v2 = (static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding) / fontTextureSize.y;

        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), color, sf::Vector2f(u1, v1)));
        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color, sf::Vector2f(u1, v2)));
        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color, sf::Vector2f(u1, v2)));
        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
        vertices.push_back(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom), color, sf::Vector2f(u2, v2)));
    }
}

namespace sfcg
{
    Text::Text() : m_string(""),
                   m_font(nullptr),
                   m_characterSize(30),
                   m_fillColor(sf::Color::White),
                   m_outlineColor(sf::Color::White),
                   m_outlineThickness(0),
                   m_letterSpacing(1.0f),
                   m_lineSpacing(1.0f),
                   m_style(0)
    {
    }

    Text::Text(const sf::String &string, const sf::Font &font, unsigned int characterSize)
        : m_string(string),
          m_font(&font),
          m_characterSize(characterSize),
          m_fillColor(sf::Color::White),
          m_outlineColor(sf::Color::White),
          m_outlineThickness(0),
          m_letterSpacing(1.0f),
          m_lineSpacing(1.0f),
          m_style(0)
    {
        update();
    }

    Text::~Text()
    {
        // TODO: Implement the destructor
    }

    void Text::setString(const sf::String &string)
    {
        m_string = string;

        if (m_string.getSize() > 0)
        {
            update();
        }
    }

    void Text::setFont(const sf::Font &font)
    {
        m_font = &font;
        update();
    }

    void Text::setCharacterSize(unsigned int size)
    {
        if (m_characterSize != size)
        {
            update();
            m_characterSize = size;
        }
    }

    void Text::setFillColor(const sf::Color &color)
    {
        if (m_fillColor != color)
        {
            m_fillColor = color;
            update();
        }
    }

    void Text::setOutlineColor(const sf::Color &color)
    {
        m_outlineColor = color;
        update();
    }

    void Text::setLineSpacing(float spacingFactor)
    {
        m_lineSpacing = spacingFactor;
        update();
    }

    void Text::setLetterSpacing(float spacingFactor)
    {

        m_letterSpacing = spacingFactor;
        update();
    }

    void Text::setStyle(sf::Uint32 style)
    {
        if (style != m_style)
        {
            m_style = style;
            update();
        }
    }

    void Text::setOutlineThickness(float thickness)
    {
        if (thickness >= 0 && thickness != m_outlineThickness)
        {
            m_outlineThickness = thickness;
            update();
        }
    }

    const sf::String &Text::getString() const
    {
        return m_string;
    }

    const sf::Font *Text::getFont() const
    {
        return m_font;
    }

    unsigned int Text::getCharacterSize() const
    {
        return m_characterSize;
    }

    float Text::getLineSpacing() const
    {
        return m_lineSpacing;
    }

    float Text::getLetterSpacing() const
    {
        return m_letterSpacing;
    }

    sf::Uint32 Text::getStyle() const
    {
        return m_style;
    }

    const sf::Color &Text::getFillColor() const
    {
        return m_fillColor;
    }

    const sf::Color &Text::getOutlineColor() const
    {
        return m_outlineColor;
    }

    float Text::getOutlineThickness() const
    {
        return m_outlineThickness;
    }

    sf::Vector2f Text::findCharacterPos(std::size_t index) const
    {
        return sf::Vector2f();
    }

    void Text::draw(sfcg::RenderTarget &target, sfcg::RenderStates states) const
    {
        if (!m_font || !m_string.getSize())
            return;

        states.transform *= getTransform();
        states.texture = &m_font->getTexture(m_characterSize);
        states.shader = states.shader == nullptr ? const_cast<Shader *>(GeometryCache::getInstance().getTextShader()) : states.shader;

        states.shader->setUniform(ShaderUniformLocations::Color, m_fillColor);

        target.draw(
            m_vbo,
            0,
            m_vbo.getVertexCount(),
            states);
    }

    void Text::update()
    {
        // Nothing to draw
        if (!m_string.getSize() || !m_font)
        {
            return;
        }

        int estimatedVertexCount = m_string.getSize() * 6;

        std::vector<sf::Vertex> vertices;
        vertices.reserve(estimatedVertexCount);

        float x = 0.0f;
        float y = 0.0f;

        bool isBold = m_style & sf::Text::Bold;
        bool isUnderlined = m_style & sf::Text::Underlined;
        bool isStrikeThrough = m_style & sf::Text::StrikeThrough;
        float italicShear = (m_style & sf::Text::Italic) ? 0.209f : 0.f; // 12 degrees in radians
        float underlineOffset = m_font->getUnderlinePosition(m_characterSize);
        float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

        sf::FloatRect xBounds = m_font->getGlyph(L'x', m_characterSize, isBold).bounds;
        float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

        sf::Uint32 previousCharacter = 0;

        const sf::Glyph &whitespaceGlyph = m_font->getGlyph(L' ', m_characterSize, isBold);

        float whitespaceWidth = whitespaceGlyph.advance;

        float letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacing - 1.f);
        whitespaceWidth += letterSpacing;

        float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacing;

        float minX = static_cast<float>(m_characterSize);
        float minY = static_cast<float>(m_characterSize);
        float maxX = 0.f;
        float maxY = 0.f;

        sf::Vector2u fontTextureSize = m_font->getTexture(m_characterSize).getSize();

        for (int i = 0; i < m_string.getSize(); i++)
        {
            auto character = m_string[i];

            // Skip the \r char to avoid weird graphical issues
            if (character == L'\r')
                continue;

            x += m_font->getKerning(
                previousCharacter, character, m_characterSize);

            previousCharacter = character;

            if ((character == L' ') || (character == L'\n') || (character == L'\t'))
            {
                // Update the current bounds (min coordinates)
                minX = std::min(minX, x);
                minY = std::min(minY, y);

                switch (character)
                {
                case L' ':
                    x += whitespaceWidth;
                    break;
                case L'\t':
                    x += whitespaceWidth * 4;
                    break;
                case L'\n':
                    y += lineSpacing;
                    x = 0;
                    break;
                }

                // Update the current bounds (max coordinates)
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);

                // Next glyph, no need to create a quad for whitespace
                continue;
            }

            const sf::Glyph &glyph = m_font->getGlyph(character, m_characterSize, isBold);

            float left = glyph.bounds.left;
            float top = glyph.bounds.top;
            float right = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top + glyph.bounds.height;

            minX = std::min(minX, x + left - italicShear * bottom);
            maxX = std::max(maxX, x + right - italicShear * top);
            minY = std::min(minY, y + top);
            maxY = std::max(maxY, y + bottom);

            if (m_outlineThickness != 0)
            {
                const sf::Glyph &glyph = m_font->getGlyph(character, m_characterSize, isBold, m_outlineThickness);

                // Add the outline glyph to the vertices
                addGlyphQuad(vertices, sf::Vector2f(x, y), m_outlineColor, glyph, italicShear, fontTextureSize);
            }

            addGlyphQuad(vertices, sf::Vector2f(x, y), m_fillColor, glyph, italicShear, fontTextureSize);

            // Advance to the next character
            x += glyph.advance + letterSpacing;
        }

        // If we're using outline, update the current bounds
        if (m_outlineThickness != 0)
        {
            float outline = std::abs(std::ceil(m_outlineThickness));
            minX -= outline;
            maxX += outline;
            minY -= outline;
            maxY += outline;
        }

        // If we're using the underlined style, add the last line
        if (isUnderlined && (x > 0))
        {
            addLine(vertices, x, y, m_fillColor, underlineOffset, underlineThickness, 0, fontTextureSize);

            if (m_outlineThickness != 0)
                addLine(vertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness, fontTextureSize);
        }

        // If we're using the strike through style, add the last line across all characters
        if (isStrikeThrough && (x > 0))
        {
            addLine(vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness, 0, fontTextureSize);

            if (m_outlineThickness != 0)
                addLine(vertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness, fontTextureSize);
        }

        m_bounds.left = minX;
        m_bounds.top = minY;
        m_bounds.width = maxX - minX;
        m_bounds.height = maxY - minY;

        std::size_t finalVertexCount = vertices.size();

        m_vbo.create(finalVertexCount);
        m_vbo.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vbo.setUsage(sfcg::VertexBuffer::Usage::Static);
        m_vbo.update(vertices.data(), finalVertexCount, 0);
    }

    sf::FloatRect Text::getLocalBounds() const
    {
        return m_bounds;
    }

    sf::FloatRect Text::getGlobalBounds() const
    {
        return getTransform().transformRect(getLocalBounds());
    }
}