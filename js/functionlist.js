<body>
<script>

'use strict';

/* scopes */

var foo = function(x) {
  var a = 10;
  let b = 1;
  if (x) {
    let a = 11;
    b = 4;
  }
  console.log(a,b);
};

//foo();
//foo(true);


/* bind */

var bar = function(x,y) {
  console.log(x,y);
  if (this) {
    console.log(this);
  }
};

//bar(1,2)
//bar.bind({foo: 1})(4);
//bar.bind({foo: 1},2)(4);
//bar.bind({foo: 1},2,3)(4);

//var baz = bar.bind(null, 1);
//baz(2,3);
//var qux = baz.bind(null, 2);
//qux();
//qux(3,4);


/* arrays */


/* cons, car, cdr */

var car = function(l) {
  return l(false);
};

var cdr = function(l) {
  return l(true);
};

var cons = function(x,y) {
  return function(sel) {
    if (sel) {
      return y;
    }
    return x;
  };
};

var nil = function() {
  throw "Nil has no head or tail";
};


var tos = function(x) {
  console.log(x, arguments);
};

</script>
</body>
