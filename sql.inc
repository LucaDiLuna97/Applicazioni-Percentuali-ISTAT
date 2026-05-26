<?php 

$sql = <<< sql
SELECT     TOP (1) tblPgPropietari.Nome
FROM         ElencoEdifici INNER JOIN
                      UtentiEdifici ON ElencoEdifici.IdCommessa = UtentiEdifici.IdCommessa AND ElencoEdifici.ID = UtentiEdifici.IdEdificio INNER JOIN
                      Utenti ON UtentiEdifici.IdUtente = Utenti.Id INNER JOIN
                      tblPgPropietari ON ElencoEdifici.IdProprietario = tblPgPropietari.IdPropietario
WHERE     (Utenti.Nome = $Utente)  AND (ElencoEdifici.IdCommessa = $IdCommessa)
sql;

$Propietario = htmlentities(ntxScalar($sql));

$sql = <<< sql
	SELECT     TOP (1) OfferteEdifici.IdOfferta
	FROM         OfferteEdifici INNER JOIN
						  ElencoEdifici ON OfferteEdifici.IdCommessaEdificio = ElencoEdifici.IdCommessa AND OfferteEdifici.IdEdificio = ElencoEdifici.ID INNER JOIN
						  UtentiEdifici ON ElencoEdifici.IdCommessa = UtentiEdifici.IdCommessa AND ElencoEdifici.ID = UtentiEdifici.IdEdificio INNER JOIN
						  Utenti ON UtentiEdifici.IdUtente = Utenti.Id
	WHERE     (Utenti.Nome = $Utente)  AND (OfferteEdifici.IdCommessaOfferta = $IdCommessa)
sql;

$mesi = array('', "gen",  "feb",  "mar",  "apr",  "mag", 
				"giu",  "lug",  "ago",  "set",  "ott", 
				"nov",  "dic");
				
$IdOfferta = ntxScalar($sql);

if ( !is_numeric($IdOfferta) )
	die("Utente non associato con edifici offerte!");

$sql = <<< sql
	SELECT     month(DataAttivazione)
	FROM         Offerte
	WHERE (Offerte.IdCommessa = $IdCommessa) 
		AND (Offerte.IdOfferta = $IdOfferta)
sql;

$mese = ntxScalar($sql);

$sql = <<< sql
	SELECT     year(DataAttivazione)
	FROM         Offerte
	WHERE (Offerte.IdCommessa = $IdCommessa) 
		AND (Offerte.IdOfferta = $IdOfferta)
sql;

$anno1 = ntxScalar($sql);
$mese1 = $mesi[$mese];
$mese2 = $mesi[($mese+10)%12+1];

if ( $mese>0 ) $anno2 = $anno1 + 1;

$anno1h = "&nbsp;$mese1/$anno1<br>-<br>&nbsp;$mese2/$anno2";
$anno1++; $anno2++;
$anno2h = "&nbsp;$mese1/$anno1<br>-<br>&nbsp;$mese2/$anno2";
$anno1++; $anno2++;
$anno3h = "&nbsp;$mese1/$anno1<br>-<br>&nbsp;$mese2/$anno2";
$anno1++; $anno2++;
$anno4h = "&nbsp;$mese1/$anno1<br>-<br>&nbsp;$mese2/$anno2";
$anno1++; $anno2++;
$anno5h = "&nbsp;$mese1/$anno1<br>-<br>&nbsp;$mese2/$anno2";

$sqlSopralluoghi = <<< sqlSopralluoghi
	SELECT  
			SopralluogoPDI.NonConsiderareUnatantum,
			SopralluogoPDI.QTA,
			TipiComponentiMestieri.Fattore, 
			TipiComponentiMestieri.UnaTantum, 
			OfferteEdificiMestieri.CalcSotto400mq, 
			tblMestieri.IdMestiere,
			tblMestieri.Calc14x100, 
			OfferteEdificiMestieri.Reperibilità, 
			OfferteEdificiMestieri.CinquePerCento, 
			OfferteEdificiMestieri.AChiamata,
			OfferteEdificiMestieri.IdEdificio,
			Offerte.IdCommessa,
			Offerte.IdOfferta,
			Offerte.DataAttivazione,
		
			Offerte.DataAttivazione 				  Data1,
			dateadd(month,12,Offerte.DataAttivazione) Data2,
			dateadd(month,24,Offerte.DataAttivazione) Data3,
			dateadd(month,36,Offerte.DataAttivazione) Data4,
			dateadd(month,48,Offerte.DataAttivazione) Data5,
			dateadd(month,60,Offerte.DataAttivazione) Data6,
			
			case when SopralluogoPDI.IdRevConsistenza is null then 
				OfferteEdificiMestieri.DataInizio 
			else
				SopralluogoPDI.DataInizio
			end DataInizio, 
			
			case when SopralluogoPDI.IdRevConsistenza is null then 
				OfferteEdificiMestieri.Durata 
			else
				SopralluogoPDI.Durata
			end Durata
			
	FROM         OfferteEdificiMestieri INNER JOIN
					  SopralluogoPDI ON OfferteEdificiMestieri.IdEdificio = SopralluogoPDI.IdEdificio AND 
					  OfferteEdificiMestieri.IdCommessaEdificio = SopralluogoPDI.IdCommessaEdificio AND 
					  OfferteEdificiMestieri.IdCommessaOfferta = SopralluogoPDI.IdCommessaOfferta AND 
					  OfferteEdificiMestieri.IdOfferta = SopralluogoPDI.IdOfferta INNER JOIN
					  tblMestieri ON OfferteEdificiMestieri.IdMestiere = tblMestieri.IdMestiere AND 
					  OfferteEdificiMestieri.IdCommessaOfferta = tblMestieri.IdCommessa INNER JOIN
					  tblComponetiMestiere ON tblMestieri.IdCommessa = tblComponetiMestiere.IdCommessa AND 
					  tblMestieri.IdMestiere = tblComponetiMestiere.IdMestiere INNER JOIN
					  TipiComponentiMestieri ON SopralluogoPDI.IdCommessaTipoComp = TipiComponentiMestieri.IdCommessa AND 
					  SopralluogoPDI.IdTipoComp = TipiComponentiMestieri.IdTipoComponenteMestiere AND 
					  tblComponetiMestiere.IdCommessa = TipiComponentiMestieri.IdCommessa AND 
					  tblComponetiMestiere.IdComponenteMestiere = TipiComponentiMestieri.IdComponenteMestiere INNER JOIN
					  Offerte ON OfferteEdificiMestieri.IdOfferta = Offerte.IdOfferta AND OfferteEdificiMestieri.IdCommessaOfferta = Offerte.IdCommessa
	WHERE 
			(Offerte.IdCommessa = $IdCommessa) 
		AND 
			(Offerte.IdOfferta = $IdOfferta)
		AND 
			tblMestieri.EscludiRM = 1
sqlSopralluoghi;

for ( $i1=1; $i1<=5; $i1++ )
{
	$i2 = $i1+1;
	
	$sqlAnnoC[$i1] = <<< sqlAnnoC
		case when DataInizio < Data{$i2} then
			case when DataInizio >= Data{$i1} then /* ANNO INIZIALE */
				case when UnaTantum=1 then 
					case when NonConsiderareUnatantum=1 then 0 else QTA * Fattore end
				else
					case when Durata % 12 = 0 then
						QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END
					else
						(Durata % 12) * QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END / 12
					end 
				end 
			else /* ANNO SUCCESSIVO */
				case when UnaTantum=1 then
					case when Calc14x100=1 then QTA * Fattore * 0.14 else 0 end
				else
					QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END
				end 
			end
		else
			0
		end
sqlAnnoC;

$sqlAnnoR[$i1] = <<< sqlAnnoR
		case when DataInizio < Data{$i2} AND (UnaTantum=0 or UnaTantum is null) and Reperibilità=1 and CinquePerCento=1 then
			case when DataInizio >= Data{$i1} then /* ANNO INIZIALE */
				case when Durata % 12 = 0 then
					0.05 * QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END
				else
					0.05 * (Durata % 12) * QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END / 12
				end 
			else /* ANNO SUCCESSIVO */
				0.05 * QTA * Fattore * CASE WHEN CalcSotto400mq = 1 THEN 0.7 ELSE 1 END 
			end
		else
			0
		end
sqlAnnoR;
}

$sqlEdifici = <<< sqlEdifici
SELECT  DISTINCT   ElencoEdifici.IdCommessa, OfferteEdificiMestieri.IdOfferta, ElencoEdifici.ID AS IdEdificio, ElencoEdifici.Ordine AS OrdEdificio, 
                      ElencoEdifici.Codice AS CodEdificio, ElencoEdifici.EdificioBreve AS NomeEdificio, tblMestieri.IdMestiere, tblMestieri.Ordine AS OrdMest, 
                      tblMestieri.codice AS CodMest, tblMestieri.Descrizione AS DesMest
FROM         tblMestieri INNER JOIN
                      OfferteEdificiMestieri ON tblMestieri.IdCommessa = OfferteEdificiMestieri.IdCommessaEdificio AND 
                      tblMestieri.IdMestiere = OfferteEdificiMestieri.IdMestiere INNER JOIN
                      ElencoEdifici ON OfferteEdificiMestieri.IdCommessaEdificio = ElencoEdifici.IdCommessa AND OfferteEdificiMestieri.IdEdificio = ElencoEdifici.ID
WHERE (ElencoEdifici.IdCommessa = $IdCommessa) 
		AND (OfferteEdificiMestieri.IdOfferta = $IdOfferta)
sqlEdifici;

$sql = <<< sql
	SELECT 
			IdEdificio,
			IdMestiere,
			OrdEdificio, 
			CodEdificio, 
			NomeEdificio, 
			OrdMest, 
			CodMest, 
			DesMest, 
			
			SUM(Anno_1c) Anno_1c, 
			SUM(Anno_2c) Anno_2c, 
			SUM(Anno_3c) Anno_3c, 
			SUM(Anno_4c) Anno_4c, 
			SUM(Anno_5c) Anno_5c,
			
			SUM(Anno_1r) Anno_1r, 
			SUM(Anno_2r) Anno_2r, 
			SUM(Anno_3r) Anno_3r, 
			SUM(Anno_4r) Anno_4r, 
			SUM(Anno_5r) Anno_5r
	FROM (
			SELECT 
					a.IdEdificio,
					a.IdMestiere,
					OrdEdificio, 
					CodEdificio, 
					NomeEdificio, 
					OrdMest, 
					CodMest, 
					DesMest, 
					
					($sqlAnnoC[1]) Anno_1c, 
					($sqlAnnoC[2]) Anno_2c, 
					($sqlAnnoC[3]) Anno_3c, 
					($sqlAnnoC[4]) Anno_4c, 
					($sqlAnnoC[5]) Anno_5c,
					
					($sqlAnnoR[1]) Anno_1r, 
					($sqlAnnoR[2]) Anno_2r, 
					($sqlAnnoR[3]) Anno_3r, 
					($sqlAnnoR[4]) Anno_4r, 
					($sqlAnnoR[5]) Anno_5r
					
			FROM ($sqlEdifici) a INNER JOIN ($sqlSopralluoghi) b
			ON a.IdCommessa = b.IdCommessa AND a.IdOfferta = b.IdOfferta AND a.IdEdificio = b.IdEdificio AND a.IdMestiere = b.IdMestiere 
			) c
	GROUP BY OrdEdificio, CodEdificio, NomeEdificio, IdEdificio, OrdMest, CodMest, DesMest, IdMestiere
	ORDER BY OrdEdificio, CodEdificio, NomeEdificio, IdEdificio, OrdMest, CodMest, DesMest, IdMestiere
sql;

$query = ntxQuery($sql);

?>