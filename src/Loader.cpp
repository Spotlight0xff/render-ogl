#include "Loader.h"
#include "Util.h"

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace loader {

bool parseObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned short>& out_elements,
    FaceFormat face_format) {
  std::ifstream in(path);
  if (!in.good()) {
    std::cerr << "Failed to open " << path << "\n";
    return false;
  }

  std::string line;
  in >> std::ws;
  while(std::getline(in, line)) {
    if        (line.substr(0,2) == "v ") { // geometric vertex
      std::istringstream ss(line.substr(2));
      glm::vec3 v;
      ss >> v.x; ss >> v.y; ss >> v.z;
      out_vertices.push_back(v);
    } else if (line.substr(0, 2) == "f ") { // face
      if (face_format == FaceFormat::SPACE) { // "a b c"
        std::istringstream ss(line.substr(2));
        unsigned short a, b, c;
        ss >> a; ss >> b; ss >> c;
        a --; b --; c--;
        out_elements.insert(out_elements.end(), {a, b, c});
      } else if (face_format == FaceFormat::SLASH) { // (vertex/texcoord/normal)
        std::istringstream ss(line.substr(2));
        std::string sa, sb, sc;
        ss >> sa; ss >> sb; ss >> sc;
        std::vector<std::string> split_a, split_b, split_c;
        util::split(sa, '/', split_a);
        util::split(sb, '/', split_b);
        util::split(sc, '/', split_c);
        GLushort a, b, c;
        a = static_cast<unsigned short>(std::stoul(split_a[0]));
        b = static_cast<unsigned short>(std::stoul(split_b[0]));
        c = static_cast<unsigned short>(std::stoul(split_c[0]));
        a --; b --; c --;
        out_elements.insert(out_elements.end(), {a, b, c});

      }
    } else if (line.substr(0, 3) == "vn ") { // normal
    } else if (line.substr(0, 2) == "o ") { // object name
    } else if (line.substr(0, 3) == "vt ") { // texture vertex
    } else if (line.substr(0, 2) == "g ") { // group name
    } else if (line.substr(0, 7) == "mtllib ") { // material library
    } else if (line.substr(0, 7) == "usemtl ") { // material name
    } else if (line.substr(0, 2) == "s ") { // smooth shading
    } else if (*line.begin() == '#' || // comment, ignore
               line.empty()) {
    } else {
      std::cout << "Unhandled line: '" << line << "'" << std::endl;
    }
    while (in.peek() == ' ' ||
           in.peek() == '\r') // skip spaces
      in.get();
  }

  // Compute normals
  out_normals.resize(out_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  for (size_t i=0; i < out_elements.size(); i += 3) {
    GLushort a = out_elements[i];
    GLushort b = out_elements[i+1];
    GLushort c = out_elements[i+2];
    glm::vec3 normal = glm::normalize(glm::cross(
          glm::vec3(out_vertices[b]) - glm::vec3(out_vertices[a]),
          glm::vec3(out_vertices[c]) - glm::vec3(out_vertices[a])));
    out_normals[a] = out_normals[b] = out_normals[c] = normal;
  }

  return true;
}

}
