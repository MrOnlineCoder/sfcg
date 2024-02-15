#ifndef SFCG_GEOMETRY_CACHE
#define SFCG_GEOMETRY_CACHE

#include <sfcg/VertexBuffer.hpp>
#include <sfcg/Shader.hpp>

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

        const Shader *getBaseShader();

    private:
        sfcg::VertexBuffer m_unitRectangleVertexBuffer;

        std::unordered_map<int, sfcg::VertexBuffer> m_unitCircleVertexBuffers;
        std::unordered_map<int, GLuint> m_unitCircleVaos;

        Shader m_baseShader;
        GLuint m_unitRectangleVao;
        GLuint m_unitCircleVao;

        static GeometryCache *m_instance;
    };

};

#endif