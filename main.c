#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int money;
int bet;
char name1[50];
int namecheck = 0;
int filecheck = 0;

struct user {
  char name[50];
  int money;
  struct user *next;
};

int cardtype(int array[52]);
int drawcard(int array[52]);


//function for each round of playing
void deal(int array[52]) {

  int total = 0;
  int acheck = 0;
  int a1check = 0;

  int ctotal = 0;
  int dtotal = 0;

  printf("\n");
  printf("******************");
  printf("\n");
  printf("*The dealer shuffles the cards and and you are dealt your hand*\n");
  
  //player drawing inital hand
  printf("You have drawn a");
  int c1 = cardtype(array);
  ctotal += c1;

  printf(", and a");
  int c2 = cardtype(array);
  printf(".\n");
  ctotal += c2;

  
  //setting variables values depending on how many Ace's the player draws
  if (c1 == 11 && c2 == 11) {
    acheck = 2;
    ctotal -= 10;
  }

  if (c2 == 11 && c1 != 11) {
    acheck = 1;
  }

  if (c1 == 11 && c2 != 11) {
    acheck = 1;
  }

  //dealer drawing initial hand
  printf("The dealer draws a");
  int d1 = cardtype(array);
  printf(".\n");
  dtotal += d1;
  if (d1 == 11) {
    a1check = 1;
  }

  printf("\n");
  printf("You have a total of %d, and the dealer has %d. \n", ctotal, dtotal);
  printf("\n");

  if (ctotal == 21) {
    printf("You have Blackjack! Payout is $%d\n", (int)(2 * bet));
    money += 2 * bet;
  }

  else {
    char playcheck[50];

    
    //loop for player drawing a card
    while (1) {
      while (1) {
        printf("************\n");
        printf("\n");
        printf("Would you like to hit or stand?\n");
        printf("(Enter 1 for hit, 0 for stand)\n");
        fgets(playcheck, 50, stdin);
        if (strcmp(playcheck, "1\n") == 0 || strcmp(playcheck, "0\n") == 0) {
          break;
        } else {
          printf("Please enter a valid option. \n");
        }
      }

      if (strcmp(playcheck, "1\n") == 0) {
        printf("You have drawn a");
        int c = cardtype(array);
        ctotal += c;
        printf(".\n");
        printf("\n");

        if (c == 11 && acheck < 1) {
          acheck += 1;
        }
        if (acheck >= 1 && c == 11) {
          acheck += 1;
          ctotal -= 10;
        }

        if (ctotal > 21 && c == 11) {
          ctotal -= 10;
        }

        printf("Your total is %d.\n", ctotal);

        if (ctotal >= 21) {
          strcpy(playcheck, "0\n");
          break;
        }
      }

      if (strcmp(playcheck, "0\n") == 0) {
        break;
      }
    }
    
    //loop for dealer drawing a card
    if (strcmp(playcheck, "0\n") == 0) {
      while (1) {

        printf("-----------\n");
        printf("\n");
        printf("The dealer draws a");
        int d = cardtype(array);
        printf(".\n");

        dtotal += d;
        if (a1check >= 1 && d == 11) {
          a1check += 1;
          dtotal -= 10;
        }

        if (d == 11 && dtotal > 21) {
          dtotal -= 10;
        }

        if (a1check == 0 && d == 11) {
          a1check += 1;
        }

        printf("The dealer total is %d. \n", dtotal);
        printf("\n");

        if (dtotal >= 17) {
          break;
        }
      }
    }

    //once dealer and player have reached tehheir final hand, winner/loser is decided
    if (dtotal > ctotal && dtotal <= 21) {
      printf("The dealer has won this round. \n");
    }

    else if (dtotal == ctotal && dtotal <= 21) {
      printf(
          "You have tied with the dealer! You will receive your bet back. \n");
      money += bet;
    }

    else if (dtotal > 21 && ctotal > 21) {
      printf(
          "You and the dealer have busted. You will receive your bet back. \n");
      money += bet;
    }

    else if (dtotal < ctotal && ctotal <= 21) {
      printf("You've beat the dealer! Payout is $%d. \n", (int)(bet * 1.5));
      money += bet * 1.5;
    }

    else if (dtotal > 21) {
      printf("The dealer busted! Payout is $%d.\n", (int)(bet * 1.5));
      money += bet * 1.5;
    }

    else {
      printf("You have busted this round. \n");
    }
  }
}

//function used to draw each card in the cardtype function
int drawcard(int array[52]) {

  int r = 0;
  while (1) {

    srand(time(0));
    r = 0;
    while (1) {
      r = (rand() % (53));

      if (r > 0 && r < 53) {
        break;
      }
    }

    int c = 0;
    for (int a = 0; a < 53; a++) {
      if (array[a] == r) {
        c = 1;
        array[a] = 0;
      }
    }

    if (c == 0) {
      break;
    }
  }

  return r;
}

//function to determine user bet
void setbet() {
  bet = 0;
  int check = 1;
  int flag = 0;
  char charbet[50];
  while (1) {
    // int bet=0;
    printf("Enter amount you would like to bet. Bets are $25 a chip. \n");

    printf("Your balance is $%d: \n", money);
    printf("\n");

    fgets(charbet, 50, stdin);
    bet = atoi(charbet);

    check = bet % 25;

    if (check != 0 || bet < 25 || bet > money) {
      printf("Please enter a valid bet that is a multiple of 25.\n");
    } else {
      break;
    }
  }

  money = money - bet;
}

//function where user plays the game
void play() {

  // for new users, initial balance will be $100
  if (namecheck == 0) {
    printf("\nTo start off, you will have $100\n");
    money = 100;
  }

  // For existing users that do not have enough money to start playing, so they
  // can still play
  if (namecheck == 1 && money < 25) {
    printf("Since you don't have enough to start playing, you will receive a "
           "little extra cash to make a bet.");

    // adds enough money to make the user balance $25
    money = money + (25 - money);
  }

  // establish a "deck of cards"
  int array[52];

  for (int x = 0; x < 52; x++) {
    array[x] = x + 1;
  }

  // loop for user to play the game until they run out of money,
  // or until they would like to quit at the end of each round
  while (1) {
    if (money < 25) {
      printf("You have run out of money to play...");
      break;
    }

    bet = 0;
    char qc[50];

    // two functions that are repeat for each round of playing
    setbet();
    deal(array);

    while (1) {
      printf("Want to quit? 0 if no, 1 if yes.\n");
      printf("CURRENT BALANCE: $%d", money);
      printf("\n");
      printf("\n");
      fgets(qc, 50, stdin);

      if (strcmp(qc, "0\n") == 0 || strcmp(qc, "1\n") == 0) {
        break;
      }

      else {
        printf("Please enter a valid selection.\n");
      }
    }
    if (strcmp(qc, "1\n") == 0) {
      break;
    }
  }
  printf("Thanks for playing!\n");
}

// function that draws a card out of the deck
int cardtype(int array[52]) {

  // pick a random number within the amount of cards in the deck
  int r = 0;
  while (1) {

    srand(time(0));
    r = 0;
    while (1) {
      r = (rand() % (53));

      if (r > 0 && r < 53) {
        break;
      }
    }

    // if that card number has already been picked, go through the loop
    // until a new card has been picked
    int c = 0;
    for (int a = 0; a < 52; a++) {
      if (array[a] == r) {
        c = 1;
        array[a] = 0;
      }
    }

    if (c == 1) {
      break;
    }
  }

  // set the card to the new card number
  int card = r;

  // depending on card number, the type of card is established, and message is
  // printed on the screen depending on card
  if (card == 1 || card == 14 || card == 27 || card == 40) {
    printf("n Ace");
    return 11;

  } else if (card == 11 || card == 24 || card == 37 || card == 50) {
    printf(" Jack");
    return 10;

  } else if (card == 12 || card == 25 || card == 38 || card == 51) {
    printf(" Queen");
    return 10;
  }

  else if (card == 13 || card == 26 || card == 39 || card == 52) {
    printf(" King");
    return 10;
  }

  else if (card == 2 || card == 15 || card == 28 || card == 41) {
    card = 2;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 3 || card == 16 || card == 29 || card == 42) {
    card = 3;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 4 || card == 17 || card == 30 || card == 43) {
    card = 4;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 5 || card == 18 || card == 31 || card == 44) {
    card = 5;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 6 || card == 19 || card == 32 || card == 45) {
    card = 6;
    printf(" card worth %d", card);
    return card;
  } else if (card == 7 || card == 20 || card == 33 || card == 46) {
    card = 7;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 8 || card == 21 || card == 34 || card == 47) {
    card = 8;
    printf(" card worth %d", card);
    return card;
  }

  else if (card == 9 || card == 22 || card == 35 || card == 48) {
    card = 9;
    printf(" card worth %d", card);
    return card;
  }

  else {
    card = 10;
    printf(" card worth %d", card);
    return card;
  }
}

// function to print the leaderboard
void printLeaderboard(struct user *user) {

  printf("LEADERBOARD: \n");
  while (user != NULL) {

    printf("  NAME: %s ", user->name);
    printf("- balance: $%d", user->money);
    printf("\n");
    user = user->next;
  }
}

// function to save the contents of the leaderboard file into a linked list
struct user *fileread(FILE *f) {
  char c[50];
  struct user *headnode = malloc(sizeof(struct user));
  int count = 0;
  int num = 0;

  int fileRead = fscanf(f, "%s %d", c, &num);

  if (fileRead != EOF) {
    strcpy(headnode->name, c);
    headnode->money = num;
    headnode->next = fileread(f);
  }

  if (fileRead == EOF) {
    return NULL;
  }
  return headnode;
}

// function to find existing user in the leaderboard
void finduser(struct user *h) {

  while (1) {
    struct user *head = h;
    printf("Please enter your name:\n");

    // fscanf(stdin,"%s[^\n]", name1);
    fgets(name1, 50, stdin);
    // printf("%s", name1);
    if (name1[strlen(name1) - 1] == '\n') {
      name1[strlen(name1) - 1] = '\0';
    }

    while (1) {

      // if the user playing is an existing user, save the money from
      // leaderboard into current balance

      if (strcmp(head->name, name1) == 0) {
        namecheck = 1;
        money = head->money;
      }
      if (head->next == NULL) {
        break;
      }

      head = head->next;
    }

    // prompt the user to search for the name again, or to play as a
    // new user if the name is not found
    char a[50];
    if (namecheck == 0) {

      printf("Sorry, that user doesn't exist.\n");
      while (1) {

        printf("Would you like to play as a new user? (enter 0)\n");
        printf("Or try to search your name again? (enter 1)\n");

        fgets(a, 50, stdin);
        if (a[strlen(a) - 1] == '\n') {
          a[strlen(a) - 1] = '\0';
        }
        if (strcmp(a, "0") == 0 || strcmp(a, "1") == 0) {
          break;
        }

        else {
          printf("Please enter a valid selection. \n");
        }
      }
    }

    if (namecheck == 1) {
      printf("Welcome back %s. you will play with your\n", name1);
      printf("previous balance: ($%d).\n", money);
      break;
    }

    if (strcmp(a, "0") == 0) {
      break;
    }
  }
}

//function to add a new user to the leaderboard list
void adduser(struct user *head) {

  char name2[50];
  while (1) {
    int flag = 0;
    printf("Enter your name: ");
    fgets(name2, 50, stdin);

    if (name2[strlen(name2) - 1] == '\n') {
      name2[strlen(name2) - 1] = '\0';
    }

    for (int a = 0; a < strlen(name2); a++) {
      if (name2[a] == 32) {
        flag = 1;
        printf("Please enter a name without any spaces.\n");
      }
    }

    if (flag == 0) {
      break;
    }
  }

  struct user *temp = malloc(sizeof(struct user));
  strcpy(temp->name, name2);
  temp->money = money;
  temp->next = NULL;

  
  if (filecheck == 1) {
    strcpy(head->name, name2);
    head->money = money;
    head->next = NULL;
  }

  //adding the new user in the leaderboard in order of money amount.
  else {
    struct user *temp2 = malloc(sizeof(struct user));
    temp2 = head;

    if (temp2->money <= temp->money) {
      temp->next = temp2;
    } else {
      while (1) {
        if (temp2->next == NULL) {
          break;
        }
        if (temp2->next->money <= temp->money) {
          break;
        }
        temp2 = temp2->next;
      }
      temp->next = temp2->next;
      temp2->next = temp;
    }
  }
}

// function updating an existing players money on the leaderboard
void updateLeaderboard(struct user *head) {

  struct user *temp0 = head;

  while (1) {
    if (strcmp(name1, temp0->name) == 0) {
      temp0->money = money;
      break;
    }
    if (temp0->next == NULL) {
      break;
    }
    temp0 = temp0->next;
  }
  
}

//function to add the leaderboard list back to the file
void addtofile(struct user *head) {

  FILE *f;
  f = fopen("leaderboard.txt", "w");

  if (f != NULL) {

    while (head != NULL) {
      fprintf(f, "%s %d\n", head->name, head->money);
      head = head->next;
    }
    fclose(f);
  }

  else {
    printf("Error opening file... ");
    printf("Could not save to leaderboard.\n");
  }
}


//main function
int main() {
  struct user *headnode = NULL;
  FILE *f;

  int usercheck = 0;
  f = fopen("leaderboard.txt", "r");

  if (NULL == f) {
    printf("Error opening leaderboard...\n");
    return 0;
  }

  //obtain the leaderboard from a txt file
  headnode = fileread(f);
  if (headnode == NULL) {
    filecheck = 1;
    headnode = malloc(sizeof(struct user));
    headnode->money = 0;
    headnode->next = NULL;
    strcpy(headnode->name, "");
  }

  fclose(f);

  printf("Welcome to Blackjack!\n");

  // prompt the user to play, see leaderboard, or to quit
  char inp1[50];
  char c;
  while (1) {

    printf("Enter 0 to play, 1 to view leaderboard, or 2 to quit: \n ");

    fgets(inp1, 50, stdin);

    if (strcmp(inp1, "0\n") == 0) {
      break;

    } else if (strcmp(inp1, "1\n") == 0) {

      printLeaderboard(headnode);

    } else if (strcmp(inp1, "2\n") == 0) {
      return 0;

    } else {
      printf("Please enter a valid selection. ");
    }
  }

  // if the user is an existing user on the leaderboard, they
  // can access their money balance from the leaderboard and continue
  // from where they left off
  if (filecheck != 1) {
    while (1) {

      printf("Are you an existing user? (0 for yes, 1 for no.)\n");
      fgets(inp1, 50, stdin);

      if (strcmp(inp1, "0\n") == 0) {
        finduser(headnode);
        break;
      } else if (strcmp(inp1, "1\n") == 0) {
        break;
      } else {
        printf("Please enter a valid selection.\n");
      }
    }
  }

  else {
    printf("You are now playing as a new user.\n");
  }

  // function for actual game
  play();

  // once game is over
  char inp2[50];
  if (namecheck == 0) {
    printf("\n");
    printf("Before you go...\n");

    // if this is a new player, they are asked if they want to save their
    // progress on the leaderboard.
    while (1) {
      printf("Would you like to save your progress on the leaderboard?\n");
      printf("0 for yes, 1 for no.");

      fgets(inp2, 50, stdin);

      if (strcmp(inp2, "0\n") == 0 || strcmp(inp2, "1\n") == 0) {
        break;
      }

      else {
        printf("Please enter a valid selection.\n");
      }
    }

    if (strcmp(inp2, "0\n") == 0) {
      adduser(headnode);
      printf("\n");
      addtofile(headnode);

      printf("Your progress has been saved... ");
    }
  }

  // if the user exists already, update the money balance saved
  // on the leaderboard.
  if (namecheck == 1) {
    updateLeaderboard(headnode);
    addtofile(headnode);
    printf("\n");
    printf("Your progress has been saved... ");
  }

  printf("Goodbye!");

  return 0;
}
