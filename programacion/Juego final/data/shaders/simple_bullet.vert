// Mesh::render searches for, and binds them, if they exist
in vec3 a_vertex;

in vec2 a_uv;
// in vec4 a_color;

uniform mat4 u_mvp;




void main()
{

	gl_Position = u_mvp * vec4( a_vertex, 1.0 );


}