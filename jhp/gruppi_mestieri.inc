<?php
	$sql_gruppiMest = <<<sql
		SELECT DISTINCT
			GruppoMestieri.Descrizione, 
			GruppoMestieri.IdGruppoRendicontoMes
		FROM 
			GruppoMestieri
				INNER JOIN tblMestieri
		on tblMestieri.IdCommessa = GruppoMestieri.IdCommessa
		and tblMestieri.IdGruppoMes = GruppoMestieri.IdGruppoRendicontoMes
				INNER JOIN OfferteEdificiMestieri
		on OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa
		and OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere
		WHERE 
				GruppoMestieri.IdCommessa = $IdCommessa
			AND 
				OfferteEdificiMestieri.IdEdificio = $IdEdificio
			AND 
				OfferteEdificiMestieri.IdOfferta = $IdOfferta
		ORDER BY GruppoMestieri.IdGruppoRendicontoMes
sql;
	//print_r($sql_gruppiMest);
	$query_gruppiMest = ntxQuery($sql_gruppiMest);
	
	$GruppiMestieri = array();
	while($rs_gruppiMest = ntxRecord($query_gruppiMest)){
		
		$IdGruppoMes = $rs_gruppiMest[IdGruppoRendicontoMes];
		
		$GruppiMestieri["DescGruppoMes"] = $rs_gruppiMest[Descrizione];
		$GruppiMestieri["IdGruppoMes"] = $IdGruppoMes;
		$GruppiMestieri["lista_mestieri"] = array();
		
		//Interrogo i mestieri del gruppo mestiere
		require("mestieri.inc");
		
		//print_r($GruppiMestieri["DescGruppoMes"]);
		
		array_push($arrayWord["dati"][$IdEdificio]["GruppoMestieri"], $GruppiMestieri);
	}
?>