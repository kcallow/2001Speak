Instituto Tecnológico de Costa Rica  
Ingeniería en Computación  
Programación Orientada a Objetos  
I Semestre 2015  
Mauricio Avilés  
Estudiantes:  
Kenneth Callow   
Sara Castro Sáenz  
#Proyecto #3 - 2001Speak 

#Contenidos

1. Antes de iniciar
2. Dependencias
3. Iniciar el programa
4. Uso del programa

##1. Antes de iniciar

Es recomendable el uso de un micrófono de buena calidad.
La acústica del espacio en el que se encuentre afecta el funcionamiento del programa.

##2. Dependencias

2001Speak depende de xte para simular las entradas de los dispositivos, y de pocketsphinx (version pre alpha 5) para recibir las entradas del microfono.

* xte:
	En Ubuntu se puede instalar fácilmente mediante:

		$ sudo apt-get install xautomation

* pocketsphinx:

	* Pocketsphinx depende de sphinxbase.  Para compilar e instalar ambos, se debe primero instalar estas dependencias:

			$ sudo apt-get install gcc automake autoconf libtool bison swig python-dev libpulse-dev 

	* Proceda a descargar el codigo fuente de sphinxbase y pocketphinx:

		- sphinxbase-5prealpha.tar.gz:

			http://sourceforge.net/projects/cmusphinx/files/sphinxbase/5prealpha/

		- pocketsphinx-5prealpha.tar.gz:

			http://sourceforge.net/projects/cmusphinx/files/pocketsphinx/5prealpha/

	* En la carpeta donde los descargo, puede descomprimirlos mediante:

			$ tar -xf sphinxbase-5prealpha.tar.gz 
			$ tar -xf pocketsphinx-5prealpha.tar.gz

	* Ahora entre a la carpeta de sphinxbase:

			$ cd sphinxbase-5prealpha/

	* Configure, compile e instale:

			$ ./configure 
			$ make
			$ sudo make install

	* Para asegurarse que su sistema carga librerias de /usr/local/:

			$ export LD_LIBRARY_PATH=/usr/local/lib
			$ export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

	* Regresese y vaya a la carpeta de pocketsphinx:

			$ cd ../pocketsphinx-5prealpha/

	* Configure, compile e instale:

			$ ./configure 
			$ make
			$ sudo make install

	* Compruebe que pocketsphinx funciona para interpretar frases en inglés mediante:

			$ pocketsphinx_continuous -inmic yes

Fuente: http://cmusphinx.sourceforge.net/wiki/tutorialpocketsphinx

##3. Iniciar el programa

Para abrir el programa, dirijase a la carpeta bin/ dentro de la carpeta del programa y haga doble-click en el ejecutable 2001SpeakGUI.  Esta es la version con interfaz grafica.  Para correr en modo texto, corra ./2001Console desde una terminal en la carpeta bin/.

##4. Uso del programa

El programa funciona por medio de comandos de voz.         
La sintaxis básica de estos es:  
				[number] action1 ... actionN.   
Donde number se refiere a la cantidad de veces que desea repetir una acción

##4.1 Modos
En el programa hay dos modos: mouse y key. El modo mouse consiste en una serie de acciones para controlar el mouse y el modo key se utiliza para controlar el teclado.
Para activar el modo mouse solo diga en voz alta ¨Mode Mouse¨ y para activar el modo key ¨Mode Key¨.

##4.2 Acciones

* Modo Mouse:
	- Botones:
		- Left Button - Selecciona el botón izquierdo del mouse
		- Middle Button - Selecciona el botón central del mouse
		- Right Button - Selecciona el botón derecho del mouse
		
	- Acciones:
		- Click - Presiona el botón que tenga seleccionado
		- Hold - Sostiene el botón presionado
		- Release - Libera el botón
		- Up - Mueve el cursor hacia arriba
		- Down - Mueve el cursor hacia abajo
		- Left - Mueve el cursor a la izquiera
		- Right - Mueve el cursor a la derecha

* Modo Key:
	- Las acciones del modo key consisten en todas las teclas disponibles en el teclado en ingles de Estados Unidos (esto excluye la ñ).
	- Para activar estas teclas solo diga en voz alta el nombre de la tecla que desee activar.
	- Las teclas modificadoras (Ctrl, Shift, Alt) las debe decir antes de cualquier tecla no modificadora, estas teclas se mantienen accionadas hasta que una no modificadora se mencione.

##4.3 Macro
 
* Acciones:
	Para activar las acciones de macro solo diga ¨Macro¨ + acción

	- Begin - Empieza a grabar el macro
	- End - Termina de grabar el macro
	- List - despliega una lista de macros y los contenidos del macro actual
	- Next - Selecciona el siguiente macro
	- Previous - Selecciona el anterior macro
	- Delete - Elimina el macro
	- Play - Reproduce el macro
	


