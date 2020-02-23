<?php
header('Content-Type: text/html; charset=UTF-8'); // importante; especifica el charset de caracteres.

// importante incluir al principio de cada una, lo de las funciones
include_once("../configuracion/config.php"); // funciones de configuración
include_once("../funciones/funciones.php"); // funciones varias de conexión a base de datos, etc.

session_start(); //activo variables de sesion 

// $fechaDeHoy = "la fecha actual es " . date("d") . " del " . date("m") . " de " . date("Y");
$fechaDeHoy = date(DATE_W3C);
$fecha = substr($fechaDeHoy,0,10); // La fecha del sistema
$hora = substr($fechaDeHoy,11,8); // la hora del sistema


if ($_POST['tiempoArduino']) {
	$link = Conectarse(); // y me conecto. //dependiendo del tipo recupero uno u otro.
	$Sql = "INSERT INTO tb_datos (FechaHora, tiempoArduino, Temperatura, Presion, Humedad)";
	$Sql.= ' VALUES ("'.$fecha." ".$hora.'",'.$_POST['tiempoArduino'].','.$_POST['temperatura'].','.$_POST['presion'].','.$_POST['humedad'].')';		
	$result = mysqli_query($link,$Sql);
	mysqli_free_result($result); 
	mysqli_close($link); 
	echo $Sql;
} else {
	echo "Dato no grabado";
}	  
?>



