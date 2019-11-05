Module["onRuntimeInitialized"] = onRuntimeInitialized;
function onRuntimeInitialized() {
  const get_position_x = Module.cwrap("get_position_x", "number", []);
  const get_position_y = Module.cwrap("get_position_y", "number", []);
  const jump = Module.cwrap("jump", "null", []);
  const move = Module.cwrap("move", "null", ["bool"]);
  const get_random = Module.cwrap("get_random", "double", ["number", "number"]);
  let score = 0;
  let possibleMaxScore = 0;
  let cherry;
  const ctx = canvas.getContext("2d");
  document.addEventListener("keydown", e => {
    if (e.code == "Space") {
      jump();
    }
    if (e.code == "ArrowLeft") {
      move(true, score);
    }
    if (e.code == "ArrowRight") {
      move(false, score);
    }
  });
  const context2 = new (window.AudioContext ||
    window.webkitAudioContext)();
  function play(frequency,time){
    const osc = context2.createOscillator();
    osc.frequency.value = frequency; // Hz
    osc.type = "toothsaw";
    osc.connect(context2.destination);
    osc.start();
    osc.stop(context2.currentTime + time);
  }

  function render() {
    document.querySelector(".speed").textContent = `Twój wynik: ${score}`;
    document.querySelector(
      ".top-speed"
    ).textContent = `Najlepszy możliwy wynik: ${possibleMaxScore}`;
    const positionX = get_position_x();
    const positionY = get_position_y();
    ctx.clearRect(0, 0, window.innerWidth, window.innerHeight);
    if (cherry) {
      ctx.fillStyle = "red";
      ctx.fillRect(cherry[0], cherry[1], 10, 10);
      if (
        cherry[0] + 10 > positionX &&
        cherry[0] < positionX + 100 - score
      ) {
        if (
          cherry[1] + 10 > positionY + score &&
          cherry[1] < positionY + score + 100 - score
        ) {
          score++;

          if (score % 10 == 0) {
            play(3000,0.1)
            play(5000,0.05)
            play(3000,0.1)
          }else{
            play(3000,0.1)
          }
          cherry = null;
        }
      }
    }
    ctx.fillStyle = "green";
    ctx.fillRect(
      positionX,
      positionY + score,
      100 - score,
      100 - score
    );
    if (possibleMaxScore !== 100) {
      window.requestAnimationFrame(render);
    }
  }
  window.requestAnimationFrame(render);
  setInterval(() => {
    possibleMaxScore++;
    cherry = [get_random(100, 500), get_random(200, 400)];
  }, 3000);
}
