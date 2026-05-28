<?php
	if ( !$_SESSION ) session_start();

	require_once('../../../_ntx/ntxconst.inc');
	require_once(NTX_PATH."jhp/jhp.inc");
	require_once(NTX_WEBPORTAL_PATH.'_database.inc'); 

	$IdUtente = ntxsse('IdUtente');
	$IdCommessa = ntxise('IdCommessa');

	$sql = <<< sql
		SELECT DISTINCT    tblPgPropietari.IdPropietario, tblPgPropietari.Codice, tblPgPropietari.Nome
		FROM         ElencoEdifici INNER JOIN
							  UtentiEdifici ON ElencoEdifici.IdCommessa = UtentiEdifici.IdCommessa AND ElencoEdifici.ID = UtentiEdifici.IdEdificio 
							  INNER JOIN
							  tblPgPropietari ON ElencoEdifici.IdProprietario = tblPgPropietari.IdPropietario
		WHERE     (UtentiEdifici.IdUtente = $IdUtente)  AND (ElencoEdifici.IdCommessa = $IdCommessa)
		ORDER BY Codice
sql;

	$query = ntxQuery($sql);

	$options = "<option value=''>";

	while( $query && $rs = ntxRecord($query) )
	{
		foreach($rs as $k=>$v)
			$html[$k] = htmlentities($v);

		$options .= "<option value='$html[IdPropietario]'>$html[Codice] - $html[Nome]";
	}

	jhp(&$options);
?>