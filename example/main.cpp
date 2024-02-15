#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <sfcg/SFCG.hpp>

#include <SFML/Graphics.hpp>

void printTransform(const sf::Transform &t)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", t.getMatrix()[i * 4 + j]);
        }

        printf("\n");
    }
}

int sfmain()
{
    sf::RenderWindow wnd(sf::VideoMode(800, 600), "SFML OpenGL", sf::Style::Default);
    wnd.setFramerateLimit(60);

    sf::RectangleShape shape(sf::Vector2f(100, 100));
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(2.0f);
    shape.setPosition(sf::Vector2f(150, 150));
    shape.setOrigin(sf::Vector2f(50, 50));

    float rotation = 0;
    while (wnd.isOpen())
    {
        sf::Event event;
        while (wnd.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                wnd.close();
        }

        shape.setRotation(rotation);

        // Actual rendering
        wnd.clear();

        wnd.draw(shape);

        rotation += 1;

        wnd.display();
    }
    return 0;
}

int sfcgmain()
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
    shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(4.0f);
    shape.setPosition(sf::Vector2f(150, 150));
    shape.setOrigin(sf::Vector2f(50, 50));

    float rotation = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Some dynamic animations
        shape.move(sf::Vector2f(0.1f, 0.1f));
        shape.setRotation(rotation);

        // Actual rendering
        target.clear();

        target.draw(shape);

        rotation += 0.1;

        window.display();
    }

    sfcg::cleanup();
    return 0;
}

int main()
{
    bool sfml = false;

    if (sfml)
    {
        return sfmain();
    }
    else
    {
        return sfcgmain();
    }
}