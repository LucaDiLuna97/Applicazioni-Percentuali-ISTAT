<?php
	//$arrayWord["dati"] = array();
	//print_r($arrayWord["dati"]);
//	$arrayWord["lista_ds"] = array();
//	$lista_ds = array();
	$arrayWord["riepilogo_complessivo"] = array();
	$riepilogo_complessivo = array();
	
	$datiTabella1 = array();
	
	$ArrdataInizio = explode("-",$_POST[MeseAnnoInizio]);
	//print_r($ArrdataInizio);
	
	$MeseInizioColonna = $ArrdataInizio[1];
	$AnnoInizioColonna = $ArrdataInizio[0];
	
	foreach($arrayWord["dati"] as $key => $value){
		
		//print_r($arrayWord["dati"]);
		$IdEdificio = $key;
		$DescEdificio = $arrayWord["dati"][$IdEdificio]["DescEdificio"];
		$GruppoMestieri = $arrayWord["dati"][$IdEdificio]["GruppoMestieri"];
		$CodiceEdificio = $arrayWord["dati"][$IdEdificio]["CodiceEdificio"];
	
//		$lista_ds["Cod_edificio"] = $CodiceEdificio;
//		$lista_ds["Nome_edificio"] = $DescEdificio;
		
		//print_r($GruppoMestieri);
		foreach($GruppoMestieri as $keyGM => $valueGM){
			//print_r("<br><br><br>");
			$DescGruppoMes = $GruppoMestieri[$keyGM]["DescGruppoMes"];
			//print_r($DescGruppoMes);
			//print_r("<br><br>");
			$IdGruppoMes = $GruppoMestieri[$keyGM]["IdGruppoMes"];
			
			$lista_mestieriMes = $GruppoMestieri[$keyGM]["lista_mestieri"];
//			print_r($lista_mestieriMes);
//			print_r("<br><br>");
			$riepilogo_complessivo["lista_mestieri"] = array();
			$lista_mestieri = array();
			foreach($lista_mestieriMes as $keyLM => $valueLM){
				
				$IdMestiere = $lista_mestieriMes[$keyLM]["IdMestiere"];
				$DescMestiere = $lista_mestieriMes[$keyLM]["DescMestiere"];
				
				$datiTabella1[$IdGruppoMes][$IdMestiere] = array();
				$annoC = $AnnoInizioColonna;
				for($i = 0; $i < $numAnni; $i++){
					$annoC += $i;
//					print_r( $annoC );
//					print_r("<br>");
				//foreach($lista_mestieriMes as $keyLM => $valueLM){
					$totMese = $lista_mestieriMes[$keyLM][$annoC]["totMese"];
					$totAnno = $lista_mestieriMes[$keyLM][$annoC]["totAnno"];
					
//					print_r( $totMese );
//					print_r("<br>");
//					print_r( $totAnno );
//					print_r("<br><br>");
					
					$datiTabella1[$IdGruppoMes][$IdMestiere][$annoC] += $totMese;
				}
/*				
				$lista_tipi_comp_mest = $lista_mestieriMes[$keyLM]["lista_tipi_componenti"];

				$arrayMestiere = array();
				foreach($lista_tipi_comp_mest as $keyTCM => $valueTCM ){
					$IdTipoComponenteMestiere = $lista_tipi_componenti["IdTipoComponenteMestiere"];
					$totAnno = $lista_tipi_componenti["totAnno"];
					//print_r($totAnno . "<br><br><br>");
					//creazione array con IdGruppoMestiere, Mestiere e Valore
					//$arrayMestiere["IdGruppoMestiere"] = $IdMestiere;
					//$arrayMestiere["DescMestiere"] = $DescMestiere;
					//$arrayMestiere["Valore"] = $totAnno;
					
					$datiTabella1[$IdGruppoMes][$IdMestiere] += $totAnno;	
				}
*/				
				//$lista_mestieri["descMestiere"] = $DescMestiere;
				
//				array_push($riepilogo_complessivo["lista_mestieri"], $lista_mestieri);
//				$lista_mestieri = array();
				
			}
			
			//$riepilogo_complessivo["GruppoMestieri"] = $DescGruppoMes;
			//print_r($DescGruppoMes);
			//print_r("<br><br>    ".$IdGruppoMes);
			
//			$arrayWord["riepilogo_complessivo"] = $riepilogo_complessivo;
			
//			if($arrayWord["riepilogo_complessivo"]["GruppoMestieri"] != $DescGruppoMes){
				//array_push($arrayWord["riepilogo_complessivo"], $riepilogo_complessivo);
				//$riepilogo_complessivo = array();	
//			}
			
			
			
		}
		//$IdEdificio = $arrayWord["dati"][$i];
		//print_r($key."<br><br>");
		
//		array_push($arrayWord["lista_ds"], $lista_ds);
//		$lista_ds = array();
	}
	
	//print_r($arrayWord["lista_ds"]);
	//print_r("<br><br><br>");
	//print_r($arrayWord["riepilogo_complessivo"]);
	
	//for($i = 0; $i < count($datiTabella1); $i++){
//	print_r("datiTabella1");
//	print_r("<br><br><br>");
//	print_r($datiTabella1);
//	print_r("<br><br><br>");
	
	
	foreach($datiTabella1 as $key => $value){
		$IdGruppoMes = $key;
		$sql = <<<sql
			select Descrizione 
			from GruppoMestieri
			where IdGruppoRendicontoMes = $IdGruppoMes
sql;

		$GruppoMestieri = ntxScalar($sql);
		
		$riepilogo_complessivo["GruppoMestieri"] = ntxAnsi2Utf8($GruppoMestieri);
		$riepilogo_complessivo["lista_mestieri"] = array();
		
		$listaMestieriGruppo = $datiTabella1[$IdGruppoMes];
//		print_r( $listaMestieriGruppo);
//		print_r( "<br><br><br>" );
		foreach($listaMestieriGruppo as $key2 => $value2){
			
			//print_r($IdGruppoMes." - ".$key2."<br><br><br>");
			//print_r($value2);
			//$totAnno = $value2;
			//print_r("<br><br>");
			
			$sqlDescMestiere = <<<sql
				SELECT Descrizione
				FROM tblMestieri
				WHERE IdMestiere = $key2
			
sql;
			
			$descMestiere = ntxScalar($sqlDescMestiere);
			
			$lista_mestieri["descMestiere"] = ntxAnsi2Utf8($descMestiere);
			$annoC = $AnnoInizioColonna;
			for($annoCiclato = 0; $annoCiclato < $numAnni; $annoCiclato++ ){
				$annoC += $annoCiclato;
				//print_r( "value2[annoC][totMese]" );
				//print_r("<br>");
				//print_r( $annoC );
				//print_r("<br>");
				//print_r( $value2[$annoC] );
				//print_r("<br>");
				//print_r( $value2[$annoC]["totMese"] );
				//print_r("<br><br><br>");
				$totMese = $value2[$annoC];
				$lista_mestieri["desc_mestiere_anno_" . ($annoCiclato+1)] += $totMese;
				$lista_mestieri["tot_desc_mestiere"] += $totMese;
				
				
				$riepilogo_complessivo["tot_gruppi_mest_anno_" . ($annoCiclato+1)] += $totMese;
				$riepilogo_complessivo["tot_gruppi_mest"] += $totMese;
				
				$arrayWord["tot_anno_".($annoCiclato+1)] += $totMese;
			}
			
			for($i = 0; $i < $numAnni; $i++ ){
			
			$lista_mestieri["desc_mestiere_anno_" . ($i+1)] = ntxRoundThousand($lista_mestieri["desc_mestiere_anno_" . ($i+1)], 2);
			}
			
			$lista_mestieri["tot_desc_mestiere"] = ntxRoundThousand($lista_mestieri["tot_desc_mestiere"], 2);
			
			array_push($riepilogo_complessivo["lista_mestieri"], $lista_mestieri);
			$totMese = 0;
			$lista_mestieri = array();
		}

		
		for($i = 0; $i < $numAnni; $i++ ){
			$riepilogo_complessivo["tot_gruppi_mest_anno_" . ($i+1)] = ntxRoundThousand($riepilogo_complessivo["tot_gruppi_mest_anno_" . ($i+1)], 2);
		}
		//print_r($IdGruppoMes."<br><br><br>");
		//print_r($listaMestieri);
		
		$riepilogo_complessivo["tot_gruppi_mest"] = ntxRoundThousand($riepilogo_complessivo["tot_gruppi_mest"],2);
		array_push($arrayWord["riepilogo_complessivo"], $riepilogo_complessivo);
		$riepilogo_complessivo = array();
	}

//	print_r($arrayWord["riepilogo_complessivo"]);

	for($ciclo = 0; $ciclo < $numAnni; $ciclo++ ){
        $tot += $arrayWord["tot_anno_" . ($ciclo+1)];
        $arrayWord["tot_anno_" . ($ciclo+1)] = ntxRoundThousand( $arrayWord["tot_anno_" . ($ciclo+1)], 2);
	}

    $tot = ntxRoundThousand( $tot , 2);
    $arrayWord["tot"] = $tot;
?>