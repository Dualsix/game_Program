#version 330
 
// the following attributes are supported by this engine
// Mesh::render searches for, and binds them, if they exist
in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_uv;
// in vec4 a_color;

uniform mat4 u_mvp;
uniform mat4 u_normal_matrix;//matriz de normales

uniform vec3 u_camera_pos;

out vec3 v_normal;//enviamos al fragment
out vec3 v_camera; 
out vec2 v_uv;

void main()
{
	v_normal = (u_normal_matrix * vec4(a_normal, 1.0)).xyz; //pasamos la normal

	v_camera = u_camera_pos - a_vertex;//calculamos el vector hacia la camara

	//calculate the position of the vertex using the matrices
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );

	v_uv = a_uv;

}




