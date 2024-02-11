#include <sfcg/Shape.hpp>

#include <sfcg/RenderStates.hpp>
#include <sfcg/RenderTarget.hpp>
#include <sfcg/GeometryCache.hpp>

namespace sfcg
{

    Shape::Shape() : m_fillColor(255, 255, 255), m_outlineColor(255, 255, 255), m_outlineThickness(0), m_insideBounds(), m_bounds(), m_sizeScale(1.f, 1.f), m_shapeVao(0)
    {
    }

    Shape::~Shape()
    {
    }

    void Shape::setFillColor(const sf::Color &color)
    {
        m_fillColor = color;
    }

    void Shape::setOutlineColor(const sf::Color &color)
    {
        m_outlineColor = color;
    }

    void Shape::setOutlineThickness(float thickness)
    {
        m_outlineThickness = thickness;
    }

    const sf::Color &Shape::getFillColor() const
    {
        return m_fillColor;
    }

    const sf::Color &Shape::getOutlineColor() const
    {
        return m_outlineColor;
    }

    float Shape::getOutlineThickness() const
    {
        return m_outlineThickness;
    }

    sf::FloatRect Shape::getLocalBounds() const
    {
        return m_insideBounds;
    }

    sf::FloatRect Shape::getGlobalBounds() const
    {
        return getTransform().transformRect(m_insideBounds);
    }

    void Shape::draw(RenderTarget &target, RenderStates states) const
    {
        auto &cache = GeometryCache::getInstance();

        auto scaleTransform = sf::Transform().scale(m_sizeScale);

        states.transform *= scaleTransform * getTransform();
        states.shader = states.shader == nullptr ? const_cast<Shader *>(cache.getBaseShader()) : states.shader;

        states.shader->setUniform(ShaderUniformLocations::FillColor, sf::Glsl::Vec4(
                                                                         getFillColor().r / 255.f,
                                                                         getFillColor().g / 255.f,
                                                                         getFillColor().b / 255.f,
                                                                         getFillColor().a / 255.f));
        states.shader->setUniform(ShaderUniformLocations::OutlineColor, sf::Glsl::Vec4(
                                                                            getOutlineColor().r / 255.f,
                                                                            getOutlineColor().g / 255.f,
                                                                            getOutlineColor().b / 255.f,
                                                                            getOutlineColor().a / 255.f));

        states.vao = m_shapeVao;

        target.draw(*m_vertexBuffer, 0, m_vertexBuffer->getVertexCount(), states);
    }
}