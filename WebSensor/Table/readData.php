
<?php
  try
  {
    //open the database
    $db = new PDO('sqlite:../powertool.db');
    $result = $db->query('SELECT * FROM detail');
    $totalSensorAvailable = 0;
    $totalValueRow = 0;
    print "<script>";
    print "parent.matrix = []; ";
    print "parent.recordId = [];";

    foreach($result as $row)
    {
      print "var value".$totalSensorAvailable." = new Array();";

      print "parent.matrix[".$totalSensorAvailable."] = value".$totalSensorAvailable.";";
      $sensorIDarray[$totalSensorAvailable]=$row['sensorID'];
      $totalSensorAvailable++;
    }


    $result = $db->query('SELECT * FROM data');
    foreach($result as $row)
    {
      print "parent.recordId.push(".$row['recordID'].");"; 
      for($i = 0; $i < $totalSensorAvailable; $i++)
      {
        $value = "value";
        $value.=$sensorIDarray[$i];
        // $value = $row['.$value.'];
        print "value".$i.".push(parseFloat((".$row[$value]."- parent.maginData[".$i."]).toFixed(4)));";
      }
      $totalValueRow++;
    }
    print "parent.totalSensorAvailable = ".$totalSensorAvailable.";";
    print "parent.totalValueRow = ".$totalValueRow.";";
    print "</script>";
    $db = NULL;
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }
?>
