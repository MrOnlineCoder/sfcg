#include <sfcg/RenderTarget.hpp>

#include <sfcg/Drawable.hpp>

#include <sfcg/GLCheck.hpp>

#include <sfcg/OpenGL.hpp>

#include <sfcg/VertexBuffer.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>

#include <SFML/Graphics/Shader.hpp>

#include <SFML/Graphics/BlendMode.hpp>

#include <sfcg/GeometryCache.hpp>

#include <SFML/System/Err.hpp>

sf::Uint32 factorToGlConstant(sf::BlendMode::Factor blendFactor)
{
    switch (blendFactor)
    {
    case sf::BlendMode::Zero:
        return GL_ZERO;
    case sf::BlendMode::One:
        return GL_ONE;
    case sf::BlendMode::SrcColor:
        return GL_SRC_COLOR;
    case sf::BlendMode::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case sf::BlendMode::DstColor:
        return GL_DST_COLOR;
    case sf::BlendMode::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case sf::BlendMode::SrcAlpha:
        return GL_SRC_ALPHA;
    case sf::BlendMode::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case sf::BlendMode::DstAlpha:
        return GL_DST_ALPHA;
    case sf::BlendMode::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    }

    sf::err() << "Invalid value for sf::BlendMode::Factor! Fallback to sf::BlendMode::Zero." << std::endl;
    return GL_ZERO;
}

namespace sfcg
{
    RenderTarget::RenderTarget() : m_vao(0), m_defaultView(), m_view(m_defaultView)
    {
    }

    RenderTarget::~RenderTarget()
    {
        glCheck(glDeleteVertexArrays(1, &m_vao));
    }

    void RenderTarget::initialize()
    {
        m_defaultView = sf::View(sf::FloatRect(0, 0, getSize().x, getSize().y));
        m_view = m_defaultView;

        glCheck(glGenVertexArrays(1, &m_vao));
        glCheck(glBindVertexArray(m_vao));

        resetGLStates();
    }

    void RenderTarget::clear(sf::Color color)
    {
        bindTexture(nullptr);

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }

    void RenderTarget::draw(const Drawable &drawable)
    {
        RenderStates states;
        states.transform = sf::Transform::Identity;
        drawable.draw(*this, states);
    }

    void RenderTarget::draw(const sfcg::VertexBuffer &vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates &states)
    {
        if (firstVertex > vertexBuffer.getVertexCount())
            return;

        vertexCount = std::min(vertexCount, vertexBuffer.getVertexCount() - firstVertex);

        if (!vertexCount || !vertexBuffer.getNativeHandle())
            return;

        if (states.shader != nullptr)
        {
            states.shader->bind();
            states.shader->setUniformMatrixArray(ShaderUniformLocations::ModelMatrix, states.transform.getMatrix(), 4);
            states.shader->setUniformMatrixArray(ShaderUniformLocations::ProjectionMatrix, m_view.getTransform().getMatrix(), 4);
        }

        if (states.texture)
        {
            bindTexture(states.texture);
        }
        else
        {
            bindTexture(
                &GeometryCache::getInstance().getWhitePixelTexture());
        }

        if (states.vao)
        {
            glCheck(glBindVertexArray(states.vao));
        }
        else
        {
            vertexBuffer.bind();
            glCheck(glBindVertexArray(m_vao));
            GeometryCache::getInstance().configureVaoAttributesForVertices();
            vertexBuffer.unbind();
        }

        drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);
    }

    void RenderTarget::drawPrimitives(sf::PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
    {
        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                       GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[type];

        // Draw the primitives
        glCheck(glDrawArrays(mode, firstVertex, vertexCount));
    }

    const sf::View &RenderTarget::getDefaultView() const
    {
        return m_defaultView;
    }

    const sf::View &RenderTarget::getView() const
    {
        return m_view;
    }

    void RenderTarget::setView(const sf::View &view)
    {
        m_view = view;
    }

    void RenderTarget::bindTexture(const sf::Texture *texture)
    {
        if (texture)
        {
            glCheck(glBindTexture(GL_TEXTURE_2D, texture->getNativeHandle()));
        }
        else
        {
            glCheck(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f RenderTarget::mapPixelToCoords(const sf::Vector2i &point, const sf::View &view) const
    {
        // First, convert from viewport coordinates to homogeneous coordinates
        sf::Vector2f normalized;
        sf::FloatRect viewport = sf::FloatRect(getViewport(view));
        normalized.x = -1.f + 2.f * (static_cast<float>(point.x) - viewport.left) / viewport.width;
        normalized.y = 1.f - 2.f * (static_cast<float>(point.y) - viewport.top) / viewport.height;

        // Then transform by the inverse of the view matrix
        return view.getInverseTransform().transformPoint(normalized);
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2i RenderTarget::mapCoordsToPixel(const sf::Vector2f &point) const
    {
        return mapCoordsToPixel(point, getView());
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2i RenderTarget::mapCoordsToPixel(const sf::Vector2f &point, const sf::View &view) const
    {
        // First, transform the point by the view matrix
        sf::Vector2f normalized = view.getTransform().transformPoint(point);

        // Then convert to viewport coordinates
        sf::Vector2i pixel;
        sf::FloatRect viewport = sf::FloatRect(getViewport(view));
        pixel.x = static_cast<int>((normalized.x + 1.f) / 2.f * viewport.width + viewport.left);
        pixel.y = static_cast<int>((-normalized.y + 1.f) / 2.f * viewport.height + viewport.top);

        return pixel;
    }

    sf::IntRect RenderTarget::getViewport(const sf::View &view) const
    {
        float width = static_cast<float>(getSize().x);
        float height = static_cast<float>(getSize().y);
        const sf::FloatRect &viewport = view.getViewport();

        return sf::IntRect(static_cast<int>(0.5f + width * viewport.left),
                           static_cast<int>(0.5f + height * viewport.top),
                           static_cast<int>(0.5f + width * viewport.width),
                           static_cast<int>(0.5f + height * viewport.height));
    }

    bool RenderTarget::isSrgb() const
    {
        return false;
    }

    void RenderTarget::resetGLStates()
    {
        // Blending mode required for sf::Text rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // No depth testing is needed
        glDisable(GL_DEPTH_TEST);
    }
}