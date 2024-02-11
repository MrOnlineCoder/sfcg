#include <sfcg/Init.hpp>
#include <sfcg/GeometryCache.hpp>
#include <sfcg/RenderStates.hpp>

namespace sfcg
{
    static GeometryCache *s_geometryCache = nullptr;

    const RenderStates RenderStates::Default = RenderStates();

    void init()
    {
        if (s_geometryCache)
        {
            return;
        }
        s_geometryCache = new GeometryCache();
    }

    void cleanup()
    {
        delete s_geometryCache;
        s_geometryCache = nullptr;
    }
}