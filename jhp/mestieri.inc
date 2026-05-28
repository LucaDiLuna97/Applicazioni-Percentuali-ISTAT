<?php
	$sql_Mestieri = <<<sql
		SELECT 
			tblMestieri.Descrizione, 
			tblMestieri.IdMestiere
		FROM 
			tblMestieri
			INNER JOIN 
				OfferteEdificiMestieri
					on 
							OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa
						and 
							OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere
		WHERE 
				OfferteEdificiMestieri.IdCommessaEdificio = $IdCommessa
			AND 
				OfferteEdificiMestieri.IdEdificio = $IdEdificio
			AND 
				tblMestieri.IdGruppoMes = $IdGruppoMes 
			AND
				OfferteEdificiMestieri.IdOfferta = $IdOfferta
sql;
	//print_r($sql_Mestieri);
	$query_Mestieri = ntxQuery($sql_Mestieri);
	
	$lista_mestieri = array();
	while($rs_Mestieri = ntxRecord($query_Mestieri)){
		
		$IdMestiere = $rs_Mestieri[IdMestiere];
//		print_r($IdMestiere);
//		print_r("<br><br>");
		
		$lista_mestieri["IdMestiere"] = $IdMestiere;
		$lista_mestieri["DescMestiere"] = $rs_Mestieri[Descrizione];
		//$lista_mestieri["totMese"] = 0;
		//$lista_mestieri["totAnno"] = 0;
		$lista_mestieri["lista_tipi_componenti"] = array();
		
		$lista_id = $IdProprietario;
		//print_r($lista_id);
		//require("riepilogo_compl_get_perc_istat.inc");
		//print_r($ArrPercIstat[$IdProprietario][$Anno."-".$Mese]);
		
		//for($i = 0; $i < count($ArrPercIstat); $i++){
		for($i = 0; $i < count( $arrayMesiAnni ); $i++){
			//contruire il $dataCiclo dato
			//$ArrPercIstat[$IdProprietario][$Anno."-".$Mese]
//			print_r("<br>");
//			print_r( $arrayMesiAnni[$i] );
			$arr_mese_anno = explode("-", $arrayMesiAnni[$i]);
			$mese = $arr_mese_anno[1];
			$anno = $arr_mese_anno[0];
			/*
			foreach($ArrPercIstat[$IdProprietario] as $key => $value){
			//foreach($ArrPercIstat[$IdProprietario] as $key => $value){
				$Key = explode("-", $key);
				$anno = $Key[0];
				$mese = $Key[1];
				$Valore = $value;
			*/	
				$dataCiclo = "'01/".$mese."/".$anno."'";
//				print_r("<br>");
				require("tipi_comp_mestieri.inc");
			/*}*/
		}
		
		//array_push($lista_mestieri["lista_tipi_componenti"], $lista_tipi_componenti);
		//$lista_tipi_componenti = array();
		
//		print_r("<br><br><br>");
//		print_r( $lista_mestieri );
//		print_r("<br><br><br>");
		array_push($GruppiMestieri["lista_mestieri"], $lista_mestieri);
	}
?>