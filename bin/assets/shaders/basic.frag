#version 420

in vec4 fragColor;

layout(location=0) out vec4 finalColor;

void main()
{
    finalColor = fragColor;
}
