var LENGHT = 100
var x = []
var listOfSymbol = {}

function value(symbol, index) {

	/** variables */
	if (symbol in listOfSymbol) {
		return listOfSymbol[symbol][index]
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
				return -1
			}
		} else {
			console.log('unknown function')
			return -1
		}
	}

	/** Error */
	else {
		console.log('unknown symbol')
		return -1
	}
}

function init() {
	for (var i = 0; i < LENGHT; i++) {
		x.push(math.sin(i * 1.0 / 10.0))
	}
	listOfSymbol['x'] = x
}

function compute(equation) {
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
		index = ex.indexOf(func + '(')
	}

	return ex
}

function formatExpression(ex) {
	ex = ex.replace(/\s/g, '')
	ex = replaceCustomFunc(ex, 'inf')
	ex = replaceCustomFunc(ex, 'dis')
	return ex
}

function main() {
	init()
	var ex = "inf(x)"
	console.log(ex)
	var result = compute(ex)
	console.log(result)
}
