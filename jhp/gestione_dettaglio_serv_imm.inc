<?php
	$arrayWord["lista_ds"] = array();
	$lista_ds = array();
	
	$datiCostiEdificio = array();
	
	foreach($arrayWord["dati"] as $key => $value){
		$IdEdificio = $key;
		$DescEdificio = $arrayWord["dati"][$IdEdificio]["DescEdificio"];
		$GruppoMestieri = $arrayWord["dati"][$IdEdificio]["GruppoMestieri"];
		$CodiceEdificio = $arrayWord["dati"][$IdEdificio]["CodiceEdificio"];
	
		$lista_ds["Cod_edificio"] = $CodiceEdificio;
		$lista_ds["Nome_edificio"] = $DescEdificio;
        $totEdificio = 0;
		
		//print_r($GruppoMestieri);
		$datiCostiEdificio[$IdEdificio] = array();
		foreach($GruppoMestieri as $keyGM => $valueGM){
		
			$DescGruppoMes = $GruppoMestieri[$keyGM]["DescGruppoMes"];
			$IdGruppoMes = $GruppoMestieri[$keyGM]["IdGruppoMes"];
			$lista_mestieriGM = $GruppoMestieri[$keyGM]["lista_mestieri"];
			
			foreach($lista_mestieriGM as $keyLM => $valueLM){
				
				$IdMestiere = $lista_mestieriGM[$keyLM]["IdMestiere"];
				$DescMestiere = $lista_mestieriGM[$keyLM]["DescMestiere"];
				
				$annoC = $AnnoInizioColonna;
				for($i = 0; $i < $numAnni; $i++){
					$annoC += $i;
					
					$totMese = $lista_mestieriGM[$keyLM][$annoC]["totMese"];
					$totAnno = $lista_mestieriGM[$keyLM][$annoC]["totAnno"];
					
					$datiCostiEdificio[$IdEdificio][$annoC] += $totMese;
				}
				
				//$totMeseMes = $lista_mestieriGM[$keyLM]["totMese"];
				//$totAnnoMes = $lista_mestieriGM[$keyLM]["totAnno"];
				
				$somma_totMeseMes += $totMeseMes;
				$somma_totAnnoMes += $totAnnoMes;
/*				
				$lista_tipi_comp_mest = $lista_mestieriGM[$keyLM]["lista_tipi_componenti"];
				
                foreach($lista_tipi_comp_mest as $keyTCM => $valueTCM){
                    $IdTipoComponenteMestiere = $lista_tipi_comp_mest[$keyTCM]["IdTipoComponenteMestiere"];
                    $totAnnoTCM = $lista_tipi_comp_mest[$keyTCM]["totAnno"];

                    $totAnno += $totAnnoTCM;
                }
*/
			}
		}

		$annoC = $AnnoInizioColonna;
        for($annoCiclato = 0; $annoCiclato < $numAnni; $annoCiclato++ ){
			$annoC += $annoCiclato;
			$somma_totMeseMes = $datiCostiEdificio[$IdEdificio][$annoC];
            $lista_ds["servizio_anno_" . ($annoCiclato+1)] += $somma_totMeseMes;
            //$lista_ds["servizio_anno_" . ($annoCiclato+1)] += $totAnno;
        }

        for($ciclo = 0; $ciclo < $numAnni; $ciclo++ ){
            $totEdificio += $lista_ds["servizio_anno_" . ($ciclo+1)];
            $arrayWord["servizio_tot_anno_" . ($ciclo+1)] += $lista_ds["servizio_anno_" . ($ciclo+1)]; 

            $lista_ds["servizio_anno_" . ($ciclo+1)] = ntxRoundThousand( $lista_ds["servizio_anno_" . ($ciclo+1)], 2);
        }
		
        $totEdificio = ntxRoundThousand( $totEdificio, 2);
        $lista_ds["tot"] = $totEdificio;

		array_push($arrayWord["lista_ds"], $lista_ds);
		$lista_ds = array();
	}

    for($ciclo = 0; $ciclo < $numAnni; $ciclo++ ){
        $tot_servizi += $arrayWord["servizio_tot_anno_" . ($ciclo+1)];
        $arrayWord["servizio_tot_anno_" . ($ciclo+1)] = ntxRoundThousand( $arrayWord["servizio_tot_anno_" . ($ciclo+1)], 2);
    }

    $tot_servizi = ntxRoundThousand( $tot_servizi , 2);
    $arrayWord["tot_servizi"] = $tot_servizi;
?>