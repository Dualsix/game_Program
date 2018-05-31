#version 330

out vec4 fragColor;

uniform sampler2D u_texture_diffuse;
uniform float transp;
in vec2 v_uv;

void main(void)
{
		
	vec3 final_color =  texture2D(u_texture_diffuse, v_uv).xyz; //el color k sale es la suma con la ambiente y la especular
		
	// simple red colour
	fragColor = vec4(texture2D(u_texture_diffuse, v_uv));
}
