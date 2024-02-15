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
        // Geometry cache will provide some default values when needed
        auto &cache = GeometryCache::getInstance();

        // Adjust the size of the shape
        // Geometry cache will provide a base 1-unit large shape - be it rectangle, circle, or whatever
        // SizeScale vector is changed when shape's setSize()/setRadius()/etc are called
        // So our scaleTransform will be used to adjust the size of the shape, being applied first
        sf::Transform scaleTransform = sf::Transform().scale(m_sizeScale);

        // Set the VAO used for rendering, that will be provided by derived shape class
        states.vao = m_shapeVao;

        // If user did not provide a shader - we use the base one
        states.shader = states.shader == nullptr ? const_cast<Shader *>(cache.getBaseShader()) : states.shader;

        // If we have an outline, draw it first
        // Outline is basically a bit scaled up version of original shape, drawn with the offset equal to the outline
        // Currently, it's the simplest solution that does not involve vertices manipulation
        if (m_outlineThickness > 0.0f)
        {
            // Determine, how much we have to scale our shape to get the (size+thickness)
            sf::Vector2f outlineScaleFactor(
                (1.0f + (2.0f * m_outlineThickness) / m_insideBounds.width),
                (1.0f + (2.0f * m_outlineThickness) / m_insideBounds.height));

            // Offset for accounting the outline thickness
            auto outlineTranslationVector = sf::Vector2f(-m_outlineThickness, -m_outlineThickness);

            // This transform will match the shape's transform but also adjust the scale and translation for the outline
            sf::Transform outlineTransform = sf::Transform();

            // NB: keep the eye on order of transformations! We are doing them in reverse order
            outlineTransform.translate(
                getPosition() - getOrigin());

            outlineTransform.rotate(
                getRotation(),
                getOrigin().x, getOrigin().y);

            outlineTransform.scale(
                getScale().x,
                getScale().y,
                getOrigin().x, getOrigin().y);

            // These 3 must be performed first, so all the other transformations are in the right space
            outlineTransform.scale(
                outlineScaleFactor.x,
                outlineScaleFactor.y);

            outlineTransform.translate(
                outlineTranslationVector.x,
                outlineTranslationVector.y);

            outlineTransform.scale(
                m_sizeScale);

            states.transform = outlineTransform;

            states.shader->setUniform(ShaderUniformLocations::Color, sf::Glsl::Vec4(
                                                                         getOutlineColor().r / 255.f,
                                                                         getOutlineColor().g / 255.f,
                                                                         getOutlineColor().b / 255.f,
                                                                         getOutlineColor().a / 255.f));

            // Draw the outline
            target.draw(*m_vertexBuffer, 0, m_vertexBuffer->getVertexCount(), states);
        }

        // Draw the inside of the shape
        states.transform = getTransform() * scaleTransform;

        states.shader->setUniform(ShaderUniformLocations::Color, sf::Glsl::Vec4(
                                                                     getFillColor().r / 255.f,
                                                                     getFillColor().g / 255.f,
                                                                     getFillColor().b / 255.f,
                                                                     getFillColor().a / 255.f));

        target.draw(*m_vertexBuffer, 0, m_vertexBuffer->getVertexCount(), states);
    }
}