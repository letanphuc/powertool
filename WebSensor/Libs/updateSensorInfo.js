var realtimeSensorInfo = []
var localSensorInfo = []
var hightlightIndex = 0
var tmpIndex = 0
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
		hightlightIndex = localSensorInfo[tmpIndex].sensorID
		document.getElementById("sensorName").value = localSensorInfo[tmpIndex].sensorName;
		document.getElementById("sensorType").value = localSensorInfo[tmpIndex].sensorType;
		document.getElementById("sensorDescription").value = localSensorInfo[tmpIndex].sensorDescription;
	}


	foundHighligh = false
	
	for (i; i < MAX_AVAILABLE_SENSORS ; i++) {
		document.getElementById("Sensor" + i).style.backgroundColor = "gray";
		document.getElementById("Sensor" + i).disabled = true
		document.getElementById("Sensor" + i).value = ""
	}

	for (i = 0; i < realtimeSensorInfo.length ; i++) {
		index = localSensorInfo[i].sensorID
		document.getElementById("Sensor" + index).style.backgroundColor = "blue";
		document.getElementById("Sensor" + index).disabled = false
		document.getElementById("Sensor" + index).value = "" + index;
		
		// Set red colour for highligh sensor
		if (hightlightIndex == index){
			document.getElementById("Sensor" + hightlightIndex).style.backgroundColor = "red";
		}
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
			for (tmpIndex = 0; i < localSensorInfo.length ; tmpIndex++) {
				if (localSensorInfo[tmpIndex].sensorID == hightlightIndex)
					break
			}
			hightlightIndex = localSensorInfo[hightlightIndex].sensorID
			updateRealtimeSensorInfo()
			// Set red colour for highligh sensor
			document.getElementById("Sensor" + hightlightIndex).style.backgroundColor = "red";
			document.getElementById("sensorName").value = localSensorInfo[tmpIndex].sensorName;
			document.getElementById("sensorType").value = localSensorInfo[tmpIndex].sensorType;
			document.getElementById("sensorDescription").value = localSensorInfo[tmpIndex].sensorDescription;
			console.log(localSensorInfo)
		});
		$("#sensorName").change(function() {
			console.log(" call change event");
			localSensorInfo[tmpIndex].sensorName= $(this).val();
			console.log(tmpIndex)
			$( "#infoLiveView" ).load( "infoLiveView.html");
		});
		$("#sensorDescription").change(function() {
			console.log(" description changed");
			console.log($(this).val());
			localSensorInfo[tmpIndex].sensorDescription = $(this).val();
		});
		
	});
}