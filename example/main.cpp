#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#define SFCG_GL_DEBUG 1

#include <sfcg/SFCG.hpp>

int main()
{
    // Request modern OpenGL context
    // 4.1 here is because MacOS now ships with it only
    sf::ContextSettings settings;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    sfcg::RenderWindow window(sf::VideoMode(800, 600), "Core SFML Graphics Test", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // Print the OpenGL version
    printf("OpenGL Version: %d, %d\n", window.getSettings().majorVersion, window.getSettings().minorVersion);

    // This must be called before any other sfcg function
    // It creates some internal shaders, VAOs and VBOs
    // May be subject to change in future!
    sfcg::init();

    // Let's make a circle
    sfcg::CircleShape circle(50, 50);
    circle.setPosition(sf::Vector2f(400, 300));
    circle.setFillColor(sf::Color::Green);
    circle.setOrigin(25, 25);

    // A rectangle
    sfcg::RectangleShape shape(sf::Vector2f(100, 100));
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(4.0f);
    shape.setPosition(sf::Vector2f(150, 150));
    shape.setOrigin(sf::Vector2f(50, 50));

    // sf::Texture works fine in this OpenGL profile
    sf::Texture tex;
    tex.loadFromFile("../example/texture.png");

    // And some fancy sprites
    sfcg::Sprite sprite(tex);
    sprite.setPosition(0, 0);
    sprite.setPosition(sf::Vector2f(100, 32));
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    // sf::Font also works fine, it does not depend on much OpenGL aside from just textures
    sf::Font fnt;
    fnt.loadFromFile("../example/roboto.ttf");

    // And some text
    sfcg::Text text("Hello World!", fnt, 32);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);
    text.setStyle(sf::Text::StrikeThrough | sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);

    sfcg::Text fpsText;
    fpsText.setFont(fnt);
    fpsText.setCharacterSize(16);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(32, 32);

    // We also support convex shapes
    sfcg::ConvexShape hexagon(6);
    hexagon.setPoint(0, sf::Vector2f(0, -1));
    hexagon.setPoint(1, sf::Vector2f(0.866f, -0.5f));
    hexagon.setPoint(2, sf::Vector2f(0.866f, 0.5f));
    hexagon.setPoint(3, sf::Vector2f(0, 1));
    hexagon.setPoint(4, sf::Vector2f(-0.866f, 0.5f));
    hexagon.setPoint(5, sf::Vector2f(-0.866f, -0.5f));
    hexagon.setFillColor(sf::Color::Magenta);
    hexagon.setScale(sf::Vector2f(100, 100));
    hexagon.setPosition(100, 500);

    float rotation = 0;

    sf::Clock fpsClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    shape.setSize(sf::Vector2f(200, 200));
                    shape.setOrigin(sf::Vector2f(100, 100));

                    sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
                }

                // Press P to switch to wireframe mode (just fancy debugging)
                if (event.key.code == sf::Keyboard::P)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    shape.setSize(sf::Vector2f(100, 100));
                    shape.setOrigin(sf::Vector2f(50, 50));
                    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                }

                if (event.key.code == sf::Keyboard::P)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        }

        // Some dynamic animations
        rotation += 0.1;
        shape.move(sf::Vector2f(0.1f, 0.1f));
        shape.setRotation(rotation);

        // Show FPS
        float fps = 1.0f / fpsClock.restart().asSeconds();

        fpsText.setString("FPS: " + std::to_string((int)fps));

        // Actual rendering
        window.clear();

        window.draw(circle);

        window.draw(shape);

        window.draw(sprite);

        window.draw(hexagon);

        window.draw(text);

        window.draw(fpsText);

        window.display();
    }

    sfcg::cleanup();
    return 0;
}