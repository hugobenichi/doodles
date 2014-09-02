document.writeln("hello world")

// testing function scopes
var foo = function () {
  var a = 5;
  var b = 7;
  var bar = function () {
    var b = 10;
    var c = 32;
    a = b + c;
  }
  document.writeln([a, b]);
  var d = bar()
  document.writeln([a, b]);
};

foo();

document.writeln([] + {})

document.writeln({} + [])

document.writeln({} + {})

document.writeln({} - {})

document.writeln(Array(16).join("wat" - 1) + ' Batman !')
