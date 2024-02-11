#ifndef SCFG_SHADER_HPP
#define SCFG_SHADER_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Glsl.hpp>

#include <sfcg/OpenGL.hpp>

#include <string>

namespace sfcg
{
    class Shader : sf::NonCopyable
    {
    public:
        Shader();
        ~Shader();

        void loadFromMemory(const std::string &vertexSource, const std::string &fragmentsSource);

        void setUniform(const std::string &name, const sf::Glsl::Vec4 &vector);
        void setUniform(const std::string &name, const sf::Glsl::Mat3 &matrix);
        void setUniformMatrixArray(const std::string &name, const float *scalarArray, std::size_t size);

        void bind() const;

        unsigned int getNativeHandle() const;

    private:
        bool compileAndLink(const std::string &vertexSource, const std::string &fragmentSource);

        GLuint m_program;
    };
};

#endif