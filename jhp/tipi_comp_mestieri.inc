<?php
	$sql_tipi_comp_mes = <<<sql
		SELECT 
			CodiceTCM, 
			FatturazioneScontoMedio,
			DurataContratto,
			/*dataDiffMesi,*/
			DataAvvioServizio,
			
			IdOfferta, IdEdificio, IdMestiere,

			/*YEAR(DataInizio) AnnoInizio,

			MONTH(DataInizio) MeseInizio,
			
			YEAR(DataFine) AnnoFine,
			MONTH(DataFine) MeseFine,

			LEFT(convert(nvarchar, DataInizio, 112), 6) AnnoMeseInizio,

			LEFT(convert(nvarchar, DataFine, 112), 6) AnnoMeseFine, */

			IdTipoComponenteMestiere,
			UM,
			QTA,
			Fattore,
			NIntOre,
			MesiErogazioneAnno,
			MoltiplicatoreFattore,
			Durata,

			Sconto,
			CinquePerCento,
			Migliorie,
			EscludiSconto,
			FacilityManagementLight,
			FattoreLight,
			DivisoreFattore,
			MoltiplicatoreFattore,
			ScontoPerStatoEdificio,
			IncrementoImporto,
			MaggPerc,
			
			FreqQ
				
			, NuovoPrezzo,

			QuotaNonRib,
			MaggManod,
			ScontoManod,

			Manodopera,
			PrezzoManodopera,
			QuotaNonRibassoMan,
			MaggiorazioneMan,
			ScontoConvenzMan
			
			FROM (
				SELECT  DISTINCT
					TipiComponentiMestieri.Codice AS CodiceTCM,
					Offerte.FatturazioneScontoMedio,
					Offerte.DurataContratto,
				
					/*DATEDIFF( 
						month , 
							dbo.cdate( 
									'01'
									+'/'
									+ CAST( MONTH(Offerte.DataAvvioServizio) as nvarchar(10) )
									+'/'
									+ CAST( YEAR(Offerte.DataAvvioServizio) as nvarchar(10) )
								) 
								, 
							dbo.CDate( $dataCiclo )
					) AS dataDiffMesi,*/
					
					
					Offerte.DataAvvioServizio,
				
					ISNULL(OfferteEdifici.Sconto,0) AS Sconto,

					ISNULL(OfferteEdificiMestieri.CinquePerCento,0) AS CinquePerCento,
					ISNULL(OfferteEdificiMestieri.Migliorie,0) AS Migliorie,
					ISNULL(OfferteEdificiMestieri.EscludiSconto,0) AS EscludiSconto,

					ISNULL(Offerte.FacilityManagementLight,0) AS FacilityManagementLight,

					ISNULL(TipiComponentiMestieri.FattoreLight,0) AS FattoreLight,
					ISNULL(TipiComponentiMestieri.DivisoreFattore,0) AS DivisoreFattore,
					ISNULL(TipiComponentiMestieri.MoltiplicatoreFattore,0) AS MoltiplicatoreFattore,
					ISNULL(TipiComponentiMestieri.ScontoPerStatoEdificio,0) AS ScontoPerStatoEdificio,

					ISNULL(SopralluogoPDI.IncrementoImporto,0) AS IncrementoImporto,
					ISNULL(SopralluogoPDI.MaggPerc,0) AS MaggPerc,

					Offerte.IdOfferta,

					tblComponetiMestiere.IdMestiere,
					OfferteEdificiMestieri.IdEdificio,

					SopralluogoPDI.IdSopralluogoPDI,
					SopralluogoPDI.QTA,
									
					ISNULL(SopralluogoPDI.NIntOre,1) NIntOre,

					case 
						when ( SopralluogoPDI.MesiErogazioneAnnoF ) > 1 then SopralluogoPDI.MesiErogazioneAnnoF 
						else ISNULL(SopralluogoPDI.MesiErogazioneAnno ,12)
					end AS MesiErogazioneAnno,

					case 
						when ( SopralluogoPDI.DurataF ) > 1 then SopralluogoPDI.DurataF 
						else ISNULL(SopralluogoPDI.Durata ,12)
					end AS Durata,
								
					TipiComponentiMestieri.IdTipoComponenteMestiere,
					TipiComponentiMestieri.UM,
					TipiComponentiMestieri.Fattore, 
					TipiComponentiMestieri.UnaTantum

					, ISNULL(SopralluogoPDI.FreqQ,1) AS FreqQ
					,(
						SELECT TOP 1 TipiComponetiMestAdeguamentoPrezzi.NuovoPrezzoRev 
						FROM TipiComponetiMestAdeguamentoPrezzi
						WHERE TipiComponetiMestAdeguamentoPrezzi.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdOfferta = SopralluogoPDI.IdOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdCommessaTipoCompMest = TipiComponentiMestieri.IdCommessa
							AND
								dbo.CDate( $dataCiclo ) between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
					) AS NuovoPrezzo,
					(
						SELECT TOP 1 TipiComponetiMestAdeguamentoPrezzi.QuotaNonRib 
						FROM TipiComponetiMestAdeguamentoPrezzi
						WHERE TipiComponetiMestAdeguamentoPrezzi.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdOfferta = SopralluogoPDI.IdOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdCommessaTipoCompMest = TipiComponentiMestieri.IdCommessa
							AND
								dbo.CDate( $dataCiclo ) between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
					) AS QuotaNonRib,
					(
						SELECT TOP 1 TipiComponetiMestAdeguamentoPrezzi.MaggManod 
						FROM TipiComponetiMestAdeguamentoPrezzi
						WHERE TipiComponetiMestAdeguamentoPrezzi.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdOfferta = SopralluogoPDI.IdOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdCommessaTipoCompMest = TipiComponentiMestieri.IdCommessa
							AND
								dbo.CDate( $dataCiclo ) between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
					) AS MaggManod,
					(
						SELECT TOP 1 TipiComponetiMestAdeguamentoPrezzi.ScontoManod 
						FROM TipiComponetiMestAdeguamentoPrezzi
						WHERE TipiComponetiMestAdeguamentoPrezzi.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdOfferta = SopralluogoPDI.IdOfferta
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
							AND
								TipiComponetiMestAdeguamentoPrezzi.IdCommessaTipoCompMest = TipiComponentiMestieri.IdCommessa
							AND
								dbo.CDate( $dataCiclo ) between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
					) AS ScontoManod,
					TipiComponentiMestieri.Manodopera,
					TipiComponentiMestieri.PrezzoManodopera,
					TipiComponentiMestieri.QuotaNonRibassoMan,
					TipiComponentiMestieri.MaggiorazioneMan,
					TipiComponentiMestieri.ScontoConvenzMan
				FROM         
						OfferteEdificiMestieri 
					INNER JOIN
						SopralluogoPDI 
							ON 
								OfferteEdificiMestieri.IdEdificio = SopralluogoPDI.IdEdificio 
							AND 
								OfferteEdificiMestieri.IdCommessaEdificio = SopralluogoPDI.IdCommessaEdificio 
							AND 
								OfferteEdificiMestieri.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta 
							AND 
								OfferteEdificiMestieri.IdOfferta = SopralluogoPDI.IdOfferta 
					INNER JOIN
						tblMestieri 
							ON 
								OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere 
							AND 
								OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa 
					INNER JOIN
						tblComponetiMestiere 
							ON 
								tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa 
							AND 
								tblMestieri.IdMestiere = tblComponetiMestiere.IdMestiere 
					INNER JOIN
						TipiComponentiMestieri 
							ON 
								SopralluogoPDI.IdCommessaTipoComp = TipiComponentiMestieri.IdCommessa 
							AND 
								SopralluogoPDI.IdTipoComp = TipiComponentiMestieri.IdTipoComponenteMestiere 
							AND 
								tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa 
							AND 
								tblComponetiMestiere.IdComponenteMestiere = TipiComponentiMestieri.IdComponenteMestiere 
					INNER JOIN
						Offerte 
							ON 
								OfferteEdificiMestieri.IdOfferta = Offerte.IdOfferta 
							AND 
								OfferteEdificiMestieri.IdCommessaOfferta = Offerte.IdCommessa
					INNER JOIN
						OfferteEdifici 
							ON 
								Offerte.IdCommessa = OfferteEdifici.IdCommessaOfferta 
							AND 
								Offerte.IdOfferta = OfferteEdifici.IdOfferta 
							AND 
								OfferteEdificiMestieri.IdEdificio = OfferteEdifici.IdEdificio
				WHERE 
						( Offerte.IdCommessa = $IdCommessa ) 
					AND 
						( Offerte.IdOfferta = $IdOfferta )
					AND
						( SopralluogoPDI.IdEdificio = $IdEdificio )
					AND
						( tblMestieri.IdMestiere = $IdMestiere )
					AND
						( ISNUMERIC(TipiComponentiMestieri.UM) = 1 )
					/*
					AND
						( tblMestieri.EscludiRM = 0 OR tblMestieri.EscludiRM is null )
					*/
			) a
sql;
	//print_r( $sql_tipi_comp_mes );
	//print_r("<br><br>");
	$query_tipi_comp_mes = ntxQuery($sql_tipi_comp_mes);
	
//	$lista_tipi_componenti = array();
	while($rs_tipi_comp_mes = ntxRecord($query_tipi_comp_mes)){
		
		$CODI_SERVIZIO  = $rs_tipi_comp_mes[CodiceTCM];
		$FatturazioneScontoMedio  = $rs_tipi_comp_mes[FatturazioneScontoMedio]==1 ? 1 : 0;
		
		$dayAtt = ntxDay($rs_tipi_comp_mes[DataAvvioServizio]);
		$monthAtt = ntxMonth($rs_tipi_comp_mes[DataAvvioServizio]);
		$yearAtt = ntxYear($rs_tipi_comp_mes[DataAvvioServizio]);	 	
		
		$IdOfferta = $rs_tipi_comp_mes[IdOfferta];
		$IdEdificio = $rs_tipi_comp_mes[IdEdificio];
		$IdMestiere = $rs_tipi_comp_mes[IdMestiere];
	
		$IdTipoComponenteMestiere = $rs_tipi_comp_mes[IdTipoComponenteMestiere];
		//$InConguaglio = $rs_tipi_comp_mes[InConguaglio];
		
		$AnnoInizio = $rs_tipi_comp_mes[AnnoInizio];
		$MeseInizio = $rs_tipi_comp_mes[MeseInizio];
		
		$AnnoFine = $rs_tipi_comp_mes[AnnoFine];
		$MeseFine = $rs_tipi_comp_mes[MeseFine];
		
		$AnnoMeseInizio = $rs_tipi_comp_mes[AnnoMeseInizio];
		$AnnoMeseFine = $rs_tipi_comp_mes[AnnoMeseFine];
		
		$QuotaNonRib = $rs_tipi_comp_mes[QuotaNonRib];
		if( is_null( $QuotaNonRib ) != true ){$rs_tipi_comp_mes[QuotaNonRibassoMan] = $QuotaNonRib;}
		$MaggManod = $rs_tipi_comp_mes[MaggManod];
		if( is_null( $MaggManod ) != true ){$rs_tipi_comp_mes[MaggiorazioneMan] = $MaggManod;}
		$ScontoManod = $rs_tipi_comp_mes[ScontoManod];
		if( is_null( $ScontoManod ) != true ){$rs_tipi_comp_mes[ScontoConvenzMan] = $ScontoManod;}
		
		$mc = $rs_tipi_comp_mes;
		$mc[mese] = $mese;
		$mc[anno] = $anno;
		require("calcolo_voci/calcolo_voci.inc");
		
//		print_r( "<br>" . $totMese . "<br>" );
//		print_r( $totAnno . "<br>" );
//		print_r( $totImporto . "<br>" );
		
		/*
			Dal file escono 3 variabili compilate e importanti:
			- totMese
			- totAnno
			- totImporto
		*/
		
//		$lista_tipi_componenti["IdTipoComponenteMestiere"] = $IdTipoComponenteMestiere;
//		$lista_tipi_componenti["dataCiclo"] = $dataCiclo;
//		$lista_tipi_componenti["totMese"] += round($totMese, 2);
//		$lista_tipi_componenti["totAnno"] += round($totAnno, 2);
//		$lista_tipi_componenti["totImporto"] += round($totImporto, 2);
//		print_r( $anno );
//		print_r("<br><br><br>");
		$lista_mestieri[$anno]["totMese"] += round($totMese, 2);
		//$lista_mestieri["totMese"] += round($totMese, 2);
		$lista_mestieri[$anno]["totAnno"] += round($totAnno, 2);
		//$lista_mestieri["totAnno"] += round($totAnno, 2);
		
//		print_r( $lista_tipi_componenti );
		//$lista_tipi_componenti["IdMestiere"] = $rs_tipi_comp_mes[IdMestiere];
		//$lista_tipi_componenti["DescMestiere"] = $rs_tipi_comp_mes[Descrizione];
		
//		array_push($lista_mestieri["lista_tipi_componenti"], $lista_tipi_componenti);
//		$lista_tipi_componenti = array();
	}
?>