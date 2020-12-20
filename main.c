// gcc main.c -lsimple2d -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lGL -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o main
#include <simple2d.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//#include "userFunctions.txt"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
int menuQuit = -1;
int scoreBoard = 0;
char name[20] = " ";
char playerNames[10][20];
int playerScores[10];
FILE *scoreFile;

S2D_Window *game;
S2D_Window *end;
S2D_Window *menu;
S2D_Window *viewScores;
S2D_Window *nameInput;
S2D_Text *scoreText;
S2D_Music *bgm;
S2D_Sound *gameOverSound;
S2D_Text *title;
S2D_Text *play;
S2D_Text *quit;
S2D_Text *highScores;
S2D_Text *score1;
S2D_Text *score2;
S2D_Text *score3;
S2D_Text *quit2;
S2D_Text *inGameScore;
S2D_Image *homeBG;
S2D_Image *background;
S2D_Text *quit3;
S2D_Text *uName;
S2D_Text *enterName;

typedef struct Box
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  float r, g, b, a;
  float x_vel, y_vel;
  float x_acc, y_acc;
  float x, y;
  float vel;
}Box;
Box player;
Box destroyer;
Box chaser1;
Box chaser2;

time_t startTime, stopTime;
int score = 0;
int l = 1;

void gameInit()
{
  background = S2D_CreateImage("Images/background3.png");
  background->x = 0;
  background->y = 0;
  background->width = 1280;
  background->height = 720;

  player.x1 = WINDOW_WIDTH / 2 - 25;
  player.y1 = WINDOW_HEIGHT - 50;
  player.x2 = player.x1;
  player.y2 = player.y1 + 50;
  player.x3 = player.x1 + 50;
  player.y3 = player.y2;
  player.x4 = player.x3;
  player.y4 = player.y1;
  player.r = 0;
  player.g = 0.4;
  player.b = 1;
  player.a = 1;
  player.x_vel = 0;
  player.y_vel = 0;
  player.x_acc = 0;
  player.y_acc = 0;
  player.x = (player.x3 - player.x1) / 2;
  player.y = (player.y2 - player.y1) / 2;
  player.vel = 5;

  destroyer.x1 = WINDOW_WIDTH / 2 - 25;
  destroyer.y1 = 0;
  destroyer.x2 = destroyer.x1;
  destroyer.y2 = 50;
  destroyer.x3 = destroyer.x1 + 50;
  destroyer.y3 = destroyer.y2;
  destroyer.x4 = destroyer.x3;
  destroyer.y4 = destroyer.y1;
  destroyer.r = 1;
  destroyer.g = 0.1;
  destroyer.b = 0;
  destroyer.a = 1;
  destroyer.x_vel = 0;
  destroyer.y_vel = 0;
  destroyer.x_acc = 0;
  destroyer.y_acc = 0;
  destroyer.x = (destroyer.x3 - destroyer.x1) / 2;
  destroyer.y = (destroyer.y2 - destroyer.y1) / 2;
  destroyer.vel = 2;

  chaser1.x1 = 0;
  chaser1.y1 = WINDOW_HEIGHT / 2 - 25;
  chaser1.x2 = chaser1.x1;
  chaser1.y2 = chaser1.y1 + 50;
  chaser1.x3 = chaser1.x1 + 50;
  chaser1.y3 = chaser1.y2;
  chaser1.x4 = chaser1.x3;
  chaser1.y4 = chaser1.y1;
  chaser1.r = 1;
  chaser1.g = 0.78;
  chaser1.b = 0;
  chaser1.a = 1;
  chaser1.x_vel = 0;
  chaser1.y_vel = -2;
  chaser1.vel = 2;

  chaser2.x1 = WINDOW_WIDTH - 50;
  chaser2.y1 = WINDOW_HEIGHT / 2 - 25;
  chaser2.x2 = chaser2.x1;
  chaser2.y2 = chaser2.y1 + 50;
  chaser2.x3 = WINDOW_WIDTH;
  chaser2.y3 = chaser2.y2;
  chaser2.x4 = chaser2.x3;
  chaser2.y4 = chaser2.y1;
  chaser2.r = 1;
  chaser2.g = 0.78;
  chaser2.b = 0;
  chaser2.a = 1;
  chaser2.x_vel = 0;
  chaser2.y_vel = -2;
  chaser2.vel = 2;

  bgm = S2D_CreateMusic("Music/BGM.mp3");
  S2D_SetMusicVolume(15);

  gameOverSound = S2D_CreateSound("Music/gameOver.mp3");
  S2D_SetSoundMixVolume(100);
  S2D_SetSoundVolume(gameOverSound, 200);

  inGameScore = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 35);
  inGameScore->x = WINDOW_WIDTH / 2 - 10;
  inGameScore->y = WINDOW_HEIGHT / 2 - 50;
  inGameScore->color.r = 0;
  inGameScore->color.g = 0;
  inGameScore->color.b = 0;
  inGameScore->color.a = 1;
}

void playerMovement()
{
  player.x_vel += player.x_acc;
  player.y_vel += player.y_acc;

  player.x1 += player.x_vel;
  player.y1 += player.y_vel;
  player.x2 += player.x_vel;
  player.y2 += player.y_vel;
  player.x3 += player.x_vel;
  player.y3 += player.y_vel;
  player.x4 += player.x_vel;
  player.y4 += player.y_vel;
  player.x = player.x1 + 25;
  player.y = player.y1 + 25;

  destroyer.x = destroyer.x1 + 25;
  destroyer.y = destroyer.y2 + 25;
}

void gravityDirection()
{
  if(player.y1 > WINDOW_HEIGHT / 2 && player.y2 <= WINDOW_HEIGHT)
  {
    if(player.x1 >= 200 && player.x3 <= WINDOW_WIDTH - 200)
    {
      player.y_acc = 1;
    }
  }
  else if(player.y1 <= WINDOW_HEIGHT / 2 && player.y1 >= 0)
  {
    if(player.x1 >= 200 && player.x3 <= WINDOW_WIDTH - 200)
    {
      player.y_acc = -1;
    }
  }
  else
  {
    player.y_acc = 0;
    player.y_vel = 0;
  }
  
  if(player.x1 < 200 && player.x1 >= 0)
  {
    player.x_acc = -1;
  }
  else if(player.x3 > WINDOW_WIDTH - 200 && player.x3 <= WINDOW_WIDTH)
  {
    player.x_acc = 1;
  }
  else
  {
    player.x_acc = 0;
    player.x_vel = 0;
  }
}

void destroyerFollow()
{
  if(player.x < destroyer.x)
  {
    destroyer.x_vel = -destroyer.vel;
  }
  else if(player.x > destroyer.x)
  {
    destroyer.x_vel = destroyer.vel;
  }
  else
  {
    destroyer.x_vel = 0;
  }

  if(player.y < destroyer.y)
  {
    destroyer.y_vel = -destroyer.vel;
  }
  else if(player.y > destroyer.y)
  {
    destroyer.y_vel = destroyer.vel;
  }
  else
  {
    destroyer.y_vel = 0;
  }

  destroyer.x1 += destroyer.x_vel;
  destroyer.y1 += destroyer.y_vel;
  destroyer.x2 += destroyer.x_vel;
  destroyer.y2 += destroyer.y_vel;
  destroyer.x3 += destroyer.x_vel;
  destroyer.y3 += destroyer.y_vel;
  destroyer.x4 += destroyer.x_vel;
  destroyer.y4 += destroyer.y_vel;
}

void chaserMovement()
{
  if(chaser1.x1 <= 0 && chaser1.y1 <= 0)
  {
    chaser1.x_vel = chaser1.vel;
    chaser1.y_vel = 0;
  }
  else if(chaser1.x4 >= WINDOW_WIDTH && chaser1.y4 <= 0)
  {
    chaser1.x_vel = 0;
    chaser1.y_vel = chaser1.vel;
  }
  else if(chaser1.y3 >= WINDOW_HEIGHT && chaser1.x3 >= WINDOW_WIDTH)
  {
    chaser1.x_vel = -chaser1.vel;
    chaser1.y_vel = 0;
  }
  else if(chaser1.x2 <= 0 && chaser1.y2 >= WINDOW_HEIGHT)
  {
    chaser1.x_vel = 0;
    chaser1.y_vel = -chaser1.vel;
  }

  chaser1.x1 += chaser1.x_vel;
  chaser1.y1 += chaser1.y_vel;
  chaser1.x2 += chaser1.x_vel;
  chaser1.y2 += chaser1.y_vel;
  chaser1.x3 += chaser1.x_vel;
  chaser1.y3 += chaser1.y_vel;
  chaser1.x4 += chaser1.x_vel;
  chaser1.y4 += chaser1.y_vel;

  if(chaser2.x1 <= 0 && chaser2.y1 <= 0)
  {
    chaser2.x_vel = 0;
    chaser2.y_vel = chaser2.vel;
  }
  else if(chaser2.x4 >= WINDOW_WIDTH && chaser2.y4 <= 0)
  {
    chaser2.x_vel = -chaser2.vel;
    chaser2.y_vel = 0;
  }
  else if(chaser2.y3 >= WINDOW_HEIGHT && chaser2.x3 >= WINDOW_WIDTH)
  {
    chaser2.x_vel = 0;
    chaser2.y_vel = -chaser2.vel;
  }
  else if(chaser2.x2 <= 0 && chaser2.y2 >= WINDOW_HEIGHT)
  {
    chaser2.x_vel = chaser2.vel;
    chaser2.y_vel = 0;
  }

  chaser2.x1 += chaser2.x_vel;
  chaser2.y1 += chaser2.y_vel;
  chaser2.x2 += chaser2.x_vel;
  chaser2.y2 += chaser2.y_vel;
  chaser2.x3 += chaser2.x_vel;
  chaser2.y3 += chaser2.y_vel;
  chaser2.x4 += chaser2.x_vel;
  chaser2.y4 += chaser2.y_vel;
}

int checkCollision(Box enemy)
{
  if(player.x4 >= enemy.x2 && player.y4 <= enemy.y2 && player.x4 <= enemy.x3 && player.y4 >= enemy.y1)
  {
    return 1;
  }
  if(player.x1 <= enemy.x3 && player.y1 <= enemy.y3 && player.x1 >= enemy.x1 && player.y1 >= enemy.y1)
  {
    return 1;
  }
  if(player.x2 <= enemy.x4 && player.y2 >= enemy.y4 && player.x2 >= enemy.x2 && player.y2 <= enemy.y2)
  {
    return 1;
  }
  if(player.x3 >= enemy.x1 && player.y3 >= enemy.y1 && player.x3 <= enemy.x3 && player.y3 <= enemy.y2)
  {
    return 1;
  }
  return 0;
}

void gameRender()
{
  S2D_DrawImage(background);
  S2D_DrawQuad(player.x1, player.y1, player.r, player.g, player.b, player.a, player.x2, player.y2, player.r, player.g, player.b, player.a, player.x3, player.y3, player.r, player.g, player.b, player.a, player.x4, player.y4, player.r, player.g, player.b, player.a);
  S2D_DrawQuad(destroyer.x1, destroyer.y1, destroyer.r, destroyer.g, destroyer.b, destroyer.a, destroyer.x2, destroyer.y2, destroyer.r, destroyer.g, destroyer.b, destroyer.a, destroyer.x3, destroyer.y3, destroyer.r, destroyer.g, destroyer.b, destroyer.a, destroyer.x4, destroyer.y4, destroyer.r, destroyer.g, destroyer.b, destroyer.a);
  S2D_DrawQuad(chaser1.x1, chaser1.y1, chaser1.r, chaser1.g, chaser1.b, chaser1.a, chaser1.x2, chaser1.y2, chaser1.r, chaser1.g, chaser1.b, chaser1.a, chaser1.x3, chaser1.y3, chaser1.r, chaser1.g, chaser1.b, chaser1.a, chaser1.x4, chaser1.y4, chaser1.r, chaser1.g, chaser1.b, chaser1.a);
  S2D_DrawQuad(chaser2.x1, chaser2.y1, chaser2.r, chaser2.g, chaser2.b, chaser2.a, chaser2.x2, chaser2.y2, chaser2.r, chaser2.g, chaser2.b, chaser2.a, chaser2.x3, chaser2.y3, chaser2.r, chaser2.g, chaser2.b, chaser2.a, chaser2.x4, chaser2.y4, chaser2.r, chaser2.g, chaser2.b, chaser2.a);
  S2D_DrawText(inGameScore);
}

void gameUpdate()
{
  playerMovement();
  gravityDirection();
  destroyerFollow();
  chaserMovement();
  time(&stopTime);
  score = difftime(stopTime, startTime);
  S2D_SetText(inGameScore, "%d", score);
  if(score > 5 && score < 40 && score % 10 == 0)
  {
    player.vel += 0.01;
    destroyer.vel += 0.01;
    chaser1.vel += 0.04;
    chaser2.vel += 0.04;
  }
  if(checkCollision(destroyer) || checkCollision(chaser1) || checkCollision(chaser2))
  {
    S2D_StopMusic();
    S2D_PlaySound(gameOverSound);
    S2D_Close(game);
    S2D_FreeMusic(bgm);
    score = difftime(stopTime, startTime);
  }
}

void game_on_key(S2D_Event input)
{
  if(!strcmp(input.key, "Right"))
  {
    if(player.x3 <= WINDOW_WIDTH - 10)
    {
        switch(input.type)
        {
          case S2D_KEY_HELD:
          player.x_vel = player.vel;
          break;
          case S2D_KEY_UP:
          player.x_vel = 0;
          break;
        }
    }
    else
    {
      player.x_vel = 0;
    }
    
  }    
  if(!strcmp(input.key, "Left"))
  {
    if(player.x1 >= 10)
    {
      switch(input.type)
      {
        case S2D_KEY_HELD:
        player.x_vel = -player.vel;
        break;
        case S2D_KEY_UP:
        player.x_vel = 0;
      }
    }
    else
    {
      player.x_vel = 0;
    }
  }
  if(!strcmp(input.key, "Up"))
  {
    if(player.y1 >= 10)
    {
      switch(input.type)
      {
        case S2D_KEY_HELD:
        player.y_vel = -player.vel;
        break;
        case S2D_KEY_UP:
        player.y_vel = 0;
        break;
      }
    }
    else
    {
      player.y_vel = 0;
    }
    
  }
  if(!strcmp(input.key, "Down"))
  {
    if(player.y2 <= WINDOW_HEIGHT - 10)
    {
      switch(input.type)
      {
        case S2D_KEY_HELD:
        player.y_vel = player.vel;
        break;
        case S2D_KEY_UP:
        player.y_vel = 0;
        break;
      }
    }
    else
    {
      player.y_vel = 0;
    }
  } 
}

void endInit()
{
  scoreText = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 100);
  S2D_SetText(scoreText, "%d", score);
  scoreText->x = WINDOW_WIDTH/2 - 45;
  scoreText->y = 100;
  scoreText->color.r = 1;
  scoreText->color.g = 1;
  scoreText->color.b = 1;
  scoreText->color.a = 1;
  
  highScores = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Highscores", 50);
  highScores->x = 490;
  highScores->y = 300;
  highScores->color.r = 1;
  highScores->color.g = 1;
  highScores->color.b = 1;
  highScores->color.a = 1;

  quit2 = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Quit", 50);
  quit2->x = 575;
  quit2->y = 400;
  quit2->color.r = 1;
  quit2->color.g = 1;
  quit2->color.b = 1;
  quit2->color.a = 1; 
}

void endRender()
{
    S2D_DrawText(scoreText);
    S2D_DrawText(highScores);
    S2D_DrawText(quit2);
}

void end_on_mouse(S2D_Event mouseIn)
{
  if(end->mouse.x >= highScores->x && end->mouse.x <= highScores->x + 300 && end->mouse.y >= 300 && end->mouse.y <= 355)
  {
    if(mouseIn.button == 1)
    {
      S2D_Close(end);
      scoreBoard = 1;
    }
  }
  else if(end->mouse.x >= quit2->x && end->mouse.x <= quit2->x + 107 && end->mouse.y >= 400 && end->mouse.y <= 450)
  {
    if(mouseIn.button == 1)
    {
      S2D_Close(end);
    }
  }
}

void menuInit()
{
  title = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "KRAZY HUES", 100);
  title->x = 300;
  title->y = 100;
  title->color.r = 0;
  title->color.g = 0.1;
  title->color.b = 0.1;
  title->color.a = 1;

  play = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Play", 75);
  play->x = 550;
  play->y = 300;
  play->color.r = 0;
  play->color.g = 0.1;
  play->color.b = 0.1;
  play->color.a = 1;

  quit = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Quit", 75);
  quit->x = 550;
  quit->y = 490;
  quit->color.r = 0;
  quit->color.g = 0.1;
  quit->color.b = 0.1;
  quit->color.a = 1;

  homeBG = S2D_CreateImage("Images/background3.png");
  homeBG->x = 0;
  homeBG->y = 0;
  homeBG->width = 1280;
  homeBG->height = 720;
}

void menuRender()
{
  S2D_DrawImage(homeBG);
  S2D_DrawText(title);
  S2D_DrawText(play);
  S2D_DrawText(quit);
}

void menu_on_mouse(S2D_Event mouseIn)
{
  if(menu->mouse.x >= play->x && menu->mouse.x <= play->x + 170 && menu->mouse.y >= 300 && menu->mouse.y <= 400)
  {
    if(mouseIn.button == 1)
    {
      S2D_Close(menu);
      menuQuit = 0;
    }
  }
  else if(menu->mouse.x >= quit->x && menu->mouse.x <= quit->x + 170 && menu->mouse.y >= 490 && menu->mouse.y <= 590)
  {
    if(mouseIn.button == 1)
    {
      S2D_Close(menu);
      menuQuit = 1;
    }
  }
}

void scoreInit()
{
  scoreFile = fopen("HighScores.txt", "r");
  for(int i = 0; i < 10 && !(feof(scoreFile)); i++)
  {
    fscanf(scoreFile, "%s%d", playerNames[i], &playerScores[i]);
  }
  fclose(scoreFile);

  for(int j = 0; j < 10; j++)
  {
    if(playerScores[j] < score)
    {
      for(int k = 8; k >= j; k--)
      {
        playerScores[k + 1] = playerScores[k];
        strcpy(playerNames[k + 1], playerNames[k]);
      }
      playerScores[j] = score;
      strcpy(playerNames[j], name);
      break;
    } 
  }
  
  fopen("HighScores.txt", "w");
  fprintf(scoreFile, "\n");
  for(int j = 0; j < 10; j++)
  {
    printf("%s %d\n", playerNames[j], playerScores[j]);
    fprintf(scoreFile, "%s %d\n", playerNames[j], playerScores[j]);
  }
  fclose(scoreFile);

  highScores = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 50);
  highScores->x = 500;
  highScores->y = 200;
  highScores->color.r = 1;
  highScores->color.g = 1;
  highScores->color.b = 1;
  highScores->color.a = 1;
  S2D_SetText(highScores, "Highscores");

  score1 = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 35);
  score1->x = 450;
  score1->y = 300;
  score1->color.r = 1;
  score1->color.g = 1;
  score1->color.b = 1;
  score1->color.a = 1;
  S2D_SetText(score1, "1. %s %d", playerNames[0], playerScores[0]);

  score2 = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 35);
  score2->x = 450;
  score2->y = 375;
  score2->color.r = 1;
  score2->color.g = 1;
  score2->color.b = 1;
  score2->color.a = 1;
  S2D_SetText(score2, "2. %s %d", playerNames[1], playerScores[1]);

  score3 = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 35);
  score3->x = 450;
  score3->y = 450;
  score3->color.r = 1;
  score3->color.g = 1;
  score3->color.b = 1;
  score3->color.a = 1;
  S2D_SetText(score3, "3. %s %d", playerNames[2], playerScores[2]);

  quit3 = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Quit", 50);
  quit3->x = 1150;
  quit3->y = 650;
  quit3->color.r = 1;
  quit3->color.g = 1;
  quit3->color.b = 1;
  quit3->color.a = 1;
}

void score_on_mouse(S2D_Event mouseIn)
{
  if(viewScores->mouse.x >= quit3->x && viewScores->mouse.x <= quit3->x + 107 && viewScores->mouse.y >= quit3->y && viewScores->mouse.y <= quit3->y + 50)
  {
    if(mouseIn.button == 1)
    {
      S2D_Close(viewScores);
    }
  }
}

void score_on_key(S2D_Event keyIn)
{
  if(!strcmp(keyIn.key, "Return"))
  {
    S2D_Close(nameInput);
  }
  else if(keyIn.type == S2D_KEY_DOWN)
  {
    strcat(name, keyIn.key);
  }
}

void scoreRender()
{
  S2D_DrawText(highScores);
  S2D_DrawText(score1);
  S2D_DrawText(score2);
  S2D_DrawText(score3);
  S2D_DrawText(quit3);
}

void nameInputInit()
{
  enterName = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "Enter your name: ", 35);
  enterName->x = 450;
  enterName->y = 250;
  enterName->color.r = 1;
  enterName->color.g = 1;
  enterName->color.b = 1;
  enterName->color.a = 1;

  uName = S2D_CreateText("Fonts/rubik/Rubik-Bold.ttf", "", 45);
  uName->x = 450;
  uName->y = 400;
  uName->color.r = 1;
  uName->color.g = 1;
  uName->color.b = 1;
  uName->color.a = 1;
}

void nameRender()
{
  S2D_DrawText(enterName);
  S2D_DrawText(uName);
}

void nameUpdate()
{
  S2D_SetText(uName, "%s", name);
}

int main()
{
  menuInit();
  menu = S2D_CreateWindow("Menu", WINDOW_WIDTH, WINDOW_HEIGHT, NULL, menuRender, S2D_RESIZABLE);
  menu->viewport.mode = S2D_SCALE;
  menu->on_mouse = menu_on_mouse;
  S2D_Show(menu);
  S2D_FreeWindow(menu);

  if(!menuQuit)
  {
    gameInit();
    game = S2D_CreateWindow("Game", WINDOW_WIDTH, WINDOW_HEIGHT, gameUpdate, gameRender, S2D_RESIZABLE);
    game->viewport.mode = S2D_SCALE;

    S2D_PlayMusic(bgm, true);

    game->on_key = game_on_key;

    time(&startTime);
    S2D_Show(game);

    S2D_FreeWindow(game);

    endInit();
    end = S2D_CreateWindow("Game Over", WINDOW_WIDTH, WINDOW_HEIGHT, NULL, endRender, S2D_RESIZABLE);
    end->viewport.mode = S2D_SCALE;
    end->on_mouse = end_on_mouse;
    S2D_Show(end);
    S2D_FreeWindow(end);

    if(scoreBoard)
    {
      nameInputInit();
      nameInput = S2D_CreateWindow("Username", WINDOW_WIDTH, WINDOW_HEIGHT, nameUpdate, nameRender, S2D_RESIZABLE);
      nameInput->viewport.mode = S2D_SCALE;
      nameInput->on_key = score_on_key;
      S2D_Show(nameInput);
      S2D_FreeWindow(nameInput);
      
      scoreInit();
      viewScores = S2D_CreateWindow("Highscores", WINDOW_WIDTH, WINDOW_HEIGHT, NULL, scoreRender, S2D_RESIZABLE);
      viewScores->viewport.mode = S2D_SCALE;
      viewScores->on_mouse = score_on_mouse;
      S2D_Show(viewScores);
      S2D_FreeWindow(viewScores);
    }
  }
  return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              