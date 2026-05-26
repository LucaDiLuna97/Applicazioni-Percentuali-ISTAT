<?php  

session_start();

require_once('../../_ntx/ntxconst.inc');
require_once(NTX_WEBPORTAL_PATH.'_database.inc');
require_once(NTX_WEBPORTAL_EXT_PHP_PATH.'/_libs/paginazione/paginazione.inc');

$Utente = ntxsse('Utente');

$IdCommessa = ntxise('IdCommessa');


?>
<html>
<head>
	<link href="/repository/css/reset-1.0.0.css" rel="stylesheet" type="text/css">
	<link href="/repository/css/grid-2.0.0.css" rel="stylesheet" type="text/css">
	<link href="/repository/libs/jquery_nameless/css/smoothness/jquery-ui.css" rel="stylesheet" type="text/css">
	<script src="/repository/libs/jquery_nameless/jquery.js" type="text/javascript"></script>
	<script src="/repository/libs/jquery_nameless/jquery-ui.js" type="text/javascript"></script>
	<script src="/repository/libs/jquery_nameless/jquery.ui.datepicker-it.js" type="text/javascript"></script>
	<script type="text/javascript" src="/repository/libs/jhp/jhp-1.5.1.min.js"></script>
</head>
<body  >

<input type=button value=excel onclick="frmExcel.submit()" />

<?php 

require('sql.inc');

$IdEdificio = "";

$table = "<table class=grid><caption>$Propietario";

$alternate = false;

$subTotAnno1 = 0;
$subTotAnno2 = 0;
$subTotAnno3 = 0;
$subTotAnno4 = 0;
$subTotAnno5 = 0;

$TotAnno1 = 0;
$TotAnno2 = 0;
$TotAnno3 = 0;
$TotAnno4 = 0;
$TotAnno5 = 0;

while($rs=ntxRecord($query))
{
	foreach($rs as $k=>$v)
		$rs[$k] = htmlentities($v);
		
	if ( $IdEdificio=="" || $IdEdificio!=$rs[IdEdificio] )
	{
		if ( $IdEdificio!="" )
		{
			$class = $alternate ? "class=alternate" : "";

			$anno1 = ntxRoundThousand($subTotAnno1, 2);
			$anno2 = ntxRoundThousand($subTotAnno2, 2);
			$anno3 = ntxRoundThousand($subTotAnno3, 2);
			$anno4 = ntxRoundThousand($subTotAnno4, 2);
			$anno5 = ntxRoundThousand($subTotAnno5, 2);
			
			$TOTALE = ntxRoundThousand($subTotAnno1
			                           +$subTotAnno2
			                           +$subTotAnno3
			                           +$subTotAnno4
			                           +$subTotAnno5
			, 2);
								  
			$subTotAnno1 = 0;
			$subTotAnno2 = 0;
			$subTotAnno3 = 0;
			$subTotAnno4 = 0;
			$subTotAnno5 = 0;

			$table .= <<< table
				<tr $class><td colspan=2 style="text-align:right;font-weight:bold">TOTALE PER LOCALIZZAZIONE
				<td style="text-align:right;font-weight:bold">$anno1
				<td style="text-align:right;font-weight:bold">$anno2
				<td style="text-align:right;font-weight:bold">$anno3
				<td style="text-align:right;font-weight:bold">$anno4
				<td style="text-align:right;font-weight:bold">$anno5
				<td style="text-align:right;font-weight:bold">$TOTALE
table;

			$alternate = !$alternate;
		}
		
		$IdEdificio = $rs[IdEdificio];
		
		$table .= <<< table
			<tr><th>$rs[CodEdificio]<th>$rs[NomeEdificio]<th>$anno1h<th>$anno2h<th>$anno3h<th>$anno4h<th>$anno5h<th>TOTALE PER MESTIERE
table;
	}
	
	$class = $alternate ? "class=alternate" : "";

	$subTotAnno1 += $rs[Anno_1c]+$rs[Anno_1r];
	$subTotAnno2 += $rs[Anno_2c]+$rs[Anno_2r];
	$subTotAnno3 += $rs[Anno_3c]+$rs[Anno_3r];
	$subTotAnno4 += $rs[Anno_4c]+$rs[Anno_4r];
	$subTotAnno5 += $rs[Anno_5c]+$rs[Anno_5r];

	$TotAnno1 += $rs[Anno_1c]+$rs[Anno_1r];
	$TotAnno2 += $rs[Anno_2c]+$rs[Anno_2r];
	$TotAnno3 += $rs[Anno_3c]+$rs[Anno_3r];
	$TotAnno4 += $rs[Anno_4c]+$rs[Anno_4r];
	$TotAnno5 += $rs[Anno_5c]+$rs[Anno_5r];
	
	$anno1 = ntxRoundThousand($rs[Anno_1c]+$rs[Anno_1r], 2);
	$anno2 = ntxRoundThousand($rs[Anno_2c]+$rs[Anno_2r], 2);
	$anno3 = ntxRoundThousand($rs[Anno_3c]+$rs[Anno_3r], 2);
	$anno4 = ntxRoundThousand($rs[Anno_4c]+$rs[Anno_4r], 2);
	$anno5 = ntxRoundThousand($rs[Anno_5c]+$rs[Anno_5r], 2);
	
	$TOTALE = ntxRoundThousand($rs[Anno_1c]+$rs[Anno_1r]
	          +$rs[Anno_2c]+$rs[Anno_2r]
	          +$rs[Anno_3c]+$rs[Anno_3r]
	          +$rs[Anno_4c]+$rs[Anno_4r]
	          +$rs[Anno_5c]+$rs[Anno_5r], 2);
			  
	$table .= <<< table
		<tr $class><td>$rs[CodMest]<td>$rs[DesMest]
		<td style="text-align:right">$anno1
		<td style="text-align:right">$anno2
		<td style="text-align:right">$anno3
		<td style="text-align:right">$anno4
		<td style="text-align:right">$anno5
		<td style="text-align:right">$TOTALE
table;

	$alternate = !$alternate;
}

if ( $IdEdificio!="" )
{
	$class = $alternate ? "class=alternate" : "";
	
	$anno1 = ntxRoundThousand($subTotAnno1, 2);
	$anno2 = ntxRoundThousand($subTotAnno2, 2);
	$anno3 = ntxRoundThousand($subTotAnno3, 2);
	$anno4 = ntxRoundThousand($subTotAnno4, 2);
	$anno5 = ntxRoundThousand($subTotAnno5, 2);
	
	$TOTALE = ntxRoundThousand($subTotAnno1
							   +$subTotAnno2
							   +$subTotAnno3
							   +$subTotAnno4
							   +$subTotAnno5
	, 2);
			  
	$table .= <<< table
		<tr $class><td colspan=2 style="text-align:right;font-weight:bold">TOTALE PER LOCALIZZAZIONE
		<td style="text-align:right;font-weight:bold">$anno1
		<td style="text-align:right;font-weight:bold">$anno2
		<td style="text-align:right;font-weight:bold">$anno3
		<td style="text-align:right;font-weight:bold">$anno4
		<td style="text-align:right;font-weight:bold">$anno5
		<td style="text-align:right;font-weight:bold">$TOTALE
table;

	$alternate = !$alternate;
	
	$class = $alternate ? "class=alternate" : "";
	
	$anno1 = ntxRoundThousand($TotAnno1, 2);
	$anno2 = ntxRoundThousand($TotAnno2, 2);
	$anno3 = ntxRoundThousand($TotAnno3, 2);
	$anno4 = ntxRoundThousand($TotAnno4, 2);
	$anno5 = ntxRoundThousand($TotAnno5, 2);
	
	$TOTALE = ntxRoundThousand($TotAnno1
							   +$TotAnno2
							   +$TotAnno3
							   +$TotAnno4
							   +$TotAnno5
	, 2);
			  
	$table .= <<< table
		<tr $class><th colspan=2 style="text-align:right;font-weight:bold;font-size:1.4em">TOTALE TOTALI
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$anno1
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$anno2
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$anno3
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$anno4
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$anno5
		<th style="text-align:right;font-weight:bold;font-size:1.4em">$TOTALE
table;

	$alternate = !$alternate;
}

$table .= "</table>";

echo $table;

?>

<form name=frmExcel method=post action="excel.php" target="iframeExcel" />

<iframe name=iframeExcel width=1px height=1></iframe>

</body>
</html>
