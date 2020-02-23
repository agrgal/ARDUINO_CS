# estacionMeteorologicaArduino
Construcción y desarrollo de una aplicación capaz de leer datos de sensores con ARDUINO y ser trasvasados a un servidor con BD MySQL.

Más información siguiendo el enlace: https://docs.google.com/presentation/d/17WwPpb2K0KYF3eI5fGD6PAXZ98kXaJr1MkpaM0k73vk/edit?usp=sharing

Breve reseña de ficheros y carpetas
===================================

A) Sensor_temperatura_conexi_n_ethernet_JSON.ino , programa en el IDE Arduino capaz de leer datos del sensor DS18B20 de Digital Dallas, usando bibliotecas y configuración OneWire. Manda datos a través de una tarjeta de red Ethernet en una configuración de servidor y con formato JSON.

B) bdestacionmeteorologica.sql: copia ejempl ode la base de datos usada. Tabla: tb_datos. Preparada para recibir parámetros de tiempo, Temperatura, Presión y Humedad.

  B1) Ficheros de las conexiones en ARDUINO, para fritzing y PNG.

C) indexCliente.html, carpeta css y carpeta imágenes: necesario para abrir un lector simple de los datos desde otra estación dentro de la misma red local (simplemente desde Mozilla-Firefox abrir fichero). Si está activada la opción de grabar, y la función AJAX correspondiente apunta a un script válido, se registrarán datos en la base de datos. 

D) index.php, resultados.php y todas las carpetas: si las copio dentro de la ruta de un servidor web con MySQL (instalada la base de datos anterior) conectado a la misma red local, seré capaz de obtener datos y obtener resultados de los datos grabados en MySQL. IMPORTANTE: modificar el fichero configuracion/config.php con los parámetros de configuración de tu servidor.

Lector de datos de Estación Meteorológica Arduino by Aurelio Gallardo Rodríguez is licensed under a Creative Commons Reconocimiento-NoComercial-CompartirIgual 4.0 Internacional License

