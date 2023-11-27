#include "ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define BS 127
#define INPUT_LENGTH 64
#define AMOUNT_OF_WORDS 10

void setupTerm(struct termios);
void printInput(char *, int, int);
int checkMissed(char *, char *, int, int);

int main(void) {
  char *words[] = {"hello", "world", "how", "dolphin", "mars", "million", "water", "charge", "dodge", "banana", "car", "project", "queue", "brick", "computer", "keyboard", "mouse", "monitor",  "speaker", "printer", "mousepad", "washing", "tropical", "blank",    "starts", "ready", "tragic", "blooms", "rainbow", "pocket",
"lampshade", "fountain", "victim", "vulture", "happy",  "sad",    "mad", "drawing", "mountain", "sunset", "sunrise", "rain",     "clouds", "cloudy", "snow", "president", "appreciate", "thanks", "giving", "seconds", "minute", "type", "check", "input", "quick", "brown", "cyan", "problem", "yellow", "green", "read", "red", "blue", "blew", "bubbles", "kettle", "amount", "time", "length", "terminal", "train", "ruining", "war", "driving", "castle", "principal", "alarm", "internet", "battery", "donkey", "horse", "planet", "quarry"};
  int c;
  int count = 0;
  int i = 0;
  int messedUp = 100;
  char input[INPUT_LENGTH] = {'\0'};
  static struct termios oldt;
  int wordsLen = *(&words + 1) - words;
  srand(time(0));
  int randomNum = rand();
  char *word = words[randomNum % wordsLen];
  char *prevWord = word;
  time_t start_t, end_t;
  double dif;
  int wordlen = strlen(word);
  
  setupTerm(oldt);
  time(&start_t);
  while (count < AMOUNT_OF_WORDS) {
    printf(COLOR_RESET);
    printf("Type this: %s\n\n", word);
    printInput(input, messedUp, wordlen);
    c = getchar();

    // check if backspace
    if (c == BS) {
      if (i > 0) {
        i--;
        input[i] = '\0';
      }
      system("clear");
      continue;
    } else {
      input[i] = (char)c;
    }

    messedUp = checkMissed(input, word, i, messedUp);

    if (strncmp(input, word, wordlen) == 0) {
      printf("\n");
      strncpy(input, "\0", wordlen);
      randomNum = rand();
      word = words[randomNum % wordsLen];
      wordlen = strlen(word);
      i = -1;
      count++;
    }
    system("clear");
    i++;
  }

  time(&end_t);
  dif = difftime(end_t, start_t);

  // restore the old settings
  printf(reset);
  printf("It took you %.2lf seconds to type %d words.\n", dif, AMOUNT_OF_WORDS);
  printf("You typed %.2lf words per minute.\n", 60/(dif/AMOUNT_OF_WORDS));
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return 0;
}

void setupTerm(struct termios oldt) {

  static struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void printInput(char *input, int messedUp, int wordlen) {

  for (int j = 0; j < wordlen + 5; j++) {
    if (j >= messedUp) {
      printf(RED);
    }
    printf("%c", input[j]);
  }
}

int checkMissed(char *input, char *string, int index, int messedUp) {
  int compared_value = strncmp(input, string, index + 1);
  if (compared_value != 0 && messedUp == 100) {
    messedUp = index;
  } else if (compared_value == 0) {
    messedUp = 100;
  }
  return messedUp;
}
