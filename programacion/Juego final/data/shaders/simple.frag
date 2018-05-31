#version 330

out vec4 fragColor;

uniform vec3 u_light;

uniform sampler2D u_texture_diffuse;

uniform sampler2D u_texture_specular;



in vec3 v_normal;

in vec3 v_camera;

in vec2 v_uv;



void main(void)
{
	vec3 N = normalize(v_normal); //normalizamos
	vec3 texture_color = texture2D(u_texture_specular, v_uv).xyz;
	vec3 L = normalize(u_light);
	//vec3 newL = vec3(1.0,1.0,1.0);
	//vec3 L = normalize(newL);

	vec3 E = normalize(v_camera);

	vec3 final_color = vec3(0.0, 0.0, 0.0); //color que sale por pantalla creo

	vec3 ambient = texture_color*0.1; //ponemos una luz ambiente

	float NdotL = dot(N, L); //producto escalar, calcula el porcentaje de color que se refleja

	if(NdotL > 0.0){	//asegurarnos que no hay luz negativa
		//vec3 diffuse = vec3(1.0, 0.0, 0.0) * NdotL; //imaginamos un objeto rojo 
		vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz * NdotL;

		vec3 R = reflect(L, N);
		float RdotE = dot(R, E);
		float spec = pow(RdotE, 40.0);
		
		vec3 specular = texture_color * spec;
		
		final_color = ambient + diffuse + specular; //el color k sale es la suma con la ambiente y la especular
		
		
	}else{
		final_color = ambient;	//si la luz es negativa solo sabe la ambiental
	}



	// simple red colour
	fragColor = vec4(final_color, 1.0);
}
