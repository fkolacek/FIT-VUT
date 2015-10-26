#!/usr/bin/php

<?php

require "./db.php";

if(!isset($argv[1]))
    die("Usage: ".$argv[0]." [scanner] [path]\n");

$NAME = $argv[1];
$PATH = (isset($argv[2])? $argv[2] : "results");

$dirHandler = opendir($PATH) or die("Cannot open input dir: ".$PATH);

$attacks = Array();
while($file = readdir($dirHandler)){
  if(preg_match("#".$NAME."#", $file)){
    $fileHandler = fopen($PATH."/".$file, "r") or die("Cannot open input file: ".$file);

    while($line = fgets($fileHandler)){
      switch($NAME){
        case "wapiti":
          if(preg_match("#^.+path\": \"/([^\"]+)\"#", $line, $parts) == 1)
              $attacks[] = $parts[1];
          
          break;
        case "w3af":
          if(preg_match("#,http://.+/(wavsep.+\.jsp)#", $line, $parts) == 1)
            $attacks[] = $parts[1];
          break;
        case "revok":
          if(preg_match("#^.+(wavsep.+\.jsp)#", $line, $parts) == 1)
            $attacks[] = $parts[1];
        default:
          break;
      }
    }

    fclose($fileHandler);
  }
}

$attacksDB = Array();
$dbQuery = mysql_query("SELECT id, link FROM attacks") or die(mysql_error($dbHandler));
while($dbResult = mysql_fetch_array($dbQuery)){
  $attacksDB[($dbResult['link'])] = $dbResult['id'];
}


echo "[*] Grabbed ".count($attacks)." vulnerabilities\n";
mysql_query(sprintf("INSERT INTO tests (scanner, date) VALUES ('%s', '%s')", mysql_real_escape_string($NAME, $dbHandler), date('Y-m-d H:i:s')), $dbHandler) or die(mysql_error($dbHandler));

$testID = mysql_insert_id($dbHandler);

echo "[*] Updating db with test ID: ".$testID."\n";

foreach($attacksDB as $link=>$id){

  if(in_array($link, $attacks)){
      $query = sprintf("INSERT INTO hits (scannerID, attackID) VALUES (%d, %d)", $testID, $id);
      mysql_query($query, $dbHandler) or die(mysql_error($dbHandler));
      //echo $query."\n";
  }
}

echo "[*] Done\n";

//echo implode("\n", $attacks);
//echo "\n";

closedir($dirHandler);
