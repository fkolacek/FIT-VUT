<?php

	$fileHandler = fopen("ims.csv", "r") or die("Cannot open input file");
	
	$keys = Array();
	$values = Array(
		'honey' => Array(),
		'consumed' => Array(),
		'total' => Array(),
	
	);
	
	while($line = fgets($fileHandler)){
		$parts = explode(";", $line);
		$values['honey'][] = $parts[2];
		$values['consumed'][] = $parts[3];
		$values['total'][] = $parts[2] + $parts[3];
		$keys[] = $parts[1];
		
	}

	fclose($fileHandler);

?>
<!DOCTYPE HTML>
<html>
<head>  
	<script type="text/javascript">
	window.onload = function () {
		var chart = new CanvasJS.Chart("chartContainer", {
						exportEnabled: true,
			exportFileName: "honey",
			zoomEnabled: false,
			title:{	text: "Honey production"	},
			axisY2:{/* valueFormatString:"0.0 bn", maximum: 1.2, interval: .2, */
				interlacedColor: "#F5F5F5", gridColor: "#D7D7D7", tickColor: "#D7D7D7", valueFormatString: "# kg",							
			},
            theme: "theme2", toolTip:{ shared: true },
			legend:{ verticalAlign: "bottom", horizontalAlign: "center", fontSize: 15, fontFamily: "Lucida Sans Unicode" },
			data: [
			{        
				type: "line",
				lineThickness:3,
				axisYType:"secondary",
				showInLegend: true,           
				name: "Produced", 
				dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['total'][$i]." }, "; ?>


				]
			},
			{        
				type: "line",
				lineThickness:3,
				axisYType:"secondary",
				showInLegend: true,           
				name: "Consumed", 
				dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['consumed'][$i]." }, "; ?>


				]
			},
			{        
				type: "line",
				lineThickness:3,
				axisYType:"secondary",
				showInLegend: true,           
				name: "Amount", 
				dataPoints: [
				<?php for($i = 0; $i < count($keys); $i++) echo "{ label: '".$keys[$i]."', y: ".$values['honey'][$i]." }, "; ?>


				]
			},
			
			],
          legend: { cursor:"pointer", itemclick : function(e) { if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) { e.dataSeries.visible = false; } else { e.dataSeries.visible = true; } chart.render(); } }
        });

chart.render();
}
</script>
  <script type="text/javascript" src="jquery.canvasjs.min.js"></script>
  <script type="text/javascript" src="canvasjs.min.js"></script>
<body>
	<div id="chartContainer" style="height: 300px; width: 100%;">
	</div>
</body>
</html>
