<?php
	$IdProprietario = "";
	$IdEdificio = "";

	$table = "";

	$alternate = false;

//	$conguaglio = ntxip('conguaglio');
//jhp_log("filtra-table-edifici.inc - conguaglio: $conguaglio");	

	$table .= "<table class=grid>";
	
	$array = explode("-", $_POST[MeseAnnoInizio]);
	$AnnoInizio = (int)$array[0];
	$MeseInizio = (int)$array[1];  

	$array = explode("-", $_POST[MeseAnnoFine]);
	$AnnoFine = (int)$array[0];
	$MeseFine = (int)$array[1]; 
	
	
	
	$ciclo = 0;
	$arrayMesiAnni = array();
	while ( ($AnnoFine-$AnnoInizio)+1 != $ciclo ){
		
		$AnnoCiclato = $AnnoInizio + $ciclo;
		
		for($meseCiclato = 1; $meseCiclato <= 12; $meseCiclato++){
			
			if( 
				! (($AnnoCiclato == $AnnoInizio && $MeseInizio > $meseCiclato)	
				||
				($AnnoCiclato == $AnnoFine && $meseCiclato > $MeseFine) )
			
			){
				array_push($arrayMesiAnni,$AnnoCiclato . "-" . $meseCiclato);

			}
			
			if( $meseCiclato == 12){ 
				$ciclo++; 
			}

		}
		
		
		//jhp_log($arrayMesiAnni);
		
	}
	
	
	$table .= "<tr>";
	$table .= "<th colspan=2><input type='button' value='Assegna Revisioni a Listino' onclick='RevisioneListino()' >";
	$table .= "<th>";
	
	//$table .= "<th>";
	
	$mesi = array('',"Gennaio","Febbraio","Marzo","Aprile","Maggio","Giugno","Luglio","Agosto","Settembre","Ottobre","Novembre","Dicembre");
	
	for($mese=$MeseInizio, $anno=$AnnoInizio; $anno<$AnnoFine || ($mese<=$MeseFine && $anno==$AnnoFine);){
		
		$table .= "<th>$mesi[$mese]<br>$anno";
		
		$mese++;
			
		if ( $mese==13 )
		{
			$mese = 1;
			$anno++;
		}

	}
	
	
	while($rs=ntxRecord($query))
	{
		foreach($rs as $k=>$v)
			$html[$k] = htmlentities($v);
		
		if ( $IdProprietario=="" || $IdProprietario<>$rs[IdPropietario] ) {
			
			$class = $alternate ? 'class=alternate' : '';
		
			$alternate = !$alternate;
			
			$table .= "<tr $class>";
			
			$IdProprietario = $rs[IdPropietario];
			
			/*$mesiNumeri = array('',"01","02","03","04","05","06","07","08","09","10","11","12");
			
			for($mese=$MeseInizio, $anno=$AnnoInizio; $anno<$AnnoFine || ($mese<=$MeseFine && $anno==$AnnoFine);){
			  $id = $rs[IdPropietario]."_".$mesiNumeri[$mese]."_".$anno;
			}*/
			
			
			$table .= "<td style='text-align:center'><input type='checkbox' selRiga id='$IdProprietario' onclick=SelezionaProprietario($IdProprietario) />";
			$table .= "<td>$html[Codice] - $html[Nome]";
			$table .= "<td><button type=button  title=Blocca id=Blocca
			class='ui-button ui-widget ui-state-default ui-corner-all'
			onclick='Blocca($IdProprietario)'>
			<img src='img/padlock-vector-icon-png_276855.jpg' style='width:25px; height:25px' >

			</button>
			";
			
			//<img src='img/padlock-vector-icon-png_276855.jpg' style='width:25px; height:25px'
			
			$mesiNumeri = array('',"01","02","03","04","05","06","07","08","09","10","11","12");
			
			for($mese=$MeseInizio, $anno=$AnnoInizio; $anno<$AnnoFine || ($mese<=$MeseFine && $anno==$AnnoFine);){
				
				//jhp_log($mese.$anno);

				$id = $rs[IdPropietario]."_".$mesiNumeri[$mese]."_".$anno;
				
				$IdCommessa = ntxise('IdCommessa');
				
				$sqlPercIstat = <<<sql
				SELECT PercIstat
				FROM   ApplicazioniPercIstat
				WHERE  Mese = $mesiNumeri[$mese]
				AND	   Anno = $anno
				AND    IdProprietario=$IdProprietario 
				AND    IdCommessa = $IdCommessa	
sql;
				/*if($IdProprietario == 30){
				//jhp_log($sqlPercIstat);
				}*/
				$RisPercIstat = ntxScalar($sqlPercIstat);
			
				//jhp_log("PercIstat: ".$RisPercIstat);
				//jhp_log($mesiNumeri[$mese]);
				
				$value = "";
				if($RisPercIstat != ""){
					
					$value = NtxRound($RisPercIstat, 2);
				}
				
				//jhp_log($value."value");
				
				$sqlBloccato = <<<sql
				SELECT Bloccato
				FROM   ApplicazioniPercIstat
				WHERE  Mese = $mesiNumeri[$mese]
				AND	   Anno = $anno
				AND    IdProprietario=$IdProprietario 
				AND    IdCommessa = $IdCommessa	
sql;
				//jhp_log($sqlBloccato);
				$RisBloccato = ntxScalar($sqlBloccato);
				
				//jhp_log($RisBloccato."sono qui");
				
				$disabled = "";
				if($RisBloccato == 1){
					
					$disabled = "disabled";
				}
				
				
				$table .= "<td style='text-align:center'><input type='text' size='3' 
				Mese=$mesiNumeri[$mese]
				Anno=$anno
				IdProprietario=$IdProprietario 
				
				id='$id'
				value='$value' 
				onchange='SelezionaCella(this)' $disabled/> ";
				
				//<!--value='5'-->
				
				
				//id='$id' 
				//PercIstat=$PercIstat
				
				//value='$PercIstat'
				//<!--onclick='SelezionaCella($rs[IdPropietario], $mesiNumeri[$mese], $anno)'-->
				//onchange='SelezionaCella($rs[IdPropietario], $mesiNumeri[$mese], $anno)'
				
				$mese++;
			
				if ( $mese==13 )
				{
					$mese = 1;
					$anno++;
				}

			}
			
		}
		
	}
	
	$table .= "</table>";
?>