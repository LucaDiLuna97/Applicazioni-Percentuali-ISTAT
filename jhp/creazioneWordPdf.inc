<?php
	/*if( $gruppiMes == 1){
		require('gruppiMes/dati_word.php');
	}else if ( $gruppiMes == 2 ) {
		require('gruppiMesColser/dati_word.php');
	}else if ( $unitaG == 1 ) {
		require('unitaG/dati_word.php');
	}else{*/
		//require('dati_word.php');
	//}
	
	
	require('_wordAllegato.php');
	
	//// ################################################################# ////
	//// $aDocumento: array documento file in word                         ////
	//// $suffixFiles: suffisso file temporaneo                            ////
	//// $pathTemplateDOCX: input file name                                ////
	//// ################################################################# ////
	
	// print_r($documento);
	$aDocumento = $arrayWord;
	//print_r($aDocumento);
	//$suffixFiles = "RendMes";

	// C:\ms4w\Apache\htdocs\build\manpronet\opzioni_comuni\stampa_offerte\rendiconto_mensile\jhp\create_word\input
	
	$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\applicazioni_percentuali_istat\\template\\Template_per_pubblicazioni_ISTAT.docx";
	
	/*if( $gruppiMes == 1 ) {
		$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\stampa_offerte\\rendiconto_mensile\\template\\gruppiMes\\wordTemplateRendicontoMensileGruppi.docx";
	}
	
	if( $gruppiMes == 2 ) {
		$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\stampa_offerte\\rendiconto_mensile\\template\\gruppiMesColser\\wordTemplateRendicontoMensileGruppi.docx";
	}
	
	if( $unitaG == 1 ) {
		if($centriCosto == 0){
			$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\stampa_offerte\\rendiconto_mensile\\template\\unitaG\\wordTemplateRendicontoMensileUdG.docx";
		} else {
			$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\stampa_offerte\\rendiconto_mensile\\template\\unitaG\\wordTemplateRendicontoMensileCdC.docx";
		}
	}
	
	if( $ts_consip == 1 ) {
		$pathTemplateDOCX =  "$_SERVER[DOCUMENT_ROOT]\\".NTX_SUBPATH_PORTALE."\\opzioni_comuni\\stampa_offerte\\rendiconto_mensile\\template\\ts_Consip\\wordTemplateRendicontoMensileTsConsip.docx";
	}*/

	$pathTemplateDOCX = str_replace( '/', '\\', $pathTemplateDOCX );
	$pathTemplateDOCX = str_replace( '\\', '/', $pathTemplateDOCX );

	require("$_SERVER[DOCUMENT_ROOT]\\repository\\nodejs\\create_word\\generatore.php");
	
	//print_r($_POST["Tipologia"].$pathDestinazioneDOCX."sono dentro download.php");
	
	if( $_POST["Tipologia"]==1 ) {
		$path = $pathDestinazioneDOCX;
		$extFileName = ".docx";
	}
	else if( $_POST["Tipologia"]==2 ) {
		$path = $pathDestinazionePDF;
		$extFileName = ".pdf";
	}
	
	if ($_POST["Tipologia"] != 3) {
		if (file_exists($path) && is_file($path)) {
			// file exist
			header('Content-Description: File Transfer');
			// header("Content-type:application/pdf");
			header('Content-Type: application/octet-stream');
			header("Content-Type: application/download");
			header('Content-Disposition: attachment; filename='."Template_per_pubblicazioni_ISTAT".$extFileName);//.basename($path));
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
	} else {
		jhp_ok(&$path);
	}
?>