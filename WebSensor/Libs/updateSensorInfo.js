var realtimeSensorInfo = []
var hightlightIndex = 0;
var foundHighligh = false
var MAX_AVAILABLE_SENSORS = 4


function updateRealtimeSensorInfo(){
	
	console.log(realtimeSensorInfo);

	

	foundHighligh = false

	for (i = 0; i < realtimeSensorInfo.length ; i++) {
		document.getElementById("Sensor" + i).style.backgroundColor = "blue";
		document.getElementById("Sensor" + i).disabled = false
		document.getElementById("Sensor" + i).value = "" + realtimeSensorInfo[i].sensorID;
		if (hightlightIndex == realtimeSensorInfo[i].sensorID){
			document.getElementById("Sensor" + i).style.backgroundColor = "red";
			document.getElementById("sensorName").value = realtimeSensorInfo[i].sensorName;
			document.getElementById("sensorType").value = realtimeSensorInfo[i].sensorType;
			document.getElementById("sensorDescription").value = realtimeSensorInfo[i].sensorDescription;
		}
	}
	for (i; i < MAX_AVAILABLE_SENSORS ; i++) {
		document.getElementById("Sensor" + i).style.backgroundColor = "gray";
		document.getElementById("Sensor" + i).disabled = true
		document.getElementById("Sensor" + i).value = ""
	}
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
						sensorName: "",
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
			hightlightIndex = this.value;
			updateRealtimeSensorInfo()
		});
		$("#sensorName").change(function() {
			console.log(" call change event");
			for (i = 0; i < realtimeSensorInfo.length ; i++) {
				if(realtimeSensorInfo[i].sensorID == hightlightIndex){
					realtimeSensorInfo[i].sensorName= $(this).val();
					$( "#infoLiveView" ).load( "infoLiveView.html");
				}
			}
		});
		$("#sensorDescription").change(function() {
			console.log(" description changed");
			console.log($(this).val());
			for (i = 0; i < realtimeSensorInfo.length ; i++) {
				if(realtimeSensorInfo[i].sensorID == hightlightIndex){
					realtimeSensorInfo[i].sensorDescription = $(this).val();
				}
			}
		});
		
	});
}