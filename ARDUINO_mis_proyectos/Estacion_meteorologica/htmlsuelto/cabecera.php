<?php header('Content-type: text/html; charset=UTF-8'); ?>
	<div id="header" title="sitename">
		<div id="logo">
			<img src="./imagenes/logoA.png" title="logo 'programa de Aurelio', realizado por Miguel Parra" alt="logo 'programa de Aurelio', realizado por Miguel Parra" width="80%" height="auto">
		</div>
		<div id="iesseritium">
			<a href="./index.php">
			<img src="./imagenes/weather.png" title="by Freepik in weather" alt="by Freepik in weather" width="100%" height="auto">
			</a>
		</div>
		<div id="calendario">
			<h2>
			<?php
		    // echo 'Fecha: '.$calendario->fechaformateadalarga("2015/01/17");
	        echo iconv("ISO-8859-15","UTF-8",'Son las '.$calendario->horactual().'<br/>del '.$calendario->fechaformateadalarga($calendario->fechadehoy()));
		    ?>
			</h2>
		</div>
	</div>
