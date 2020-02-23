<?php 
/* ****************************************************************
Incluyo una función con los datos de conexión
****************************************************************** */
include_once("./configuracion/config.php");

/* ****************************************************************
Esta función conecta a una base de datos en concreto
****************************************************************** */
function Conectarse()
{ // para conectarse a una base de datos, que ya se define en config.php
global $mysql_server, $mysql_login, $mysql_pass, $bd; // defino variables como globales
/* ANTIGUO mysql ********************************************************
if
// (!($link=mysql_connect("","pepe","pepa")))
(!($link=mysql_connect($mysql_server,$mysql_login,$mysql_pass)))
{
echo "<p>Error conectando a la base de datos. Datos incorrectos de servidor, login o contraseña</p>";
exit();
}
if (!mysql_select_db($bd,$link)) //base de datos:conexión
{
echo "<p>Error cuando selecciono la base de datos. No existe esa base de datos.</p>";
exit();
}
return $link;
********************************************************************** */

$con=mysqli_connect($mysql_server, $mysql_login, $mysql_pass, $bd);
// Check connection
if (mysqli_connect_errno())
  {
  echo mysqli_connect_error();
  } else {
  return $con; 
  }
}

/* ****************************************************************
Esta función recupera el valor de un campo en concreto...
****************************************************************** */
function dado_Id($Id,$tipo,$tabla,$nombreid) 
	{ // Recupera el valor de la base de datos	
	$link=Conectarse(); // y me conecto. //dependiendo del tipo recupero uno u otro.
	// $Sql="SELECT ".$tipo." from ".$tabla." WHERE ".$nombreid."='".$Id."'";
    $Sql="SELECT ".$tipo." from ".$tabla." WHERE ".$nombreid."='%s'"; 
    $Sql = sprintf($Sql, mysqli_real_escape_string($link, $Id)); // Seguridad que evita los ataques SQL Injection
	// echo $Sql;
    $result=mysqli_query($link,$Sql); // ejecuta la cadena sql y almacena el resultado el $result
	$row=mysqli_fetch_array($result);
	if (!is_null($row[$tipo]) or !empty($row[$tipo])) { // si no lo recupera, el valor por defecto)
					  return $row[$tipo];
					  } else {
				      return "Error";
					  }
	mysqli_free_result($result);
	mysqli_close($link);
	}
	


/* ********************************************************************************************
Comprueba si una cadena es o no vacía
***********************************************************************************************/
function IsNullOrEmptyString($question){
    return (!isset($question) || trim($question)==='');
}

/* ********************************************************************************************
Cambiar fecha MySQL a formato elegido datepicker
* 
***********************************************************************************************/
function fechaMySQL2DatePicker($fec) {
    return date("d-m-Y", strtotime($fec));
}

/* ********************************************************************************************
Cambiar fecha MySQL a formato largo
***********************************************************************************************/
function fechaMySQL2Larga($fec) {
	// Mejor así, por si el servidor no tiene SETLOCALE activado...	
	$dias = array("Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sábado");
	$meses = array("Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre");
	return $dias[date('w',strtotime($fec))].", ".date('d',strtotime($fec))." de ".$meses[date('n',strtotime($fec))-1]. " de ".date('Y',strtotime($fec)) ;
    // return date("l, d \d\e M \d\e Y", strtotime($fec));
}

/* ********************************************************************************************
Cambiar fecha MySQL a formato todo junto
***********************************************************************************************/
function fechaMySQL2together($fec) {
    return date("Ymd", strtotime($fec));
}

/* ********************************************************************************************
Dado un array con valores, calculo la media
***********************************************************************************************/
function mediaArray($a) {
	if (count($a)>0){
		return array_sum($a)/count($a);
	} else {
		return NULL;
	}
}

/* ********************************************************************************************
Dado un array con valores, calculo la varianza
***********************************************************************************************/
function varianzaArray($a) {
  $varianza = 0;
  $media = mediaArray($a);
  if (!(is_null($media))){
	  forEach ($a as $clave=>$valor) {
		  $varianza=$varianza+pow($valor-$media,2);
	  }
	  $varianza = $varianza/count($a);
	  return $varianza;
  } else {
	  return NULL;
  }
}

/* ********************************************************************************************
Dado un array con valores, calculo la desviación típica
***********************************************************************************************/
function desviaciontipicaArray($a) {
	if(!(is_null(varianzaArray($a)))){
		return sqrt(varianzaArray($a));
	} else {
		return NULL;
	}
}

/* ********************************************************************************************
quitar tildes
***********************************************************************************************/
function normaliza($cadena){
    $originales = 'ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõöøùúûýýþÿŔŕ';
    $modificadas = 'AAAAAAACEEEEIIIIDNOOOOOOUUUUYbbaaaaaaaceeeeiiiidnoooooouuuyybyRr';
    $cadena = utf8_decode($cadena);
    $cadena = strtr($cadena, utf8_decode($originales), $modificadas);
    // $cadena = strtolower($cadena);
    return utf8_encode($cadena);
}

/* ********************************************************************************************
Valida el formato de fecha
********************************************************************************************** */
function validateDate($date, $format = 'Y-m-d')
{
    $d = DateTime::createFromFormat($format, $date);
    return $d && $d->format($format) == $date;
}

?>
