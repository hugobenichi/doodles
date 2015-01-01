<header>
  <style>
    canvas {
      border: 1px solid black;
    }
  </style>
  <script src='geom.js'></script>
</header>
<body>
  <canvas id='cnv' width='400px' height='400px'>

<script>

var main = function() {

  var cnv = document.getElementById('cnv');
  var ctx = cnv.getContext('2d');


  var redraw = function() {
  };

  redraw();
  cnv.addEventListener('click', redraw);
};

main();

</script>
</body>
