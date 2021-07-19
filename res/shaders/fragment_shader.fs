#version 330 core
out vec4 FragColor;
  
in vec3 ourColor1;
in vec2 TexCoord;

uniform vec4 ourColor2;

uniform sampler2D texture1;



void main()
{
    FragColor = texture(texture1, TexCoord) * vec4(ourColor2); // use OurColor1 for a rainbow
}