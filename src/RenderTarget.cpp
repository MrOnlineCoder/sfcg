#include <sfcg/RenderTarget.hpp>

#include <sfcg/Drawable.hpp>

#include <sfcg/GLCheck.hpp>

#include <sfcg/OpenGL.hpp>

#include <sfcg/VertexBuffer.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>

#include <SFML/Graphics/Shader.hpp>

#include <sfcg/GeometryCache.hpp>

namespace sfcg
{
    RenderTarget::RenderTarget() : m_vao(0)
    {
        glCheck(glGenVertexArrays(1, &m_vao));
        glCheck(glBindVertexArray(m_vao));
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);

        glViewport(0, 0, 800, 600);

        m_defaultView = sf::View(sf::FloatRect(0, 0, 800, 600));
        m_view = m_defaultView;
    }

    void RenderTarget::clear(sf::Color color)
    {
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
            // states.shader->setUniformMatrixArray(ShaderUniformLocations::ScaleMatrix, scl.getMatrix(), 4);
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
}