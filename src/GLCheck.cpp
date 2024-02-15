#include <sfcg/OpenGL.hpp>
#include <SFML/System.hpp>

#include <string>

// #define SFCG_GL_DEBUG

std::string extractFilenameForPath(const std::string &path)
{
    return path.substr(path.find_last_of("\\/") + 1);
}

namespace sfcg
{
    void glDumpBoundObjectsFromFile(const char *file, unsigned int line)
    {
        GLint vbo = 0;
        GLint vao = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        sf::err() << "Dump for " << extractFilenameForPath(std::string(file)) << "(" << line << "):"
                  << "Bound VBO: " << vbo << " | Bound VAO: " << vao << std::endl;
    }

    void glCheckError(const char *file, unsigned int line, const char *expression)
    {

        // Get the last error
        GLenum errorCode = glGetError();
        auto filename = extractFilenameForPath(std::string(file));

#ifdef SFCG_GL_DEBUG
        sf::err() << "GL Debug: Call " << expression << " in " << filename << "(" << line << ") "
                  << " => " << errorCode << std::endl;
#endif

        if (errorCode != GL_NO_ERROR)
        {
            std::string fileString = file;
            std::string error = "Unknown error";
            std::string description = "No description";

            // Decode the error code
            switch (errorCode)
            {
            case GL_INVALID_ENUM:
            {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE:
            {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION:
            {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in the current state.";
                break;
            }

            case GL_STACK_OVERFLOW:
            {
                error = "GL_STACK_OVERFLOW";
                description = "This command would cause a stack overflow.";
                break;
            }

            case GL_STACK_UNDERFLOW:
            {
                error = "GL_STACK_UNDERFLOW";
                description = "This command would cause a stack underflow.";
                break;
            }

            case GL_OUT_OF_MEMORY:
            {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute the command.";
                break;
            }
            }

            // Log the error
            sf::err() << "An internal OpenGL call failed in "
                      << filename << "(" << line << ")."
                      << "\nExpression:\n   " << expression
                      << "\nError description:\n   " << error << "\n   " << description << "\n"
                      << std::endl;
        }
    }
}