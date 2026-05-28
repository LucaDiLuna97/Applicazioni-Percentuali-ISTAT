<?php
	$IdAA  = $_POST['IdAA'];
	
	if( $AttiAggiuntivi==1 && is_numeric($IdAA) )
	{
		$IdOfferta = $IdAA;
	}
	else
	{
		//$IdOfferta = ntxip('IdOfferta');
	}

	$sqlEdifici = <<< sqlEdifici
		SELECT  DISTINCT 
			tblPgPropietari.IdPropietario, tblPgPropietari.Codice,
			tblPgPropietari.Nome,
			
			ApplicazioniPercIstat.PercIstat,
			ApplicazioniPercIstat.Mese,
			ApplicazioniPercIstat.Anno
		  
		FROM				
			tblPgPropietari 
	
			LEFT JOIN	
						ApplicazioniPercIstat
							ON tblPgPropietari.IdPropietario = ApplicazioniPercIstat.idproprietario	
		/*WHERE 				
			ApplicazioniPercIstat.IdCommessa = $IdCommessa*/
		
		ORDER BY tblPgPropietari.Codice
sqlEdifici;

	$query = ntxQuery($sqlEdifici);
	//jhp_log($sqlEdifici);
?>