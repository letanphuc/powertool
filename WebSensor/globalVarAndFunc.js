         // Global Variables
    var arrayColorForLiveView =["#ff0000", "#00ff00", "#0000ff", "#000000", "#ffff00", "#ff8c1a", "#660066"];
    var liveViewSelected =[];
    var maginData = [0,0,0,0,0,0,0];
    var focusTab = 0;         //
    var dpsForLiveView = []; // dataPoints
    var matrix = [];     // content values of table
    var recordId = [];  // content record id
    var totalValueRow = 0;    // total rows of table (get from database)
    var tableLable =[];       // content lable of table include RECORDID
    var columnInfo =[];       // show information of column which is sellected
    var columnSelected = 0;   // ID of sellected column
    var tabID = 0;            // iD of sellected graph
    var arrayGraphValue =[];  // to get value from add graph dialog to Graph Info
    var GraphInfo = [];       // show info of graph
    var tabIndex = 0;
    var totalSensor = 0
        //init value
        for (i = 0; i < totalSensor; i++) { 
          liveViewSelected[i] = true;
        }
        function updateLiveViewValues () {
          for (i = 0; i < realtimeSensorInfo.length; i++) { 
            liveViewSelected[i] = document.getElementById('liveViewCheckBox'+i).checked;
          }
        }
    function resizeIframe(obj) {
      obj.style.height = obj.contentWindow.document.body.scrollHeight + 'px';
    }
	function reCompute(index){
		var newColumnValues = []
		listOfSymbol = {}
		listOfConst = columnInfo[index].ConstantArray
		LENGHT = totalValueRow
		tableLable.forEach(function myFunction(item, index) {
			listOfSymbol[item.replace(/\s/g, '')] = matrix[index]
		})
		listOfSymbol['time'] = recordId;

		try{
			var ex = formatExpression(columnInfo[index].Equalition)
			console.log(ex)
			console.log(listOfConst)
			newColumnValues = compute(ex)
			console.log(newColumnValues)

		}
		catch(e){
			console.log(e)
			alert("Syntax error : " + e.message);
		}

		return newColumnValues

	}

	function updateColumn(index)
	{
		var newArray = reCompute(index);
		console.log(newArray);
		matrix[index] = newArray;
		var iframe = document.getElementById('showData');
		var innerDoc = iframe.contentDocument || iframe.contentWindow.document;
		var selectRow;
		var table = innerDoc.getElementById("dataTableID")
		for(var i = 1; i <= totalValueRow; i++)
		{
			selectRow = table.rows[i];
			selectRow.cells[index+1].innerHTML = newArray[i - 1];
		}
	}

	function updateColumnFromGraph(index)
	{
		var realindex	
		if($.type(index) === "string")
			realindex = parseInt(index, 10)
		else
			realindex = index
				   
		
		var newArray = reCompute(realindex);
		console.log(newArray);
		matrix[realindex] = newArray;
		
		var iframe = document.getElementById('showData');
		var innerDoc = iframe.contentDocument || iframe.contentWindow.document;
		var table = innerDoc.getElementById("dataTableID")
		
		for(var i = 1; i <= totalValueRow; i++)
		{
			var selectRow = table.rows[i]
			var cell = selectRow.cells[realindex + 1]
			cell.innerHTML = newArray[i - 1]
		}
	}
	function updateInfoGraph()
	{
		$( "#infoLiveView" ).load( "infoNewGraph.html" );
	}
  function reloadInfoColumn () {
    $( "#infoLiveView" ).load( "Table/infoColumn.html" );
  }