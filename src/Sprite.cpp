#include <sfcg/Sprite.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <sfcg/RenderTarget.hpp>
#include <sfcg/RenderStates.hpp>
#include <sfcg/GeometryCache.hpp>

namespace sfcg
{
    Sprite::Sprite() : m_texture(NULL),
                       m_textureRect(),
                       m_color(sf::Color::White)
    {
    }

    Sprite::Sprite(const sf::Texture &texture) : m_texture(&texture),
                                                 m_textureRect(), m_color(sf::Color::White)
    {
        setTexture(texture, true);
    }

    Sprite::Sprite(const sf::Texture &texture, const sf::IntRect &rectangle)
    {
        setTexture(texture, false);
        setTextureRect(rectangle);
    }

    void Sprite::setTexture(const sf::Texture &texture, bool resetRect)
    {
        // Recompute the texture area if requested, or if there was no valid texture & rect before
        if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
        {
            sf::Vector2i size = sf::Vector2i(texture.getSize());
            setTextureRect(sf::IntRect(0, 0, size.x, size.y));
        }

        // Assign the new texture
        m_texture = &texture;
    }

    void Sprite::setTextureRect(const sf::IntRect &rectangle)
    {
        if (rectangle != m_textureRect)
        {
            m_textureRect = rectangle;
            update();
        }
    }

    const sf::Texture *Sprite::getTexture() const
    {
        return m_texture;
    }

    const sf::IntRect &Sprite::getTextureRect() const
    {
        return m_textureRect;
    }

    const sf::Color &Sprite::getColor() const
    {
        return m_color;
    }

    sf::FloatRect Sprite::getLocalBounds() const
    {
        float width = static_cast<float>(std::abs(m_textureRect.width));
        float height = static_cast<float>(std::abs(m_textureRect.height));

        return sf::FloatRect(0.f, 0.f, width, height);
    }

    sf::FloatRect Sprite::getGlobalBounds() const
    {
        return getTransform().transformRect(getLocalBounds());
    }

    void Sprite::draw(RenderTarget &target, RenderStates states) const
    {
        if (m_texture)
        {
            states.transform *= getTransform();
            states.shader = states.shader == nullptr ? const_cast<Shader *>(GeometryCache::getInstance().getBaseShader()) : states.shader;
            states.shader->setUniform(ShaderUniformLocations::Color, m_color);
            states.texture = m_texture;
            target.draw(m_vertexBuffer, 0, m_vertexBuffer.getVertexCount(), states);
        }
    }

    void Sprite::update()
    {
        m_vertices[0].position = sf::Vector2f(0, 0);
        m_vertices[1].position = sf::Vector2f(0, m_textureRect.height);
        m_vertices[2].position = sf::Vector2f(m_textureRect.width, m_textureRect.height);
        m_vertices[3].position = sf::Vector2f(m_textureRect.width, 0);

        m_vertices[0].color = m_color;
        m_vertices[1].color = m_color;
        m_vertices[2].color = m_color;
        m_vertices[3].color = m_color;

        sf::FloatRect convertedTextureRect = sf::FloatRect(m_textureRect);

        convertedTextureRect.left = convertedTextureRect.left / m_texture->getSize().x;
        convertedTextureRect.width = convertedTextureRect.width / m_texture->getSize().x;
        convertedTextureRect.top = convertedTextureRect.top / m_texture->getSize().y;
        convertedTextureRect.height = convertedTextureRect.height / m_texture->getSize().y;

        m_vertices[0].texCoords = sf::Vector2f(convertedTextureRect.left, convertedTextureRect.top);
        m_vertices[1].texCoords = sf::Vector2f(convertedTextureRect.left, convertedTextureRect.top + convertedTextureRect.height);
        m_vertices[2].texCoords = sf::Vector2f(convertedTextureRect.left + convertedTextureRect.width, convertedTextureRect.top + convertedTextureRect.height);
        m_vertices[3].texCoords = sf::Vector2f(convertedTextureRect.left + convertedTextureRect.width, convertedTextureRect.top);

        if (!m_vertexBuffer.getNativeHandle())
        {
            m_vertexBuffer.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
            m_vertexBuffer.setUsage(sfcg::VertexBuffer::Usage::Static);

            m_vertexBuffer.create(4);
        }

        m_vertexBuffer.update(m_vertices);
    }
}