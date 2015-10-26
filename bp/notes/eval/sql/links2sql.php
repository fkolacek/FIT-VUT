#!/usr/bin/php

<?php

$fileHandler = fopen("links.txt", "r") or die("Cannot open input file");

$attacks = Array();

echo "use wavsep;\n";
echo "DELETE FROM attacks;\n";

while($line = fgets($fileHandler)){

  if(preg_match("#index.jsp$#", $line) == 1)
      continue;
  else if(preg_match("#^wavsep/passive/#", $line) == 1)
      continue;
  else if(preg_match("#^wavsep/(active|passive)/([^\/]+)/([^\/]+)/(.+)\.jsp$#", $line, $parts) == 1){
    list($all, $type, $category, $subcategory, $name) = $parts;

    if(!array_key_exists($type, $attacks))
        $attacks[$type] = Array();

    if(!array_key_exists($category, $attacks[$type]))
        $attacks[$type][$category] = Array();

    if(!array_key_exists($subcategory, $attacks[$type][$category]))
        $attacks[$type][$category][$subcategory] = Array();

    $attacks[$type][$category][$subcategory][$name] = $line;
    echo sprintf("INSERT INTO attacks (type, category, subcategory, name, link) VALUES ('%s', '%s', '%s', '%s', '%s');\n", $type, $category, $subcategory, $name, $all);
  }
  else
    die("DEBUG: ". $line);
}

fclose($fileHandler);
