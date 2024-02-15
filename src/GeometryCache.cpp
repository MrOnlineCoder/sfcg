#include <sfcg/GeometryCache.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <sfcg/GLCheck.hpp>

static const char *BASE_VERTEX_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) in vec2 position;

    uniform mat4 sfcg_modelViewMatrix;
    uniform mat4 sfcg_projectionMatrix;

    void main()
    {
        gl_Position = sfcg_projectionMatrix * sfcg_modelViewMatrix * vec4(position.x, position.y, 0.0f, 1.0f);
    }
)";

static const char *BASE_FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) out vec4 fragColor;

    uniform vec4 sfcg_color;

    void main()
    {
        fragColor = sfcg_color;
    }
)";

namespace sfcg
{
    GeometryCache *GeometryCache::m_instance = nullptr;

    GeometryCache::GeometryCache()
    {
        if (m_instance)
        {
            delete m_instance;
        }

        m_instance = this;
        m_unitRectangleVao = 0;
    }

    GeometryCache::~GeometryCache()
    {
        m_instance = nullptr;
        glCheck(glDeleteVertexArrays(1, &m_unitRectangleVao));
    }

    GeometryCache &GeometryCache::getInstance()
    {
        return *m_instance;
    }

    const sfcg::VertexBuffer &GeometryCache::getUnitRectangleVertexBuffer()
    {
        if (m_unitRectangleVertexBuffer.getNativeHandle())
        {
            return m_unitRectangleVertexBuffer;
        }

        sf::Vertex vertices[] = {
            sf::Vertex(sf::Vector2f(0.0f, 0.0f)),
            sf::Vertex(sf::Vector2f(1.0f, 0.0f)),
            sf::Vertex(sf::Vector2f(1.0f, 1.0f)),
            sf::Vertex(sf::Vector2f(0.0f, 1.0f))};

        m_unitRectangleVertexBuffer.create(4);
        m_unitRectangleVertexBuffer.update(vertices);
        m_unitRectangleVertexBuffer.setPrimitiveType(sf::PrimitiveType::TriangleFan);

        glCheck(glGenVertexArrays(1, &m_unitRectangleVao));
        glCheck(glBindVertexArray(m_unitRectangleVao));
        m_unitRectangleVertexBuffer.bind();
        glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(sf::Vertex), (void *)0));
        glCheck(glEnableVertexAttribArray(0));

        return m_unitRectangleVertexBuffer;
    }

    const Shader *GeometryCache::getBaseShader()
    {
        if (m_baseShader.getNativeHandle())
        {
            return &m_baseShader;
        }

        m_baseShader.loadFromMemory(
            BASE_VERTEX_SHADER_SOURCE,
            BASE_FRAGMENT_SHADER_SOURCE);

        return &m_baseShader;
    }

    GLuint GeometryCache::getUnitRectangleVao()
    {
        return m_unitRectangleVao;
    }
} // namespace sfcg
