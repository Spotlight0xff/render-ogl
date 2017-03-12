from conans import ConanFile, CMake

class EngineBuildConan(ConanFile):
    name = "render-ogl"
    license = "MIT"
    url = "https://github.com/spotlight0xff/render-ogl"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "CMakeLists.txt"
    requires = "glew/2.0.0@coding3d/stable","assimp/3.3.1@lasote/vcpkg","glfw3/3.2.1@lasote/vcpkg","freetype/2.6.3@lasote/stable"
    

    def build(self):
        cmake = CMake(self.settings)
        cmake.configure(self)
        cmake.build(self)

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = []