#version 330 core
out vec4 FragColor;
uniform vec4 ourColor_temp;
void main()
{
	FragColor = ourColor_temp;
}