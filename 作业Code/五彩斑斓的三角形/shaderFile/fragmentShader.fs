#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

uniform vec4 updateColor;

void main()
{
	FragColor = vec4(ourColor.r + updateColor.r, 
	ourColor.g + updateColor.g, 
	ourColor.b + updateColor.b, 
	1.0);
}