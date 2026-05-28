<?php
// $IdCommessa
// $IdOfferta 
// $IdEdificio
// $IdMestiere
// $IdTipoComponenteMestiere

$checkMezzoMese = 0;
$normalMeseSelezionato = substr(str_repeat(0, 2).$mese, - 2);
$fineGiornoMeseCorrente = ntxScalar("select DAY(EOMONTH('".$anno.$normalMeseSelezionato."01'))");

$checkDataInizioSQL = <<<SQL
	select distinct
		day(eomonth (DataInizio)) as GiorniDelMeseInizio
		, DAY(SopralluogoPDI.DataInizio) as GiornoInizio
		, MONTH(SopralluogoPDI.DataInizio) as MeseInizio
		, YEAR(SopralluogoPDI.DataInizio) as AnnoInizio
		
		--, DataFine
		, day(eomonth (DataFine)) as GiorniDelMeseFine
		, DAY(SopralluogoPDI.DataFine) as GiornoFine
		, MONTH(SopralluogoPDI.DataFine) as MeseFine
		, YEAR(SopralluogoPDI.DataFine) as AnnoFine

	from SopralluogoPDI 
		left join 
			TipiComponentiMestieri on 
					TipiComponentiMestieri.IdCommessa = SopralluogoPDI.IdCommessaEdificio 
				and 
					TipiComponentiMestieri.IdTipoComponenteMestiere = SopralluogoPDI.IdTipoComp
		left join 
			tblComponetiMestiere on 
					tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa 
				and 
					TipiComponentiMestieri.IdComponenteMestiere = tblComponetiMestiere.IdComponenteMestiere
		left join 
			tblMestieri on 
					tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa 
				and 
					tblComponetiMestiere.IdMestiere = tblMestieri.IdMestiere
	where 
		IdCommessaEdificio = $IdCommessa
	and 
		IdOfferta = $IdOfferta
	and 
		IdEdificio = $IdEdificio
	and 
		tblMestieri.IdMestiere = $IdMestiere
	and 
		TipiComponentiMestieri.IdTipoComponenteMestiere = $IdTipoComponenteMestiere
SQL;

$checkDataInizioQUERY = ntxQuery($checkDataInizioSQL);

while ( $checkDataInizioQUERY && $rsDI=ntxRecord($checkDataInizioQUERY) )
{		
	foreach($rsDI as $key=>$value)
	{
		$js[$key] = ntxJs($value);
		$html[$key] = htmlentities($value);
	}

	$GiorniDelMeseInizio = $html[GiorniDelMeseInizio];
	$GiornoInizio = $html[GiornoInizio];
	$MeseInizio = $html[MeseInizio];
	$AnnoInizio = $html[AnnoInizio];

	if ($anno == $AnnoInizio && $mese == $MeseInizio) {
//		if ((1 < $GiornoInizio) && ($GiornoInizio < $fineGiornoMeseCorrente)) {
//			$checkMezzoMese = 1;
//		}

		$totMese = ( ( $totMese / $GiorniDelMeseInizio) * ( $GiorniDelMeseInizio - $GiornoInizio + 1) );
	}

	$GiorniDelMeseFine = $html[GiorniDelMeseFine];
	$GiornoFine = $html[GiornoFine];
	$MeseFine = $html[MeseFine];
	$AnnoFine = $html[AnnoFine];
	//jhp_log("############".$GiornoFine);
	if ($anno == $AnnoFine && $mese == $MeseFine) {
//		if ((1 < $GiornoFine) && ($GiornoFine < $fineGiornoMeseCorrente)) {
//			$checkMezzoMese = 1;
//		}
		$totMese = ( ( $totMese / $GiorniDelMeseFine) * ( $GiornoFine ) );
	}
}
?>