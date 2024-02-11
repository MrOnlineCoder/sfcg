#ifndef SFCG_GEOMETRY_CACHE
#define SFCG_GEOMETRY_CACHE

#include <sfcg/VertexBuffer.hpp>
#include <sfcg/Shader.hpp>

namespace sfcg
{
    class GeometryCache
    {
    public:
        GeometryCache();
        ~GeometryCache();

        static GeometryCache &getInstance();

        const sfcg::VertexBuffer &getUnitRectangleVertexBuffer();

        GLuint getUnitRectangleVao();

        const Shader *getBaseShader();

    private:
        sfcg::VertexBuffer m_unitRectangleVertexBuffer;
        Shader m_baseShader;
        GLuint m_unitRectangleVao;

        static GeometryCache *m_instance;
    };

};

#endif