
#include <sfcg/VertexBuffer.hpp>
#include <sfcg/OpenGL.hpp>
#include <sfcg/GLCheck.hpp>
#include <sfcg/RenderTarget.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Window/GlResource.hpp>
#include <cstring>

GLenum bufferUsageToGlEnum(sfcg::VertexBuffer::Usage usage)
{
    switch (usage)
    {
    case sfcg::VertexBuffer::Usage::Static:
        return GL_STATIC_DRAW;
    case sfcg::VertexBuffer::Usage::Dynamic:
        return GL_DYNAMIC_DRAW;
    default:
        return GL_STREAM_DRAW;
    }
}

namespace sfcg
{

    VertexBuffer::VertexBuffer() : m_buffer(0),
                                   m_size(0),
                                   m_primitiveType(sf::Points),
                                   m_usage(Stream)
    {
    }

    VertexBuffer::VertexBuffer(sf::PrimitiveType type) : m_buffer(0),
                                                         m_size(0),
                                                         m_primitiveType(type),
                                                         m_usage(Stream)
    {
    }

    VertexBuffer::VertexBuffer(VertexBuffer::Usage usage) : m_buffer(0),
                                                            m_size(0),
                                                            m_primitiveType(sf::Points),
                                                            m_usage(usage)
    {
    }

    VertexBuffer::VertexBuffer(sf::PrimitiveType type, VertexBuffer::Usage usage) : m_buffer(0),
                                                                                    m_size(0),
                                                                                    m_primitiveType(type),
                                                                                    m_usage(usage)
    {
    }

    VertexBuffer::~VertexBuffer()
    {
        if (m_buffer)
        {
            glCheck(glDeleteBuffers(1, &m_buffer));
        }
    }

    VertexBuffer::VertexBuffer(const VertexBuffer &copy)
    {
        if (copy.m_buffer && copy.m_size)
        {
            if (!create(copy.m_size))
            {
                sf::err() << "Could not create vertex buffer for copying" << std::endl;
                return;
            }

            if (!update(copy))
                sf::err() << "Could not copy vertex buffer" << std::endl;
        }
    }

    bool VertexBuffer::update(const VertexBuffer &vertexBuffer)
    {
        if (!m_buffer || !vertexBuffer.m_buffer)
            return false;

        sf::GlResource::TransientContextLock contextLock;

        glCheck(glBindBuffer(GL_COPY_READ_BUFFER, vertexBuffer.m_buffer));
        glCheck(glBindBuffer(GL_COPY_WRITE_BUFFER, m_buffer));

        glCheck(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, static_cast<GLsizeiptr>(sizeof(sf::Vertex) * vertexBuffer.m_size)));

        glCheck(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
        glCheck(glBindBuffer(GL_COPY_READ_BUFFER, 0));

        return true;
    }

    void VertexBuffer::swap(VertexBuffer &right)
    {
        std::swap(m_size, right.m_size);
        std::swap(m_buffer, right.m_buffer);
        std::swap(m_primitiveType, right.m_primitiveType);
        std::swap(m_usage, right.m_usage);
    }

    ////////////////////////////////////////////////////////////
    bool VertexBuffer::create(std::size_t vertexCount)
    {

        if (!m_buffer)
            glCheck(glGenBuffers(1, &m_buffer));

        if (!m_buffer)
        {
            sf::err() << "Could not create vertex buffer, generation failed" << std::endl;
            return false;
        }

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
        glCheck(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(sf::Vertex) * vertexCount), NULL, bufferUsageToGlEnum(m_usage)));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

        m_size = vertexCount;

        return true;
    }

    std::size_t VertexBuffer::getVertexCount() const
    {
        return m_size;
    }

    bool VertexBuffer::update(const sf::Vertex *vertices)
    {
        return update(vertices, m_size, 0);
    }

    bool VertexBuffer::update(const sf::Vertex *vertices, std::size_t vertexCount, unsigned int offset)
    {
        // Sanity checks
        if (!m_buffer)
            return false;

        if (!vertices)
            return false;

        if (offset && (offset + vertexCount > m_size))
            return false;

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));

        if (vertexCount >= m_size)
        {
            glCheck(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(sf::Vertex) * vertexCount), 0, bufferUsageToGlEnum(m_usage)));

            m_size = vertexCount;
        }

        glCheck(glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(sizeof(sf::Vertex) * offset), static_cast<GLsizeiptr>(sizeof(sf::Vertex) * vertexCount), (void *)vertices));

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

        return true;
    }

    ////////////////////////////////////////////////////////////
    VertexBuffer &VertexBuffer::operator=(const VertexBuffer &right)
    {
        VertexBuffer temp(right);

        swap(temp);

        return *this;
    }

    unsigned int VertexBuffer::getNativeHandle() const
    {
        return m_buffer;
    }

    void VertexBuffer::bind() const
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_buffer));
    }
    void VertexBuffer::unbind() const
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::setPrimitiveType(sf::PrimitiveType type)
    {
        m_primitiveType = type;
    }

    sf::PrimitiveType VertexBuffer::getPrimitiveType() const
    {
        return m_primitiveType;
    }

    void VertexBuffer::setUsage(VertexBuffer::Usage usage)
    {
        m_usage = usage;
    }
    VertexBuffer::Usage VertexBuffer::getUsage() const
    {
        return m_usage;
    }

    void VertexBuffer::draw(RenderTarget &target, RenderStates states) const
    {
        if (m_buffer && m_size)
            target.draw(*this, 0, m_size, states);
    }

}; // namespace sfcg
