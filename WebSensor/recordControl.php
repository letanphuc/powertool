<?php 

	
	if (strcmp($_POST["action"] , "start") == 0)
	{
		/* Creat a socket then connect, send start record */
		$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($socket === false) {
		    echo "-1";
		}
		else {
			$msg = "start record";
			$result = socket_connect($socket, "localhost", 5438);
			socket_write($socket, $msg, strlen($msg));
			$out = socket_read($socket, 2048);
			echo $out;
		}

	}
	elseif (strcmp($_POST["action"] , "stop") == 0) 
	{

		/* Creat a socket then connect, send start record */
		$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($socket === false) {
		    echo "-1";
		}
		else {
			$msg = "stop record ".$_POST["name"];
			$result = socket_connect($socket, "localhost", 5438);
			socket_write($socket, $msg, strlen($msg));
			$out = socket_read($socket, 2048);



			try
			{
				//open the database
				$db = new PDO('sqlite:'.$out);
				$result = $db->query('SELECT * FROM detail');
				$totalSensorAvailable = 0;
				$totalValueRow = 0;
				echo "matrix = []; ";
				echo "recordId = [];";


				/* sensorInfo */
				print "sensorInfo = [];";
				foreach ( $result as $row ) {
					$total ++;
					print "sensorInfo.push({
						sensorID: ". $row ['sensorID'] .",
						sensorName: \"" . $row ['sensorName'] . "\",
						sensorType: \"" . $row ['sensorType'] . "\",
						sensorDescription: \"\",
						});";

					echo "var value".$totalSensorAvailable." = new Array();";

					echo "matrix[".$totalSensorAvailable."] = value".$totalSensorAvailable.";";
					$sensorIDarray[$totalSensorAvailable]=$row['sensorID'];
					$totalSensorAvailable++;
				}
				print "totalSensor = " . $total . ";";


				$result = $db->query('SELECT * FROM data');
				foreach($result as $row)
				{
					echo "recordId.push(".$row['recordTime'].");"; 
					for($i = 0; $i < $totalSensorAvailable; $i++)
					{
						$j = $i + 1;
						$value = "value".$j;
						echo "value".$i.".push(parseFloat((".$row[$value]."- maginData[".$sensorIDarray[$i]."]).toFixed(4)));";
					}

					$totalValueRow++;
				}
				echo "totalSensorAvailable = ".$totalSensorAvailable.";";
				echo "totalValueRow = ".$totalValueRow.";";



				$db = NULL;
			}
			catch(PDOException $e)
			{
				echo 'Exception : '.$e->getMessage();
			}
		}
	}
?>