var LENGHT = 100
var x = []
var listOfSymbol = {}
var listOfConst = {}

function dis(x0, x1, x2, t0, t1, t2) {
	a0 = (t0 * t1 * x2 * (t0 - t1) - t0 * t2 * x1 * (t0 - t2) + t1 * t2 * x0 * (t1 - t2)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
    a2 = (x0 * (t1 - t2) - x1 * (t0 - t2) + x2 * (t0 - t1)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
    a1 = (-x0 * (t1 ** 2 - t2 ** 2) + x1 * (t0 ** 2 - t2 ** 2) - x2 * (t0 ** 2 - t1 ** 2)) / (t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)
    d = (a1 + 2 * a2 * t1)
    return d
}

function dis3(x0, x1, x2, x3, t0, t1, t2, t3) {
	    a1 = (x0 * (
    t1 ** 3 * t2 ** 2 - t1 ** 3 * t3 ** 2 - t1 ** 2 * t2 ** 3 + t1 ** 2 * t3 ** 3 + t2 ** 3 * t3 ** 2 - t2 ** 2 * t3 ** 3) - x1 * (
          t0 ** 3 * t2 ** 2 - t0 ** 3 * t3 ** 2 - t0 ** 2 * t2 ** 3 + t0 ** 2 * t3 ** 3 + t2 ** 3 * t3 ** 2 - t2 ** 2 * t3 ** 3) + x2 * (
          t0 ** 3 * t1 ** 2 - t0 ** 3 * t3 ** 2 - t0 ** 2 * t1 ** 3 + t0 ** 2 * t3 ** 3 + t1 ** 3 * t3 ** 2 - t1 ** 2 * t3 ** 3) - x3 * (
          t0 ** 3 * t1 ** 2 - t0 ** 3 * t2 ** 2 - t0 ** 2 * t1 ** 3 + t0 ** 2 * t2 ** 3 + t1 ** 3 * t2 ** 2 - t1 ** 2 * t2 ** 3)) / (
         t0 ** 3 * t1 ** 2 * t2 - t0 ** 3 * t1 ** 2 * t3 - t0 ** 3 * t1 * t2 ** 2 + t0 ** 3 * t1 * t3 ** 2 + t0 ** 3 * t2 ** 2 * t3 - t0 ** 3 * t2 * t3 ** 2 - t0 ** 2 * t1 ** 3 * t2 + t0 ** 2 * t1 ** 3 * t3 + t0 ** 2 * t1 * t2 ** 3 - t0 ** 2 * t1 * t3 ** 3 - t0 ** 2 * t2 ** 3 * t3 + t0 ** 2 * t2 * t3 ** 3 + t0 * t1 ** 3 * t2 ** 2 - t0 * t1 ** 3 * t3 ** 2 - t0 * t1 ** 2 * t2 ** 3 + t0 * t1 ** 2 * t3 ** 3 + t0 * t2 ** 3 * t3 ** 2 - t0 * t2 ** 2 * t3 ** 3 - t1 ** 3 * t2 ** 2 * t3 + t1 ** 3 * t2 * t3 ** 2 + t1 ** 2 * t2 ** 3 * t3 - t1 ** 2 * t2 * t3 ** 3 - t1 * t2 ** 3 * t3 ** 2 + t1 * t2 ** 2 * t3 ** 3)
    a2 = (-x0 * (t1 ** 3 * t2 - t1 ** 3 * t3 - t1 * t2 ** 3 + t1 * t3 ** 3 + t2 ** 3 * t3 - t2 * t3 ** 3) + x1 * (
    t0 ** 3 * t2 - t0 ** 3 * t3 - t0 * t2 ** 3 + t0 * t3 ** 3 + t2 ** 3 * t3 - t2 * t3 ** 3) - x2 * (
          t0 ** 3 * t1 - t0 ** 3 * t3 - t0 * t1 ** 3 + t0 * t3 ** 3 + t1 ** 3 * t3 - t1 * t3 ** 3) + x3 * (
          t0 ** 3 * t1 - t0 ** 3 * t2 - t0 * t1 ** 3 + t0 * t2 ** 3 + t1 ** 3 * t2 - t1 * t2 ** 3)) / (
         t0 ** 3 * t1 ** 2 * t2 - t0 ** 3 * t1 ** 2 * t3 - t0 ** 3 * t1 * t2 ** 2 + t0 ** 3 * t1 * t3 ** 2 + t0 ** 3 * t2 ** 2 * t3 - t0 ** 3 * t2 * t3 ** 2 - t0 ** 2 * t1 ** 3 * t2 + t0 ** 2 * t1 ** 3 * t3 + t0 ** 2 * t1 * t2 ** 3 - t0 ** 2 * t1 * t3 ** 3 - t0 ** 2 * t2 ** 3 * t3 + t0 ** 2 * t2 * t3 ** 3 + t0 * t1 ** 3 * t2 ** 2 - t0 * t1 ** 3 * t3 ** 2 - t0 * t1 ** 2 * t2 ** 3 + t0 * t1 ** 2 * t3 ** 3 + t0 * t2 ** 3 * t3 ** 2 - t0 * t2 ** 2 * t3 ** 3 - t1 ** 3 * t2 ** 2 * t3 + t1 ** 3 * t2 * t3 ** 2 + t1 ** 2 * t2 ** 3 * t3 - t1 ** 2 * t2 * t3 ** 3 - t1 * t2 ** 3 * t3 ** 2 + t1 * t2 ** 2 * t3 ** 3)
    a3 = (x0 * (t1 ** 2 * t2 - t1 ** 2 * t3 - t1 * t2 ** 2 + t1 * t3 ** 2 + t2 ** 2 * t3 - t2 * t3 ** 2) - x1 * (
    t0 ** 2 * t2 - t0 ** 2 * t3 - t0 * t2 ** 2 + t0 * t3 ** 2 + t2 ** 2 * t3 - t2 * t3 ** 2) + x2 * (
          t0 ** 2 * t1 - t0 ** 2 * t3 - t0 * t1 ** 2 + t0 * t3 ** 2 + t1 ** 2 * t3 - t1 * t3 ** 2) - x3 * (
          t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2)) / (
         t0 ** 3 * t1 ** 2 * t2 - t0 ** 3 * t1 ** 2 * t3 - t0 ** 3 * t1 * t2 ** 2 + t0 ** 3 * t1 * t3 ** 2 + t0 ** 3 * t2 ** 2 * t3 - t0 ** 3 * t2 * t3 ** 2 - t0 ** 2 * t1 ** 3 * t2 + t0 ** 2 * t1 ** 3 * t3 + t0 ** 2 * t1 * t2 ** 3 - t0 ** 2 * t1 * t3 ** 3 - t0 ** 2 * t2 ** 3 * t3 + t0 ** 2 * t2 * t3 ** 3 + t0 * t1 ** 3 * t2 ** 2 - t0 * t1 ** 3 * t3 ** 2 - t0 * t1 ** 2 * t2 ** 3 + t0 * t1 ** 2 * t3 ** 3 + t0 * t2 ** 3 * t3 ** 2 - t0 * t2 ** 2 * t3 ** 3 - t1 ** 3 * t2 ** 2 * t3 + t1 ** 3 * t2 * t3 ** 2 + t1 ** 2 * t2 ** 3 * t3 - t1 ** 2 * t2 * t3 ** 3 - t1 * t2 ** 3 * t3 ** 2 + t1 * t2 ** 2 * t3 ** 3)
    a0 = (t0 * t1 * t2 * x3 * (
    t0 ** 2 * t1 - t0 ** 2 * t2 - t0 * t1 ** 2 + t0 * t2 ** 2 + t1 ** 2 * t2 - t1 * t2 ** 2) - t0 * t1 * t3 * x2 * (
          t0 ** 2 * t1 - t0 ** 2 * t3 - t0 * t1 ** 2 + t0 * t3 ** 2 + t1 ** 2 * t3 - t1 * t3 ** 2) + t0 * t2 * t3 * x1 * (
          t0 ** 2 * t2 - t0 ** 2 * t3 - t0 * t2 ** 2 + t0 * t3 ** 2 + t2 ** 2 * t3 - t2 * t3 ** 2) - t1 * t2 * t3 * x0 * (
          t1 ** 2 * t2 - t1 ** 2 * t3 - t1 * t2 ** 2 + t1 * t3 ** 2 + t2 ** 2 * t3 - t2 * t3 ** 2)) / (
         t0 ** 3 * t1 ** 2 * t2 - t0 ** 3 * t1 ** 2 * t3 - t0 ** 3 * t1 * t2 ** 2 + t0 ** 3 * t1 * t3 ** 2 + t0 ** 3 * t2 ** 2 * t3 - t0 ** 3 * t2 * t3 ** 2 - t0 ** 2 * t1 ** 3 * t2 + t0 ** 2 * t1 ** 3 * t3 + t0 ** 2 * t1 * t2 ** 3 - t0 ** 2 * t1 * t3 ** 3 - t0 ** 2 * t2 ** 3 * t3 + t0 ** 2 * t2 * t3 ** 3 + t0 * t1 ** 3 * t2 ** 2 - t0 * t1 ** 3 * t3 ** 2 - t0 * t1 ** 2 * t2 ** 3 + t0 * t1 ** 2 * t3 ** 3 + t0 * t2 ** 3 * t3 ** 2 - t0 * t2 ** 2 * t3 ** 3 - t1 ** 3 * t2 ** 2 * t3 + t1 ** 3 * t2 * t3 ** 2 + t1 ** 2 * t2 ** 3 * t3 - t1 ** 2 * t2 * t3 ** 3 - t1 * t2 ** 3 * t3 ** 2 + t1 * t2 ** 2 * t3 ** 3)

    d = (a1 + 2 * a2 * t1 + 3 * a3 * t1 ** 2)
    return d
}

function smooth(data, i) {
	if ((i > 2) && (i < (data.length - 2))){
        d = (data[i-2] + 2*data[i-1] + 3*data[i] + 2*data[i+1] + data[i+2]) / 9.0
	}
    else {
        d = data[i]
    }
    return d
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
				listOfSymbol[symbol][index] = smooth(listOfSymbol[symbol],index)
				if (index == 0) {
					index = 1 
				}
				else if (index == LENGHT) {
					index = LENGHT - 1
				}
				return dis(listOfSymbol[symbol][index-1], listOfSymbol[symbol][index], listOfSymbol[symbol][index+1], recordId[index-1], recordId[index], recordId[index+1])

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
