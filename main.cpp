#include <math.h>
#include <emscripten/html5.h>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <iostream>

using namespace std;

extern "C"
{
  double speed[] = {10.0, 2.0};
  double top_speed = 0.0;
  double position[] = {0.0, 700.0};
  double previous_position[] = {0.0, 700.0};
  bool perform_jump = false;
  double get_position_x()
  {
    return position[0];
  }
  double get_position_y()
  {
    return position[1];
  }

  double get_random(double min, double max)
  {
    srand(time(NULL));
    return rand() % (int)(max - min) + max;
  }

  double calculate_speed(double old_speed, int current_position, int window_width, bool gravity)
  {
    const double ACCELERATION = 1;
    const double BOUNCE_RATE = 0.6;
    double new_speed = old_speed;
    if (gravity)
    {
      new_speed += ACCELERATION;
    }
    if (current_position > window_width)
    {
      new_speed = -(abs(old_speed)) * BOUNCE_RATE;
    }
    if (current_position < 0)
    {
      new_speed = abs(old_speed) * BOUNCE_RATE;
    }
    return new_speed;
  }
  int render(double time, void *user_data)
  {
    speed[0] = calculate_speed(speed[0], position[0], 1340, false);
    speed[1] = calculate_speed(speed[1], position[1], 721, true);
    if (perform_jump == true && position[1] >= 720)
    {
      speed[1] = -30;
      perform_jump = false;
    }
    position[0] += speed[0];
    position[1] += speed[1];
    emscripten_request_animation_frame(render, 0);
    return 0;
  }
  void jump()
  {
    if (position[1] >= 500 && speed[1]>=-1)
    {
      perform_jump = true;
    }
  }
  void move(bool left, int level)
  {
    if (left)
    {
      speed[0] -= 2.0+level*0.1;
    }
    else
    {
      speed[0] += 2.0+level*0.1;
    }
  }
  int main(int argc, char const *argv[])
  {
    emscripten_request_animation_frame(render, 0);
    return 0;
  }
}
