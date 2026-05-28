<?php
	//$lista_id = substr(ntxsp('lista_id'), 1, -1); //lista dei proprietari
	$IdCommessa = ntxise('IdCommessa');

	//converto la lista degli id in un array
	//$Arrlista_id = explode(",",$lista_id);
	$ArrPercIstat = array();
	//ciclo array con idproprietari che deve interrogare singolarmente un id alla volta
	for($i = 0; $i < count($lista_id); $i++){
		
		$IdProprietario = $lista_id[$i];
		$ArrPercIstat[$IdProprietario] = array();

		$sql = <<<sql
			 SELECT * 
			 FROM ApplicazioniPercIstat	
			 WHERE IdCommessa = $IdCommessa
			 AND IdProprietario = $IdProprietario
			 AND Bloccato = 1
			 AND PercIstat is not null
			 ORDER BY 
				IdCommessa, IdProprietario, Anno, Mese
sql;
		//jhp_log($sql);
		$query = ntxQuery($sql);

		//$arrMesiAnni = array();
		while($rs = ntxRecord($query)){
			
			$Mese = $rs[Mese];
			$Anno = $rs[Anno];
			$PercIstat = $rs[PercIstat];
			
			if($Mese < 10){ $Mese = "0".$Mese; }
			
			$ArrPercIstat[$IdProprietario][$Anno."-".$Mese] = $PercIstat;
		}
	}
?>