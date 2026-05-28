<?php
	$IdAA  = $_POST['IdAA'];
	
	if( $AttiAggiuntivi == 1 && is_numeric($IdAA) ) {
		$IdOfferta = $IdAA;
	}
	else {
		$IdOfferta = "";
	}
	

	$wherePeriodiOfferte = " WHERE (Offerte.IdCommessa = $IdCommessa) /*AND UtentiEdifici.IdUtente=$IdUtente */ AND (tblMestieri.EscludiRM = 0 OR tblMestieri.EscludiRM is null ) ";

	if ( is_numeric($IdProprietario) ) $wherePeriodiOfferte .= " AND Offerte.IdPropietario=$IdProprietario ";
	if ( is_numeric($IdOfferta) ) $wherePeriodiOfferte .= " AND Offerte.IdOfferta=$IdOfferta ";
	
	$sqlGiornoFineCheck = <<< sqlGiornoFineCheck
		SELECT DISTINCT (
							CASE WHEN SopralluogoPDI.IdRevConsistenza IS NULL THEN 
							DAY (dateadd(month,OfferteEdificiMestieri.Durata,OfferteEdificiMestieri.DataInizio))
							else
							DAY (dateadd(month,SopralluogoPDI.Durata,SopralluogoPDI.DataInizio))
							end
						) GiornoFine

		FROM   OfferteEdificiMestieri 
			INNER JOIN
				SopralluogoPDI 
					ON OfferteEdificiMestieri.IdEdificio = SopralluogoPDI.IdEdificio 
					AND OfferteEdificiMestieri.IdCommessaEdificio = SopralluogoPDI.IdCommessaEdificio 
					AND OfferteEdificiMestieri.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta 
					AND OfferteEdificiMestieri.IdOfferta = SopralluogoPDI.IdOfferta 
			INNER JOIN
				tblMestieri 
					ON OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere 
					AND OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa 
			INNER JOIN
				tblComponetiMestiere 
					ON tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa 
					AND tblMestieri.IdMestiere = tblComponetiMestiere.IdMestiere 
			INNER JOIN
				TipiComponentiMestieri 
					ON SopralluogoPDI.IdCommessaTipoComp = TipiComponentiMestieri.IdCommessa 
					AND SopralluogoPDI.IdTipoComp = TipiComponentiMestieri.IdTipoComponenteMestiere 
					AND tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa 
					AND tblComponetiMestiere.IdComponenteMestiere = TipiComponentiMestieri.IdComponenteMestiere 
			INNER JOIN
				Offerte 
					ON OfferteEdificiMestieri.IdOfferta = Offerte.IdOfferta 
					AND OfferteEdificiMestieri.IdCommessaOfferta = Offerte.IdCommessa	
			$wherePeriodiOfferte
sqlGiornoFineCheck;

	$CheckGiornoFine = ntxScalar($sqlGiornoFineCheck);
	if($CheckGiornoFine > 1)
		$durataQuery = "OfferteEdificiMestieri.Durata";
	else
		$durataQuery ="OfferteEdificiMestieri.Durata-1";
	
	
	$sqlPeriodiOfferte = <<< sqlPeriodiOfferte
		SELECT 
			IdOfferta, 
			YEAR(DataInizio) AnnoInizio, 
			MONTH(DataInizio) MeseInizio, 
			YEAR(DataFine) AnnoFine, 
			MONTH(DataFine) MeseFine,
			
			LEFT(convert(nvarchar, DataInizio, 112), 6) AnnoMeseInizio,
			
			LEFT(convert(nvarchar, DataFine, 112), 6) AnnoMeseFine
			
		FROM (
			SELECT IdOfferta,
			
					case when DataInizio>='$data1' then DataInizio else '$data1' end DataInizio,
					case when DataFine<='$data2' then DataFine else '$data2' end DataFine
					
			FROM (
				SELECT  Offerte.IdOfferta,
				
						MIN(
							case when SopralluogoPDI.IdRevConsistenza is null then 
								OfferteEdificiMestieri.DataInizio 
							else
								SopralluogoPDI.DataInizio
							end 
							) DataInizio, 
						
						MAX(
							case when SopralluogoPDI.IdRevConsistenza is null then 
								dateadd(month,$durataQuery,OfferteEdificiMestieri.DataInizio)
							else
								dateadd(month,$durataQuery,SopralluogoPDI.DataInizio)
							end
							) DataFine
							
				FROM    OfferteEdificiMestieri 
					INNER JOIN
						SopralluogoPDI 
							ON OfferteEdificiMestieri.IdEdificio = SopralluogoPDI.IdEdificio 
							AND OfferteEdificiMestieri.IdCommessaEdificio = SopralluogoPDI.IdCommessaEdificio 
							AND OfferteEdificiMestieri.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta 
							AND OfferteEdificiMestieri.IdOfferta = SopralluogoPDI.IdOfferta 
					INNER JOIN
						tblMestieri 
							ON OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere 
							AND OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa 
					INNER JOIN
						tblComponetiMestiere 
							ON tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa 
							AND tblMestieri.IdMestiere = tblComponetiMestiere.IdMestiere 
					INNER JOIN
						TipiComponentiMestieri 
							ON SopralluogoPDI.IdCommessaTipoComp = TipiComponentiMestieri.IdCommessa 
							AND SopralluogoPDI.IdTipoComp = TipiComponentiMestieri.IdTipoComponenteMestiere 
							AND tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa 
							AND tblComponetiMestiere.IdComponenteMestiere = TipiComponentiMestieri.IdComponenteMestiere 
					INNER JOIN
						Offerte 
							ON OfferteEdificiMestieri.IdOfferta = Offerte.IdOfferta 
							AND OfferteEdificiMestieri.IdCommessaOfferta = Offerte.IdCommessa
							
					/*INNER JOIN
						UtentiEdifici 
							ON OfferteEdificiMestieri.IdCommessaEdificio = UtentiEdifici.IdCommessa 
							AND OfferteEdificiMestieri.IdEdificio = UtentiEdifici.IdEdificio
					*/	
						
				$wherePeriodiOfferte
					
				GROUP BY 
					Offerte.IdOfferta
			) a
		) b
sqlPeriodiOfferte;
//jhp_log($sqlPeriodiOfferte);

	$query = ntxQuery($sqlPeriodiOfferte);

	$periodiOfferte = array();

	while( $rs = ntxRecord($query) )
	{
		$AnnoInizio = $rs[AnnoInizio];
		$AnnoFine = $rs[AnnoFine];
		
		if ( !is_numeric($AnnoInizio) ||  !is_numeric($AnnoFine) ) continue;
		
		$IdOfferta = $rs[IdOfferta];
		$MeseInizio = $rs[MeseInizio];
		$MeseFine = $rs[MeseFine];
		//jhp_log($IdOfferta);
		//jhp_log($MeseInizio.$MeseFine.$AnnoInizio.$AnnoFine);
		
		$periodiOfferte[$IdOfferta] = array(IdOfferta=>$IdOfferta,
											MeseInizio=>$MeseInizio,AnnoInizio=>$AnnoInizio,
											MeseFine=>$MeseFine,AnnoFine=>$AnnoFine,
											AnnoMeseInizio=>$AnnoMeseInizio,
											AnnoMeseFine=>AnnoMeseFine);
	}


	function ThMesi($IdOfferta) 
	{
		global $periodiOfferte, $mesi;
		global $attivaDelta , $AttiAggiuntivi, $conguaglio;
		
		$rs = $periodiOfferte[$IdOfferta];
		$MeseInizio = $rs[MeseInizio];
		$MeseFine = $rs[MeseFine];
		$AnnoInizio = substr($rs[AnnoInizio],2);
		$AnnoFine = substr($rs[AnnoFine],2);
		
		//jhp_log($periodiOfferte);

		$result = "";
		
		for($mese=$MeseInizio, $anno=$AnnoInizio; $anno<$AnnoFine || ($mese<=$MeseFine && $anno==$AnnoFine);)
		{
			//costruzione della data presente nell'intestazione
			
			$result .= "<th>".$mesi[$mese]."-"."<!--<br>-->$anno";
			/*if( $attivaDelta == 1 && $AttiAggiuntivi == 1 )
			{
				$result .= "<th> DELTA <br>".$mesi[$mese]."<br>$anno";
			}
		
			if($conguaglio == 1)
			{
				$result .= "<th>Conguaglio<br>".$mesi[$mese]."<br>".($anno);
			}*/
			
			$mese++;
			
			if ( $mese==13 )
			{
				$mese = 1;
				$anno++;
			}
		}
		return $result;
	}

?>