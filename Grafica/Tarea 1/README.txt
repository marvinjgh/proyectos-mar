Para que se pueda compilar el c�digo, se tienen las cabeceras del freeglut en la carpeta "src/GL/", el freeglut.lib 
en "lib/" para ser colocadas donde mejor prefiera, y el freeglut.dll est� en "bin/".

Para cambiar en las distintas figuras se ha provisto de un men�, que se despliega al presionar
bot�n derecho del mouse.

Tanto l�nea, rect�ngulo, c�rculo se debe dar al bot�n izquierdo y arrastrar el mouse para crear la figura,
una vez se tiene la figura se suelta el bot�n.

EL circulo : el clip inicial fija el centro, y a medida que arrastre el mouse se crea el circulo hasta
que se suelte el bot�n

Elipse: la forma de crear la elipse es similar al de crear un rect�ngulo

El Tri�ngulo: primero crea una l�nea tal como fue explicado previamente, luego es necesario un ciclo completo
del clip(presionar y soltar) donde mejor prefiera para concluir as� el dibujo del tri�ngulo.

En el men� del rat�n se puede escoger el tipo de l�nea, como tambi�n limpiar o salir

Al presionar la tecla 'c' se limpiara la pantalla, y al presionar la 'q' se sale del programa
