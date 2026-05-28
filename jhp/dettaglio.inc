<?php
	$documento["lista_ds"] = array();
	$dettaglio_servizi_immobile = array();
	
	//print_r("IdCommessa:   ".$IdCommessa);
	
	$sql_edifici = <<<sql
		SELECT DISTINCT 
			OfferteEdificiMestieri.IdEdificio, 
			ElencoEdifici.Codice,
			ElencoEdifici.EdificioBreve,
			ElencoEdifici.Indirizzo,
			ElencoEdifici.CITTA,
			ElencoEdifici.PROVINCIA
		FROM 
			ElencoEdifici 
			INNER JOIN OfferteEdificiMestieri  
				ON ElencoEdifici.IdCommessa=OfferteEdificiMestieri.IdCommessaEdificio  
				AND ElencoEdifici.Id=OfferteEdificiMestieri.IdEdificio 
			LEFT JOIN tblPgPropietari 
				ON ElencoEdifici.IdProprietario=tblPgPropietari.IdPropietario 
		WHERE 
				OfferteEdificiMestieri.IdCommessaOfferta = $IdCommessa
			/* AL POSTO DI IDOFFERTA DOVREBBE ANDARCI IDPROPRIETARIO */	
			/*AND 
				OfferteEdificiMestieri.IdOfferta = $IdOfferta*/
		ORDER BY 
			Codice, 
			OfferteEdificiMestieri.IdEdificio
sql;
	$inEdifici = "";
	//print_r($sql_edifici);
	$query_edifici = ntxQuery($sql_edifici);
	while( $rsEdif = ntxRecord($query_edifici) ) { $inEdifici .= $rsEdif[IdEdificio] . ","; }

	$inEdifici = substr( $inEdifici , 0 , -1 );
	
	$sql2 = <<<sql
		SELECT 
			SopralluogoPDI.IdEdificio,
			IdTipoComp,  
			IncrementoImporto, 
			QTA, 
			QtaFreqPdi, 
			NotaQta, 
			LivelloDocCensimento Liv, 
			DataInizio, 
			
			Durata, 
			DurataF, 
			
			UnaTantumExtraCanone, 
			Ore, 
			Note, 
			MesiErogazioneAnno,
			MesiErogazioneAnnoF
		FROM 
			SopralluogoPDI 
		WHERE 
				SopralluogoPDI.IdCommessaOfferta=$IdCommessa  
			/* AL POSTO DI IDOFFERTA CI DOVREBBE ANDARE IDPROPRIETARIO */	
			/*AND 
				SopralluogoPDI.IdOfferta=$IdOfferta */
			AND 
				SopralluogoPDI.IdEdificio in ($inEdifici)
sql;
	$arrayEdifici = array();
	
	//print_r($sql2);
	$query2 = ntxQuery($sql2);
	while( $rs2 = ntxRecord($query2) ) {
		$DataMin = 0;
		$DurataMax = 0;
		
		$IdEdificio = $rs2[IdEdificio];
		$DataInizio = $rs2[DataInizio];
		
		if(($DataMin == 0) || (strtotime($DataInizio) < strtotime($DataMin))){$DataMin = $DataInizio;}
		
		$Durata = $rs2[Durata];
		$DurataF = $rs2[DurataF];
		
		if(!is_null($DurataF)){
			$Durata = $DurataF;
		}
		
		if($Durata > $DurataMax){$DurataMax = $Durata;}
		
		$arrayEdifici[$IdEdificio][Durata] = $Durata;
		
		$MesiErogazioneAnno 	= $rs2[MesiErogazioneAnno] ? $rs2[MesiErogazioneAnno] : 12;
		$MesiErogazioneAnnoF 	= $rs2[MesiErogazioneAnnoF];
		
		if( !is_null($MesiErogazioneAnnoF) ){
			$MesiErogazioneAnno = $MesiErogazioneAnnoF;
		}
		
		$arrayEdifici[$IdEdificio][MesiErogazioneAnno] = $MesiErogazioneAnno;
		
		$DataMarray = explode(" ",$DataMin);
		$Data1 = str_replace("-","",$DataMarray[0]);
		
		$sql3 = <<<sql
			select CAST(DATEADD(day, -1, DATEADD(month, $DurataMax, '$Data1')) as DATE) as DataFine
sql;
		
		$DataFine =ntxScalar($sql3);
		$DataFine_array = explode("-",$DataFine);
		$DataFine = $DataFine_array[2]."/".$DataFine_array[1]."/".$DataFine_array[0];
		
		$numMesi = 12;
		$DataInizio_ = ntxDate($DataInizio);
		$DataFine_ = $DataFine;
		$fineMese = "31/12";
		$sql = <<<sql
			SELECT DATEDIFF(year, '$DataInizio_', '$DataFine_')+1 AS DateDiff
sql;
		$numAnni = ntxScalar($sql);
		//$listaAnni = "";
		$arrayCountMesiAnno = array();
		$annoInizio = explode("/",$DataInizio_);
		for($i=0;$i < $numAnni;$i++){			
			$arrayEdifici[$IdEdificio][numAnni] = $numAnni;
			if($arrayEdifici["Max"] <= $numAnni){
				$arrayEdifici["Max"] = $numAnni;
			}
		}
		$annoMax = $annoInizio[2] + ($numAnni-1);
	}
	
	// $listaEdifici_RE = "<tr>";
	$sqlEdifici = <<<sql
		SELECT DISTINCT 
			OfferteEdificiMestieri.IdEdificio, 
			CAST(ElencoEdifici.Codice AS INT) AS Codice,
			ElencoEdifici.EdificioBreve,
			ElencoEdifici.Indirizzo,
			ElencoEdifici.CITTA,
			ElencoEdifici.PROVINCIA
		FROM 
			ElencoEdifici 
			INNER JOIN OfferteEdificiMestieri  
				ON ElencoEdifici.IdCommessa=OfferteEdificiMestieri.IdCommessaEdificio  
				AND ElencoEdifici.Id=OfferteEdificiMestieri.IdEdificio
			INNER JOIN tblMestieri
				ON tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere
				AND tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaEdificio
			LEFT JOIN tblPgPropietari 
				ON ElencoEdifici.IdProprietario=tblPgPropietari.IdPropietario 
		WHERE OfferteEdificiMestieri.IdCommessaOfferta=$IdCommessa
		/* AL POSTO DI IDOFFERTA CI DOVREBBE ANDARE IDPROPRIETARIO */
		/*AND OfferteEdificiMestieri.IdOfferta=$IdOfferta  */
		ORDER BY Codice, OfferteEdificiMestieri.IdEdificio
sql;
	$soldiTotaleAnno = array();

	$totPDA = 0;
	$queryEdifici = ntxQuery($sqlEdifici);
	
	while( $rs = ntxRecord($queryEdifici) ) {
		
		$IdEdificio = $rs[IdEdificio];

		$Codice = $rs[Codice];
		$EdificioBreve 	= ntxAnsi2Utf8($rs[EdificioBreve]);
		$Indirizzo 		= ntxAnsi2Utf8($rs[Indirizzo]);
		$CITTA 			= ntxAnsi2Utf8($rs[CITTA]);
		$PROVINCIA 		= ntxAnsi2Utf8($rs[PROVINCIA]);
		
		$dettaglio_servizi_immobile[Cod_edificio] = $Codice;
		$dettaglio_servizi_immobile[Nome_edificio] = $EdificioBreve;
		//$dettaglio_servizi_immobile[edificio_citta] = $CITTA;
		//$dettaglio_servizi_immobile[edificio_prov] = $PROVINCIA;

		$totPDARiga = 0;
		
		//QUESTA PORZIONE DI CODICE SI RIFERISCE AI MESI
		/*for( $mesEdif = 1 ; $mesEdif <= count($arrayPosizioniMestieri); $mesEdif++ ){
			$dettaglio_servizi_immobile["edificio_" . $mesEdif ] = ntxRoundThousand($totRiepilogoEconomicoEdificio[$IdEdificio][ $mesEdif ],2) ? ntxRoundThousand($totRiepilogoEconomicoEdificio[$IdEdificio][ $mesEdif ],2) : "0,00";
			$totPDARiga += $totRiepilogoEconomicoEdificio[$IdEdificio][ $mesEdif ];
		}*/
		
		//$dettaglio_servizi_immobile[edificio_tot] = ntxRoundThousand($totPDARiga,2);		  
		$totPDA += $totPDARiga;

		// #### CAMPO NUMANNI
		$documento["numAnni".($arrayEdifici["Max"])."_ds"] = true;
		
		$div = (($arrayEdifici[$IdEdificio][MesiErogazioneAnno] / 12) * $arrayEdifici[$IdEdificio][Durata]);
		
		$totEdifMese = ($totPDARiga / $div);
		
		for($l=0;$l < $arrayEdifici["Max"];$l++){
			
			$totEdif = ( $arrayRiepilogoEconomico[$IdEdificio][$l] );
			$soldiTotaleAnno[$l] += $totEdif;
			
			$dettaglio_servizi_immobile["servizio_anno_".($l+1)] = ntxRoundThousand($totEdif,2) ? ntxRoundThousand($totEdif,2) : "0,00";
		}
		
		$dettaglio_servizi_immobile["tot"] = ntxRoundThousand($totPDARiga,2);
		
		array_push($documento["lista_ds"], $dettaglio_servizi_immobile);
		$dettaglio_servizi_immobile = array();
	}

	
	for( $l=0 ; $l < $arrayEdifici["Max"] ; $l++ ){
		$documento["servizio_tot_anno_".($l+1)] = ntxRoundThousand($soldiTotaleAnno[$l],2);
		$documento["anno_".($l+1)] = ($annoInizio[2]+$l);
	}
	
	$documento["div"] = $div;
	
	/*for( $z = 1 ; $z <= count($arrayPosizioniMestieri); $z++ ){
		$documento["tot_re_$z"] = ntxRoundThousand($totRiepilogoEconomico[ $z ],2) ? ntxRoundThousand($totRiepilogoEconomico[ $z ],2) : "0,00";
	}*/
	
	//QUESTA PARTE COMMENTATA SI RIFERISCE AL MESE
	//$documento["tot"] = ntxRoundThousand($totPDA,2);
	$documento["tot_servizi"] = ntxRoundThousand($totPDA,2);
?>