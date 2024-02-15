#ifndef SFCG_TEXTURE_HPP
#define SFCG_TEXTURE_HPP

#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Vector2.hpp>

namespace sfcg
{
    class Texture
    {
    public:
        Texture();
        Texture(const Texture &copy);
        ~Texture();

        bool create(unsigned int width, unsigned int height);

        bool loadFromFile(const std::string &filename, const sf::IntRect &area = sf::IntRect());
        bool loadFromMemory(const void *data, std::size_t size, const sf::IntRect &area = sf::IntRect());
        bool loadFromStream(sf::InputStream &stream, const sf::IntRect &area = sf::IntRect());
        bool loadFromImage(const sf::Image &image, const sf::IntRect &area = sf::IntRect());

        sf::Vector2u getSize() const;
        void update(const sf::Uint8 *pixels);
        void update(const sf::Uint8 *pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

        void update(const Texture &texture);
        void update(const Texture &texture, unsigned int x, unsigned int y);
    };
};

#endif