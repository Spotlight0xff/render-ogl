#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <vector>


#include "opengl.h"

//! Collection of useful methods
namespace util {

/*!
 * Splits the given string on `delimiter`.
 *
 * @param s string to process
 * @param delimiter delimiting character
 * @param splits vector to save splits to
 * @return number of splits (size of `splits`)
 */
size_t split(const std::string& s, char delimiter, std::vector<std::string>& splits);

/*!
 * Checks if the file exists.
 *
 * @param file file to check
 * @return `true` if it exists, `false` otherwise.
 */
bool fileExists(std::string const& file);


/*!
 * Replaces all occurences of `from` to `to` in the given string.
 *
 * @param source string to process
 * @param from substring to be replaced
 * @param to substring to replace
 */
void replaceAllOccurences( std::string& source, std::string const& from, std::string const& to);


/*!
 * Debug method to print vertex data.
 *
 * @param count_elements number of elements in `flat_elements`
 * @param flat_elements pointer to array of elements
 * @param count_vertices number of vertices in `flat_vertices`
 * @param flat_vertices pointer to array of vertices
 */
void printVertices(size_t count_elements, GLushort const* flat_elements, size_t count_vertices, GLfloat const* flat_vertices);

//! OpenGL Debug function
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei /*length*/,
                            const GLchar *message,
                            const void * /*userParam*/);

//! Enable OpenGL debug functionality
//! \return `true` on success, `false` otherwise.
bool enableDebugOutput();

std::vector<std::string> getEligiblePaths(const std::string& path1, const std::string& path2);

}  // end namespace util

#endif
