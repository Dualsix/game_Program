#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

using namespace std;

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
	//for (int i = 0; i < vao.size(); i++) {//inicialicar con multicarga
	//	vao[i] = 0;
	//}
	vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
	//for (int i = 0; i < vao.size(); i++) {	//delete con multicarga
	//	if (vao[i])glDeleteVertexArrays(1, &vao[i]);
	//}
	if (vao) glDeleteBuffers(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
	vao = 0;
}

void Mesh::genBuffers() {
	assert(vertices.size() && "No vertices in this mesh");


	//delete old
	if (vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if (normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if (uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if (colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);

	//bind vao to store state
	/*for (int i = 0; i < vao.size(); i++) {
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);
	}*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
	//for (int i = 0; i < vao.size(); i++) {//render multicarga
	//	glBindVertexArray(vao[i]);
	//}
	glBindVertexArray(vao);
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
}

void Mesh::setCollisionModel() {
	collision_model = newCollisionModel3D();

	collision_model->setTriangleNumber(indices.size() / 3);

	for (size_t i = 0; i < indices.size() / 3; i++) {
		Vector3 v1 = vertices[indices[i * 3]];
		Vector3 v2 = vertices[indices[i * 3 + 1]];
		Vector3 v3 = vertices[indices[i * 3 + 2]];
		collision_model->addTriangle(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
	}
	collision_model->finalize();
}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}

bool Mesh::loadASE(const char* filename) {
	
	TextParser t;

	if (t.create(filename) == false) {
		std::cout << "couldn't open file" << std::endl;
		return false;
	}
	//t.seek("*MATERIAL_COUNT");
	//int num_materiales = t.getint();
	//intento de multiobjetos
	//for (int k = 0; k < num_materiales; k++){
		t.seek("*MESH_NUMVERTEX");
		int num_vertices = t.getint();

		t.seek("*MESH_NUMFACES");
		int num_faces = t.getint();
		vertices.resize(num_faces * 3);
		normals.resize(num_faces * 3);
		uvs.resize(num_faces * 3);




		vector<Vector3> unique_verts;

		unique_verts.resize(num_vertices);//equival a un malloc en c

		for (int i = 0; i < num_vertices; i++) { //cojemos los vertices de las mallas
			t.seek("*MESH_VERTEX"); //pasamos el nombre
			int new_index = t.getint(); //cojemos el indice
			float new_x = t.getfloat(); //cojemos los valores de x, y i z
			float new_y = t.getfloat();
			float new_z = t.getfloat();



			Vector3 new_vec3(-new_x, new_z, new_y);
			unique_verts[new_index] = new_vec3;

		}
		for (int i = 0; i < num_faces; i++) { //cojemos los vertices de las mallas
			t.seek("*MESH_FACE"); //pasamos el nombre
			t.seek("A:");
			int vert1 = t.getint(); //cojemos el indice de la cara
			t.seek("B:");
			int vert2 = t.getint();
			t.seek("C:");
			int vert3 = t.getint();

			vertices[i * 3 + 0] = unique_verts[vert1];
			vertices[i * 3 + 1] = unique_verts[vert2];
			vertices[i * 3 + 2] = unique_verts[vert3];

		}

		vector<Vector2> unique_uvs;
		t.seek("*MESH_NUMTVERTEX");
		unique_uvs.resize(t.getint());

		for (int i = 0; i < unique_uvs.size(); i++) {
			t.seek("*MESH_TVERT");
			int index = t.getint();
			float u = t.getfloat();
			float v = t.getfloat();
			unique_uvs[index].set(u, v);

		}


		for (int i = 0; i < num_faces; i++) {
			t.seek("*MESH_TFACE"); //pasamos el nombre
			int text_indice = t.getint();
			int vert1 = t.getint(); //cojemos los vertices
			int vert2 = t.getint();
			int vert3 = t.getint();

			uvs[i * 3 + 0] = unique_uvs[vert1]; //lo guardamos a la uvs
			uvs[i * 3 + 1] = unique_uvs[vert2];
			uvs[i * 3 + 2] = unique_uvs[vert3];

		}




		t.seek("*MESH_NORMALS");
		for (int i = 0; i < num_faces; i++) {
			for (int j = 0; j < 3; j++) {
				t.seek("*MESH_VERTEXNORMAL");
				int index = t.getint();
				float nx = t.getfloat();
				float ny = t.getfloat();
				float nz = t.getfloat();
				Vector3 new_norm = Vector3(-nx, nz, ny);
				normals[i * 3 + j] = new_norm;
			}
		}



		//solo si no estamos usando normales
		 // estamos cambiando el puntero a unique_verts
		//vertices = unique_verts;

		
		

	//}
	this->genBuffers(); //copia la informacion de la gpu
	return true;
}

void Mesh::setAABB() {
	aabb.halfsize = Vector3(0, 0, 0);
	for (int i = 0; i < vertices.size(); i++) {
		//calculamos para x
		float distanciax = vertices[i].x - aabb.center.x; //calculamos la distancia desde el centro
		if (distanciax < 0) {
			distanciax = -distanciax;
		}
		if (distanciax > aabb.halfsize.x) { //cambiamos la distancia
			aabb.halfsize.x = distanciax;
		}

		//calculamos para y
		float distanciay = vertices[i].y - aabb.center.y; //calculamos la distancia desde el centro
		if (distanciay < 0) {
			distanciay = -distanciay;
		}
		if (distanciay > aabb.halfsize.y) { //cambiamos la distancia
			aabb.halfsize.y = distanciay;
		}

		//calculamos para z
		float distanciaz = vertices[i].z - aabb.center.z; //calculamos la distancia desde el centro
		if (distanciaz < 0) {
			distanciaz = -distanciaz;
		}
		if (distanciaz > aabb.halfsize.z) { //cambiamos la distancia
			aabb.halfsize.z = distanciaz;
		}
	}
}

void Mesh::createAABBGeometry() {
	std::vector<float> line_verts;
	line_verts.resize(24);
	//0
	line_verts[0] = aabb.center.x - aabb.halfsize.x;
	line_verts[1] = aabb.center.y + aabb.halfsize.y;
	line_verts[2] = aabb.center.z + aabb.halfsize.z;
	//1
	line_verts[3] = aabb.center.x - aabb.halfsize.x;
	line_verts[4] = aabb.center.y - aabb.halfsize.y;
	line_verts[5] = aabb.center.z + aabb.halfsize.z;
	//2
	line_verts[6] = aabb.center.x + aabb.halfsize.x;
	line_verts[7] = aabb.center.y - aabb.halfsize.y;
	line_verts[8] = aabb.center.z + aabb.halfsize.z;
	//3
	line_verts[9] = aabb.center.x + aabb.halfsize.x;
	line_verts[10] = aabb.center.y + aabb.halfsize.y;
	line_verts[11] = aabb.center.z + aabb.halfsize.z;
	//4
	line_verts[12] = aabb.center.x - aabb.halfsize.x;
	line_verts[13] = aabb.center.y + aabb.halfsize.y;
	line_verts[14] = aabb.center.z - aabb.halfsize.z;
	//5
	line_verts[15] = aabb.center.x - aabb.halfsize.x;
	line_verts[16] = aabb.center.y - aabb.halfsize.y;
	line_verts[17] = aabb.center.z - aabb.halfsize.z;
	//6
	line_verts[18] = aabb.center.x + aabb.halfsize.x;
	line_verts[19] = aabb.center.y - aabb.halfsize.y;
	line_verts[20] = aabb.center.z - aabb.halfsize.z;
	//7
	line_verts[21] = aabb.center.x + aabb.halfsize.x;
	line_verts[22] = aabb.center.y + aabb.halfsize.y;
	line_verts[23] = aabb.center.z - aabb.halfsize.z;


	//bind vao to store state
	glGenVertexArrays(1, &aabb.vao);
	glBindVertexArray(aabb.vao);

	GLuint aabb_vertices_vbo_id;
	glGenBuffers(1, &aabb_vertices_vbo_id); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, aabb_vertices_vbo_id); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, line_verts.size() * sizeof(float), &line_verts[0], GL_STATIC_DRAW); //upload data

																									  //shader must a_vertex attribute, error if not
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::vector<unsigned int> aabb_line_indices;
	aabb_line_indices.resize(24);
	aabb_line_indices[0] = 0;
	aabb_line_indices[1] = 1;
	aabb_line_indices[2] = 1;
	aabb_line_indices[3] = 2;
	aabb_line_indices[4] = 2;
	aabb_line_indices[5] = 3;
	aabb_line_indices[6] = 3;
	aabb_line_indices[7] = 0;
	aabb_line_indices[8] = 4;
	aabb_line_indices[9] = 5;
	aabb_line_indices[10] = 5;
	aabb_line_indices[11] = 6;
	aabb_line_indices[12] = 6;
	aabb_line_indices[13] = 7;
	aabb_line_indices[14] = 7;
	aabb_line_indices[15] = 4;
	aabb_line_indices[16] = 4;
	aabb_line_indices[17] = 0;
	aabb_line_indices[18] = 7;
	aabb_line_indices[19] = 3;
	aabb_line_indices[20] = 5;
	aabb_line_indices[21] = 1;
	aabb_line_indices[22] = 6;
	aabb_line_indices[23] = 2;

	//create index array
	GLuint aabb_indices_vbo_id;
	glGenBuffers(1, &aabb_indices_vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabb_indices_vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aabb_line_indices.size() * sizeof(unsigned int), &aabb_line_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
}

void Mesh::recta(Vector3 direccion) {
	std::vector<float> line;
	line.resize(3);

	line[0] = aabb.center.x - direccion.x*300;
	line[1] = aabb.center.y - direccion.y*300;
	line[2] = aabb.center.z - direccion.z*300;

	//bind vao to store state
	glGenVertexArrays(1, &aabb.vao);
	glBindVertexArray(aabb.vao);

	GLuint aabb_vertices_vbo_id;
	glGenBuffers(1, &aabb_vertices_vbo_id); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, aabb_vertices_vbo_id); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, line.size() * sizeof(float), &line[0], GL_STATIC_DRAW); //upload data

																								  //shader must a_vertex attribute, error if not
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	std::vector<unsigned int> line_indices;
	line_indices.resize(2);
	line_indices[0] = 0;
	line_indices[1] = 1;

	GLuint aabb_indices_vbo_id;
	glGenBuffers(1, &aabb_indices_vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabb_indices_vbo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, line_indices.size() * sizeof(unsigned int), &line_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer

}

void Mesh::renderAABB(Shader* sh) {
	glBindVertexArray(aabb.vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::renderline(Shader* sh) {
	glBindVertexArray(aabb.vao);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


