#include <math.h>
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

extern "C"
{
  int possibleMaxScore = 0;
  int score = 0;
  int speedBonus = 0;
  double speed[] = {10.5, 2.0};
  int cherry[] = {600, 650};
  double position[] = {0.0, 700.0};
  bool perform_jump = false;

  void calculate_prime_factors(int number)
  {
    for (int i = 2; i <= sqrt(number); i++)
    {
      while (number % i == 0)
      {
        EM_ASM_({
          document.querySelector(".score-prime-factors").textContent = document.querySelector(".score-prime-factors").textContent + " " + $0;
        },
                i);
        number = number / i;
      }
    }

    if (number > 2)
    {
      EM_ASM_({
        document.querySelector(".score-prime-factors").textContent = document.querySelector(".score-prime-factors").textContent + " " + $0;
      },
              number);
    }
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

  void play(int frequency, float time)
  {
    EM_ASM_({
      const context2 = new (window.AudioContext ||
                            window.webkitAudioContext)();
      const osc = context2.createOscillator();
      osc.frequency.value = $0; // Hz
      osc.type = "toothsaw";
      osc.connect(context2.destination);
      osc.start();
      osc.stop(context2.currentTime + $1);
    },
            frequency, time);
  }

  void generateCherry(void *data)
  {
    cherry[0] = get_random(100, 500);
    cherry[1] = get_random(200, 400);
    possibleMaxScore++;
  }

  int tick(double time, void *user_data)
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

    EM_ASM_({
      document.querySelector(".score").textContent = "Twój wynik : " + $0;
      document.querySelector(".speed-bonus").textContent = "Bonus za prędkość : " + $1;
    },
            score, speedBonus);

    EM_ASM_({
      ctx.clearRect(0, 0, window.innerWidth, window.innerHeight);
    });

    if (cherry[0] != 0)
    {
      EM_ASM_({
        ctx.fillStyle = "red";
        ctx.fillRect($2, $3, 10, 10);
      },
              position[0], position[1], cherry[0], cherry[1]);

      if (
          cherry[0] + 10 > position[0] &&
          cherry[0] < position[0] + 100 - score / 2000)
      {
        if (
            cherry[1] + 10 > position[1] + score / 2000 &&
            cherry[1] < position[1] + score / 2000 + 100 - score / 2000)
        {
          speedBonus = speed[0] * speed[0];
          score = score + 1000 + speedBonus;
          EM_ASM_({
            document.querySelector(".score-prime-factors").textContent = "Rozkład wyniku na czynniki pierwsze : ";
          });
          calculate_prime_factors(score);
          if (speedBonus > 750)
          {
            play(3000, 0.1);
            play(5000, 0.05);
            play(3000, 0.1);
          }
          else
          {
            play(3000, 0.1);
          }

          cherry[0] = 0;
        }
      }
    }

    EM_ASM_({
      ctx.fillStyle = "green";
      ctx.fillRect($0, $1 + $2, 100 - $2, 100 - $2);
    },
            position[0], position[1], score / 2000);

    if (possibleMaxScore != 100)
    {
      emscripten_request_animation_frame(tick, 0);
    }

    return 0;
  }

  void jump()
  {
    if (position[1] >= 500 && speed[1] >= -1)
    {
      perform_jump = true;
    }
  }

  void move(bool left, int level)
  {
    if (left)
    {
      speed[0] -= 2.0 + level * 0.1;
    }
    else
    {
      speed[0] += 2.0 + level * 0.1;
    }
  }

  int main(int argc, char const *argv[])
  {
    EM_ASM_({
      ctx = canvas.getContext("2d");
    });

    emscripten_request_animation_frame(tick, 0);

    emscripten_set_interval(generateCherry, 3000.0, 0);

    return 0;
  }

  //API Getters
  double get_player_x()
  {
    return position[0];
  }

  double get_player_y()
  {
    return position[1];
  }

  int get_cherry_x()
  {
    return cherry[0];
  }

  int get_cherry_y()
  {
    return cherry[1];
  }

  double get_speed_x()
  {
    return speed[0];
  }

  double get_speed_y()
  {
    return speed[1];
  }

  int get_score()
  {
    return score;
  }

  int get_highest_possible_score()
  {
    return possibleMaxScore;
  }
}
