<?php

	$fileHandler = fopen("ims.csv", "r") or die("Cannot open input file");
	
	$keys = Array();
	$values = Array(
		'honey' => Array(),
		'workers' => Array(),
	
	);
	
	while($line = fgets($fileHandler)){
		$parts = explode(";", $line);
		$values['honey'][] = $parts[2];
		$values['dust'][] = $parts[3];
		$values['COUNTER_EGGS'][] = $parts[4];
		$values['WORKER_EGG'][] = $parts[5];
		$values['WORKER_MAGGOT'][] = $parts[6];
		$values['WORKER_CAWL'][] = $parts[7];
		$values['WORKER_CLEANER'][] = $parts[8];
		$values['WORKER_FEEDER'][] = $parts[9];
		$values['WORKER_BUILDER'][] = $parts[10];
		$values['WORKER_GUARD'][] = $parts[11];
		$values['WORKER_OUT'][] = $parts[12];
		$values['WORKER_OUT_SUMMER'][] = $parts[13];
		$values['WORKER_OUT_WINTER'][] = $parts[14];
		$values['MOTHER_EGG'][] = $parts[15];
		$values['DRONE_EGG'][] = $parts[21];
		$values['DRONE_READY'][] = $parts[25];
		$values['ACCIDENTALLY_KILLED'][] = $parts[26];
		$values['WORKER_TOTAL'][] = $parts[27];
		$keys[] = $parts[1];

	}

	fclose($fileHandler);

?>
<!DOCTYPE HTML>
<html>
<head>  
  <script type="text/javascript" src="jquery.canvasjs.min.js"></script>
  <script type="text/javascript" src="canvasjs.min.js"></script>
	<script type="text/javascript">
	window.onload = function () {
		var chart = new CanvasJS.Chart("chartContainer", {
			exportEnabled: true,
			exportFileName: "lifecycle",
			zoomEnabled: false,
			title:{	text: "Beehive lifecycle"	},
			axisY2:{/* valueFormatString:"0.0 bn", maximum: 1.2, interval: .2, */
				interlacedColor: "#F5F5F5", gridColor: "#D7D7D7", tickColor: "#D7D7D7", valueFormatString: "# pcs",							
			},
            theme: "theme2", toolTip:{ shared: true },
			legend:{ verticalAlign: "bottom", horizontalAlign: "center", fontSize: 15, fontFamily: "Lucida Sans Unicode" },
			data: [
				{name: "WORKER_EGG", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_EGG'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_MAGGOT", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_MAGGOT'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_CAWL", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_CAWL'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_CLEANER", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_CLEANER'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_BUILDER", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_BUILDER'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_FEEDER", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_FEEDER'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_OUT_SUMMER", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_OUT_SUMMER'][$i]." }, "; ?>
				]},

				{name: "WORKER_GUARD", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_GUARD'][$i]." }, "; ?>
				]},
				
				{name: "WORKER_OUT_WINTER", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['WORKER_OUT_WINTER'][$i]." }, "; ?>
				]},
				
				{name: "MOTHER_EGG", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['MOTHER_EGG'][$i]." }, "; ?>
				]},
				
				{name: "DRONE_EGG", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['DRONE_EGG'][$i]." }, "; ?>
				]},
				
				{name: "DRONE_READY", type: "line", lineThickness:3, axisYType:"secondary", showInLegend: true, dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['DRONE_READY'][$i]." }, "; ?>
				]},
			
			],
          legend: { cursor:"pointer", itemclick : function(e) { if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) { e.dataSeries.visible = false; } else { e.dataSeries.visible = true; } chart.render(); } }
        });

chart.render();
}
</script>
<script type="text/javascript">

</script>
<body>
	<div id="chartContainer" style="height: 500px; width: 100%;">
	</div>
</body>
</html>
