#include <sfcg/GeometryCache.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <sfcg/GLCheck.hpp>

#include <cmath>

static const char *BASE_VERTEX_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec4 color;
    layout (location = 2) in vec2 texCoord;

    out vec2 frag_texCoord;
    out vec4 frag_vertexColor;

    uniform mat4 sfcg_modelViewMatrix;
    uniform mat4 sfcg_projectionMatrix;

    void main()
    {
        gl_Position = sfcg_projectionMatrix * sfcg_modelViewMatrix * vec4(position.x, position.y, 0.0f, 1.0f);
        frag_texCoord = texCoord;
        frag_vertexColor = color;
    }
)";

static const char *BASE_FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) out vec4 fragColor;

    in vec2 frag_texCoord;

    uniform vec4 sfcg_color;

    uniform sampler2D sfcg_texture;

    void main()
    {
        fragColor = texture(sfcg_texture, frag_texCoord) * sfcg_color;
    }
)";

static const char *TEXT_FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) out vec4 fragColor;

    in vec2 frag_texCoord;
    in vec4 frag_vertexColor;

    uniform vec4 sfcg_color;

    uniform sampler2D sfcg_texture;

    void main()
    {
        vec4 sampled = texture(sfcg_texture, frag_texCoord);
        fragColor = sampled * frag_vertexColor;
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

        m_unitCircleVertexBuffers.reserve(4);
        m_unitCircleVaos.reserve(4);

        sf::Image whitePixel;
        whitePixel.create(1, 1, sf::Color::White);
        m_whitePixelTexture.loadFromImage(whitePixel);
    }

    GeometryCache::~GeometryCache()
    {
        m_instance = nullptr;
        glCheck(glDeleteVertexArrays(1, &m_unitRectangleVao));

        for (auto &pair : m_unitCircleVaos)
        {
            glCheck(glDeleteVertexArrays(1, &pair.second));
        }

        m_unitCircleVaos.clear();
    }

    GeometryCache &GeometryCache::getInstance()
    {
        return *m_instance;
    }

    const sfcg::VertexBuffer &GeometryCache::getUnitCircleVertexBuffer(int pointCount)
    {
        auto it = m_unitCircleVertexBuffers.find(pointCount);

        if (it != m_unitCircleVertexBuffers.end())
        {
            return it->second;
        }

        // Create the unit circle vertices (+2 for center and last point to close the circle)
        sf::Vertex vertices[pointCount + 2];

        vertices[0] = sf::Vertex(sf::Vector2f(0.f, 0.f));

        for (int i = 0; i < pointCount; i++)
        {
            float angle = static_cast<float>(i) * 2.f * M_PI / static_cast<float>(pointCount) - M_PI / 2.f;

            float x = std::cos(angle);
            float y = std::sin(angle);

            vertices[i + 1] = sf::Vertex(sf::Vector2f(x, y));
        }

        // Last point always finishes the circle at this position
        vertices[pointCount + 1] = sf::Vertex(sf::Vector2f(0, -1));

        m_unitCircleVertexBuffers.insert(std::make_pair(pointCount, sfcg::VertexBuffer()));
        m_unitCircleVertexBuffers[pointCount].setPrimitiveType(sf::PrimitiveType::TriangleFan);
        m_unitCircleVertexBuffers[pointCount].setUsage(sfcg::VertexBuffer::Usage::Static);
        m_unitCircleVertexBuffers[pointCount].create(pointCount + 2);
        m_unitCircleVertexBuffers[pointCount].update(vertices);

        GLuint unitCircleVao = 0;

        glCheck(glGenVertexArrays(1, &unitCircleVao));
        glCheck(glBindVertexArray(unitCircleVao));
        m_unitCircleVertexBuffers[pointCount].bind();
        configureVaoAttributesForVertices();
        glCheck(glBindVertexArray(0));

        m_unitCircleVaos.insert(std::make_pair(pointCount, unitCircleVao));

        return m_unitCircleVertexBuffers[pointCount];
    }

    GLuint GeometryCache::getUnitCircleVao(int pointCount)
    {
        return m_unitCircleVaos[pointCount];
    }

    const sfcg::VertexBuffer &GeometryCache::getUnitRectangleVertexBuffer()
    {
        // Use cache if needed
        if (m_unitRectangleVertexBuffer.getNativeHandle())
        {
            return m_unitRectangleVertexBuffer;
        }

        sf::Vertex vertices[] = {
            sf::Vertex(sf::Vector2f(0.0f, 0.0f)),
            sf::Vertex(sf::Vector2f(1.0f, 0.0f)),
            sf::Vertex(sf::Vector2f(1.0f, 1.0f)),
            sf::Vertex(sf::Vector2f(0.0f, 1.0f))};

        // Create VBO for unit rectangle and fill it
        m_unitRectangleVertexBuffer.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
        m_unitRectangleVertexBuffer.setUsage(sfcg::VertexBuffer::Usage::Static);
        m_unitRectangleVertexBuffer.create(4);
        m_unitRectangleVertexBuffer.update(vertices);

        // The VAO
        glCheck(glGenVertexArrays(1, &m_unitRectangleVao));
        glCheck(glBindVertexArray(m_unitRectangleVao));

        m_unitRectangleVertexBuffer.bind();

        configureVaoAttributesForVertices();

        glCheck(glBindVertexArray(0));

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

    const Shader *GeometryCache::getTextShader()
    {
        if (m_textShader.getNativeHandle())
        {
            return &m_textShader;
        }

        m_textShader.loadFromMemory(
            BASE_VERTEX_SHADER_SOURCE,
            TEXT_FRAGMENT_SHADER_SOURCE);

        return &m_textShader;
    }

    void GeometryCache::configureVaoAttributesForVertices()
    {
        // Position
        glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(sf::Vertex), (void *)0));
        glCheck(glEnableVertexAttribArray(0));

        // Color
        glCheck(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(sf::Vertex), (void *)sizeof(sf::Vector2f)));
        glCheck(glEnableVertexAttribArray(1));

        // Texture Coord
        glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sf::Vertex), (void *)(sizeof(sf::Vector2f) + sizeof(sf::Color))));
        glCheck(glEnableVertexAttribArray(2));
    }

    GLuint GeometryCache::getUnitRectangleVao()
    {
        return m_unitRectangleVao;
    }

    const sf::Texture &GeometryCache::getWhitePixelTexture()
    {
        return m_whitePixelTexture;
    }
} // namespace sfcg
