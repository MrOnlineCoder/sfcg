#ifndef SFCG_RENDER_TARGET_HPP
#define SFCG_RENDER_TARGET_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <sfcg/OpenGL.hpp>
#include <sfcg/VertexBuffer.hpp>
#include <sfcg/RenderStates.hpp>

namespace sfcg
{
    class Drawable;

    namespace ShaderUniformLocations
    {
        const static char *Color = "sfcg_color";
        const static char *ModelMatrix = "sfcg_modelViewMatrix";
        const static char *ProjectionMatrix = "sfcg_projectionMatrix";
    };

    class RenderTarget
    {
    public:
        RenderTarget();
        virtual ~RenderTarget();

        void clear(sf::Color color = sf::Color::Black);

        void draw(const Drawable &drawable);
        void draw(const sfcg::VertexBuffer &vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates &states = RenderStates::Default);

        const sf::View &getDefaultView() const;
        const sf::View &getView() const;
        void setView(const sf::View &view);

        ////////////////////////////////////////////////////////////
        /// \brief Bind a texture for rendering
        /// \param texture Texture to bind or NULL pointer to cancel any binding
        ////////////////////////////////////////////////////////////
        void bindTexture(const sf::Texture *texture);

        sf::Vector2f mapPixelToCoords(const sf::Vector2i &point, const sf::View &view) const;
        sf::Vector2f mapPixelToCoords(const sf::Vector2i &point) const;
        sf::Vector2i mapCoordsToPixel(const sf::Vector2f &point, const sf::View &view) const;
        sf::Vector2i mapCoordsToPixel(const sf::Vector2f &point) const;

        sf::IntRect getViewport(const sf::View &view) const;

        ////////////////////////////////////////////////////////////
        /// \brief Return the size of the rendering region of the target
        ///
        /// \return Size in pixels
        ///
        ////////////////////////////////////////////////////////////
        virtual sf::Vector2u getSize() const = 0;

        ////////////////////////////////////////////////////////////
        /// \brief Tell if the render target will use sRGB encoding when drawing on it
        ///
        /// \return True if the render target use sRGB encoding, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        virtual bool isSrgb() const;

        ////////////////////////////////////////////////////////////
        /// \brief Activate or deactivate the render target for rendering
        ///
        /// This function makes the render target's context current for
        /// future OpenGL rendering operations (so you shouldn't care
        /// about it if you're not doing direct OpenGL stuff).
        /// A render target's context is active only on the current thread,
        /// if you want to make it active on another thread you have
        /// to deactivate it on the previous thread first if it was active.
        /// Only one context can be current in a thread, so if you
        /// want to draw OpenGL geometry to another render target
        /// don't forget to activate it again. Activating a render
        /// target will automatically deactivate the previously active
        /// context (if any).
        ///
        /// \param active True to activate, false to deactivate
        ///
        /// \return True if operation was successful, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        // TODO
        // virtual bool setActive(bool active = true);

        ////////////////////////////////////////////////////////////
        /// \brief Reset OpenGL states to values, required by SFML
        ///
        /// This function can be used when you mix SFML drawing with OpenGL
        /// Just make sure to call it before starting 2D rendering
        /// and to reset it afterwards for your needs
        ///
        ////////////////////////////////////////////////////////////
        void resetGLStates();

    protected:
        void initialize();

    private:
        void drawPrimitives(sf::PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount);

        GLuint m_vao;

        sf::View m_view;
        sf::View m_defaultView;
    };
}; // namespace sfcg

#endif