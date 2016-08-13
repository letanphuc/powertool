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
				echo "sensorInfo = [];";
				foreach ( $result as $row ) {
					$total ++;
					$sensorID = $row ['sensorID'];
					echo "sensorInfo.push({
						sensorID: ". $row ['sensorID'] .",
						sensorName: \"" . $row ['sensorName'] . "\",
						sensorType: \"" . $row ['sensorType'] . "\",
						sensorDescription: \"\",
						});";

					echo "var value".$sensorID." = new Array();";
					$totalSensorAvailable++;


					$result2 = $db->query('SELECT * FROM data');
					$totalValueRow = 0;
					foreach($result2 as $row)
					{
						echo "recordId.push(".$row['recordTime'].");"; 
						$j = $sensorID + 1;
						$value = "value".$j;
						echo "value".$sensorID.".push(parseFloat((".$row[$value]."- maginData[".$sensorID."]).toFixed(4)));";

						$totalValueRow++;
					}

					echo "matrix.push(value".$sensorID.");";


				}
				echo "totalSensor = " . $total . ";";
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