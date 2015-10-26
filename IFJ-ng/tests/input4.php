<?php

	function fce1(){
		//parser.c 599
		//$a;
	
		//$b = $a + 1;
	
	
		//return $b + 1;
	}
	
	function fce2($text){
		return $text;
	}

	
	$x = fce1();
	
	$x = fce2(" svete");
	
	$y = intval("ahoj svete");
	
	// parser.c 642
	//$test = "agoj";
	
	$x = 1;
	
	if($x === 1){
		$x = 1 + 1;
	}
	else{
		$x = 2.0;
	}

	
	$i = 0;
	
	while($i < 10){
	
		$i = $i + 1;
	}