#ifndef SFCG_GEOMETRY_CACHE
#define SFCG_GEOMETRY_CACHE

#include <sfcg/VertexBuffer.hpp>
#include <sfcg/Shader.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <unordered_map>

namespace sfcg
{
    class GeometryCache
    {
    public:
        GeometryCache();
        ~GeometryCache();

        static GeometryCache &getInstance();

        const sfcg::VertexBuffer &getUnitRectangleVertexBuffer();

        const sfcg::VertexBuffer &getUnitCircleVertexBuffer(int pointCount);

        GLuint getUnitRectangleVao();
        GLuint getUnitCircleVao(int pointCount);
        GLuint getSpriteVao();

        const Shader *getBaseShader();
        const Shader *getSpriteShader();

        void configureVaoAttributesForVertices();

        const sf::Texture &getWhitePixelTexture();

    private:
        sfcg::VertexBuffer m_unitRectangleVertexBuffer;

        std::unordered_map<int, sfcg::VertexBuffer> m_unitCircleVertexBuffers;
        std::unordered_map<int, GLuint> m_unitCircleVaos;

        Shader m_baseShader;
        Shader m_spriteShader;

        sf::Texture m_whitePixelTexture;

        GLuint m_unitRectangleVao;
        GLuint m_spriteVao;

        static GeometryCache *m_instance;
    };

};

#endif