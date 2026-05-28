<?php
	session_start();
	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");
	require_once(NTX_WEBPORTAL_PATH.'_database_senza_sicurezza.inc');
	
	//print_r($_POST);
	$StringaIdProprietario = ntxsp('IdProprietario');
	//print_r($ArrIdProprietario);
	
	$IdProprietario = /*explode(",",*/substr($StringaIdProprietario, 1, -1)/*)*/;
	//print_r($IdProprietario);
	
	$ArrIdProprietario = explode(",", $IdProprietario);
	//print_r($ArrIdProprietario);
	//print_r(count($ArrIdProprietario));
	
	//AL MOMENTO LASCIA COMMENTATO IL CICLO FOR
	//for($i = 1; $i <= count($ArrIdProprietario); $i++){
		
		//print_r("sono qui");
		
	//	print_r("------".$ArrIdProprietario[$i]."fine ciclo");
		$IdProp = $ArrIdProprietario[$i];
	
		$fileNameRendiconto = "Template_per_pubblicazioni_ISTAT";
	
		require("creazioneWordPdf.inc");
		
		if ( $_POST["Tipologia"] == 1 || $_POST["Tipologia"] == 2 ) {
			
			//print_r($path."sono dentro download.php");
			
			if (file_exists($path) && is_file($path)) {
				// file exist
				header('Content-Description: File Transfer');
				// header("Content-type:application/pdf");
				header('Content-Type: application/octet-stream');
				header("Content-Type: application/download");
				header('Content-Disposition: attachment; filename='.$fileNameRendiconto.$extFileName);
				header('Content-Transfer-Encoding: binary');
				header('Expires: 0');
				header('Cache-Control: must-revalidate');
				header('Pragma: public');
				header('Content-Length: ' . filesize($path));
				set_time_limit(0);
				@readfile($path);//"@" is an error control operator to suppress errors
				
			} else {
				// file doesn't exist
				die('Error: The file ' . basename($path) . ' does not exist!');
			}
		}
		
	//}
	
	
	
?>