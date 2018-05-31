#include "LevelParser.h"
#include "managers.h"
#include "GameObjectEnvironment.h"
#include "GameObjectMesh.h"
#include "GameObjectLight.h"
#include "GameObjectPlayer.h"
#include "bulletManager.h"
#include "escudo.h"



namespace LevelParser{
	void vectorcolisiones(std::vector<GameObject*>& objetos, GameObject* padre) {
		for (int i = 0; i < padre->children.size(); i++) {
			if (padre->children[i]->colision) {
				objetos.push_back(padre->children[i]);
				if (padre->children[i]->children.size() > 0) {
					vectorcolisiones(objetos, padre->children[i]);
				}
			}
		}
	}

	void loadObject(xml_node<> *mesh_node, GameObject* parent) {
		
		//codigo para añadir objeto al scene graph
		//crear gameobject nuevo
		GameObjectMesh* new_mesh = new GameObjectMesh();
		new_mesh->colision = true;
		//load properties from mesh_node
		xml_node<> *node_name = mesh_node->first_node("name");
		xml_node<> *node_mesh = mesh_node->first_node("mesh");
		xml_node<> *node_shader = mesh_node->first_node("shader");
		xml_node<> *node_shader_aabb = mesh_node->first_node("shader-aabb");
		xml_node<> *node_texture = mesh_node->first_node("texture");
		xml_node<> *node_texture2 = mesh_node->first_node("texture1");
		xml_node<> *node_transform = mesh_node->first_node("transform");
		float tipo = (float)atof(node_transform->first_node("type")->value());
		float px = (float)atof(node_transform->first_node("pos-x")->value());
		float py = (float)atof(node_transform->first_node("pos-y")->value());
		float pz = (float)atof(node_transform->first_node("pos-z")->value());
		float sx = (float)atof(node_transform->first_node("scale-x")->value());
		float sy = (float)atof(node_transform->first_node("scale-y")->value());
		float sz = (float)atof(node_transform->first_node("scale-z")->value());
		//el angulo se lee rn radiantes
		float rx = (float)atof(node_transform->first_node("rotate-x")->value());
		float ry = (float)atof(node_transform->first_node("rotate-y")->value());
		float rz = (float)atof(node_transform->first_node("rotate-z")->value());

		float x, y, z;
		x = y = z = 0;
		xml_node<> *node_waypoints = mesh_node->first_node("waypoints");
		for (xml_node<> *child = node_waypoints->first_node("waypoint"); child; child = child->next_sibling("waypoint")) {
			//cargamos waypoints
			x = (float)atof(child->first_node("way-x")->value());
			y = (float)atof(child->first_node("way-y")->value());
			z = (float)atof(child->first_node("way-z")->value());
			new_mesh->waypoints.push_back(Vector3(x, y, z));

		}
		
		new_mesh->name = node_name->value();
		new_mesh->mesh = g_resourceManager.getMesh(node_mesh->value());
		new_mesh->mesh->setCollisionModel();
		new_mesh->mesh->setAABB();
		new_mesh->mesh->createAABBGeometry();
		new_mesh->shader = g_resourceManager.getShader(node_shader->value());
		new_mesh->aabb_shader = g_resourceManager.getShader(node_shader_aabb->value());
		new_mesh->texture = g_resourceManager.getTexture(node_texture->value());
		new_mesh->texture1 = g_resourceManager.getTexture(node_texture2->value());
		new_mesh->type = tipo;
		new_mesh->model.rotate(rx, Vector3(1, 0, 0));
		new_mesh->model.rotate(ry, Vector3(0, 1, 0));
		new_mesh->model.rotate(rz, Vector3(0, 0, 1));
		new_mesh->model.scale(sx, sy, sz);
		new_mesh->model.traslate(px, py, pz);
		

		parent->addChild(new_mesh);

		for (xml_node<> *child = mesh_node->first_node("node"); child; child = child->next_sibling("node"))
		{
			loadObject(child, new_mesh);
		}
	}

	void loadPlayer(xml_node<> *mesh_node, GameObject* parent) {

		//codigo para añadir objeto al scene graph
		//crear gameobject nuevo
		GameObjectPlayer* new_mesh = new GameObjectPlayer();
		new_mesh->colision = true;
		//load properties from mesh_node
		xml_node<> *node_name = mesh_node->first_node("name");
		xml_node<> *node_mesh = mesh_node->first_node("mesh");
		xml_node<> *node_shader = mesh_node->first_node("shader");
		xml_node<> *node_shader_aabb = mesh_node->first_node("shader-aabb");
		xml_node<> *node_texture = mesh_node->first_node("texture");
		xml_node<> *node_texture2 = mesh_node->first_node("texture1");
		xml_node<> *node_transform = mesh_node->first_node("transform");
		float tipo1 = (float)atof(node_transform->first_node("type")->value());
		float px = (float)atof(node_transform->first_node("pos-x")->value());
		float py = (float)atof(node_transform->first_node("pos-y")->value());
		float pz = (float)atof(node_transform->first_node("pos-z")->value());
		float sx = (float)atof(node_transform->first_node("scale-x")->value());
		float sy = (float)atof(node_transform->first_node("scale-y")->value());
		float sz = (float)atof(node_transform->first_node("scale-z")->value());
		//el angulo se lee rn radiantes
		float rx = (float)atof(node_transform->first_node("rotate-x")->value());
		float ry = (float)atof(node_transform->first_node("rotate-y")->value());
		float rz = (float)atof(node_transform->first_node("rotate-z")->value());

		new_mesh->name = node_name->value();
		new_mesh->mesh = g_resourceManager.getMesh(node_mesh->value());
		new_mesh->mesh->setCollisionModel();
		new_mesh->mesh->recta(new_mesh->model.frontVector().normalize());
		new_mesh->shader = g_resourceManager.getShader(node_shader->value());
		new_mesh->aabb_shader = g_resourceManager.getShader(node_shader_aabb->value());
		new_mesh->texture = g_resourceManager.getTexture(node_texture->value());
		new_mesh->texture1 = g_resourceManager.getTexture(node_texture2->value());
		new_mesh->type = tipo1;
		new_mesh->model.rotate(rx, Vector3(1, 0, 0));
		new_mesh->model.rotate(ry, Vector3(0, 1, 0));
		new_mesh->model.rotate(rz, Vector3(0, 0, 1));
		new_mesh->model.scale(sx, sy, sz);
		new_mesh->model.traslate(px, py, pz);

		parent->addChild(new_mesh);

		for (xml_node<> *child = mesh_node->first_node("player-node"); child; child = child->next_sibling("player-node"))
		{
			loadPlayer(child, new_mesh);
		}
	}

	void loadLevel(char* level_url) {
		rapidxml::file<> xmlFile(level_url);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		int meshes = 0;
		xml_node<> *scene = doc.first_node();
		xml_node<> *resources = scene->first_node("resources");
		xml_node<> *resource_meshes = resources->first_node("meshes");
		//buscar las meshes
		for (xml_node<> *child = resource_meshes->first_node("mesh"); child; child = child->next_sibling("mesh")){
				xml_node<> *mesh_name = child->first_node("name");
				xml_node<> *mesh_url = child->first_node("url");
				g_resourceManager.loadMesh(mesh_name->value(), mesh_url->value());
		}
		//buscar texturas
		xml_node<> *resource_tex = resources->first_node("textures");
		for (xml_node<> *child = resource_tex->first_node("texture"); child; child = child->next_sibling("texture")) {
			//cargamos  tex
			xml_node<> *tex_name = child->first_node("name");
			xml_node<> *tex_url = child->first_node("url");
			g_resourceManager.loadTexture(tex_name->value(), tex_url->value());	
		}
		xml_node<> *resource_shaders = resources->first_node("shaders");
		for (xml_node<> *child = resource_shaders->first_node("shader"); child; child = child->next_sibling("shader")) {
			//cargamos shaders
			xml_node<> *shaders = child->first_node("shader");
			xml_node<> *shaders_name = child->first_node("name");
			xml_node<> *shaders_url1 = child->first_node("url1");
			xml_node<> *shaders_url2 = child->first_node("url2");
			g_resourceManager.loadShader(shaders_name->value(), shaders_url1->value(), shaders_url2->value());
		}
		//cargamos la camara
		xml_node<> *resource_camera = resources->first_node("camera");
		float ex = (float)atof(resource_camera->first_node("eye-x")->value());
		float ey = (float)atof(resource_camera->first_node("eye-y")->value());
		float ez = (float)atof(resource_camera->first_node("eye-z")->value());

		float cx = (float)atof(resource_camera->first_node("center-x")->value());
		float cy = (float)atof(resource_camera->first_node("center-y")->value());
		float cz = (float)atof(resource_camera->first_node("center-z")->value());

		float ux = (float)atof(resource_camera->first_node("up-x")->value());
		float uy = (float)atof(resource_camera->first_node("up-y")->value());
		float uz = (float)atof(resource_camera->first_node("up-z")->value());

	
		Camera* camera = new Camera();
		camera->lookAt(Vector3(ex, ey, ez), Vector3(cx, cy, cz), Vector3(ux, uy, uz)); //position the camera and point to 0,0,0
		g_scene.camera = camera;

		//cargamos la luz
		xml_node<> *resource_light = resources->first_node("light");
		float lx = (float)atof(resource_light->first_node("x")->value());
		float ly = (float)atof(resource_light->first_node("y")->value());
		float lz = (float)atof(resource_light->first_node("z")->value());
		GameObjectLight new_light;
		new_light.posllum = Vector3(lx, ly, lz);
		g_scene.lights.push_back(&new_light);

		//bullet
		xml_node<> *resource_bullet = resources->first_node("bullet");
		xml_node<> *bullet_name = resource_bullet->first_node("name");
		xml_node<> *bullet_url = resource_bullet->first_node("url");
		xml_node<> *bullet_shader = resource_bullet->first_node("name-shader");
		xml_node<> *bullet_shader_enemy = resource_bullet->first_node("enemy-shader-bullet");
		xml_node<> *bullet_url1 = resource_bullet->first_node("url-shader");
		xml_node<> *bullet_url2 = resource_bullet->first_node("url-shader1");
		g_resourceManager.loadMesh(bullet_name->value(), bullet_url->value());
		g_resourceManager.getMesh(bullet_name->value())->setCollisionModel();
		g_resourceManager.loadShader(bullet_shader->value(), bullet_url1->value(), bullet_url2->value());
		Shader* pepe = g_resourceManager.getShader(bullet_shader->value());
		Shader* pepe2 = g_resourceManager.getShader(bullet_shader_enemy->value());
		bulletManager* balas = new bulletManager(g_resourceManager.getMesh(bullet_name->value()), 500, pepe, pepe2);
		bulletManager* balas2 = new bulletManager(g_resourceManager.getMesh(bullet_name->value()), 100, pepe, pepe2);
		Game* g = Game::instance;
		g->bullet_manager = balas;
		g->bullet_manager1 = balas2;

		//escudo
		xml_node<> *resource_escudo = resources->first_node("escudo");
		
		xml_node<> *escudo_name = resource_escudo->first_node("name");
		xml_node<> *escudo_mesh = resource_escudo->first_node("mesh");
		xml_node<> *escudo_shader = resource_escudo->first_node("name-shader");
		xml_node<> *escudo_transform = resource_escudo->first_node("transform");
		float tipo4 = (float)atof(escudo_transform->first_node("type")->value());
		xml_node<> *escudo_transform_scale = escudo_transform->first_node("scale");
		float xescudo = (float)atof(escudo_transform_scale->first_node("x")->value());
		float yescudo = (float)atof(escudo_transform_scale->first_node("y")->value());
		float zescudo = (float)atof(escudo_transform_scale->first_node("z")->value());
		escudo* shield1 = new escudo(g_resourceManager.getMesh(escudo_mesh->value()), g_resourceManager.getShader(escudo_shader->value()));
		g_resourceManager.getMesh(escudo_mesh->value())->setCollisionModel();
		shield1->escalar(xescudo, yescudo, zescudo);
		shield1->mesh = g_resourceManager.getMesh(escudo_mesh->value());
		shield1->type = tipo4;
		shield1->shader = g_resourceManager.getShader(escudo_shader->value());
		g->shield = shield1;
		//cargamos environment
		xml_node<> *root_c = scene->first_node("root");
		xml_node<> *root_environment = root_c->first_node("environment");
		xml_node<> *environment_name = root_environment->first_node("name");
		xml_node<> *environment_mesh = root_environment->first_node("mesh");
		xml_node<> *environment_shader = root_environment->first_node("shader");
		xml_node<> *environment_texture = root_environment->first_node("texture");
		xml_node<> *environment_transform = root_environment->first_node("transform");
		xml_node<> *environment_transform_scale = environment_transform->first_node("scale");
		float x = (float)atof(environment_transform_scale->first_node("x")->value());
		float y = (float)atof(environment_transform_scale->first_node("y")->value());
		float z = (float)atof(environment_transform_scale->first_node("z")->value());
		GameObjectEnvironment* new_environment = new GameObjectEnvironment();
		new_environment->colision = false;
		new_environment->name = environment_name->value();
		new_environment->mesh = g_resourceManager.getMesh(environment_mesh->value());
		new_environment->shader = g_resourceManager.getShader(environment_shader->value());
		new_environment->texture = g_resourceManager.getTexture(environment_texture->value());
		new_environment->escalar(x, y, z);
		g_scene.root->addChild(new_environment);
		//cargamos los modelos
		for (xml_node<> *child = root_c->first_node("player-node"); child; child = child->next_sibling("player-node")) {
			
			loadPlayer(child, g_scene.root);
		}
		
		for (xml_node<> *child = root_c->first_node("node"); child;	child = child->next_sibling("node")){
			loadObject(child, g_scene.root);
		}

		vectorcolisiones(g->bullet_manager->objetos, g_scene.root);
		g->bullet_manager->objetos.push_back(g->shield);

		g->bullet_manager1->objetos = g->bullet_manager->objetos;
	}
}