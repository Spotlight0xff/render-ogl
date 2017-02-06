//
// Created by spotlight on 1/28/17.
//

#ifndef GRAPHICS_ENGINE_FRAMEHANDLER_H
#define GRAPHICS_ENGINE_FRAMEHANDLER_H

namespace engine {
namespace handler {

class FrameHandler {
public:
    virtual void FrameCallback(float delta_time) = 0;
    virtual ~FrameHandler() {}

    virtual void SetWindow(GLFWwindow* window) = 0;
};

} // end namespace engine::handler
}
#endif //GRAPHICS_ENGINE_FRAMEHANDLER_H
