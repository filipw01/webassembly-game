Module["onRuntimeInitialized"] = onRuntimeInitialized;
function onRuntimeInitialized() {
  const jump = Module.cwrap("jump", "null", []);
  const move = Module.cwrap("move", "null", ["bool"]);
  const get_player_x = Module.cwrap("get_player_x", "number");
  const get_player_y = Module.cwrap("get_player_y", "number");
  const get_cherry_x = Module.cwrap("get_cherry_x", "number");
  const get_cherry_y = Module.cwrap("get_cherry_y", "number");
  const get_speed_x = Module.cwrap("get_speed_x", "number");
  const get_speed_y = Module.cwrap("get_speed_y", "number");
  const get_score = Module.cwrap("get_score", "number");
  
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
