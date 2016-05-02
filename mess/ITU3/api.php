<?php

	$remoteScript = "http://www.stud.fit.vutbr.cz/~xmlich02/itu-ajax/api.php";
	//$remoteScript = "http://projects.dev/ITU3/dump.php";
	
	$c = curl_init();
	
	curl_setopt($c, CURLOPT_URL, $remoteScript);
	curl_setopt($c, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($c, CURLOPT_FOLLOWLOCATION, 0);

	curl_setopt($c, CURLOPT_REFERER, "~xdagod1لاا");	
	//curl_setopt($c, CURLOPT_REFERER, "~xvanat01");	
	
	if(count($_GET) > 0)
		curl_setopt($c, CURLOPT_URL, $remoteScript."?".http_build_query($_GET));
	
	if(count($_POST) > 0){
		curl_setopt($c, CURLOPT_POST, count($_POST));
		curl_setopt($c, CURLOPT_POSTFIELDS, $_POST);
	}
	
	$headers = Array();
	foreach($_SERVER as $key=>$val){
		if(preg_match("/^HTTP/", $key) == 1){
				$headers[$key] = $val;
		}
	}
	
	curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
	
	echo curl_exec($c);

	curl_close($c);
