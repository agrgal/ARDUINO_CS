<?php
header('Content-Type: text/html; charset=UTF-8'); // importante; especifica el charset de caracteres.

// importante incluir al principio de cada una, lo de las funciones
include_once("../configuracion/config.php"); // funciones de configuración
include_once("../funciones/funciones.php"); // funciones varias de conexión a base de datos, etc.

session_start(); //activo variables de sesion 

$Sql =""; // define la variable $Sql.

if (validateDate($_POST["fechaDesde"]) and validateDate($_POST["fechaHasta"])) {
	// echo "Fecha recibida";	
	if (is_null($_POST["horaDesde"]) or empty($_POST["horaDesde"])) { $horaDesde="00:00:00"; } else { $horaDesde=$_POST["horaDesde"]; }
	if (is_null($_POST["horaHasta"]) or empty($_POST["horaHasta"])) { $horaHasta="23:59:59"; } else { $horaHasta=$_POST["horaHasta"]; }
	$fechaINI=$_POST["fechaDesde"]." ".$horaDesde;
	$fechaFIN=$_POST["fechaHasta"]." ".$horaHasta;
	if ($fechaINI>$fechaFIN) {
		$ordenado = $fechaINI; $fechaINI = $fechaFIN; $fechaFIN = $ordenado;
	}	
	$Sql = "SELECT * FROM `tb_datos` WHERE `FechaHora` BETWEEN '%s' AND '%s'";
	$Sql = sprintf($Sql, $fechaINI, $fechaFIN);		

} else if (validateDate($_POST["fechaDesde"]) and !(validateDate($_POST["fechaHasta"])) ) { // No hay fecha HASTA
	if (is_null($_POST["horaDesde"]) or empty($_POST["horaDesde"])) { $horaDesde="00:00:00"; } else { $horaDesde=$_POST["horaDesde"]; }
	$fechaINI=$_POST["fechaDesde"]." ".$horaDesde;
	$Sql = "SELECT * FROM `tb_datos` WHERE `FechaHora` >= '%s'";
	$Sql = sprintf($Sql, $fechaINI);		
} else if (validateDate($_POST["fechaHasta"]) and !(validateDate($_POST["fechaDesde"])) ) { // No hay fecha DESDE
	if (is_null($_POST["horaHasta"]) or empty($_POST["horaHasta"])) { $horaHasta="23:59:59"; } else { $horaHasta=$_POST["horaHasta"]; }
	$fechaFIN=$_POST["fechaHasta"]." ".$horaHasta;
	$Sql = "SELECT * FROM `tb_datos` WHERE `FechaHora` <= '%s'";
	$Sql = sprintf($Sql, $fechaFIN);
} else { // Si no, no hace filtro...
	$Sql = "SELECT * FROM `tb_datos`";
}


$Sql.=" ORDER BY FechaHora";

if ($_POST["limite"]>0) { // Si hay un número mayor que cero 
	$Sql.= " LIMIT ".$_POST["limite"];
} else {  // Siempre hay un límite de 10000
	$Sql.= " LIMIT 10000";
}


// Conexión y obtención de datos
		$link=Conectarse(); // y me conecto. //dependiendo del tipo recupero uno u otro.
	    $result=mysqli_query($link,$Sql); // ejecuta la cadena sql y almacena el resultado el $result
	    $datos=array();
	    $minimo=0;
        $ii=0;
	    while ($row=mysqli_fetch_array($result)) {			
                $datos[$ii]["temperatura"]=$row["Temperatura"];
                $datos[$ii]["presion"]=$row["Presion"];
                $datos[$ii]["humedad"]=$row["Humedad"];
                $datos[$ii]["fecha"]=substr($row["FechaHora"],0,10);
                $datos[$ii]["hora"]=substr($row["FechaHora"],11,8);
                $datos[$ii]["tiempoarduino"]=$row["tiempoArduino"];
                if ($ii==0) { $minimo=$row["tiempoArduino"]; } // mínimo valor registrado por Arduino de su cuenta de tiempo
				$ii++;
		        } // fin del while	
		$datos[0]["minimo"]=$minimo;
		$datos_json=json_encode($datos); // codifica como json...
		mysqli_free_result($result); 
	    mysqli_close($link); 
		echo $datos_json;   
 
?>



