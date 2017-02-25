var LENGHT = 10000
var x = []
var listOfSymbol = {}
var listOfConst = {}


// function dis(x0, x1, x2, t0, t1, t2) {
// 	a0 = (t0 * t1 * x2 * (t0 - t1) - t0 * t2 * x1 * (t0 - t2) + t1 * t2 * x0 * (t1 - t2)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
//     a2 = (x0 * (t1 - t2) - x1 * (t0 - t2) + x2 * (t0 - t1)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
//     a1 = (-x0 * (t1 ** 2 - t2 ** 2) + x1 * (t0 ** 2 - t2 ** 2) - x2 * (t0 ** 2 - t1 ** 2)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
//     d = (a1 + 2 * a2 * t1)
//     return d
// }

// function smooth(data, i) {
// 	if ((i > 2) && (i < (data.length - 2))){
//         d = (data[i-2] + 2*data[i-1] + 3*data[i] + 2*data[i+1] + data[i+2]) / 9.0
// 	}
//     else {
//         d = data[i]
//     }
//     return d
// }

function dis_sub(x0, x1, x2, t0, t1, t2){
    a2 = (x0 * (t1 - t2) - x1 * (t0 - t2) + x2 * (t0 - t1)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
    a1 = (-x0 * (t1 ** 2 - t2 ** 2) + x1 * (t0 ** 2 - t2 ** 2) - x2 * (t0 ** 2 - t1 ** 2)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
    d = (a1 + 2 * a2 * t1)
    return d
}

var return_dis = []
function smooth(data){
	var return_value =[]
	for (var i = 0; i < data.length; i++) {
        if (i > 2 && (i < (data.length - 2))){
            d = (data[i - 2] + 2 * data[i - 1] + 3 * data[i] + 2 * data[i + 1] + data[i + 2]) / 9.0
        }
        else if (i > 1 && (i < (data.length - 1))){
            d = (data[i - 1] + 2 * data[i] + data[i + 1]) / 4.0
        }
        else{
            d = data[i]
        }
        return_value.push(d)
    }
    return return_value
}


function dis(x, t, index){
    var temp = 0
    if(index == 0) {
    	return_dis = []
	    for (var i = 0; i < x.length; i++) {
	        if( i == 0 && x.length > 0){
	            temp = (x[1] - x[0]) / (t[1] - t[0])
	        }
	        else if ( i == (x.length -1)){
	            temp = (x[x.length - 1] - x[x.length - 2]) / (t[x.length - 1] - t[x.length - 2])
	        }
	        else{
	            temp = dis_sub(x[i - 1], x[i], x[i + 1], t[i - 1], t[i], t[i + 1])
	        }
			return_dis.push(temp)
	    }
	    console.log("before smooths : ")
	    console.log(return_dis)
	    return_dis = smooth(return_dis)
	    console.log("after smooths: ")
	    console.log(return_dis)
	}
	return return_dis[index]
}
function value(symbol, index) {

	/** variables */
	if (symbol in listOfSymbol) {
		var s = listOfSymbol[symbol]
		if (Array.isArray(s))
			return s[index]
		else
			return s
	}

	/** function */
	else if (symbol.includes('_')) {
		var tmp = symbol.split('_')
		var fun = tmp[0]
		symbol = tmp[1]

		if (symbol in listOfSymbol) {

			if (fun == 'inf') {
				return listOfSymbol[symbol].slice(0, index).reduce(
						function sum(a, b) {
							return a + b
						}, 0)
			}

			else if (fun == 'dis') {
				return dis(listOfSymbol[symbol], recordId,index)

			}

			else {
				console.log('unknown function')
				throw 'unknown function'
				return -1
			}
		} else {
			console.log('unknown function')
			throw 'unknown function'
			return -1
		}
	}

	/** Error */
	else {
		console.log('unknown symbol')
		throw 'unknown symbol'
		return -1
	}
}

function init() {
	for (var i = 0; i < LENGHT; i++) {
		x.push(math.sin(i * 1.0 / 10.0))
	}
	listOfSymbol['x'] = x
}


function filter(f, newVal){
	f.p = f.p + f.q;
	f.k = f.p / (f.p + f.r);
	f.x = f.x + f.k * (newVal - f.x);
	f.p = (1 - f.k) * f.p;
	return f.x;
}

function compute(equation) {
	console.log("compute")
	var node = math.parse(equation);
	var filtered = node.filter(function(node) {
		return node.isSymbolNode
	});

	var ret = []
	for (var i = 0; i < LENGHT; i++) {
		var scope = {}
		filtered.forEach(function myFunction(item, index) {
			scope[item.name] = value(item.name, i)
		})

		var ans = node.eval(scope)
		ans = parseFloat(ans)
		if (!isNaN(ans))
      		ret.push(ans)
	}
	return ret

}
String.prototype.replaceAt = function(index, character) {
	return this.substr(0, index) + character + this.substr(index + 1);
}

function replaceCustomFunc(ex, func) {
	var index = ex.indexOf(func + '(')
	while (index != -1) {
		var indexOpen = index + func.length;
		ex = ex.replaceAt(indexOpen, '_')
		var nextClose = ex.indexOf(')', index)
		ex = ex.replaceAt(nextClose, '')

		if (func == 'const') {
			var s = ex.substring(indexOpen + 1, nextClose)
			if (!(s in listOfConst)) {
				listOfConst[s] = 1
			}
			listOfSymbol['const_' + s] = listOfConst[s]
		}

		/* Check syntax */
		var insideFunc = ex.substring(indexOpen + 1, nextClose)
		try {
			var node = math.parse(insideFunc);
			var filtered = node.filter(function(node) {
				return node.isSymbolNode || node.type == ConstantNode
						|| node.type == FunctionNode
			});
			if (filtered.length != 1)
				throw 'Must be only one symbol inside inf, dis and const function'
		} catch (e) {
			throw 'Must be only one symbol inside inf, dis and const function'
		}

		/* next function */
		index = ex.indexOf(func + '(')
	}

	return ex
}

function formatExpression(ex) {
	ex = ex.replace(/\s/g, '')
	ex = replaceCustomFunc(ex, 'inf')
	ex = replaceCustomFunc(ex, 'dis')
	ex = replaceCustomFunc(ex, 'const')
	return ex
}

function main() {
	init()
	var ex = "inf(x) + const(A) - const(AA)"
	ex = formatExpression(ex)
	console.log(ex)
	var result = compute(ex)
	console.log(result)
	console.log(listOfConst)

}
