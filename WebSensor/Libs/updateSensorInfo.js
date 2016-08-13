var realtimeSensorInfo = []
var localSensorInfo = []
var hightlightIndex = 0;
var foundHighligh = false
var MAX_AVAILABLE_SENSORS = 4


function updateRealtimeSensorInfo(){
	
	// console.log(realtimeSensorInfo);
	// console.log(localSensorInfo)
	if(localSensorInfo.length != realtimeSensorInfo.length)
	{
		console.log(" update localSensorInfo ============>")
		localSensorInfo.length = 0
		for (i = 0; i < realtimeSensorInfo.length ; i++) {
			localSensorInfo.push(realtimeSensorInfo[i])
		}
		document.getElementById("sensorName").value = localSensorInfo[hightlightIndex].sensorName;
		document.getElementById("sensorType").value = localSensorInfo[hightlightIndex].sensorType;
		document.getElementById("sensorDescription").value = localSensorInfo[hightlightIndex].sensorDescription;
	}


	foundHighligh = false

	for (i = 0; i < realtimeSensorInfo.length ; i++) {
		document.getElementById("Sensor" + i).style.backgroundColor = "blue";
		document.getElementById("Sensor" + i).disabled = false
		document.getElementById("Sensor" + i).value = "" + realtimeSensorInfo[i].sensorID;
		
		// Set red colour for highligh sensor
		if (hightlightIndex == i){
			document.getElementById("Sensor" + i).style.backgroundColor = "red";
		}
	}
	for (i; i < MAX_AVAILABLE_SENSORS ; i++) {
		document.getElementById("Sensor" + i).style.backgroundColor = "gray";
		document.getElementById("Sensor" + i).disabled = true
		document.getElementById("Sensor" + i).value = ""
	}
	if(focusTab == 0)
		$( "#infoLiveView" ).load( "infoLiveView.html");
	
}


function onLoadSensorInfo(){
	console.log("dddddd")
	websocketUpdateSensor = new WebSocket("ws://" + window.location.host + ":5439/");

	function initUpdateSensor() {
		console.log("initUpdateSensor")
		
		websocketUpdateSensor.onopen = function(evt) {
			console.log(open)
		};
		websocketUpdateSensor.onclose = function(evt) {
			console.log(close)
		};
		websocketUpdateSensor.onmessage = function(evt) {
			/** update sensor info */
			var res = evt.data.split("|");
			realtimeSensorInfo.length = 0
			for (var i = 0; i < res.length; i ++)
			{
				var data = res[i].split(",")
				if (data.length == 2)
				{
					realtimeSensorInfo.push({
						sensorID: parseInt(data[0]),
						sensorName: "Sensor"+parseInt(data[0]),
						sensorType: data[1],
						sensorDescription: ""
					})
				}
				else
				{
					console.log("Data invalid:" + res)
				}
			}

			console.log(realtimeSensorInfo)

			updateRealtimeSensorInfo()

		};
		websocketUpdateSensor.onerror = function(evt) {
			console.log(error)
		};
	}
	initUpdateSensor()
	setInterval(function() {
			websocketUpdateSensor.send("sensor");
		},
		3000
	);


	$(document).ready(function() 
		{$("#Sensor0,#Sensor1,#Sensor2,#Sensor3,#Sensor4,#Sensor5,#Sensor6").click(function() {
			$(".collapse").collapse('show');
			this.style.backgroundColor = "red";
			hightlightIndex = parseInt($(this).attr('id')[$(this).attr('id').length - 1],10);
			updateRealtimeSensorInfo()
			// Set red colour for highligh sensor
			document.getElementById("Sensor" + hightlightIndex).style.backgroundColor = "red";
			document.getElementById("sensorName").value = localSensorInfo[hightlightIndex].sensorName;
			document.getElementById("sensorType").value = localSensorInfo[hightlightIndex].sensorType;
			document.getElementById("sensorDescription").value = localSensorInfo[hightlightIndex].sensorDescription;
			console.log(localSensorInfo)
		});
		$("#sensorName").change(function() {
			console.log(" call change event");
			localSensorInfo[hightlightIndex].sensorName= $(this).val();
			console.log(hightlightIndex)
			$( "#infoLiveView" ).load( "infoLiveView.html");
		});
		$("#sensorDescription").change(function() {
			console.log(" description changed");
			console.log($(this).val());
			localSensorInfo[hightlightIndex].sensorDescription = $(this).val();
		});
		
	});
}