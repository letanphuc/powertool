<?php
try {
	// open the database
	$db = new PDO ( 'sqlite:powertool.db' );
	$result = $db->query ( 'SELECT * FROM detail' );
	$total = 0;
	print "<script>";
	print "var sensorInfo = [];";
	
	foreach ( $result as $row ) {
		$total ++;
		print "sensorInfo.push({
      	sensorID: ". $row ['sensorID'] .",
      	sensorName: \"" . $row ['sensorName'] . "\",
      	sensorType: \"" . $row ['sensorType'] . "\",
      	sensorDescription: \"\",

      });";
	}
	print "var totalSensor = " . $total . ";";
	print "</script>";
	$db = NULL;
} catch ( PDOException $e ) {
	print 'Exception : ' . $e->getMessage ();
}
?>



