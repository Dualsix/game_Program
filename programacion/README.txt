Carlos Muntané Fuentes
Noa Durán Plass

Emails:
Munta92@gmail.com
Noaduran@hotmail.com

Instruccions:

Con el ratón dirigues la nave, la nave se mueve hacia adelante por si misma. Con el Mayus la nave accelera, con el boton izquierdo del raton la nave dispara, 
si pulsas la W mientras disparas dispara en rafagas de 3 disparos, con la Q y la E la nave hace un roll, con el espacio creas una barrera.

Si en el menu, escribes "secret" puedes jugar el nivel secreto, en ese nivel te enfrentas a un jefe que te quita 20 puntos de vida de tus 100 puntos, en el nivel 
normal te quitan 10. Si pulsas Esc te sale el menu de pause. 

En ambos niveles debes acabar con tus enemigos.


Cosas que hemos implementdo a tener en cuenta:

-2 niveles, uno secreto, que se activa con contraseña
-2 tipos de disparos el normal y rafaga
-Escudo protector
-Una linea de ayuda para disprar
-El AABB de los enemigos visibles
-Carga desde XML
-Enemigos que se mueven por waypoints
-Boss la linea de disparo sigue al protagonista
-3 tipos de enemigos, con diferente vida y poder
-Colisiones de las balas, con libreria Coldet
-Menus
-HUD, vida del jugador y del boss secreto
-Trasparencia en el escudo
-Bullet manager implementado
-Diferentes Shaders
-Bullet manager optimizado creando una mesh para todas las balas
-Colisiones optimizadas usando booleanos en vez de ir creando un vector de colisionables cada frame
-Intento de carga de multicarga en el mesh.cpp, carga bn pero se ve la pantalla negra
-He hecho zonas de colisiones asi solo miro la colision si estan en la misma zona
-El movimiento del protagonista esta bloqueado, solo puede moverse en una zona
-Implementada la musica y los sonidos
-Modo debbug para el programador pulsando la "p"

URL del gampley en youtube: https://youtu.be/MbsiSj5RGVY