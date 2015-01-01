// assume there is a function node() that creates new points

var dist = function(a,b) {
  dx = a.x - b.x;
  dy = a.y - b.y;
  return Math.sqrt(dx*dx + dy*dy);
};

var equi = function(a,b) {
  var dx = b.x - a.x;
  var dy = b.y - a.y;
  var d = Math.sqrt(dx*dx + dy*dy);

  //var v = node(dx/d, dy/d);
  var v = {x: dx/d, y: dy/d};
  var u = rot(v, Math.PI/3);

  return node(a.x + d*u.x, a.y + d*u.y);
};

var rot = function(v, angle) {
  var c = Math.cos(angle);
  var s = Math.sin(angle);
  //return node(c*v.x + s*v.y, c*v.y - s*v.x);
  return {x: c*v.x + s*v.y, y: c*v.y - s*v.x};
};

var scaler = function(center, alpha) {
  return function(n) {
    var dx = n.x - center.x;
    var dy = n.y - center.y;
    n.x = center.x + alpha * dx;
    n.y = center.y + alpha * dy;
    return n;
  };
};

var scaleAll = function(sclr, n) {
  while (n) {
    sclr(n);
    n = n.next;
  }
};
