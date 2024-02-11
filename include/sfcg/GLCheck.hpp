#ifndef SFCG_GL_CHECK_HPP
#define SFCG_GL_CHECK_HPP

#define glCheck(expr)                                  \
    do                                                 \
    {                                                  \
        expr;                                          \
        sfcg::glCheckError(__FILE__, __LINE__, #expr); \
    } while (false)

namespace sfcg
{
    void glCheckError(const char *file, unsigned int line, const char *expression);
};
#endif