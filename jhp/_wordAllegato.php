<?php
	session_set_cookie_params(3600,"/");
	session_start();
	
	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");
	require_once(NTX_WEBPORTAL_PATH.'_database.inc');
	
	$arrayWord = array();
	//Ottengo le informazioni
	$StringaIdProprietario = ntxsp('IdProprietario');
	$IdProprietario = /*explode(",",*/substr($StringaIdProprietario, 1, -1)/*)*/;
	//print_r($IdProprietario);
	//Per IdProp si intendono gli id proprietari che vengono selezionati
//	$IdProp = $ArrIdProprietario[$i];
	//print_r($IdProp);
	//print_r($_POST);
	$selezionaAnno = ntxip('SelezionaAnno');
	//$SelezionaAnno = ntxip('SelezionaAnno');
	$ArrdataInizio = explode("-",$_POST[MeseAnnoInizio]);
	//print_r($ArrdataInizio);
	
	$MeseInizio = $ArrdataInizio[1];
	$AnnoInizio = $ArrdataInizio[0];
	
	$ArrdataFine = explode("-",$_POST[MeseAnnoFine]);
	//print_r($ArrdataInizio);
	$MeseFine = $ArrdataFine[1];
	$AnnoFine = $ArrdataFine[0];
	
	$sqlProprietario = <<<sql
		SELECT DISTINCT  
			tblPgPropietari.Nome,
			Offerte.Referente,
			tblPgPropietari.Pec,
			tblPgPropietari.email_1,
			tblPgPropietari.Codice,
			Ordini.NumOrdine,
			Offerte.IdOfferta,
			Offerte.NOPF,
			Offerte.DataNOPF,
			ReferentiOfferte.Funzione
		FROM         
			Offerte 
				INNER JOIN
					tblPgPropietari ON Offerte.IdPropietario = tblPgPropietari.IdPropietario 
				INNER JOIN 
					Ordini
					ON ordini.IDCommessa = offerte.IdCommessa
				INNER JOIN
					ReferentiOfferte
					ON Offerte.IdCommessa = ReferentiOfferte.IdCommessa
					AND Offerte.IdOfferta = ReferentiOfferte.IdOfferta
					
		WHERE 
				Offerte.IdCommessa = $IdCommessa 
			AND 
				tblPgPropietari.IdPropietario IN ( $IdProprietario )
sql;
	//print_r($sqlProprietario);
	$queryProprietario = ntxQuery( $sqlProprietario );
	$rsProprietario = ntxRecord( $queryProprietario );
		
	/** INIZIO INTESTAZIONE **/
	$arrayWord["nomeProprietario"] = ntxAnsi2Utf8("$rsProprietario[Nome]");
	$arrayWord["nomeReferente"] = ntxAnsi2Utf8("$rsProprietario[Referente]");
	$arrayWord["funzione_Supervisore"] = ntxAnsi2Utf8("$rsProprietario[Funzione]");
	$arrayWord["pec"] = ntxAnsi2Utf8("$rsProprietario[Pec]");
	$arrayWord["email"] = ntxAnsi2Utf8("$rsProprietario[email_1]");
	/** FINE INTESTAZIONE **/
	
	/** INIZIO PARAGRAFO NUMERO 1 **/
	$arrayWord["codProprietario"] = ntxAnsi2Utf8("$rsProprietario[Codice]");
	//NumOrdine forse lo dobbiamo spostare nel while del percIstat
	$arrayWord["numOrdine"] = ntxAnsi2Utf8("$rsProprietario[NumOrdine]");
	/** FINE PARAGRAFO NUMERO 1 **/
	
	/** INIZIO ELENCO PRIMA DI TABELLA**/
	$arrayWord["NOPF"] = ntxAnsi2Utf8("$rsProprietario[NOPF]");
	$DataNOPF = ntxDate("$rsProprietario[DataNOPF]");
	$arrayWord["DataNOPF"] = ntxAnsi2Utf8( $DataNOPF );
	$arrDataNOPF = explode("/",$DataNOPF);
	$MeseAnnoDataNOPF = $arrDataNOPF[1] . "/" . $arrDataNOPF[2];
	/** FINE ELENCO PRIMA DI TABELLA**/
	
	$arrayWord["selezionaAnno"] = $selezionaAnno;
	$IdOfferta = $rsProprietario[IdOfferta];

	$sql_percIstat = <<<sql
			SELECT DISTINCT /*Ordini.NumOrdine,*/
					/*count(ApplicazioniPercIstat.PercIstat) as conta,*/
				   ApplicazioniPercIstat.PercIstat,
				   ApplicazioniPercIstat.Mese,
				   ApplicazioniPercIstat.Anno
				   
			FROM ApplicazioniPercIstat
			/*INNER JOIN ordini
			ON ordini.IDCommessa = ApplicazioniPercIstat.IdCommessa*/
			WHERE ApplicazioniPercIstat.IdCommessa = $IdCommessa 
			AND ApplicazioniPercIstat.IdProprietario IN ($IdProprietario)
			AND ApplicazioniPercIstat.PercIstat is not null
			AND ApplicazioniPercIstat.Anno BETWEEN $AnnoInizio AND $AnnoFine
			--AND ApplicazioniPercIstat.Mese BETWEEN $MeseInizio AND $MeseFine
			
			/*GROUP BY ApplicazioniPercIstat.PercIstat, 
					ApplicazioniPercIstat.Mese, 
					ApplicazioniPercIstat.Anno */
				
sql;
	//print_r($sql_percIstat);
	$query_percIstat = ntxQuery($sql_percIstat);
	//print_r($query_percIstat);
	
	$arrayWord["listaPercIstat"] = array();
	$listaPercIstat = array();
	while($rs_percIstat = ntxRecord($query_percIstat)){
		$PercIstat = ntxRound($rs_percIstat[PercIstat], 2);
		//$MeseAnnoDataNOPF = $rs_percIstat[""];
		
		if( $rs_percIstat[Mese] < 10 ){ $rs_percIstat[Mese] = "0".$rs_percIstat[Mese]; }
		$MeseAnnopercIstat = "01/".$rs_percIstat[Mese]."/".$rs_percIstat[Anno];
		
		$listaPercIstat["percIstat"] = $PercIstat;
		$listaPercIstat["MeseAnnoDataNOPF"] = $MeseAnnoDataNOPF;
		$listaPercIstat["MeseAnnopercIstat"] = $MeseAnnopercIstat;

		array_push($arrayWord["listaPercIstat"], $listaPercIstat);
		//print_r($arrayWord["listaPercIstat"]);
	}
	//RIEPILOGO COMPLESSIVO DEGLI IMPORTI INCLUSO ADEGUAMENTO ISTAT
	$arrayWord["dati"] = array();
	//$riepilogo_complessivo = array();
	require("riepilogo_complessivo.php");
?>