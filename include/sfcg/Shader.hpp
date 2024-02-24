#ifndef SCFG_SHADER_HPP
#define SCFG_SHADER_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Window/GlResource.hpp>

#include <sfcg/OpenGL.hpp>

#include <string>

namespace sfcg
{
    class Shader : sf::NonCopyable, sf::GlResource
    {
    public:
        Shader();
        ~Shader();

        void loadFromMemory(const std::string &vertexSource, const std::string &fragmentsSource);
        void loadFromFile(const std::string &vertexPath, const std::string &fragmentPath);

        void setUniform(const std::string &name, float scalar);
        void setUniform(const std::string &name, int scalar);
        void setUniform(const std::string &name, bool value);
        void setUniform(const std::string &name, const sf::Glsl::Vec2 &vector);
        void setUniform(const std::string &name, const sf::Glsl::Vec3 &vector);
        void setUniform(const std::string &name, const sf::Glsl::Vec4 &vector);
        void setUniform(const std::string &name, const sf::Glsl::Mat3 &matrix);
        void setUniform(const std::string &name, const sf::Glsl::Mat4 &matrix);

        ////////////////////////////////////////////////////////////
        /// \brief Set a 4-component normalized (values ranging from 0 to 1) vector uniform from Color
        ///
        /// \param color Target color to set the uniform from
        ///
        ////////////////////////////////////////////////////////////
        void setUniform(const std::string &name, const sf::Color &color);
        void setUniformMatrixArray(const std::string &name, const float *scalarArray, std::size_t size);

        void bind() const;

        unsigned int getNativeHandle() const;

    private:
        bool compileAndLink(const std::string &vertexSource, const std::string &fragmentSource);

        GLuint m_program;
    };
};

#endif