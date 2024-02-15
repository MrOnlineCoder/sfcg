#include <sfcg/Shader.hpp>
#include <sfcg/GLCheck.hpp>
#include <SFML/System.hpp>

namespace sfcg
{
    class UniformContextSaver
    {
    public:
        UniformContextSaver(GLuint newProgram)
        {
            glGetIntegerv(GL_CURRENT_PROGRAM, &m_activeProgram);
            glCheck(glUseProgram(newProgram));
        }

        ~UniformContextSaver()
        {
            glCheck(glUseProgram(m_activeProgram));
        }

    private:
        GLint m_activeProgram;
    };

    bool Shader::compileAndLink(const std::string &vertexSource, const std::string &fragmentSource)
    {
        m_program = glCreateProgram();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char *vertexSourceCStr = vertexSource.c_str();
        const char *fragmentSourceCStr = fragmentSource.c_str();

        glCheck(glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr));
        glCheck(glCompileShader(vertexShader));

        GLint vertexCompileStatus;
        glCheck(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompileStatus));

        if (vertexCompileStatus != GL_TRUE)
        {
            GLint logLength;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

            std::string log(logLength, ' ');
            glGetShaderInfoLog(vertexShader, logLength, &logLength, &log[0]);

            sf::err() << "Failed to compile vertex shader:" << std::endl
                      << log << std::endl;

            glCheck(glDeleteShader(vertexShader));
            glCheck(glDeleteShader(fragmentShader));
            glCheck(glDeleteProgram(m_program));

            return false;
        }

        glCheck(glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr));
        glCheck(glCompileShader(fragmentShader));

        GLint fragmentCompileStatus;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompileStatus);

        if (fragmentCompileStatus != GL_TRUE)
        {
            GLint logLength;
            glCheck(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength));

            std::string log(logLength, ' ');
            glCheck(glGetShaderInfoLog(fragmentShader, logLength, &logLength, &log[0]));

            sf::err() << "Failed to compile fragment shader:" << std::endl
                      << log << std::endl;

            glCheck(glDeleteShader(vertexShader));
            glCheck(glDeleteShader(fragmentShader));
            glCheck(glDeleteProgram(m_program));

            return false;
        }

        glCheck(glAttachShader(m_program, vertexShader));
        glCheck(glAttachShader(m_program, fragmentShader));

        glCheck(glLinkProgram(m_program));

        GLint linkStatus;
        glCheck(glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus));

        if (linkStatus != GL_TRUE)
        {
            GLint logLength;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

            std::string log(logLength, ' ');
            glGetProgramInfoLog(m_program, logLength, &logLength, &log[0]);

            sf::err() << "Failed to link shader program:" << std::endl
                      << log << std::endl;

            glCheck(glDeleteShader(vertexShader));
            glCheck(glDeleteShader(fragmentShader));
            glCheck(glDeleteProgram(m_program));

            return false;
        }

        glCheck(glDeleteShader(vertexShader));
        glCheck(glDeleteShader(fragmentShader));

        return true;
    }

    Shader::Shader() : m_program(0)
    {
    }
    Shader::~Shader()
    {
        if (m_program)
        {
            glCheck(glDeleteProgram(m_program));
        }
    }

    void Shader::loadFromMemory(const std::string &vertexSource, const std::string &fragmentsSource)
    {
        if (m_program)
            return;

        compileAndLink(vertexSource, fragmentsSource);
    }

    void Shader::setUniform(const std::string &name, const sf::Glsl::Vec4 &vector)
    {
        UniformContextSaver contextSaver(m_program);

        auto location = glGetUniformLocation(
            m_program,
            name.c_str());

        if (location != -1)
        {
            glCheck(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
        }
    }
    void Shader::setUniform(const std::string &name, const sf::Glsl::Mat3 &matrix)
    {
        UniformContextSaver contextSaver(m_program);

        auto location = glGetUniformLocation(
            m_program,
            name.c_str());

        if (location != -1)
        {
            glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, matrix.array));
        }
    }

    void Shader::setUniform(const std::string &name, const sf::Color &color)
    {
        UniformContextSaver contextSaver(m_program);

        auto location = glGetUniformLocation(
            m_program,
            name.c_str());

        if (location != -1)
        {
            glCheck(glUniform4f(location, (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f));
        }
    }

    void Shader::bind() const
    {
        if (m_program)
            glCheck(glUseProgram(m_program));
    }

    void Shader::setUniformMatrixArray(const std::string &name, const float *scalarArray, std::size_t size)
    {
        UniformContextSaver contextSaver(m_program);
        auto location = glGetUniformLocation(
            m_program,
            name.c_str());

        if (location != -1)
        {
            if (size == 2)
                glCheck(glUniformMatrix2fv(location, 1, GL_FALSE, scalarArray));
            else if (size == 3)
                glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, scalarArray));
            else if (size == 4)
                glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, scalarArray));
        }
    }

    unsigned int Shader::getNativeHandle() const
    {
        return m_program;
    }
}