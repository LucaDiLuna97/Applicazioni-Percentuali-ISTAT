<?php

	$arraySelect = array("MeseErogGen", "MeseErogFeb", "MeseErogMar", "MeseErogApr", "MeseErogMag", "MeseErogGiu", "MeseErogLug", "MeseErogAgo", "MeseErogSet", "MeseErogOtt", "MeseErogNov", "MeseErogDic");

	$select = $arraySelect[$mese-1];


	$sql = <<<sql
		SELECT	$select
		FROM 
				SopralluogoPDI 
		LEFT JOIN 
				TipiComponentiMestieri 
			ON 
				TipiComponentiMestieri.IdCommessa = SopralluogoPDI.IdCommessaEdificio 
			AND 
				TipiComponentiMestieri.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
		LEFT JOIN 
				tblComponetiMestiere 
			ON 
				tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa 
			AND 
				TipiComponentiMestieri.IdComponenteMestiere = tblComponetiMestiere.IdComponenteMestiere
		LEFT JOIN  
			tblMestieri 
			ON 
				tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa 
			AND 
				tblComponetiMestiere.IdMestiere = tblMestieri.IdMestiere
		WHERE 
			IdCommessaEdificio = $IdCommessa
		AND 
			IdOfferta = $IdOfferta
		AND 
			IdEdificio = $IdEdificio
		AND 
			tblMestieri.IdMestiere = $IdMestiere
		AND 
			TipiComponentiMestieri.IdTipoComponenteMestiere = $IdTipoComponenteMestiere
sql;

$bit = ntxScalar ($sql);
if($bit == 0) $totMese = 0;
?>