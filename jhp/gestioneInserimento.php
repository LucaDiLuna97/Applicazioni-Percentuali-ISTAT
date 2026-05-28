<?php
	if ( !$_SESSION ) session_start();

	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");

	require_once(NTX_WEBPORTAL_PATH.'_database.inc'); 

	$IdProprietario = ntxip('IdProprietario');
	$IdCommessa = ntxise('IdCommessa');
	$Mese = ntxip('Mese');
	$Anno = ntxip('Anno');
	$PercIstat = ntxfp('PercIstat');
	
	if($PercIstat == ""){
	} else {
		$PercIstat = Round($PercIstat, 2);
	}
	
	$gestioneInserimentoSQL = <<<sql
		SELECT     
			count(Id)
		FROM         
			ApplicazioniPercIstat
		where 
				IdCommessa = $IdCommessa
			and
				IdProprietario = $IdProprietario
			and 
				Mese = $Mese
			and 
				Anno = $Anno
			
sql;
	$gestioneInserimento = ntxScalar($gestioneInserimentoSQL);
	
	if($gestioneInserimento > 0){
		$update = <<<update
			UPDATE    
				ApplicazioniPercIstat
			SET       
				PercIstat = $PercIstat
			WHERE	  
					IdProprietario = $IdProprietario
				AND 	  
					Mese = $Mese
				AND 	  
					Anno = $Anno
				AND 	  
					IdCommessa = $IdCommessa
update;
		ntxQuery($update);
	}
	else {
		$insert = <<<insert
		INSERT INTO 
			ApplicazioniPercIstat 
				(IdProprietario, IdCommessa,Mese, Anno, PercIstat, Bloccato)
			VALUES 
				($IdProprietario, $IdCommessa, $Mese, $Anno, $PercIstat, 1)
insert;
		ntxQuery($insert);
	}
	
	if( (int) $PercIstat == 0 ){
		
		$delete = <<<delete
			DELETE FROM 
				ApplicazioniPercIstat
			WHERE	  
					IdProprietario = $IdProprietario
				AND 	  
					Mese = $Mese
				AND 	  
					Anno = $Anno
				AND 	  
					IdCommessa = $IdCommessa
				AND 	  
					PercIstat = $PercIstat
delete;
		ntxQuery($delete);
		
		$DeleteTipicompMest = <<<sql
			DELETE FROM 
				TipiComponetiMestAdeguamentoPrezzi
			FROM 
				TipiComponetiMestAdeguamentoPrezzi
					INNER JOIN
						Offerte 
							ON 
									TipiComponetiMestAdeguamentoPrezzi.IdCommessaOfferta = Offerte.IdCommessa 
								AND 
									TipiComponetiMestAdeguamentoPrezzi.IdOfferta = Offerte.IdOfferta 
					INNER JOIN
						tblPgPropietari 
							ON 
									tblPgPropietari.IdPropietario = Offerte.IdPropietario
			WHERE 
					IdCommessaOfferta = $IdCommessa
				AND 
					IdCommessaTipoCompMest = $IdCommessa
				AND 
					tblPgPropietari.IdPropietario = $IdProprietario
				AND 
					month(Inizio) = $Mese
				AND 
					year(Inizio) = $Anno
sql;
		jhp_log($DeleteTipicompMest);
		ntxQuery($DeleteTipicompMest);
	}
	jhp_ok();
?>
