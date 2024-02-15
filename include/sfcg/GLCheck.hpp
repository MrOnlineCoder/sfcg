#ifndef SFCG_GL_CHECK_HPP
#define SFCG_GL_CHECK_HPP

#define glCheck(expr)                                  \
    do                                                 \
    {                                                  \
        expr;                                          \
        sfcg::glCheckError(__FILE__, __LINE__, #expr); \
    } while (false)

#define glDumpBoundObjects() sfcg::glDumpBoundObjectsFromFile(__FILE__, __LINE__)

namespace sfcg
{
    void glCheckError(const char *file, unsigned int line, const char *expression);
    void glDumpBoundObjectsFromFile(const char *file, unsigned int line);
};
#endif