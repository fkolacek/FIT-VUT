#!/usr/bin/php

<?php

function getCategory($category){
	$r = "";
	
	if(preg_match("#false#i", $category) == 1)
		$r = "false-";
	
	if(preg_match("#xss#i", $category) == 1)
		$r .= "xss";
	else if(preg_match("#sql#i", $category) == 1)
		$r .= "sql";
	else if(preg_match("#lfi#i", $category) == 1)
		$r .= "lfi";
	else if(preg_match("#rfi#i", $category) == 1)
		$r .= "rfi";
	else if(preg_match("#rfi#i", $category) == 1)
		$r .= "rfi";
	else
		$r .= "other";
	
	return $r;
}

require "db.php";

$dbQuery = mysql_query("SELECT * FROM attacks") or die(mysql_error());

$attacks = Array();
$attacksSum = Array('xss' => 0, 'lfi' => 0, 'rfi' => 0, 'sql' => 0, 'other' => 0, 'sum' => 0);
while($dbResult = mysql_fetch_array($dbQuery)){
	$attacks[($dbResult['id'])] = Array(
		'def' => getCategory($dbResult['category']),
		'type' => $dbResult['type'],
		'category' => $dbResult['category'],
		'subcategory' => $dbResult['subcategory'],
		'name' => $dbResult['name'],
		'link' => $dbResult['link'],
	);
	
	$attacksSum[(getCategory($dbResult['category']))]++;
	$attacksSum['sum']++;
}

echo "[*] Loaded ".$attacksSum['sum']." tests\n";
echo " - SQLi (".$attacksSum['sql']."), XSS (".$attacksSum['xss']."), LFI (".$attacksSum['lfi']."), RFI (".$attacksSum['rfi']."), others (".$attacksSum['other'].")\n";
echo "\n";

$dbQuery = mysql_query("SELECT id, scanner, date FROM tests") or die(mysql_error());

while($dbResult = mysql_fetch_array($dbQuery)){
	$counters = Array('xss' => 0, 'lfi' => 0, 'rfi' => 0, 'sql' => 0, 'other' => 0, 'sum' => 0);
	
	echo "[*] Evaluating scanner: ".$dbResult['scanner']." (tested: ".$dbResult['date'].")\n";
	
	$dbScannerQuery = mysql_query(sprintf("SELECT scannerID, attackID FROM hits WHERE scannerID = '%d'", intval($dbResult['id']))) or die(mysql_error());
	while($dbScannerResult= mysql_fetch_array($dbScannerQuery)){
		$attackID = $dbScannerResult['attackID'];
		$attack = $attacks[$attackID];
		$attackType = $attack['def'];
		$counters[$attackType]++;
    $counters['sum']++;

    //echo " - [".$attackType."] ".$attack['category']." - ".$attack['name']."\n";
  }
  echo "\n";
	
	echo " - SQLi (".$counters['sql']."/".$attacksSum['sql']."), XSS (".$counters['xss']."/".$attacksSum['xss']."), LFI (".$counters['lfi']."/".$attacksSum['lfi']."), RFI (".$counters['rfi']."/".$attacksSum['rfi']."), others (".$counters['other']."/".$attacksSum['other'].")\n";
	echo " - SQLi (".round(100/$attacksSum['sql']*$counters['sql'],1)."%), XSS (".round(100/$attacksSum['xss']*$counters['xss'],1)."%), LFI (".round(100/$attacksSum['lfi']*$counters['lfi'],1)."%), RFI (".round(100/$attacksSum['rfi']*$counters['rfi'],1)."%), others (".round(100/$attacksSum['other']*$counters['other'],1)."%)\n";
	echo "\n";
}
