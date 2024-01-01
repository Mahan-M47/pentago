// Mahan Madani

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

// this struct is used to save player info.
struct info {
  int games_played;
  string name;
  int wins;
  int draws;
  int losses;
  int score;
};

info p1, p2, allplayers[50];   // p1 and p2 are player 1 and player 2.
int marble [6][6];             // saves the marble placements on the board.
string lines [28];             // stores the game board to be printed later.


// 1- functions accessed from the menu
void show_menu         ();
void start_game        ();
void display_rules     ();
void show_rankings     ();


// 2- initializer functions
void get_player_info   ();
void clear_strings     ();
void clear_array       (bool arr[4][3][3], bool arrbox[2][2]);


// 3- these function create the board
void print_board       (bool arr[4][3][3], bool arrbox[2][2], int player);
void board             (int series, bool arr[4][3][3], bool arrbox[2][2]);
void output            ();


// 4- used by the above functions to draw different parts of the board.
// the game board has 36 small squares and 4 big boxes.
void box_ho            (int i);
void box_ver           (int i);
void selected_box_ho   (int i);
void selected_box_ver  (int i);
void square            (int i, int player);
void selected_square   (int i, int player);
void menu_line         ();


// 5- these functions receive gameplay inputs input from the user and  act accordingly.
void movement          (bool arr[4][3][3], int& i, int& j, int& k, char action, int& player);
void box_movement      (bool arrbox[2][2], int& x, int& y, char action, int& round);
void rotate_L          (int x, int y);
void rotate_R          (int x, int y);
void change_player     (int& player);


// 6- logic functions determine the winner;
string logic           ();
void result_logic      (string winner);


// 7- functions responsible for ranking the players.
void save_to_array     ();
void sort_players      (int n);
void swap_players      (int n);
void save_to_file      (int n);


int main() {
	SetConsoleOutputCP(CP_UTF8);
  char choice;

  while (1)
  {
    system ("color 8F");
      show_menu();
      choice = getch();

      switch (choice) {
        case '1': start_game();
          break;
        case '2': display_rules();
          break;
        case '3': show_rankings();
          break;
        case '4':
          return 0;
        default:
          system ("cls");
      }
  }

}


void show_menu() {
  cout << "\n     \u250F\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
  "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2513\n     "
  "\u2503   P E N T A G O    \u2503\n     \u2503                    \u2503\n     "
  "\u2503  1. Start Game     \u2503\n     \u2503  2. Rules          \u2503\n     "
  "\u2503  3. View Rankings  \u2503\n     \u2503  4. Exit           \u2503\n     "
  "\u2517\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
  "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u251B\n     ";
}

void start_game() {

  // making sure all elements in the marble array is set to 0;
  for (int a = 0; a < 6; a++) {
    for (int b = 0; b < 6; b++) {
      marble[a][b] = 0;
    }
  }

  int roundtemp, round = 0, playertemp, player = 1;

  bool arr[4][3][3];          // this array marks the selected square for the
  int i = 0, j = 0, k = 0;    // 'movement' function.

  bool arrbox[2][2];          // this array marks the selected box for the
  int x = 0, y = 0;           // 'box_movement' function.

  char action;
  string winner;              // the logic function determines the winner by adding
                              // 1 and 2 to this string.

  get_player_info ();

  while (1)
  {
    clear_array (arr, arrbox);
    playertemp = player;
    roundtemp = round;

    //this loop will allow the player to move on the 36 smals squares. once they
    //place a marble, 'player' will change and end the loop.
    while (playertemp == player)
      {
          system("cls");
          cout << "\n      Use W A S D to move. Press E or Enter to place a marble.\n\n\n";
          print_board (arr, arrbox, playertemp);

          action = getch();
          movement (arr, i, j, k, action, player);
      }

    winner = logic();
    if (winner.size() > 0){ break; }

    arrbox[0][0] = true;

    //this loop will allow the player to move on the 4 big boxes. once they
    //turn a box, 'round' will change and end the loop.
    while (roundtemp == round)
      {
          system("cls");
          cout << "\n      Use W A S D to move between boxes.\n"
          "      Press R to rotate Clockwise. Press L to rotate Counter Clockwise.\n\n";
          print_board (arr, arrbox, playertemp);

          action = getch();
          box_movement (arrbox, x, y, action, round);
      }

    winner = logic();
    if (winner.size() > 0){ break; }
    else if (round == 36) { break; }  // if round reaches 36 this means the board
                                      // is full and the game is a draw.
  }

  system("cls");
  cout << "\n\n\n\n";
  print_board (arr, arrbox, 0);

  result_logic (winner);

  p1.score = p1.wins * 3 + p1.draws * 1;
  p2.score = p2.wins * 3 + p2.draws * 1;

  save_to_array();
}

void display_rules() {
  system("cls");

  menu_line();
  cout <<
  "\n    The game starts with an empty game board. The gmae board consists of the four separate\n"
  "    boards. The starting player places their sign in a square of their choosing.\n"
  "    After placing a sign the player turns one of the four boards one notch (90 degrees)\n"
  "    Clock or Counter Clockwise. A board must be turned with each move.\n"
  "    Then the second player does the same, i.e. places a sign and turns a board, so on and forth.\n\n"
  "    The first player to get 5 signs in a row wins!\n"
  "    The row can be horizontal, vertical or diagonal and run over two or three boards. If a player\n"
  "    gets 5 in a row after placing a sign they don't need to turn a board. If all the squares have\n"
  "    been filled without any player getting 5 in a row the game is a draw. If both players get 5\n"
  "    in a row as a player turns a board the game is also a draw.\n\n";
  menu_line();
}

void show_rankings() {
  system("cls");

  ofstream ranking_create ("ranking.dat", ios::app);  // this will create the ranking file
  ranking_create.close ();                            // if it doesn't exist.

  //-------syncing allplayers[] with data from the ranking file-----
  fstream ranking ("ranking.dat", ios::in);
  if(!ranking)
  {
     cout << "cannot access ranking file.";
     cin.get();
     exit(0);
  }

  for (int i = 0; i < 50; i++) {
    allplayers[i].name =  "";    // making sure all names are empty.
  }

  // this loop copies the data from our file to the allplayers array.
  for (int i = 0; !ranking.eof() ; i++){
      ranking.read ( reinterpret_cast <char *> (&allplayers[i]), sizeof(info) );
  }

  ranking.close();


//-----------------------display rankings-------------------------
  menu_line();

  cout << setw(9) << "RANK" << setw(20) << "PLAYER NAME" << setw(17)
       << "GAMES PLAYED" << setw(12) << "GAMES WON" << setw(9) << "DRAWS"
       << setw(14) << "GAMES LOST" << setw(16) << "TOTAL SCORE\n";

  for (int i = 0; allplayers[i].name != ""; i++)
  {
    cout << endl << setw(9)  << i + 1
         << setw(20) << allplayers[i].name   << setw(12) << allplayers[i].games_played
         << setw(13) << allplayers[i].wins   << setw(11) << allplayers[i].draws
         << setw(12) << allplayers[i].losses << setw(14) << allplayers[i].score
         << endl;
  }

  menu_line();
}

//----------------------------------------------------------------------------
void get_player_info() {
  system("cls");

  cout << "\n  Player 1, please enter your name (with no spaces):\n  ";
  cin >> p1.name;

  // this loop transforms the name to lower case.
  for (int i = 0; i < p1.name.size(); i++) {
    if (p1.name[i] >= 'A' && p1.name[i] <= 'Z') { p1.name[i] += 32; }
  }

  cout << "\n  Player 2, please enter your name (with no spaces):\n  ";
  cin >> p2.name;

  for (int i = 0; i < p2.name.size(); i++) {
    if (p2.name[i] >= 'A' && p2.name[i] <= 'Z') { p2.name[i] += 32; }
  }

  while (p1.name == p2.name)
  {
      cout << "\n  Error. Player 2 must enter a different name.\n"
      "  Player 2, please enter your name (with no spaces):\n  ";
      cin >> p2.name;

      for (int i = 0; i < p2.name.size(); i++) {
          if (p2.name[i] >= 'A' && p2.name[i] <= 'Z') { p2.name[i] += 32; }
      }
  }


  p1.games_played = 1,  p2.games_played = 1;
  p1.wins = 0,          p2.wins = 0;
  p1.draws = 0,         p2.draws = 0;
  p1.losses = 0,        p2.losses = 0;
  p1.score = 0,         p2.score = 0;
}

void clear_strings() {
  for (int h = 0; h < 28; h++){
    lines[h] = "";
  }
}

void clear_array (bool arr[4][3][3], bool arrbox[2][2]) {

  // arr and arrbox arrays need to be cleared after every round to reset the
  // position of the player's selected board and box.

  for (int a = 0; a < 4; a++){
    for (int b = 0; b < 3; b++){
      for (int c = 0; c < 3; c++){
        arr[a][b][c] = false;
      }
    }
  }

  arr[0][0][0] = true;

  for (int a = 0; a < 6; a++) {
    for (int b = 0; b < 6; b++) {
      arrbox[a][b] = 0;
    }
  }

  clear_strings();
}

//----------------------------------------------------------------------------
void print_board (bool arr[4][3][3], bool arrbox[2][2], int player) {

  if (player == 1) {
    cout << "      It's " << p1.name << "\'s turn as X:\n\n";
  }
  else if (player == 2) {
    cout << "      It's " << p2.name << "\'s turn as O:\n\n";
  }
  else if (player == 0) {
    cout << "                          The game has ended.\n\n";
  }


  for (int h = 0; h < 28; h++){
    lines[h] += "        ";
  }

  board (0, arr, arrbox);

  for (int h = 0; h < 14; h++){
    lines[h] += " ";
  }

  board (1, arr, arrbox);
  board (2, arr, arrbox);

  for (int h = 14; h < 28; h++){
    lines[h] += " ";
  }

  board (3, arr, arrbox);

  output();
  clear_strings();
}

void board (int section, bool arr[4][3][3], bool arrbox[2][2]) {

  // section tells the program which one of the 4 boxes it needs to draw.

  int player = 0, n = 0, x = 0, y = 0;
  if (section > 1) {n = 14;}

  if (section == 1) { y = 1;}
  else if (section == 2) {x = 1;}
  else if (section == 3) {x = 1, y = 1;}

  if (arrbox[x][y] == true){
      selected_box_ho (n);
  }
  else box_ho (n);

  n++;

  if (arrbox[x][y] == true){
      selected_box_ver (n);
  }
  else box_ver (n);

  int m = n;

  for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j ++)
      {
          if (section == 0) {player = marble[i][j];}
          else if (section == 1) {player = marble[i][j + 3];}
          else if (section == 2) {player = marble[i + 3][j];}
          else if (section == 3) {player = marble[i + 3][j + 3];}

          if (arr[section][i][j] == true)
          {
              selected_square (n, player);
              arr[section][i][j] = false;
          }
          else
              square (n, player);
      }
      n += 4;
  }

  if (arrbox[x][y] == true){
      selected_box_ver (m);
  }
  else box_ver (m);

  arrbox[x][y] = false;
}

void output() {
  for (int k = 0; k < 28; k++){
    if (k == 14) { cout << endl; }
    cout << lines[k] << endl;
  }
}

//----------------------------------------------------------------------------
void box_ho (int i) {
  lines[i] += " \u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
  "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
  "\u2500\u2500\u2510 ";

  i += 13;

  lines[i] += " \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
  "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"
  "\u2500\u2500\u2518 ";
}

void box_ver (int i) {
  int n = i + 12;
  for (; i < n; i++)
  {
    lines[i] += " \u2502 ";
  }
}

void selected_box_ho (int i) {
  lines[i] += " \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
  "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
  "\u2550\u2550\u2557 ";

  i += 13;

  lines[i] += " \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
  "\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550"
  "\u2550\u2550\u255D ";
}

void selected_box_ver (int i) {
  int n = i + 12;
  for (; i < n; i++)
  {
    lines[i] += " \u2551 ";
  }
}

void square (int i, int player) {
  lines[i] += "\u250C\u2500\u2500\u2500\u2500\u2500\u2510";
  i++;
  int n = i + 2;

  if (player == 1){
    while (i < n)
    {
      lines[i] += "\u2502 X X \u2502";
      i++;
    }
  }
  else if (player == 2){
    while (i < n)
    {
      lines[i] += "\u2502 O O \u2502";
      i++;
    }
  } else {
      while (i < n)
      {
        lines[i] += "\u2502     \u2502";
        i++;
      }
    }

  lines[n] += "\u2514\u2500\u2500\u2500\u2500\u2500\u2518";
}

void selected_square (int i, int player) {
  lines[i] += "\u2554\u2550\u2550\u2550\u2550\u2550\u2557";
  i++;
  int n = i + 2;

  if (player == 1){
    while (i < n)
    {
      lines[i] += "\u2551 X X \u2551";
      i++;
    }
  }
  else if (player == 2){
    while (i < n)
    {
      lines[i] += "\u2551 O O \u2551";
      i++;
    }
  } else {
      while (i < n)
      {
        lines[i] += "\u2551     \u2551";
        i++;
      }
    }

  lines[n] += "\u255A\u2550\u2550\u2550\u2550\u2550\u255D";
}

void menu_line() {
  cout << "\n   \u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501"
   "\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\n";
}

//----------------------------------------------------------------------------
void movement (bool arr[4][3][3], int& i, int& j, int& k, char action, int& player) {

  // this function moves the player's selected square across the board. if it
  // reaches an edge, it won't move any further in that direction.

  switch (action){

    case 'd':
      k++;
      if (k == 3)
      {
        if (i == 1 || i == 3) {k = 2;}
        else {
          k = 0;
          i++;
        }
      }
      arr[i][j][k] = true;
      break;

    case 'a':
      k--;
      if (k == -1)
      {
        if (i == 0 || i == 2){
          k = 0;
        }
        else {
          k = 2;
          i--;
        }
      }
      arr[i][j][k] = true;
      break;

    case 's':
      j++;
      if (j == 3)
      {
        if (i == 2 || i == 3) {j = 2;}
        else {
          j = 0;
          i += 2;
        }
      }
      arr[i][j][k] = true;
      break;

    case 'w':
      j--;
      if (j == -1)
      {
        if (i == 0 || i == 1){
          j = 0;
        }
        else {
          j = 2;
          i -= 2;
        }
      }
      arr[i][j][k] = true;
      break;

      default:
        if (action == 'e' || action == '\r')    // pressing e or enter will place a marble.
        {
            if (i == 0 && marble[j][k] == 0) {
              marble[j][k] = player;
              change_player (player);
              }
            else if (i == 1 && marble[j][k + 3] == 0) {
              marble[j][k + 3] = player;
              change_player (player);
              }
            else if (i == 2 && marble[j + 3][k] == 0) {
              marble[j + 3][k] = player;
              change_player (player);
              }
            else if (i == 3 && marble[j + 3][k + 3] == 0) {
              marble[j + 3][k +3] = player;
              change_player (player);
              }
        }
        else arr[i][j][k] = true;
    }
}

void box_movement (bool arrbox[2][2], int& x, int& y, char action, int& round) {
  switch (action){

  case 'd':
    y++;
    if (y == 2) { y--; }
    arrbox[x][y] = true;
    break;

  case 'a':
    y--;
    if (y == -1) { y++; }
    arrbox[x][y] = true;
    break;

  case 's':
    x++;
    if (x == 2) { x--; }
    arrbox[x][y] = true;
    break;

  case 'w':
    x--;
    if (x == -1) { x++; }
    arrbox[x][y] = true;
    break;

  case 'r':
    rotate_R (x, y);
    x = 0, y = 0;
    round++;
    break;

  case 'l':
    rotate_L (x, y);
    x = 0, y = 0;
    round++;
    break;

  default:
    arrbox[x][y] = true;
    // if the player enters anything but w a s d or r l, the selected box remains selected.
  }

}

void rotate_L (int x, int y) {
  int n1, n2, marble_temp[6][6];

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
        marble_temp[i][j] = marble[i][j];
    }
  }

  if (x == 0 && y == 0) {n1 = 0, n2 = 0;}
  else if (x == 0 && y == 1) {n1 = 0, n2 = 3;}
  else if (x == 1 && y == 0) {n1 = 3, n2 = 0;}
  else if (x == 1 && y == 1) {n1 = 3, n2 = 3;}

  // rotation is done line by line.

  for (int i = 0; i < 3; i++){
      marble[i + n1][0 + n2] = marble_temp[0 + n1][2 - i + n2];
  }
  for (int i = 0; i < 3; i++){
      marble[i + n1][1 + n2] = marble_temp[1 + n1][2 - i + n2];
  }
  for (int i = 0; i < 3; i++){
      marble[i + n1][2 + n2] = marble_temp[2 + n1][2 - i + n2];
  }
}

void rotate_R (int x, int y) {
  int n1, n2, marble_temp[6][6];

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
        marble_temp[i][j] = marble[i][j];
    }
  }

  if (x == 0 && y == 0) {n1 = 0, n2 = 0;}
  else if (x == 0 && y == 1) {n1 = 0, n2 = 3;}
  else if (x == 1 && y == 0) {n1 = 3, n2 = 0;}
  else if (x == 1 && y == 1) {n1 = 3, n2 = 3;}

  // rotation is done line by line.

  for (int i = 0; i < 3; i++){
      marble[0 + n1][2 - i + n2] = marble_temp[i + n1][0 + n2];
  }
  for (int i = 0; i < 3; i++){
      marble[1 + n1][2 - i + n2] = marble_temp[i + n1][1 + n2];
  }
  for (int i = 0; i < 3; i++){
      marble[2 + n1][2 - i + n2] = marble_temp[i + n1][2 + n2];
  }
}

void change_player (int& player) {
  if (player == 1) {player = 2;}
  else player = 1;
}

//----------------------------------------------------------------------------
string logic() {

  string winner;
  // if the program detects five marbles in a row, it will add the winner's marble
  // number (1 or 2) to the 'winner' string. if it detects more than 1 winners in
  // the same round, it will all of the marble numbers which may or may not be
  // equal. the 'result_logic' function will analyse this string to identify a winner
  // or call the game a draw.

  for (int s = 1; s <= 2; s++)
  {

  //-----------------horizontal win------------------------------
      for (int i = 0; i < 6; i++){
          for (int j = 0; j < 5; j++)
          {
              if (marble[i][j] != s){break;}
              else if (j == 4) {winner += to_string(s);}
          }

          for (int j = 1; j < 6; j++)
          {
              if (marble[i][j] != s){break;}
              else if (j == 5) {winner += to_string(s);}
          }
      }

  //-------------------Vertical win---------------------------------
      for (int i = 0; i < 6; i++){
          for (int j = 0; j < 5; j++)
          {
              if (marble[j][i] != s){break;}
              else if (j == 4) {winner += to_string(s);}
          }

          for (int j = 1; j < 6; j++)
          {
              if (marble[j][i] != s){break;}
              else if (j == 5) {winner += to_string(s);}
          }
      }

  //-------------------Diagonal win---------------------------------
  //-------------------Main Diameter--------------------------------
      for (int i = 0; i < 5; i++)
          {
            if (marble[i][i] != s){break;}
            else if (i == 4) {winner += to_string(s);}
          }

      for (int i = 0; i < 5; i++)
          {
            if (marble[i][5 - i] != s){break;}
            else if (i == 4) {winner += to_string(s);}
          }

      for (int i = 1; i < 6; i++)
          {
            if (marble[i][i] != s){break;}
            else if (i == 5) {winner += to_string(s);}
          }

      for (int i = 1; i < 6; i++)
          {
            if (marble[i][5 - i] != s){break;}
            else if (i == 5) {winner += to_string(s);}
          }

  //-------------------other Diagonals------------------------------
      for (int i = 0; i < 5; i++)
          {
              if (marble[i][i + 1] != s){break;}
              else if (i == 4) {winner += to_string(s);}
          }

      for (int i = 0; i < 5; i++)
          {
              if (marble[i][4 - i] != s){break;}
              else if (i == 4) {winner += to_string(s);}
          }

      for (int i = 1; i < 6; i++)
          {
            if (marble[i][i - 1] != s){break;}
            else if (i == 5) {winner += to_string(s);}
          }

      for (int i = 1; i < 6; i++)
          {
            if (marble[i][6 - i] != s){break;}
            else if (i == 5) {winner += to_string(s);}
          }
  //----------------------end of game logic--------------------------
  }

  return winner;
}

void result_logic (string winner) {
  // based on the size of the winner string (aka the number of five marbles in a row)
  // this function will identify the winner or call it a draw.

  //-----------------no one gets five in a row------------------------------
  if (winner.size() == 0) {
    cout << "\n                          " << "It's a draw!\n";
    p1.draws++;
    p2.draws++;
  }

  //-----------------only 1 person get five---------------------------------
  else if (winner.size() == 1)
  {
      if (winner[0] == '1') {
          cout << "\n                           " << p1.name << " wins!\n";
          p1.wins++;
          p2.losses++;
      }
      else if (winner[0] == '2') {
          cout << "\n                           " << p2.name << " wins!\n";
          p2.wins++;
          p1.losses++;
      }
  }

  //--------------two or more separate five in a rows-----------------------
  //----------all possible conditions are counted this way!-----------------
  else if (winner.size() >= 2)
  {
    for (int i = 0; i < winner.size() - 1; i++)
    {
        if (winner[i] != winner[i + 1]) {
            cout << "\n                          " << "It's a draw!\n";
            p1.draws++;
            p2.draws++;
            break;
        }

        else if (i == winner.size() - 2) {
            switch (winner[0])
            {
              case '1':
                cout << "\n                           " << p1.name << " Wins\n";
                p1.wins++;
                p2.losses++;
                  break;
              case '2':
                cout << "\n                           " << p2.name << " Wins!\n";
                p2.wins++;
                p1.losses++;
                  break;
            }
        }
    }
  }

}

//----------------------------------------------------------------------------
void save_to_array() {

  //-------syncing allplayers[] with data from the ranking file-----
  ofstream ranking_create ("ranking.dat", ios::app);
  ranking_create.close ();

  ifstream ranking ("ranking.dat", ios::in);
  if(!ranking)
  {
     cout << "cannot access ranking file.";
     cin.get();
     exit(0);
  }

  for (int i = 0; i < 50; i++) {
    allplayers[i].name =  "";
  }

  for (int i = 0; !ranking.eof() ; i++){
      ranking.read ( reinterpret_cast <char *> (&allplayers[i]), sizeof(info) );
  }

  ranking.close();

  //--------------------first player--------------------------------
  int i, j;

  // if the player's name already exists, thier results will be combined.
  for (i = 0; i < 50 && allplayers[i].name != "" ; i++)
  {
      if (p1.name == allplayers[i].name) {
          allplayers[i].games_played  +=  p1.games_played;
          allplayers[i].wins          +=  p1.wins;
          allplayers[i].draws         +=  p1.draws;
          allplayers[i].losses        +=  p1.losses;
          allplayers[i].score         +=  p1.score;
          break;
      }
  }

  // if the player's name doesn't exist, a new entry with their name will be created.
  if (allplayers[i].name == "") {
      allplayers[i].games_played  =  p1.games_played;
      allplayers[i].name          =  p1.name;
      allplayers[i].wins          =  p1.wins;
      allplayers[i].draws         =  p1.draws;
      allplayers[i].losses        =  p1.losses;
      allplayers[i].score         =  p1.score;
  }

//--------------------second player--------------------------------
  for (j = 0; j < 50 && allplayers[j].name != "" ; j++)
  {
      if (p2.name == allplayers[j].name) {
          allplayers[j].games_played  +=  p2.games_played;
          allplayers[j].wins          +=  p2.wins;
          allplayers[j].draws         +=  p2.draws;
          allplayers[j].losses        +=  p2.losses;
          allplayers[j].score         +=  p2.score;
          break;
      }
  }

  if (allplayers[j].name == "") {
      allplayers[j].games_played  =  p2.games_played;
      allplayers[j].name          =  p2.name;
      allplayers[j].wins          =  p2.wins;
      allplayers[j].draws         =  p2.draws;
      allplayers[j].losses        =  p2.losses;
      allplayers[j].score         =  p2.score;
  }

  int n = 0;
  while (allplayers[n].name != "") {
    n++;                              // n is the number of unique player names.
  }

  sort_players (n);
  save_to_file (n);
}

void sort_players (int n) {

  // the bubble sort method is used here.

  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++)
    {

        if (allplayers[j].score < allplayers[j + 1].score) {
          swap_players (j);
        }

        else if (allplayers[j].score == allplayers[j + 1].score &&
                 allplayers[j].games_played > allplayers[j + 1].games_played)
        {
          swap_players (j);
        }

    }
  }

}

void swap_players (int n) {
  info temp;

  temp.name          =  allplayers[n].name;
  temp.games_played  =  allplayers[n].games_played;
  temp.wins          =  allplayers[n].wins;
  temp.draws         =  allplayers[n].draws;
  temp.losses        =  allplayers[n].losses;
  temp.score         =  allplayers[n].score;

  allplayers[n].name          =  allplayers[n + 1].name;
  allplayers[n].games_played  =  allplayers[n + 1].games_played;
  allplayers[n].wins          =  allplayers[n + 1].wins;
  allplayers[n].draws         =  allplayers[n + 1].draws;
  allplayers[n].losses        =  allplayers[n + 1].losses;
  allplayers[n].score         =  allplayers[n + 1].score;

  allplayers[n + 1].name          =  temp.name;
  allplayers[n + 1].games_played  =  temp.games_played;
  allplayers[n + 1].wins          =  temp.wins;
  allplayers[n + 1].draws         =  temp.draws;
  allplayers[n + 1].losses        =  temp.losses;
  allplayers[n + 1].score         =  temp.score;
}

void save_to_file (int n) {

  // in order to save the ranking data to a file, the program will write all of
  // the entries in the 'allplayers' array (which is an 'info' array) to the file.

  ofstream ranking ("ranking.dat", ios::out);
  if(!ranking)
  {
     cout << "cannot access ranking file.";
     cin.get();
     exit(0);
  }

  for (int i = 0; i < n; i++){
      ranking.write ( reinterpret_cast <const char *> (&allplayers[i]), sizeof(info) );
  }

  ranking.close();
}
