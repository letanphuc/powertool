	var chart
	var dps =[]; // dataPoints
	var xValue
	function initData(){
		dps[0] = [];
		dps[1] = [];
		dps[2] = [];
		dps[3] = [];
		xValue = parent.GraphInfo[parent.tabIndex].X;
		console.log(xValue);
		console.log(parent.GraphInfo);
		if( xValue==-1){
			for (var i=0; i<parent.recordId.length; i++) {
				if(parent.GraphInfo[parent.tabIndex].Y1!= -1)
				{
					dps[0].push({
						x : parseFloat(parent.recordId[i]),
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y1][i]
					})
				}
				if(parent.GraphInfo[parent.tabIndex].Y2!= -1)
				{
					dps[1].push({
						x : parseFloat(parent.recordId[i]),
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y2][i]})
				}
				if(parent.GraphInfo[parent.tabIndex].Y3!= -1)
				{
					dps[2].push({
						x : parseFloat(parent.recordId[i]),
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y3][i]})
				}
				if(parent.GraphInfo[parent.tabIndex].valueCompare.length!=0){
					dps[3].push({
						x : parseFloat(parent.recordId[i]),
						y : parent.GraphInfo[parent.tabIndex].valueCompare[i]})
				}
			}
		}
		else
		{
			for (var i=0; i<parent.recordId.length; i++) {
				if(parent.GraphInfo[parent.tabIndex].Y1!= -1)
				{
					dps[0].push({
						x : parent.matrix[xValue][i],
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y1][i]
					})
				}
				if(parent.GraphInfo[parent.tabIndex].Y2!= -1)
				{
					dps[1].push({
						x : parent.matrix[xValue][i],
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y2][i]})
				}
				if(parent.GraphInfo[parent.tabIndex].Y3!= -1)
				{
					dps[2].push({
						x : parent.matrix[xValue][i],
						y : parent.matrix[parent.GraphInfo[parent.tabIndex].Y3][i]})
				}
				if(parent.GraphInfo[parent.tabIndex].valueCompare.length > 0){
					dps[3].push({
						x : parent.matrix[xValue][i],
						y : parent.GraphInfo[parent.tabIndex].valueCompare[i]})
				}
			}
		}
		dps[0]= _.sortBy(dps[0], 'x');
		dps[1]= _.sortBy(dps[1], 'x');
		dps[2]= _.sortBy(dps[2], 'x');
				var nameOfX;
		var nameOfY =[];
		var valueOfshowInLegend =[];
		if(xValue == -1){
			nameOfX = "Time"
		}
		else{
			nameOfX = parent.tableLable[xValue]
		}
		if(parent.GraphInfo[parent.tabIndex].Y1!=-1)
		{
			nameOfY.push(parent.tableLable[parent.GraphInfo[parent.tabIndex].Y1])
			valueOfshowInLegend.push(true)
		}
		else
		{
			nameOfY.push("DoNotSet")
			valueOfshowInLegend.push(false)
		}
		if(parent.GraphInfo[parent.tabIndex].Y2!=-1)
		{
			nameOfY.push(parent.tableLable[parent.GraphInfo[parent.tabIndex].Y2])
			valueOfshowInLegend.push(true)
		}
		else
		{
			nameOfY.push("DoNotSet")
			valueOfshowInLegend.push(false)
		}
		if(parent.GraphInfo[parent.tabIndex].Y3!=-1)
		{
			nameOfY.push(parent.tableLable[parent.GraphInfo[parent.tabIndex].Y3])
			valueOfshowInLegend.push(true)
		}
		else
		{
			nameOfY.push("DoNotSet")
			valueOfshowInLegend.push(false)
		}
		if(parent.GraphInfo[parent.tabIndex].valueCompare.length > 0){
			nameOfY.push("CompareValue")
			valueOfshowInLegend.push(true)
		}
		else
		{
			nameOfY.push("DoNotSet")
			valueOfshowInLegend.push(false)
		}
		chart = new CanvasJS.Chart("chartContainer2", {
			title : {
				text : parent.nameOfGraph.value
			},
			exportEnabled: true,
			zoomEnabled: true, 
			axisY:{
	        gridColor: "#f5f5f0",
	        lineColor: "#0f0f0a",
	        tickColor: "#0f0f0a",
	        labelFontColor:"#0f0f0a",
	       	includeZero: false,
	    },
	    axisX:{
	    	title: nameOfX,
	        lineColor: "#0f0f0a",
	        tickColor: "#0f0f0a",
	        labelFontColor:"#0f0f0a",
	    },
	    dataSeries:{
	    	cursor: "pointer"  
	    },
	    legend: {
	    	cursor: "pointer",
	    	fontSize: 22,
	    	itemclick: function (e) {
	    		if (typeof (e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
	    			e.dataSeries.visible = false;
	    		} else {
	    			e.dataSeries.visible = true;
	    		}

	    		e.chart.render();
	    	}
	    },
	    data : [ {
	    	legendText :  nameOfY[0],
	    	showInLegend: valueOfshowInLegend[0],
	    	type : "line",
	    	color: parent.colorForY1.value,
	    	dataPoints : dps[0]
	    }, {
	    	legendText :   nameOfY[1],
	    	showInLegend: valueOfshowInLegend[1],
	    	type : "line",
	    	color: parent.colorForY2.value,
	    	dataPoints : dps[1]
	    },
	    {
	    	legendText :   nameOfY[2],
	    	showInLegend: valueOfshowInLegend[2],
	    	type : "line",
	    	color: parent.colorForY3.value,
	    	dataPoints : dps[2]
	    }, 
	    {
	    	legendText :   nameOfY[3],
	    	showInLegend: valueOfshowInLegend[3],
	    	type : "line",
	    	dataPoints : dps[3]
	    },
	    ]
	});
	}
	window.onload = function() {
		initData()
		console.log( parent.colorForY1.value);
		console.log( parent.colorForY2.value);
		console.log( parent.colorForY3.value);
		chartRender()
	}
	function chartRender()
	{
		chart.render()
		if((parent.GraphInfo[parent.tabIndex].StartPoint.x > 0) && (parent.GraphInfo[parent.tabIndex].EndPoint.x > 0))
		{
			chart.plotAreaMouseDown(parent.GraphInfo[parent.tabIndex].StartPoint.x ,parent.GraphInfo[parent.tabIndex].StartPoint.y );
			chart.plotAreaMouseUp(parent.GraphInfo[parent.tabIndex].EndPoint.x ,parent.GraphInfo[parent.tabIndex].EndPoint.y);
		}
	}
	function updateStartPointEndPoint() {
		parent.GraphInfo[parent.tabIndex].StartPoint = chart.getStartPoint()
		parent.GraphInfo[parent.tabIndex].EndPoint = chart.getEndPoint()
	}
