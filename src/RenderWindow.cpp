#include <sfcg/RenderWindow.hpp>

namespace sfcg
{
    RenderWindow::RenderWindow() : m_defaultFrameBuffer(0)
    {
    }

    RenderWindow::RenderWindow(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings)
        : m_defaultFrameBuffer(0)
    {
        Window::create(mode, title, style, settings);
    }

    RenderWindow::RenderWindow(sf::WindowHandle handle, const sf::ContextSettings &settings)
    {
        Window::create(handle, settings);
    }

    RenderWindow::~RenderWindow()
    {
        }

    bool RenderWindow::setActive(bool active)
    {
        bool result = Window::setActive(active);

        // // Update RenderTarget tracking
        // if (result)
        //     RenderTarget::setActive(active);

        return result;
    }

    bool RenderWindow::isSrgb() const
    {
        return getSettings().sRgbCapable;
    }

    void RenderWindow::onCreate()
    {
        RenderTarget::initialize();
    }

    void RenderWindow::onResize()
    {
        setView(getView());
    }

    sf::Vector2u RenderWindow::getSize() const
    {
        return Window::getSize();
    }
};