<?php
	if ( !$_SESSION ) session_start();

	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");
	require_once(NTX_WEBPORTAL_PATH.'_database.inc'); 

	$IdUtente = ntxsse('IdUtente');

	$IdCommessa = ntxise('IdCommessa');
	
	$array = explode("-", $_POST[MeseAnnoInizio]);
	$AnnoInizio = /*(int)*/$array[0];
	$MeseInizio = /*(int)*/$array[1];  


	$array = explode("-", $_POST[MeseAnnoFine]);
	$AnnoFine = /*(int)*/$array[0];
	$MeseFine = /*(int)*/$array[1]; 
	
	//jhp_log($AnnoInizio.$MeseInizio);
	
	
	$AttiAggiuntivi = $_POST['AttiAggiuntivi'];
	
	$result = array();

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
	
	
	$where = " WHERE    /* (UtentiEdifici.IdUtente = $IdUtente)  AND*/ (OfferteEdifici.IdCommessaOfferta = $IdCommessa) ";

	//if ( is_numeric($IdProprietario) ) $where .= " AND Offerte.IdPropietario = $IdProprietario";
	
	if( is_numeric($AttiAggiuntivi) && $AttiAggiuntivi>0 )
	{
		$where.= "AND (Offerte.AttiAggiuntivi IS NULL OR Offerte.AttiAggiuntivi = 0)";
	}

	$sqloff = <<< sql
	
		SELECT DISTINCT IdOfferta, DataOfferta, DataAttivazione, NRPF,  Sigla, NOPF
			
		FROM (
				SELECT DISTINCT IdOfferta, DataOfferta,DataAttivazione, NRPF,  Sigla, NOPF,
	
				case when DataInizio>='$data1' then DataInizio else '$data1' end DataInizio,
				case when DataFine<='$data2' then DataFine else '$data2' end DataFine
				
				FROM (
						SELECT	Offerte.IdOfferta, Offerte.DataOfferta, Offerte.DataAttivazione, Offerte.NRPF,  Offerte.Sigla, 
							Offerte.NOPF, OfferteEdificiMestieri.DataInizio, 
							dateadd(month, OfferteEdificiMestieri.Durata-1,OfferteEdificiMestieri.DataInizio)	DataFine
								
						FROM	OfferteEdificiMestieri 
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
			
		ORDER BY IdOfferta
sql;

	$query = ntxQuery($sqloff);

	$options = "<option value=''>";
	
	$conta=0;
	
	while( $query && $rs = ntxRecord($query) ) 
	{
		
		$rs[DataOfferta] = ntxDate($rs[DataOfferta]);
		$rs[DataAttivazione] = ntxDate($rs[DataAttivazione]);
		
		$conta++;
		
		foreach($rs as $k=>$v)
			$html[$k] = htmlentities($v);
			
		if(is_numeric($AttiAggiuntivi)&& $AttiAggiuntivi>0)
		{
			
			$options .= "<option value='$html[IdOfferta]'>Codice RPF: $html[Sigla] - Codice OPF: $html[NOPF] - Data Attivazione: $html[DataAttivazione]";	
		
		}
		else
		{
			
			$options .= "<option value='$html[IdOfferta]'>$html[Nome] - NRPF $html[NRPF] - Data Offerta $html[DataOfferta] - Data Attivazione $html[DataAttivazione]";
		
		}
	}

	$result[options] = $options;
	$result[conta] = $conta;
	
	jhp(&$result);
?>
