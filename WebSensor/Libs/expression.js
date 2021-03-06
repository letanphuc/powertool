var LENGHT = 100
var x = []
var listOfSymbol = {}
var listOfConst = {}

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
				if (index == 0)
					index = 1
				return (listOfSymbol[symbol][index] - listOfSymbol[symbol][index - 1])

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
	var KMfilter = {
		q:1.0,
		r:15.0,
		x:0.0,
		p:1.0,
		k:0.0,
	}
	for (var i = 0; i < LENGHT; i++) {
		var scope = {}
		filtered.forEach(function myFunction(item, index) {
			scope[item.name] = value(item.name, i)
		})

		var ans = node.eval(scope)
		ans = parseFloat(ans.toFixed(4))
		if (i == 0){
			KMfilter.x = ans
		}
		else {
			ans = filter(KMfilter, ans)
		}
		if (!isNaN(ans))
      ret.push(math.round(ans,4))
	}
  ret.shift()
  ret.shift()
  ret.shift()
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
