<?php
//------------------------------------------------ 
class micalendario {
//------------------------------------------------
/**
 * Autor: Aurelio Gallardo
 * Date : 20/11/2005
 *  
 * Description: Clase de PHP que permite trabajar con fechas...
 * 
 */
 
 	// Mis variables
	var $dia;
	var $anno;
	var $mes;
	var $mesarray;
	var $campodefecha;
	var $diaactual;
	var $mesactual;
	var $annoactual;

	
	// para utilizar esta función debo llamarla DENTRO DE UN FORM.
	// recupera la fecha en un formato admisible para MySQL
	// este formato es año guión mes guión día. YYYY - MM - DD
	// $tipo-> tipo del campo creado, si texto u oculto...
	// $fechaini --> fecha que se pone de inicio...
	// $bloq = 1 --> no permite cambios en el campo...
	// $bloq = 0 --> si permite cambios en el campo...
	// empezar en: año en que empieza la lista
	// num años: número de años a rellenar en la lista...
	function obtenfecha ($nombredecampo,$tipo,$fechaini,$bloq,$empezaren,$numannos) {

		$this->annoactual = adodb_date("Y"); // obtiene el año actual
		$this->diaactual = adodb_date("d"); // obtiene el dia actual
		$this->mesactual = adodb_date("m"); // obtiene el mes actual
		
		if ($bloq==1) {$this->bloqueado="disabled";} else {$this->bloqueado="";}

		if ($fechaini=="") {$fechaini=$this->fechadehoy();}
		
		$this->mes=substr($fechaini,5,2)+0;
		$this->dia=substr($fechaini,8,2)+0;
		$this->anno=substr($fechaini,0,4)+0;
				
		 // Nombre de los meses
 		 setlocale(LC_TIME,"SP");
		 for ($i=1;$i<=12;$i++) {
		 	$this->mesarray[$i-1]=ucfirst(strftime("%B",adodb_mktime(0,0,0,$i,1,$this->annoactual)));
 		 }
		 
 	    // aquí empieza lo "gordo".
		// calcula la cadena que hará que cambie el campo cuyo nombre paso.
		$onchange=$nombredecampo.".value=".$nombredecampo."anno.value + '-' + ".$nombredecampo."mes.value + '-' + ".$nombredecampo."dia.value; refresh";
		// valor del campo
		echo '<input name="'.$nombredecampo.'" value="'.$fechaini.'" type="'.$tipo.'" size="20" maxlength="20">';
		
		// dia
		echo '<select class="botones" '.$this->bloqueado.' name="'.$nombredecampo.'dia" id="'.$nombredecampo.'dia" 
		     OnChange="'.$onchange.'" OnFocus="this.selectedIndex='.$this->diaactual.';">';
			 for ($i=1;$i<=31;$i++) {
			 	if ($i==$this->dia) {$poner="selected";} else {$poner="";}
			 	echo '<option value="'.sprintf("%02d",$i).'" '.$poner.'>'.$i.'</option>';
			 }
		echo '</select>&nbsp;';
		
		// mes
		echo '<select class="botones" '.$this->bloqueado.' name="'.$nombredecampo.'mes"  id="'.$nombredecampo.'mes" OnChange="'.$onchange.'" OnFocus="this.selectedIndex='.(-1+$this->mesactual).';">';
  			 for ($i=0;$i<=11;$i++) {
			 	if ($i+1==$this->mes) {$poner="selected";} else {$poner="";}
			 	echo '<option value="'.sprintf("%02d",$i+1).'" '.$poner.'>'.$this->mesarray[$i].'</option>';
			 // echo '<option value="'.$i.'">'.$i.'</option>';
			 }
		echo '</select>&nbsp;';
		
		//año
		echo '<select class="botones" '.$this->bloqueado.' name="'.$nombredecampo.'anno" id="'.$nombredecampo.'anno" OnChange="'.$onchange.'"  OnFocus="this.selectedIndex='.$this->annoactual.';">';
  			 // empieza en el 2006 y acaba en el 31-enero-2036
			 // problemas en windows con mktime a partir del 18 de enero de 2038
			 // el día 19 ya da errores de valores negativos de la función en windows.
			 // SOLUCIóN: usar la biblioteca adodb-time.inc.php
			 for ($i=$empezaren;$i<=$numannos+$empezaren;$i++) {
			 	if ($i==$this->anno) {$poner="selected";} else {$poner="";}
			 	echo '<option value="'.$i.'" '.$poner.'>'.$i.'</option>';
			 }
		echo '</select>';
	}
	
	function fechavalida ($fecha) { 
		//dan una fecha en formato de MySQL anno - mes -dia y comprueba que está bien.
		$this->mes=substr($fecha,5,2);
		$this->dia=substr($fecha,8,2);
		$this->anno=substr($fecha,0,4);
		// echo $this->dia." ".$this->mes." ".$this->anno;
		$valor=0;
		if (checkdate($this->mes,$this->dia,$this->anno)) { $valor=1; } 
		return $valor; // uno si está bien.
	}
	
	function fechaformateada ($fecha) { 
		//dan una fecha en formato de MySQL anno - mes -dia y se entrega con un formato dado...
		$this->mes=substr($fecha,5,2);
		$this->dia=substr($fecha,8,2);
		$this->anno=substr($fecha,0,4);
		return $this->dia."/".$this->mes."/".$this->anno;
	}

        function fechaformateadalarga ($fecha) { 
		//dan una fecha en formato de MySQL anno - mes -dia y se entrega con un formato dado...
		$this->mes=substr($fecha,5,2);
		$this->dia=substr($fecha,8,2);
		$this->anno=substr($fecha,0,4);
        $diasdelasemana = array("Domingo","Lunes","Martes","Miércoles","Jueves","Viernes","Sábado");
        $meses = array("Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre");
 	            $ds = gmdate("w",mktime(12,0,0,$this->mes,$this->dia,$this->anno));
                $dd = gmdate("j",mktime(12,0,0,$this->mes,$this->dia,$this->anno));
                $mm = gmdate("n",mktime(12,0,0,$this->mes,$this->dia,$this->anno));
		// return $this->dia."/".$this->mes."/".$this->anno." ==> ".$dias[$diasemana].", ".$dd." de ".$meses[$mm-1]." de ".$this->anno;
        // return iconv("ISO-8859-1","UTF-8",$diasdelasemana[$ds]).", ".$dd." de ".$meses[$mm-1]." de ".$this->anno;
		return $diasdelasemana[$ds].", ".$dd." de ".$meses[$mm-1]." de ".$this->anno;
        // return $diasemana;
	}
	
	function comprueba($fechauna,$fechados) {// comprueba dos fechas en formato MySQL
		$this->mes=substr($fechauna,5,2);
		$this->dia=substr($fechauna,8,2);
		$this->anno=substr($fechauna,0,4);
		$una = adodb_mktime(0,0,0,$this->mes,$this->dia,$this->anno);
		$this->mes=substr($fechados,5,2);
		$this->dia=substr($fechados,8,2);
		$this->anno=substr($fechados,0,4);
		$dos = adodb_mktime(0,0,0,$this->mes,$this->dia,$this->anno);
		if ($una==$dos) {$valor=0;} else {$valor=($una-$dos)/abs($una-$dos);}
		// echo ($una-$dos)/abs($una-$dos);
		// retorna 0, si una y dos son iguales. Si una es mayor, un 1 y si es menor un -1
		return $valor;
	}
	
	function fechadehoy() {// obtiene la fecha de hoy en formato MySQL
		$this->annoactual = date("Y"); // obtiene el año actual
		$this->diaactual = date("d"); // obtiene el dia actual
		$this->mesactual = date("m"); // obtiene el mes actual
		return $this->annoactual."-".$this->mesactual."-".$this->diaactual;
	}
	
	function horactual()  {// obtiene la hora actual
		$this->hora = date("H"); // obtiene la hora actual
		$this->minuto = date("i"); // obtiene el minuto actual
		return $this->hora.":".$this->minuto;
	}
	
  	
	// para utilizar esta función debo llamarla DENTRO DE UN FORM.
	// recupera la fecha en un formato admisible para MySQL
	// este formato es año guión mes guión día. YYYY - MM - DD
	function obtenhora($nombredecampo,$tipo,$horaini,$bloq) {

		$this->horactual = adodb_date("G"); // obtiene la hora actual
		$this->minactual = adodb_date("i"); // obtiene el minuto actual
		
		if ($bloq==1) {$this->bloqueado="disabled";} else {$this->bloqueado="";}

		if ($horaini=="") {$horaini=$this->horactual.":".$this->minactual;}
		
		$this->hora=substr($horaini,0,2);
		$this->minuto=substr($horaini,3,2);
 
 	    // aquí empieza lo gordo.
		// calcula la cadena que hará que cambie el campo cuyo nombre paso.
		$onchange=$nombredecampo.".value=".$nombredecampo."hora.value + ':' + ".$nombredecampo."minuto.value; refresh";
		// valor del campo
		echo '<input name="'.$nombredecampo.'" value="'.$horaini.'" type="'.$tipo.'" size="20" maxlength="20">';
		
		// hora
		echo '<select class="botones" '.$this->bloqueado.' name="'.$nombredecampo.'hora" id="'.$nombredecampo.'hora" 
		     OnChange="'.$onchange.'" OnFocus="this.selectedIndex='.$this->horactual.';">';
			 for ($i=0;$i<=23;$i++) {
			 	if ($i==$this->hora) {$poner="selected";} else {$poner="";}
			 	echo '<option value="'.sprintf("%02d",$i).'" '.$poner.'>'.sprintf("%02d",$i).'</option>';
			 }
		echo '</select>&nbsp;:';
		
		// minuto
		echo '<select class="botones" '.$this->bloqueado.' name="'.$nombredecampo.'minuto"  id="'.$nombredecampo.'minuto" 
			 OnChange="'.$onchange.'" OnFocus="this.selectedIndex='.($this->minactual).';">';
  			 for ($i=0;$i<=59;$i++) {
			 	if ($i==$this->minuto) {$poner="selected";} else {$poner="";}
			 	echo '<option value="'.sprintf("%02d",$i).'" '.$poner.'>'.sprintf("%02d",$i).'</option>';
			 // echo '<option value="'.$i.'">'.$i.'</option>';
			 }
		echo '</select>&nbsp;';
	}
}

?>
