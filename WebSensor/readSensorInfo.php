<?php
  try
  {
    //open the database
    $db = new PDO('sqlite:powertool.db');
    $result = $db->query('SELECT * FROM detail');
    $total = 0;
    print "<script>";
    print "var sensorID    = new Array();";
    print "var sensorName = [];";
    print "var sensorType = [];";
    print "var sensorDescription =[];";

    foreach($result as $row)
    {
      $total++;
      print "sensorID.push(".$row['sensorID'].");"; 
      // print "document.getElementById(\"Sensor".$row['sensorID']."\").disabled = false;";
      print "sensorName[".$row['sensorID']."]=\" ".$row['sensorName']." \";";
      print "sensorType[".$row['sensorID']."]=\" ".$row['sensorType']." \";";
      print "sensorDescription[".$row['sensorID']."]=\" sameText \";";
    }
    print "var totalSensor = ".$total.";";
    print "</script>";
    $db = NULL;
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }
?>



