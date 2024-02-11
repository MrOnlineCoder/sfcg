#ifndef SFCG_DRAWABLE_HPP
#define SFCG_DRAWABLE_HPP

namespace sfcg
{
    class RenderTarget;
    class RenderStates;

    class Drawable
    {
    public:
        virtual ~Drawable() {}

    protected:
        friend class RenderTarget;

        virtual void draw(RenderTarget &target, RenderStates states) const = 0;
    };
};

#endif