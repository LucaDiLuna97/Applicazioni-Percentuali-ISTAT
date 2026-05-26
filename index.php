<?php  
	session_start();

	require_once('../../_ntx/ntxconst.inc');
	require_once(NTX_WEBPORTAL_PATH.'_database.inc');
	require_once("$_SERVER[DOCUMENT_ROOT]/repository/func/paginazione/form.inc");
	require_once("$_SERVER[DOCUMENT_ROOT]/repository/traduttore/traduttore_v4.inc");
	
	$traduttore = new NtxTraduttore("", "IT", $_SESSION[NTX_LINGUA_DEST]);
	//$traduttore = new NtxCreaTraduzione("", "IT", __FILE__);
	
	$vers = time();

	$AttiAggiuntivi = $_GET[AttiAggiuntivi] == 1 ? 1 : /*($_GET[AttiAggiuntivi] == 2 ? 2 :*/ 0 /*)*/;
	$_POST[AttiAggiuntivi] = $AttiAggiuntivi;
	
	$sblocca = $_GET[sblocca] == 1 ? 1 : 0;
	$_POST[sblocca] = $sblocca;
	
	$attivaDelta = $_GET[attivaDelta] == 1 ? 1 : 0;
	$conguaglio = $_GET[conguaglio] == 1 ? 1 : 0;
	
	if(is_numeric($_GET[CifreSignif]) && $_GET[CifreSignif]>0){
		$CifreSignif = $_GET[CifreSignif];
	}else{
		$CifreSignif = 0;
	}
	
	$_POST[attivaDelta] = $attivaDelta;
	
	$_POST[CifreSignif] = $CifreSignif;
	
	//print_r($_POST);
	
	//print_r("conguaglio: ".$conguaglio);
	
	//****** LA MOD AttiAggiuntivi IMPLEMENTA LA LOGICA DEGLI ATTI AGGIUNTIVI SU QUESTA OPZIONE ******//
	
	//****** LA MOD attivaDelta AGGIUNGE IL CALCOLO DEL DELTA SCELTE DUE OFFERTE******//
	
	//****** LA MOD CifreSignif offre la possibilità di scegliere quante cifre significative mostrare ******//
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
	
	<style>
		.cifreSignificative{
			background-color: #73ff73 !important;
		}
	</style>
		<script> 
			var AttiAggiuntivi = <?php echo $AttiAggiuntivi?>; 
			var attivaDelta = <?php echo $attivaDelta?>; 
			var conguaglio = <?php echo $conguaglio?>; 
			var CifreSignif = <?php echo $CifreSignif?>; 
			var sblocca = <?php echo $sblocca?>; 
		</script>
	
		<link href="/repository/css/reset-1.0.0.css" rel="stylesheet" type="text/css">
		<link href="/repository/css/grid-2.0.0.css" rel="stylesheet" type="text/css">
		<!--<link href="/repository/libs/jquery_nameless/css/smoothness/jquery-ui.css" rel="stylesheet" type="text/css">-->
		<script src="/repository/libs/jquery_nameless/jquery.js" type="text/javascript"></script>
		<script src="/repository/libs/jquery_nameless/jquery-ui.js" type="text/javascript"></script>
	<!--	<script src="/repository/libs/jquery_nameless/jquery.ui.datepicker-it.js" type="text/javascript"></script>-->
	<!--	<script src="/repository/libs/jquery_nameless/jquery-ui-buttonsetv.js" type="text/javascript"></script> -->
	
		<script src="/repository/libs/jhp/jhp-1.5.1.min.js" type="text/javascript"></script>
		<script src="/repository/func/paginazione/paginazione.js" type="text/javascript"></script>
		
		<script src="js/index.js?vers=<?php echo $vers ?>" type="text/javascript"></script>
		
	</head>
	<body>
		<br>
		<br>
		<form name="form1" method="post" action="" target=frame20190208_1568ww>
		
			<input type=hidden name=AttiAggiuntivi value="<?php echo $AttiAggiuntivi ?>" />	
			<input type=hidden name=attivaDelta value="<?php echo $attivaDelta ?>" />
			<input type=hidden name=conguaglio value="<?php echo $conguaglio ?>" />
			<input type=hidden name=CifreSignif value="<?php echo $CifreSignif ?>" />
			<input type=hidden name=sblocca value="<?php echo $sblocca ?>" />
			<input type=hidden name=UsaCifreSignificative value="0" />
			<input type="hidden" id="Tipologia" />
			<input type="hidden" id="IdProprietario" value= ""/>
			<input type="hidden" id="Mese" value= "" />
			<input type="hidden" id="Anno" value= "" />
			<input type="hidden" id="PercIstat" value= "" />
			<input type="hidden" id="IdMeseAnnoDaBloc" value= "" />
			
			<!--<input type=hidden name=bitConPadre value="" />	
			<input type=hidden name=bitSenzaPadre value="" />	
			<input type=hidden name=ListaIdAA value="" />-->
					
			<?php 
			$style = "";
			if($sblocca == 1){
				$style = 'style="width:1000px"';
			 } else {
				$style = 'style="width:800px"';
			} 
			?>
			
			<table class=grid <?php echo $style ?> >	 
				<!-- <tr>
					<th><?php //$traduttore->html('Proprietario') ?><td colspan=7><select name=IdProprietario style="width:100%"></select> -->
				
				<tr>
					<!-- DA SBLOCCARE SE CHIEDONO UN FILTRO STILE MONTH PER MESE ANNO INIZIO
					E MESE ANNO FINE -->
					<th><?php $traduttore->html('Inizio Ricerca') ?><td><input type=month name=MeseAnnoInizio id=MeseAnnoInizio size=3 maxlength=2 onchange="Sblocca()" />
					<th><?php $traduttore->html('Fine Ricerca') ?><td><input type=month name=MeseAnnoFine id=MeseAnnoFine onchange="Sblocca()" size=3 maxlength=2 />
					<!--<th><?php $traduttore->html('Mese Inizio') ?><td><input type=text name=MeseInizio size=3 maxlength=2 />
					<th><?php $traduttore->html('Anno Inizio') ?><td><input type=text name=AnnoInizio size=5 maxlength=4 />
					<th><?php $traduttore->html('Mese Fine') ?><td><input type=text name=MeseFine size=3 maxlength=2 />
					<th><?php $traduttore->html('Anno Fine') ?><td><input type=text name=AnnoFine size=5 maxlength=4 /> -->
					<th><?php $traduttore->html('Seleziona Anno per Documento ISTAT') ?><td><input type=number name=SelezionaAnno size=5 maxlength=4
					value="<?php echo $annoCorrente = date("Y"); ?>"
					min="<?php echo $annoCorrente = date("Y"); ?>" max="<?php echo $annoMassimo = date("Y")+13; ?>"
					/> <?php //print_r($_POST); ?>
					
					<td><input type=button name=ScaricaWord value="Scarica Word" onclick="Scarica(1)" size=3 maxlength=2 />
					<td><input type=button name=ScaricaPDF value="Scarica PDF" onclick="Scarica(2)" size=3 maxlength=2 />
					
				<?php if($sblocca == 1){?>
							<td><input type=button name=SbCelle id=SbCelle class="SbCelle" value="Sblocca" onclick="SbloccaCelle()" size=3 maxlength=2 />
					
				<?php } ?>
				
				<tr><th colspan=15 style="text-align:right">
				<input type=button class=btn value="Seleziona tutto" onclick="SelezionaTutto()"  />
				<input type=button class=btn value="Deseleziona tutto" onclick="DeselezionaTutto()" />
					<input type=button value="&#10006; <?php $traduttore->html('Reset') ?>" onclick="Reset()" />
					<input type=button id=CercaDefault value="&#10095;&#10095; <?php $traduttore->html('Cerca') ?>" onclick="CaricaOfferte(); Filtra(0);"/>
			</table>
		</form>
		
		<form method="post" name="formScarica" action="jhp/download.php">
			<input type="hidden" name="Tipologia" />
			<!--<input type="hidden" name="MeseInizio" />
			<input type="hidden" name="AnnoInizio" />
			<input type="hidden" name="MeseFine" />
			<input type="hidden" name="AnnoFine" /> -->
			<input type="hidden" name="SelezionaAnno" />
			<input type="hidden" name="MeseAnnoInizio" />
			<input type="hidden" name="MeseAnnoFine" />
			<input type="hidden" name="IdProprietario" />
		</form>
		
		<?php 
			if(is_numeric($CifreSignif) && $CifreSignif > 0)
			{
				$style = "";
			} else
			{
				$style = "display : none;";
			}
		?>
		<div id=cifreSignificative style= "width:795px; border: solid #405259 2px; <?php echo $style?>">
			<div style="width: 20%; display: inline-block;">
				<input type=button id=cercaCifreSignif value="&#10095;&#10095; Cerca" onclick="Filtra(1)" />
				<input type=button id=excelCifreSignif value="&#x274B; <?php $traduttore->html('Excel') ?>" onclick="Excel(1)" />
				</div>
			<div style="width: 65%; display: inline-block; ">
				<p> <b> Per visualizzare/scaricare l’importo sui Mesi con (<?php echo $CifreSignif?>) cifre significative, clicca sui bottoni in questa sezione</b></p>
			</div>
		</div>
		
	
		<br>
		<div id=risultato></div>
		
		<iframe name=frame20190208_1568ww style="width:1px;height:1px;border:none"></iframe>
		
		<?php $traduttore->Salva() ?>
	</body>
</html>