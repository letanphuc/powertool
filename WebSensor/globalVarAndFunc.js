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
    var totalSensor = 0;
    var needToReloadTable = false;
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
        function sum(array)
        {
			var total = 0;
			array.forEach( function (item, index){
				 total+=item;
				 });
			return total;
        }
        function suggestSinExpression(arr){
        	/** Asin(Bx + C) + D */ 
        	var result = []
        	var firsrZero = -1;
        	var secondZero = -1;
        	var A = 0;
        	var B = 0;
        	var C = 0;
        	var D = 0;

        	var min = arr[0];
        	var max = arr[0];
        	var total = sum(arr)
        	var average = total *1.0 / arr.length;
        	console.log(average)

        	for (var i = 0; i < arr.length; i ++){
        		if (arr[i] > max)
        			max = arr[i];
        		if (arr[i] < min)
        			min = arr[i];

        		if ((arr[i] - average)*(arr[i + 1]-average) < 0 && (arr[i] < average)){
        			/* Found zero */
        			if (firsrZero != -1){
        				secondZero = i;
        				B = 2.0* math.PI / (secondZero - firsrZero);
        				A = (max - min) / 2.0
        				D = (max + min) / 2.0;
        				C = math.asin(0) - B*i;
                        B = B*30;

        				break;
        			}
        			else{
        				firsrZero = i;
        			}
        		}
        	}
        	result['expression'] = "const(A)*sin(const(B)*time + const(C)) + const(D)"
            result['A'] = math.round(A,4);
            result['B'] = math.round(B,4);
            result['C'] = math.round(C,4);
            result['D'] = math.round(D,4);


        	return result;
        }

	function updateInfoGraph()
	{
		$( "#infoLiveView" ).load( "infoNewGraph.html" );
	}
  function reloadInfoColumn () {
    $( "#infoLiveView" ).load( "Table/infoColumn.html" );
  }