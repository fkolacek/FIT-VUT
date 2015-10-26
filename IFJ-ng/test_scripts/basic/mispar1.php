<?php

$a = 5 * 5;
$b = fun(5);

$x=put_string($b);

function fun($a, $b) {
	$a = 5 * $b;
	return $b;
}
