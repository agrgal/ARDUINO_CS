<?php
header('Content-Type: text/html; charset=UTF-8'); // importante; especifica el charset de caracteres.

// importante incluir al principio de cada una, lo de las funciones
include_once("../configuracion/config.php"); // funciones de configuración
include_once("../funciones/funciones.php"); // funciones varias de conexión a base de datos, etc.

session_start(); //activo variables de sesion 


if ($_POST["lee"]==1) {
$Sql ="TRUNCATE TABLE tb_datos"; // define la variable $Sql.

// Conexión y obtención de datos
		$link=Conectarse(); // y me conecto. //dependiendo del tipo recupero uno u otro.
	    $result=mysqli_query($link,$Sql); // ejecuta la cadena sql y almacena el resultado el $result
	    mysqli_free_result($result); 
	    mysqli_close($link); 
		echo "Tabla datos borrada"; 
} else {
	   	echo "Fallido"; 
}
?>



