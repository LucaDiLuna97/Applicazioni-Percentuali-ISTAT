<?php
	if ( !$_SESSION ) session_start();

	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");
	require_once(NTX_WEBPORTAL_PATH.'_database.inc'); 
	
	$IdProprietario = ntxsp('IdProprietario');
	$StringaIdProprietario = substr($IdProprietario,1,-1);
	
	$IdCommessa = ntxise('IdCommessa');
	
	$arrayInizio = explode("-", $_POST[MeseAnnoInizio]);
	$AnnoInizio = (int)$arrayInizio[0];
	$MeseInizio = (int)$arrayInizio[1];  

	$arrayFine = explode("-", $_POST[MeseAnnoFine]);
	$AnnoFine = (int)$arrayFine[0];
	$MeseFine = (int)$arrayFine[1]; 
	
	$mesiNumeri = array('',"01","02","03","04","05","06","07","08","09","10","11","12");
	
	//jhp_log($_POST);
	//jhp_log($IdProprietario);
	//jhp_log($MeseInizio." - ".$AnnoInizio);
	//jhp_log($MeseFine." - ".$AnnoFine);
	
	$ciclo = 0;
	//$arrayMesiAnni = array();
	
	$arrayColonneDaSbloc = array();
	while ( ($AnnoFine-$AnnoInizio)+1 != $ciclo ){
		
		$annoCiclato = $AnnoInizio + $ciclo;
		//jhp_log("PHP: annoCiclato:    ".$annoCiclato);
		
		for($meseCiclato = 1; $meseCiclato <= 12; $meseCiclato++){
			
			if( 
				! (($annoCiclato == $AnnoInizio && $MeseInizio > $meseCiclato)	
				||
				($annoCiclato == $AnnoFine && $meseCiclato > $MeseFine) )
			
			){
				$sqlPercIstat = <<<sql
					SELECT PercIstat
					FROM   ApplicazioniPercIstat
					WHERE  Mese = $meseCiclato
					AND	   Anno = $annoCiclato
					AND    IdProprietario IN ($StringaIdProprietario)/*=$IdProprietario */
					AND    IdCommessa = $IdCommessa	
sql;
				//jhp_log($sqlPercIstat);
				$RisPercIstat = ntxScalar($sqlPercIstat);
				//jhp_log($RisPercIstat);
		
				if($RisPercIstat > 0){
			
					$update = <<<update
						UPDATE    ApplicazioniPercIstat
						SET       Bloccato = 0
						WHERE  Mese = $meseCiclato
						AND	   Anno = $annoCiclato
						AND    IdProprietario IN ($StringaIdProprietario) /*=$IdProprietario*/ 
						AND    IdCommessa = $IdCommessa	
update;

					//jhp_log($update);
					ntxQuery($update);
					
					if($meseCiclato < 10){
							
						$meseCiclato = "0".$meseCiclato;
					}
					
					array_push($arrayColonneDaSbloc, $StringaIdProprietario."_".$meseCiclato."_".$annoCiclato);
					
					//Ora devo bloccare per i prossimi 11 mesi
					//ciclo per 11 mesi
						//Esiste record per l'anno successivo?
							//Si -> faccio update
							//No -> faccio insert
					
					
					//jhp_log("meseCiclato: ".$meseCiclato);
					//jhp_log("annoCiclato: ".$annoCiclato);
					
					//partiamo da marzo e dobbiamo arrivare a febbraio dell'anno successivo
					//MeseCiclato = 3
					//AnnoCiclato = 2022
					
					//partiamo da Marzo
					$addMese = 0;  //variabile che va a sommarsi con il meseCiclato
					$nextAnno = false;
					for($i = 1; $i <= 11; $i++){
						//jhp_log($addMese);
						
						$addMese++;  //addMese aumenta di 1
					
						//jhp_log($addMese);
						
						//se siamo all'anno corrente
						if($nextAnno == false){
							$meseDaSbloccare = $meseCiclato+$addMese;
							$annoDaSbloccare = $annoCiclato;
							//meseDaBloccare diventa Aprile (3+1)
							//annoDaBloccare è 2022 (anno corrente)
							
						} else {
							
							//altrimenti se siamo all'anno nuovo
							$meseDaSbloccare = 1+$addMese; //$addMese;
							//meseDaBloccare corrisponde a addMese
							
							//jhp_log($meseDaBloccare." - ".$annoDaBloccare);
						}
						//jhp_log($meseCiclato." - ".$addMese);
						
						if($meseDaSbloccare > 12){
							
							//se meseDaBloccare è maggiore di 12
							
							$meseDaSbloccare = 1;  //meseDaBloccare è 1 quindi Gennaio
							$annoDaSbloccare++;    //annoDaBloccare aumenta di 1
													//quindi da 2022 diventa 2023
							$addMese = 0;		  //addMese diventa 0
							$nextAnno = true;     //nextAnno diventa true
						} 
						

						//jhp_log(/*"PHP: ".*/"Ciclata: ".$i." - AddMese: ".$addMese."  IdProprietario: ".$IdProprietario." - Mese: ".$meseDaBloccare." - Anno: ".$annoDaBloccare);
						
						//controllo record se presente su db
						$sqlAnnoSuccessivo = <<<sql
									SELECT count(*)
									FROM   ApplicazioniPercIstat
									WHERE  IdProprietario IN ($StringaIdProprietario) /*= $IdProprietario*/
									AND	   IdCommessa = $IdCommessa
									AND    Mese = $meseDaSbloccare
									AND	   Anno = $annoDaSbloccare
sql;
						//jhp_log($sqlAnnoSuccessivo);
						$checkControlloRecord = /*(int)*/ntxScalar($sqlAnnoSuccessivo);

						//jhp_log($checkControlloRecord);

						if($checkControlloRecord > 0 ){
							//jhp_log("faccio update");
							$updateRecord = <<<update
									UPDATE    ApplicazioniPercIstat
									SET       Bloccato = 0
									WHERE  IdProprietario IN ($StringaIdProprietario) /*= $IdProprietario*/
									AND	   IdCommessa = $IdCommessa
									AND    Mese = $meseDaSbloccare
									AND	   Anno = $annoDaSbloccare
update;
							
							ntxQuery($updateRecord);
							//jhp_log($updateRecord);
							
						} else {
							//jhp_log("faccio insert");
							$insertRecord = <<<insert
									INSERT INTO ApplicazioniPercIstat (IdProprietario, IdCommessa, PercIstat, Mese, Anno, Bloccato)
									VALUES ($StringaIdProprietario, $IdCommessa, NULL, $meseDaSbloccare, $annoDaSbloccare, 0)
insert;
							ntxQuery($insertRecord);
							//jhp_log($insertRecord);
						}
						
						if($meseDaSbloccare < 10){
							
							$meseDaSbloccare = "0".$meseDaSbloccare;
						}
						array_push($arrayColonneDaSbloc, $StringaIdProprietario."_".$meseDaSbloccare."_".$annoDaSbloccare);
					}	
							
				}

			}

			if( $meseCiclato == 12){ 
				$ciclo++; 
			}
			
			

		}
	}
	
	jhp(&$arrayColonneDaSbloc);

?>
