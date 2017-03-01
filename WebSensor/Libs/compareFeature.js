
function addFunctionCompare()
{
	listOfSymbol = {}
	listOfConst = {}
	LENGHT = parent.recordId.length
	parent.tableLable.forEach(function myFunction(item, index) {
		listOfSymbol[item.replace(/\s/g, '')] = parent.matrix[index]
	})
	listOfSymbol['time'] = parent.recordId;


	try{
		if(useSuggestion)
		{
			listOfConst['A'] = 	document.getElementById("constAValue").value 
			listOfConst['B'] = 	document.getElementById("constBValue").value 
			listOfConst['C'] = 	document.getElementById("constCValue").value 
			listOfConst['D'] = 	document.getElementById("constDValue").value 
		}
		var ex = formatExpression(NewGraphEquation.value)
		console.log(ex)
		console.log(listOfConst)
		newColumnValues = compute(ex)
		console.log(newColumnValues)
		parent.GraphInfo[parent.tabIndex].funtionCompare = NewGraphEquation.value
		parent.GraphInfo[parent.tabIndex].valueCompare = newColumnValues
		parent.GraphInfo[parent.tabIndex].constantCompare = listOfConst
			// document.location.href = document.location.href;
			initData()
			chartRender()
			parent.updateInfoGraph()

		}
		catch(e){
			console.log(e)
			alert("Syntax error : " + e.message);
		}

	}

	function suggestionSinFun(arr)
	{
		var resultSuggestion = parent.suggestSinExpression(arr)
		document.getElementById("NewGraphEquation").value = resultSuggestion.expression
		document.getElementById("constAValue").value = resultSuggestion.A
		document.getElementById("constBValue").value = resultSuggestion.B
		document.getElementById("constCValue").value = resultSuggestion.C
		document.getElementById("constDValue").value = resultSuggestion.D
		document.getElementById("displayForAB").style.display ="block"
		document.getElementById("displayForCD").style.display ="block"
		useSuggestion = true
	}
	function suggestionSinFunY1()
	{
		suggestionSinFun(parent.matrix[parent.GraphInfo[parent.tabIndex].Y1])
	}
	function suggestionSinFunY2()
	{
		suggestionSinFun(parent.matrix[parent.GraphInfo[parent.tabIndex].Y2])
	}
	function suggestionSinFunY3()
	{
		suggestionSinFun(parent.matrix[parent.GraphInfo[parent.tabIndex].Y3])
	}

	var useSuggestion = false
	function openCompareForm()
	{
		useSuggestion = false
		document.getElementById("displayForAB").style.display ="none"
		document.getElementById("displayForCD").style.display ="none"
		if(parent.GraphInfo[parent.tabIndex].Y1 == -1){
			document.getElementById("suggestionForY1").style.display = "none";
		}
		if(parent.GraphInfo[parent.tabIndex].Y2 == -1){
			document.getElementById("suggestionForY2").style.display = "none";
		}
		if(parent.GraphInfo[parent.tabIndex].Y3 == -1){
			document.getElementById("suggestionForY3").style.display = "none";
		}
		$("#compareDialog").dialog("open");
	}
	$("#NewGraphEquation").change(function() {
		if(useSuggestion)
		{
			useSuggestion = false
			document.getElementById("displayForAB").style.display ="none"
			document.getElementById("displayForCD").style.display ="none"		
		}
	});