// Vertex shader:
// ==============
#version 330 core
    
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform float xCentralOffset;
uniform float yCentralOffset;

void main()
{
    gl_Position = vec4(aPos.x-xCentralOffset, aPos.y-yCentralOffset, aPos.z, 1.0);
    ourColor = aColor;
}