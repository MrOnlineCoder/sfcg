#ifndef SFCG_VERTEXBUFFER_HPP
#define SFCG_VERTEXBUFFER_HPP

#include <string>

#include <sfcg/Drawable.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <SFML/Window/GlResource.hpp>

namespace sfcg
{
    class RenderTarget;
    class Vertex;
    class RenderStates;

    class VertexBuffer : public Drawable, public sf::GlResource
    {
    public:
        enum Usage
        {
            Stream,
            Dynamic,
            Static
        };

        VertexBuffer();

        explicit VertexBuffer(sf::PrimitiveType type);
        explicit VertexBuffer(Usage usage);

        VertexBuffer(sf::PrimitiveType type, Usage usage);

        ////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// \param copy instance to copy
        ///
        ////////////////////////////////////////////////////////////
        VertexBuffer(const VertexBuffer &copy);
        VertexBuffer &operator=(const VertexBuffer &right);
        void swap(VertexBuffer &right);
        ~VertexBuffer();

        bool create(std::size_t vertexCount);
        bool update(const VertexBuffer &vertexBuffer);
        bool update(const sf::Vertex *vertices);
        bool update(const sf::Vertex *vertices, std::size_t vertexCount, unsigned int offset);

        std::size_t getVertexCount() const;
        unsigned int getNativeHandle() const;
        void setPrimitiveType(sf::PrimitiveType type);
        sf::PrimitiveType getPrimitiveType() const;
        void setUsage(Usage usage);

        Usage getUsage() const;

        void bind() const;
        void unbind() const;

    protected:
        virtual void draw(RenderTarget &target, RenderStates states) const;

    private:
        unsigned int m_buffer;
        std::size_t m_size;
        sf::PrimitiveType m_primitiveType;
        Usage m_usage;
    };
}; // namespace sfcg
#endif
