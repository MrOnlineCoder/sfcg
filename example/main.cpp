#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <sfcg/SFCG.hpp>

int main()
{
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    sf::Window window(sf::VideoMode(800, 600), "Core SFML Graphics Test", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    printf("OpenGL Version: %d, %d\n", window.getSettings().majorVersion, window.getSettings().minorVersion);

    sfcg::init();

    sfcg::RenderTarget target;

    sfcg::RectangleShape shape(sf::Vector2f(100, 100));
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Some dynamic animations
        shape.move(sf::Vector2f(0.01f, 0.01f));

        // Actual rendering
        target.clear();

        target.draw(shape);

        window.display();
    }

    sfcg::cleanup();
}