<?php  
	$IdCommessa = ntxise('IdCommessa');
//	$IdUtente = ntxise('IdUtente');
	
	//$IdProprietario = ntxip('IdProprietario');
	
//	$AttiAggiuntivi = ntxip('AttiAggiuntivi');
//	$conguaglio = ntxip('conguaglio');
//	$CifreSignif = ntxip('CifreSignif');
	
	$array = explode("-", $_POST[MeseAnnoInizio]);
	$AnnoInizio = (int)$array[0];
	$MeseInizio = (int)$array[1];  


	$array = explode("-", $_POST[MeseAnnoFine]);
	$AnnoFine = (int)$array[0];
	$MeseFine = (int)$array[1]; 

//	$cifreSignificative = 2;
	//se il bit è attivo, per gli arrotondamenti si utilizza il numero passato tramite mod, di default è 2 
//	$UsaCifreSignificative = $_POST['UsaCifreSignificative'];
	
//	if($UsaCifreSignificative == 1)
//		$cifreSignificative = $CifreSignif;

/*	
	$IdAA  = $_POST['IdAA'];
	
	if( $AttiAggiuntivi == 1 && is_numeric($IdAA) )
	{
		$IdOfferta = $IdAA;
	}
	else
	{
		//$IdOfferta = ntxip('IdOfferta');
	}
*/
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

	//require('filtra-periodi-offerte.inc');
	//jhp_log($periodiOfferte);
	
	//require('filtra-canoni.inc');

	require('filtra-sql-edifici.inc');
	
	require('filtra-table-edifici.inc');
?>