<?php
	$oEdificio["lista_riepilogo"] = array();
	$oEdificio["lista_riepilogo_anni"] = array();
	$oRiepilogo = array();
	$oRiepilogoAnni = array();
	
	$numMesi = 12;
	$DataInizio_ = ntxDate($DataInizio);
	$DataFine_ = $DataFine;
	$fineMese = "31/12";
	$sql = <<<sql
		SELECT DATEDIFF(year, '$DataInizio_', '$DataFine_')+1 AS DateDiff
sql;
	$numAnni = ntxScalar($sql);
	$tr = "";
	$arrayCountMesiAnno = array();
	$annoInizio = explode("/",$DataInizio_);
	for($i=0;$i < $numAnni;$i++) { 
		$tr .= "<th>".($annoInizio[2]+$i)."</th>"; 
	}
	$annoMax = $annoInizio[2] + ($numAnni-1);
	
	/* Calcolo diffanni */
	$sqlDiffAnni = <<<sql
		select
			YEAR(MIN(CAST(DataInizio as date))) as DataIni
			, DATEDIFF(YEAR, MIN(CAST(DataInizio as date)), MAX(CAST(DATEADD(DAY, -1, DATEADD(MONTH, Durata, DataInizio)) as  date))) as diffanni
		from SopralluogoPDI
		where 
			SopralluogoPDI.IdCommessaOfferta=$IdCommessa  
				AND SopralluogoPDI.IdOfferta=$IdOfferta  
				AND SopralluogoPDI.IdEdificio=$IdEdificio
sql;
	$queryDiffAnni = ntxQuery($sqlDiffAnni);
	
	$diffanni = "";
	while( $rs = ntxRecord($queryDiffAnni) ) {
		$DataIni = $rs[DataIni];
		//$DataFin = $rs[DataFIne];
		$diffanni = ($rs[diffanni])+1;
	}
	
	$arrayCanoneAnnoServizio = array();
	
	$CanoneMensileTot = 0;
	$CanoneAnnoTot = 0;
	$CanoneContrattoTot = 0;
	
	$arrayAnni = array();
	
	//Ora per i 3 GRUPPI di SERVIZI OTTENGO i DATI richiesti
	
	$sql_ls_PS = <<<sql
		SELECT TOP 1 
			OfferteEdificiMestieri.IdMestiere, 
			tblMestieri.Descrizione,
			tblMestieri.Codice,
			OfferteEdificiMestieri.Durata,  
			OfferteEdificiMestieri.DurataF,
			OfferteEdificiMestieri.DataInizio,
			OfferteEdificiMestieri.DataFine
		FROM 
			tblMestieri INNER JOIN 
                     OfferteEdificiMestieri ON tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere AND 
                     tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaOfferta 
		WHERE 
				OfferteEdificiMestieri.IdCommessaOfferta = $IdCommessa  
			AND 
				OfferteEdificiMestieri.IdOfferta = $IdOfferta  
			AND 
				OfferteEdificiMestieri.IdEdificio = $IdEdificio
			AND 
				tblMestieri.codice LIKE 'PUL%'
sql;
	$query = ntxQuery($sql_ls_PS);
	while( $rs = ntxRecord($query) ) {
		$Descrizione = 'SERVIZIO DI PULIZIA';
		$Codice = explode(".",$rs[Codice]);
		$Codice = $Codice[0];
		require('somma_del_servizio.inc');
	}
	
/** INIZIO DIS **/
	$sql_ls_DIS = <<<sql
		SELECT TOP 1 
			OfferteEdificiMestieri.IdMestiere, 
			tblMestieri.Descrizione,
			tblMestieri.Codice,
			OfferteEdificiMestieri.Durata, 
			OfferteEdificiMestieri.DurataF,
			OfferteEdificiMestieri.DataInizio,
			OfferteEdificiMestieri.DataFine
		FROM 
			tblMestieri INNER JOIN 
                     OfferteEdificiMestieri ON tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere AND 
                     tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaOfferta 
		WHERE 
				OfferteEdificiMestieri.IdCommessaOfferta=$IdCommessa  
			AND 
				OfferteEdificiMestieri.IdOfferta=$IdOfferta  
			AND 
				OfferteEdificiMestieri.IdEdificio=$IdEdificio
			AND 
				tblMestieri.codice LIKE 'DIS%'
sql;

	$query = ntxQuery($sql_ls_DIS);
	while( $rs = ntxRecord($query) ) {
		$Descrizione = "SERVIZIO DI DISINFESTAZIONE";
		$Codice = explode(".",$rs[Codice]);
		$Codice = $Codice[0];
		require('somma_del_servizio.inc');
	}
/** Fine DIS **/	
	
	$sql_ls_ICS = <<<sql
		SELECT TOP 1 
			OfferteEdificiMestieri.IdMestiere, 
			tblMestieri.Descrizione,
			tblMestieri.Codice,
			OfferteEdificiMestieri.Durata,
			OfferteEdificiMestieri.DurataF,			
			OfferteEdificiMestieri.DataInizio,
			OfferteEdificiMestieri.DataFine
		FROM 
			tblMestieri INNER JOIN 
                     OfferteEdificiMestieri ON tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere AND 
                     tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaOfferta 
		WHERE 
				OfferteEdificiMestieri.IdCommessaOfferta=$IdCommessa  
			AND 
				OfferteEdificiMestieri.IdOfferta=$IdOfferta  
			AND 
				OfferteEdificiMestieri.IdEdificio=$IdEdificio
			AND 
				tblMestieri.codice LIKE 'ICS'
sql;
	$query = ntxQuery($sql_ls_ICS);
	while( $rs = ntxRecord($query) ) {
		$Descrizione = ntxAnsi2Utf8($rs[Descrizione]);
		$Codice = explode(".",$rs[Codice]);
		$Codice = $Codice[0];
		require('somma_del_servizio.inc');
	}
	
	$sql_ls_WC = <<<sql
		SELECT TOP 1 
			OfferteEdificiMestieri.IdMestiere, 
			tblMestieri.Descrizione,
			tblMestieri.Codice,
			OfferteEdificiMestieri.Durata,
			OfferteEdificiMestieri.DurataF,			
			OfferteEdificiMestieri.DataInizio,
			OfferteEdificiMestieri.DataFine
		FROM 
			tblMestieri INNER JOIN 
                     OfferteEdificiMestieri ON tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere AND 
                     tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaOfferta 
		WHERE 
				OfferteEdificiMestieri.IdCommessaOfferta=$IdCommessa  
			AND 
				OfferteEdificiMestieri.IdOfferta=$IdOfferta  
			AND 
				OfferteEdificiMestieri.IdEdificio=$IdEdificio
			AND 
				tblMestieri.codice LIKE 'WC'
sql;
	$query = ntxQuery($sql_ls_WC);
	while( $rs = ntxRecord($query) ) {
		$Descrizione = 'FORNITURA DI MATERIALE IGIENICO SANITARIO';
		$Codice = explode(".",$rs[Codice]);
		$Codice = $Codice[0];
		require('somma_del_servizio.inc');
	}
	
	$totMes = round( $totMes , 2 );
	if( $totMes == 0 ){
		$totAnno = 0;
		$totContratto = 0;
	}
	
	for($k=0;$k < $diffanni;$k++){
		if( $totAnno == 0 ){ $arrayAnni[$k] = 0; }
		$oEdificio["tot_canone_anno_".($k+1)] = ntxRoundThousand($arrayAnni[$k],2);
		$arrayRiepilogoEconomico[$IdEdificio][$k] = $arrayAnni[$k];
	}
	
	$oEdificio["tot_canone_mese"] = ntxRoundThousand($totMes,2);
	
	$oEdificio["tot_canone_anno"] = ntxRoundThousand($totAnno,2);
	$oEdificio["tot_canone_contratto"] = ntxRoundThousand($totContratto,2);
	$oEdificio["tot_canoni"] = ntxRoundThousand($totContratto,2);
?>