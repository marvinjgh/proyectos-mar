Para que se pueda compilar el código, se tienen las cabeceras del freeglut en la carpeta "src/GL/", el freeglut.lib 
en "lib/" para ser colocadas donde mejor prefiera, y el freeglut.dll está en "bin/".

Para cambiar en las distintas figuras se ha provisto de un menú, que se despliega al presionar
botón derecho del mouse.

Tanto línea, rectángulo, círculo se debe dar al botón izquierdo y arrastrar el mouse para crear la figura,
una vez se tiene la figura se suelta el botón.

EL circulo : el clip inicial fija el centro, y a medida que arrastre el mouse se crea el circulo hasta
que se suelte el botón

Elipse: la forma de crear la elipse es similar al de crear un rectángulo

El Triángulo: primero crea una línea tal como fue explicado previamente, luego es necesario un ciclo completo
del clip(presionar y soltar) donde mejor prefiera para concluir así el dibujo del triángulo.

En el menú del ratón se puede escoger el tipo de línea, como también limpiar o salir

Al presionar la tecla 'c' se limpiara la pantalla, y al presionar la 'q' se sale del programa
