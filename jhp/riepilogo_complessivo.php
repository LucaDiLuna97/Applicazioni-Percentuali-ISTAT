<?php
	$IdCommessa = ntxise('IdCommessa');
	$ArrdataInizio = explode("-",$_POST[MeseAnnoInizio]);
	//print_r($ArrdataInizio);
	
	$MeseInizio = $ArrdataInizio[1];
	$AnnoInizio = $ArrdataInizio[0];
	$dataInizio = "01/".$MeseInizio."/".$AnnoInizio;
	//print_r($dataInizio);
	
	$ArrdataFine = explode("-",$_POST[MeseAnnoFine]);
	//print_r($ArrdataFine);
	
	$MeseFine = $ArrdataFine[1];
	$AnnoFine = $ArrdataFine[0];
	
	$GiornoFine = "31";
	if($MeseFine == "04" || $MeseFine == "06" || $MeseFine == "09" || $MeseFine == "11"){
		$GiornoFine = "30";
	} else if($MeseFine == "02"){
		$GiornoFine = "28";
	}
	
	$dataFine = $GiornoFine."/".$MeseFine."/".$AnnoFine;
	//print_r($dataFine);
	
	$DataInizio_ = /*ntxDate(*/$dataInizio/*)*/;
	$DataFine_ = $dataFine;
	$fineMese = "31/12";
	
	//Gestione dell'anno
	$sqlAnno = <<<sql
		SELECT DATEDIFF(year, '$DataInizio_', '$DataFine_')+1 AS DateDiff
sql;
	//print_r($sqlAnno);
	$numAnni = ntxScalar($sqlAnno);
	$arrayWord["numAnni".$numAnni] = true;
	
	//$tr = "";
	for($i = 0; $i < $numAnni; $i++){
		//$tr .= "<th>".($annoInizio+$i)."</th>"; 
		$arrayWord["dataavvioservizio_".($i+1)] = $AnnoInizio+$i;
		//print_r($arrayWord["dataavvioservizio"]);
	}
	
	$ciclo = 0;
	$arrayMesiAnni = array();
	while( ($AnnoFine - $AnnoInizio)+1 != $ciclo ){
		$AnnoCiclato = $AnnoInizio + $ciclo;
		
		for( $meseCiclato = 1; $meseCiclato <= 12; $meseCiclato++ ){
			if(
				! (
					( $AnnoCiclato == $AnnoInizio && $MeseInizio > $meseCiclato )
					||
					( $AnnoCiclato == $AnnoFine && $meseCiclato > $MeseFine ) 
				)
				
			){
				array_push( $arrayMesiAnni, $AnnoCiclato . "-" . $meseCiclato );
			}
			
			if( $meseCiclato == 12 ){
				$ciclo++;
			}
			
		}
	}
	
	
//INIZIO TABELLA


//print_r($IdOfferta);


//PER ADESSO LASCIA COMMENTATO
/*
$sqlGruppoMes = <<<sql
		SELECT DISTINCT GruppoMestieri.Descrizione AS GruppoMes,
						GruppoMestieri.IdGruppoRendicontoMes
						
		FROM GruppoMestieri 
		INNER JOIN tblMestieri 
		ON GruppoMestieri.IdCommessa =  tblMestieri.IdCommessa 
		AND GruppoMestieri.IdGruppoRendicontoMes = tblMestieri.IdGruppoMes
		INNER JOIN ElencoEdifici
		ON ElencoEdifici.IdCommessa = GruppoMestieri.IdCommessa
		AND ElencoEdifici.IdCommessa = tblMestieri.IdCommessa
		inner join tblPgPropietari
		on tblPgPropietari.IdPropietario = ElencoEdifici.IdProprietario
		inner join Offerte
		on Offerte.IdPropietario = tblPgPropietari.IdPropietario
		and Offerte.IdPropietario = ElencoEdifici.IdProprietario
		WHERE GruppoMestieri.IdCommessa = $IdCommessa
		AND tblPgPropietari.IdPropietario = $IdProprietario
		AND Offerte.IdOfferta = $IdOfferta
		order by GruppoMestieri.IdGruppoRendicontoMes
sql;
//print_r($sqlGruppoMes."    ------      ");

$queryGruppoMes = ntxQuery($sqlGruppoMes);
//print_r($QueryGruppoMes);

while($rsGruppoMes = ntxRecord($queryGruppoMes)){

	$IdGruppoMes = $rsGruppoMes[IdGruppoRendicontoMes];
	$GruppoMestieri = $rsGruppoMes[GruppoMes];
	
	$sql_get_lista_mes = <<<sql
		SELECT * 
		FROM tblMestieri
		WHERE IdCommessa = $IdCommessa
		AND IdGruppoMes = $IdGruppoMes
		ORDER BY Descrizione
sql;
	
	$query_get_lista_mes = ntxQuery($sql_get_lista_mes);
	
	$lista_mestieri = array();
	$riepilogo_complessivo["lista_mestieri"] = array();
	while($rs_get_lista_mes = ntxRecord($query_get_lista_mes)){
		
		$IdMestiere = $rs_get_lista_mes[IdMestiere];
		$DescrizioneMes = $rs_get_lista_mes[Descrizione];
		
		$lista_mestieri["descMestiere"] = $DescrizioneMes;
		
		array_push($riepilogo_complessivo["lista_mestieri"], $lista_mestieri);
		$lista_mestieri = array();
	}
	
	$riepilogo_complessivo["GruppoMestieri"] = $GruppoMestieri;

	array_push($arrayWord["riepilogo_complessivo"], $riepilogo_complessivo);
}

//print_r($arrayWord["riepilogo_complessivo"]);
*/


//

//query del riepilogo complessivo
$sql_riepilogo_complessivo = <<<sql
	SELECT 
		elencoedifici.ID,
		elencoedifici.EdificioBreve,
		elencoedifici.Codice	
		FROM ElencoEdifici
		inner join tblPgPropietari
			on tblPgPropietari.IdPropietario = ElencoEdifici.IdProprietario
		inner join OfferteEdifici
			on  OfferteEdifici.IdEdificio = ElencoEdifici.ID
			and OfferteEdifici.IdCommessaEdificio = ElencoEdifici.IDCommessa
		where OfferteEdifici.IdCommessaEdificio = $IdCommessa
		and tblPgPropietari.IdPropietario = $IdProprietario
		and OfferteEdifici.IdOfferta = $IdOfferta
		order by Codice
sql;
//print_r($sql_riepilogo_complessivo);

$query_riepilogo_complessivo = ntxQuery($sql_riepilogo_complessivo);

//$lista_mestieri = array();
//$riepilogo_complessivo["riepilogo_complessivo"] = array();
//$GruppoMestieri["GruppoMestieri"] = array();
//$DescMestiere["DescMestiere"] = array();
$lista_edif_prop = array();
while($rs_riepilogo_complessivo = ntxRecord($query_riepilogo_complessivo)){
	$IdEdificio = $rs_riepilogo_complessivo[ID];
	$DescEdificio = $rs_riepilogo_complessivo[EdificioBreve];
	$CodiceEdificio = $rs_riepilogo_complessivo[Codice];
	//otteniamo l'idedificio per metterlo come chiave nel nostro array principale
	//$riepilogo_complessivo[$IdEdificio] = array(); 

	$arrayWord["dati"][$IdEdificio]["IdEdificio"] = $IdEdificio;
	//$lista_edif_prop[$IdEdificio]["IdEdificio"] = $IdEdificio;
	$arrayWord["dati"][$IdEdificio]["DescEdificio"] = $DescEdificio;
	$arrayWord["dati"][$IdEdificio]["CodiceEdificio"] = $CodiceEdificio;
	$arrayWord["dati"][$IdEdificio]["GruppoMestieri"] = array();
	
	require("gruppi_mestieri.inc");
	//$lista_edif_prop[$IdEdificio]["DescEdificio"] = $DescEdificio;
	
	//print_r($riepilogo_complessivo);
	
	//DA RIVEDERE
	/*$riepilogo_complessivo["GruppoMestieri"] = $GruppiMestieri["DescGruppoMes"];
	
	//print_r($lista_mestieri["DescMestiere"]);
	$riepilogo_complessivo["descMestiere"] = $lista_mestieri["DescMestiere"];
	
	for($i = 0; $i < $numAnni; $i++){
			
		$riepilogo_complessivo["tot_gruppi_mest_anno_".($i+1)] = $lista_tipi_componenti["totAnno"];
	}*/
	
	
	//array_push($arrayWord["dati"], $lista_edif_prop);
	//$lista_edif_prop = array();
	
}
//print_r( $arrayWord["dati"] );
require("gestione_riepilogo_complessivo.inc");
require("gestione_dettaglio_serv_imm.inc");
//print_r($riepilogo_complessivo);
?>