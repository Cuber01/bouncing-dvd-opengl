# Bouncing DVD

Bouncing DVD logo in OpenGL.

To compile, run make, you may need to compile lib/glfw3.a (glfw3 window library) yourself. 

Glad: https://glad.dav1d.de/

GLFW https://www.glfw.org/

glm: https://github.com/g-truc/glm

stb_image.h: https://github.com/nothings/stb/blob/master/stb_image.h

Great tutorial for opengl: https://learnopengl.com/

## Warning for future programmers

Do **NOT** #include .cpp files in your projects. The correct way to share a class is:
1. Class.hpp with:
```cpp
class exampleClass
{
    public:
        void update();
};
```
2. Class.cpp with:
```cpp
#include "Class.hpp"

void exampleClass::update(uint16_t x, uint16_t y)
{
    // do stuff
}
```
3. #include Class.hpp
```cpp
#include "Class.hpp"

exampleClass exampleObject;
```
