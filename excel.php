<?php  

session_start();

header('Pragma: private');
header('Cache-Control: private, must-revalidate');

header('Content-type: application/vnd.ms-excel');
header("Content-Disposition: attachment; filename=report.xls");

require_once('../../_ntx/ntxconst.inc');
require_once(NTX_WEBPORTAL_PATH.'_database.inc');

?>
<html>
<head>
<style>
* {
	font-size: 100%;
	font: inherit;
}

html , body { 
	margin: 0px; 
	padding: 0px;
}

body {
	background: white; 
	color: black;
	font-family: Arial, Helvetica, sans-serif;
	font-size: 8pt;
}
table.grid  { 
	border-collapse: collapse;
	border-spacing: 0;
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 8pt;
	color: white;
	background-color: #778899;
	border: solid 1px black;
	empty-cells:show;
}

table.grid caption { 
	color: white;
	font-size: 12pt;
	background: #293e43;
	font-weight: bold;
	vertical-align: middle;
	text-align:center;
	text-transform:uppercase;
	padding-left: 10px;
	padding-right: 10px;
	padding-top: 5px;
	padding-bottom: 5px;
}

table.grid tr th { 
	background: #293e43;
	color: white;
	border-left:1px solid #777777;
	border-right:1px solid #333333;
	font-weight:bold;
	text-align:center;
	padding-left: 5px;
	padding-right: 5px;
}

table.grid tr {
	background-color: white;
	color: black;
}

table.grid tr td {
	background-color: white;
	color: black;
	border-bottom:1px solid #cccccc;
	border-right:1px solid #AAA0A0;
	border-left:1px solid #777777;
	text-align:left;
	padding-left: 5px;
	padding-right: 5px;
}

table.grid tr.alternate td {
	background-color: #E0E0F0;
}

table.grid .left { text-align: left }
table.grid .center { text-align: center }
table.grid .right { text-align: right }

</style>
</head>
<body  >
<?php 

$isExcel = true;

require_once('jhp/filtra.inc');

echo $table;

?>

</body>
</html>
