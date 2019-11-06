Module["onRuntimeInitialized"] = onRuntimeInitialized;
function onRuntimeInitialized() {
  const jump = Module.cwrap("jump", "null", []);
  const move = Module.cwrap("move", "null", ["bool"]);
  document.addEventListener("keydown", e => {
    if (e.code == "Space") {
      jump();
    }
    if (e.code == "ArrowLeft") {
      move(true);
    }
    if (e.code == "ArrowRight") {
      move(false);
    }
  });
}
