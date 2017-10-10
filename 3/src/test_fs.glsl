#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
	vec4 t1 = texture(texture1, TexCoord);
	vec4 t2 = texture(texture2, TexCoord);
	vec4 t3 = texture(texture3, TexCoord);
	//FragColor = mix(t1, t2, 0.2) + t3.a;
   FragColor = mix(t1, t2, 0.2) + t3;
}