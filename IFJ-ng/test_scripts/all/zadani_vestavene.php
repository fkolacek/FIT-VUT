<?php
/* Program 3: Prace s retezci a vestavenymi funkcemi */
function MyMain($str)
{
$str1 = $str;
$str2 = $str1 . ", ktery jeste trochu obohatime";
$x = put_string($str1, "\n", $str2, "\n");
$p = find_string($str2, "text");
$x = put_string("Pozice retezce \"text\" v \$str2: ", $p, "\n");
$x = put_string("Zadejte posloupnost vsech malych pismen a-h, ");
$x = put_string("aby se pismena v posloupnosti neopakovala:\n");
$str1 = get_string();
$str2 = sort_string($str1);
while ($str2 !== "abcdefgh") {
$x = put_string("Spatne zadana posloupnost, zkuste znovu:\n");
$str1 = get_string();
$str2 = sort_string($str1);
}
return 0;
}
$foo = MyMain("Toto je nejaky text v programu jazyka IFJ13");
