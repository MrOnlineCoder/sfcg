#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <sfcg/RenderTarget.hpp>

namespace sfcg
{
    class RenderWindow : public sf::Window, public RenderTarget
    {
    public:
        RenderWindow();

        ////////////////////////////////////////////////////////////
        /// \brief Construct a new window
        ///
        /// This constructor creates the window with the size and pixel
        /// depth defined in \a mode. An optional style can be passed to
        /// customize the look and behavior of the window (borders,
        /// title bar, resizable, closable, ...).
        ///
        /// The fourth parameter is an optional structure specifying
        /// advanced OpenGL context settings such as antialiasing,
        /// depth-buffer bits, etc. You shouldn't care about these
        /// parameters for a regular usage of the graphics module.
        ///
        /// \param mode     Video mode to use (defines the width, height and depth of the rendering area of the window)
        /// \param title    Title of the window
        /// \param style    %Window style, a bitwise OR combination of sf::Style enumerators
        /// \param settings Additional settings for the underlying OpenGL context
        ///
        ////////////////////////////////////////////////////////////
        RenderWindow(sf::VideoMode mode, const sf::String &title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings &settings = sf::ContextSettings());

        ////////////////////////////////////////////////////////////
        /// \brief Construct the window from an existing control
        ///
        /// Use this constructor if you want to create an SFML
        /// rendering area into an already existing control.
        ///
        /// The second parameter is an optional structure specifying
        /// advanced OpenGL context settings such as antialiasing,
        /// depth-buffer bits, etc. You shouldn't care about these
        /// parameters for a regular usage of the graphics module.
        ///
        /// \param handle   Platform-specific handle of the control (\a HWND on
        ///                 Windows, \a %Window on Linux/FreeBSD, \a NSWindow on OS X)
        /// \param settings Additional settings for the underlying OpenGL context
        ///
        ////////////////////////////////////////////////////////////
        explicit RenderWindow(sf::WindowHandle handle, const sf::ContextSettings &settings = sf::ContextSettings());

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Closes the window and frees all the resources attached to it.
        ///
        ////////////////////////////////////////////////////////////
        virtual ~RenderWindow();

        ////////////////////////////////////////////////////////////
        /// \brief Get the size of the rendering region of the window
        ///
        /// The size doesn't include the titlebar and borders
        /// of the window.
        ///
        /// \return Size in pixels
        ///
        ////////////////////////////////////////////////////////////
        virtual sf::Vector2u getSize() const;

        ////////////////////////////////////////////////////////////
        /// \brief Tell if the window will use sRGB encoding when drawing on it
        ///
        /// You can request sRGB encoding for a window by having the sRgbCapable flag set in the ContextSettings
        ///
        /// \return True if the window use sRGB encoding, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        virtual bool isSrgb() const;

        ////////////////////////////////////////////////////////////
        /// \brief Activate or deactivate the window as the current target
        ///        for OpenGL rendering
        ///
        /// A window is active only on the current thread, if you want to
        /// make it active on another thread you have to deactivate it
        /// on the previous thread first if it was active.
        /// Only one window can be active on a thread at a time, thus
        /// the window previously active (if any) automatically gets deactivated.
        /// This is not to be confused with requestFocus().
        ///
        /// \param active True to activate, false to deactivate
        ///
        /// \return True if operation was successful, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        bool setActive(bool active = true);

    protected:
        ////////////////////////////////////////////////////////////
        /// \brief Function called after the window has been created
        ///
        /// This function is called so that derived classes can
        /// perform their own specific initialization as soon as
        /// the window is created.
        ///
        ////////////////////////////////////////////////////////////
        virtual void onCreate();

        ////////////////////////////////////////////////////////////
        /// \brief Function called after the window has been resized
        ///
        /// This function is called so that derived classes can
        /// perform custom actions when the size of the window changes.
        ///
        ////////////////////////////////////////////////////////////
        virtual void onResize();

    private:
        unsigned int m_defaultFrameBuffer;
    };
};