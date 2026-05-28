<?php
	$IdAA  = $_POST['IdAA'];
	
	if($AttiAggiuntivi == 1 && is_numeric($IdAA))
	{
		$IdOfferta = $IdAA;
	}
	else
	{
		//$IdOfferta = ntxip('IdOfferta');
	}
		
	$periodoOfferta = $periodiOfferte[$IdOfferta];
	
	$celleImporti = array();
	$celleImporti_C = array();
		
	for($mese = $periodoOfferta[MeseInizio], $anno=$periodoOfferta[AnnoInizio]; $anno<$periodoOfferta[AnnoFine] || ($mese<=$periodoOfferta[MeseFine] && $anno==$periodoOfferta[AnnoFine]);) 
	{

		if( 1 == 1 )
		{
			$subquery_select_1 = ", NuovoPrezzo, 
									Manodopera,
									PrezzoManodopera,
									QuotaNonRibassoMan,
									MaggiorazioneMan,
									ScontoConvenzMan
								 ";
			
			$subquery_select_2 = <<<sql
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
							dbo.CDate('1/$mese/$anno') between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
				) AS NuovoPrezzo,
				TipiComponentiMestieri.Manodopera,
				TipiComponentiMestieri.PrezzoManodopera,
				TipiComponentiMestieri.QuotaNonRibassoMan,
				TipiComponentiMestieri.MaggiorazioneMan,
				TipiComponentiMestieri.ScontoConvenzMan
sql;
		}
		
		if($conguaglio == 1){
			
			$annoConguaglio = $anno+1;
			
			$subquery_select_conguaglio_1 = ", NuovoPrezzo_C, 
									Manodopera_C,
									PrezzoManodopera_C,
									QuotaNonRibassoMan_C,
									MaggiorazioneMan_C,
									ScontoConvenzMan_C,InConguaglio
								 ";
								 
			$subquery_select_conguaglio_2 = <<<sql
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
							dbo.CDate('1/$mese/$annoConguaglio') between TipiComponetiMestAdeguamentoPrezzi.Inizio AND TipiComponetiMestAdeguamentoPrezzi.Fine
				) AS NuovoPrezzo_C,
				TipiComponentiMestieri.Manodopera AS Manodopera_C,
				TipiComponentiMestieri.PrezzoManodopera AS PrezzoManodopera_C,
				TipiComponentiMestieri.QuotaNonRibassoMan AS QuotaNonRibassoMan_C,
				TipiComponentiMestieri.MaggiorazioneMan AS MaggiorazioneMan_C,
				TipiComponentiMestieri.ScontoConvenzMan AS ScontoConvenzMan_C,
				TipiComponentiMestieri.InConguaglio
sql;
		}

		$sqlCanone = <<< sqlCanone
			SELECT 
				CodiceTCM, 
				FatturazioneScontoMedio,
				DurataContratto,
				dataDiffMesi,
				DataAvvioServizio,
				
				IdOfferta, IdEdificio, IdMestiere,

				YEAR(DataInizio) AnnoInizio,

				MONTH(DataInizio) MeseInizio,
				
				YEAR(DataFine) AnnoFine,
				MONTH(DataFine) MeseFine,

				LEFT(convert(nvarchar, DataInizio, 112), 6) AnnoMeseInizio,

				LEFT(convert(nvarchar, DataFine, 112), 6) AnnoMeseFine,

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
				
				$subquery_select_1
				$subquery_select_conguaglio_1
			FROM (
					SELECT  DISTINCT
						TipiComponentiMestieri.Codice AS CodiceTCM,
						Offerte.FatturazioneScontoMedio,
						Offerte.DurataContratto,
					
						DATEDIFF( 
									month , 
									dbo.cdate( 
												'01'
												+'/'
												+ CAST( MONTH(Offerte.DataAvvioServizio) as nvarchar(10) )
												+'/'
												+ CAST( YEAR(Offerte.DataAvvioServizio) as nvarchar(10) )
											) 
											, 
											dbo.CDate('1/$mese/$anno')
								) AS dataDiffMesi,
					
					
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
						TipiComponentiMestieri.UnaTantum, 

					--	case 
					--		when SopralluogoPDI.IdRevConsistenza is null then OfferteEdificiMestieri.DataInizio 
					--		else 
								SopralluogoPDI.DataInizio,
					--		end DataInizio, 
								SopralluogoPDI.DataFine
					--			,
					--	case 
					--		when SopralluogoPDI.IdRevConsistenza is null then 	

					--		dateadd(month,OfferteEdificiMestieri.Durata-1,OfferteEdificiMestieri.DataInizio)
					--		else dateadd(month,SopralluogoPDI.Durata-1,SopralluogoPDI.DataInizio)
					--		end DataFine,
		
							, ISNULL(SopralluogoPDI.FreqQ,1) AS FreqQ
							$subquery_select_2
							$subquery_select_conguaglio_2
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
							( ISNUMERIC(TipiComponentiMestieri.UM) = 1 )
					AND
							(tblMestieri.EscludiRM = 0 OR tblMestieri.EscludiRM is null )
			 ) a
sqlCanone;
//jhp_log($sqlCanone);
		$query = ntxQuery($sqlCanone);

		$IdMestiere_old = 0;

		while( $rs = ntxRecord($query) ) {
			$CODI_SERVIZIO  = $rs[CodiceTCM];
			$FatturazioneScontoMedio  = $rs[FatturazioneScontoMedio]==1 ? 1 : 0;
			
			$dayAtt = ntxDay($rs[DataAvvioServizio]);
			$monthAtt = ntxMonth($rs[DataAvvioServizio]);
			$yearAtt = ntxYear($rs[DataAvvioServizio]);	 	
			
			$IdOfferta = $rs[IdOfferta];
			$IdEdificio = $rs[IdEdificio];
			$IdMestiere = $rs[IdMestiere];
		
			$IdTipoComponenteMestiere = $rs[IdTipoComponenteMestiere];
			$InConguaglio = $rs[InConguaglio];
			
			$AnnoInizio = $rs[AnnoInizio];
			$MeseInizio = $rs[MeseInizio];
			
			$AnnoFine = $rs[AnnoFine];
			$MeseFine = $rs[MeseFine];
			
			$AnnoMeseInizio = $rs[AnnoMeseInizio];
			$AnnoMeseFine = $rs[AnnoMeseFine];
			
			if ( !is_numeric($AnnoMeseInizio) ||  !is_numeric($AnnoMeseFine) ) continue;	
			
			//jhp_log( "InConguaglio: $InConguaglio" );
			/***** Miraccomando usare $rs *****/
			if( $conguaglio == 1 && $InConguaglio == 1 ){
				//jhp_log( " $conguaglio == 1 && $InConguaglio == 1 " );
				$mc = $rs;
				$mc[mese] = $mese;
				$mc[anno] = $annoConguaglio;
				
				if( strlen($mc[NuovoPrezzo]) > 0 ){
					$mc[NuovoPrezzo] = $rs[NuovoPrezzo_C] - $rs[NuovoPrezzo];
				}else{
					$mc[NuovoPrezzo] = $rs[NuovoPrezzo_C] - $rs[Fattore];
				}
				
				$mc[Manodopera] = $rs[Manodopera_C];
				$mc[PrezzoManodopera] = $rs[QuotaNonRibassoMan_C];
				$mc[QuotaNonRibassoMan] = $rs[QuotaNonRibassoMan_C];
				$mc[MaggiorazioneMan] = $rs[MaggiorazioneMan_C];
				$mc[ScontoConvenzMan] = $rs[ScontoConvenzMan_C];
				//jhp_log( $mc );
				$anno += 1;
				$key = "K_{$IdOfferta}_{$IdEdificio}_{$IdMestiere}_{$mese}_{$anno}";
				$celleImporti[$key] = 0;
				
				require("calcolo_voci.inc");
				//jhp_log( $celleImporti );
				$anno -= 1;
			}
			
			$mc = $rs;
			$mc[mese] = $mese;
			$mc[anno] = $anno;
			require("calcolo_voci.inc");
		}
		
		//jhp_log( $celleImporti );

		$mese++;
		
		if ( $mese == 13 ) 
		{
			$mese = 1;
			$anno++;
		}
	}
?>