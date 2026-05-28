<?php
	if ( !$_SESSION ) session_start();

	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");

	require_once(NTX_WEBPORTAL_PATH.'_database.inc'); 

	$IdUtente = ntxsse('IdUtente');

	$IdCommessa = ntxise('IdCommessa');

	//$IdProprietario = ntxip('IdProprietario');
	//$IdOfferta = ntxip ('IdOfferta');
	
	$MeseInizio = ntxip('MeseInizio');
	$AnnoInizio = ntxip('AnnoInizio');
	$MeseFine = ntxip('MeseFine');
	$AnnoFine = ntxip('AnnoFine');
	
	$result= array ();

	if ( is_numeric($MeseInizio) && ($MeseInizio<1 || $MeseInizio>12 ) )
		jhp_error("Mese inizio errato!");

	if ( is_numeric($MeseFine) && ($MeseFine<1 || $MeseFine>12 ) )
		jhp_error("Mese fine errato!");

	if ( is_numeric($AnnoInizio) && ($AnnoInizio<1980 || $AnnoInizio>2200 ) )
		jhp_error("Anno inizio errato!");

	if ( is_numeric($AnnoFine) && ($AnnoFine<1980 || $AnnoFine>2200 ) )
		jhp_error("Anno fine errato!");

	$data1 = "19800101";

	if ( is_numeric($MeseInizio) && is_numeric($AnnoInizio) )
	{
		$sql = "SELECT convert(nvarchar,dbo.cdate('1/$MeseInizio/$AnnoInizio'),112)";
		
		$data1 = ntxScalar($sql);
	}

	$data2 = "22001231";

	if ( is_numeric($MeseFine) && is_numeric($AnnoFine) )
	{
		$sql = "SELECT convert(nvarchar,dateadd(day,-1,dateadd(month,1,dbo.cdate('1/$MeseFine/$AnnoFine'))),112)";
		
		$data2 = ntxScalar($sql);
	}

	$mesi = array('',"gen","feb","mar","apr","mag","giu","lug","ago","set","ott","nov","dic");
	
	
	$where = " WHERE  (OfferteEdifici.IdCommessaOfferta = $IdCommessa) ";

	if ( is_numeric($IdProprietario) ) $where .= " AND Offerte.IdPropietario = $IdProprietario";
	
	//if (is_numeric($IdOfferta)) $where .= "AND Offerte.AttiAggiuntivi = 1  AND IdOffertaAttoAgiuntivo = $IdOfferta";
	 
	$where .= " AND Offerte.ValiditaInRendiconto <= '$data2'";
				

	$sqloff = <<< sql
		SELECT DISTINCT IdOfferta, AAcodice, AAnumero, anno, mese
			
		FROM (
				SELECT DISTINCT IdOfferta, AAcodice, AAnumero, anno,  mese,
	
				case when DataInizio>='$data1' then DataInizio else '$data1' end DataInizio
				
				FROM (
						SELECT  Offerte.IdOfferta, Offerte.AAcodice, Offerte.AAnumero, 
								YEAR (Offerte.ValiditaInRendiconto) as anno, 
								MONTH (Offerte.ValiditaInRendiconto) as mese,
								OfferteEdificiMestieri.DataInizio, 
								dateadd(month, OfferteEdificiMestieri.Durata-1,OfferteEdificiMestieri.DataInizio) DataFine
								
						FROM    OfferteEdificiMestieri 
								INNER JOIN
									Offerte 
										ON OfferteEdificiMestieri.IdOfferta = Offerte.IdOfferta 
										AND OfferteEdificiMestieri.IdCommessaOfferta = Offerte.IdCommessa

								INNER JOIN
									OfferteEdifici 
										ON OfferteEdifici.IdCommessaOfferta = Offerte.IdCommessa 
										AND OfferteEdifici.IdOfferta = Offerte.IdOfferta	
										
								LEFT JOIN
									tblPgPropietari 
										ON Offerte.IdPropietario=tblPgPropietari.IdPropietario
						$where
					) a
			) b
			
		ORDER BY AAnumero DESC
sql;

//jhp_log($sqloff);
	$query = ntxQuery($sqloff);
	
	$options = "<option value=''>";
	
	//$IdAA= array();
	//$i = 0;
	
	while( $query && $rs = ntxRecord($query) )
	{
		foreach($rs as $k=>$v)
			$html[$k] = htmlentities($v);

		$options .= "<option value='$html[IdOfferta]'> Numero AA: $html[AAnumero] - Codice AA: $html[AACodice] - $html[mese]/$html[anno]";	
		//$IdAA[$i] = $html[IdOfferta];
		//$i++;
	}
	$result[options] = $options;
	
	//$result[i]=$i;
	//$result[IdAA]=$IdAA;
	
	jhp(&$result);
?>
