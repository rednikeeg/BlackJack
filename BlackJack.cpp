#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <ctime>

#if defined(_WIN32) || defined(__MSDOS__)
  #include <windows.h>
  #define SLEEP   Sleep(310.310)
  #define SPADE   "\x06"
  #define CLUB    "\x05"
  #define HEART   "\x03"
  #define DIAMOND "\x04"
#else
  #include <unistd.h>
  #define SLEEP    usleep(310310)
  #define SPADE   "\xE2\x99\xA0"
  #define CLUB    "\xE2\x99\xA3"
  #define HEART   "\xE2\x99\xA5"
  #define DIAMOND "\xE2\x99\xA6"
#endif

using namespace std;

std::vector<string> deck;
std::map<char, int> valueMap = {
  {'2', 2},
  {'3', 3},
  {'4', 4},
  {'5', 5},
  {'6', 6},
  {'7', 7},
  {'8', 8},
  {'9', 9},
  {'1', 10},
  {'J', 10},
  {'Q', 10},
  {'K', 10},
  {'A', 11}
};

void formDeck(){
  std::vector<string> ranks = {
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "J",
    "Q",
    "K",
    "A"
  };

  std::vector<string> suits = {
    SPADE,
    CLUB,
    HEART,
    DIAMOND
  };

  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 13; j++)
      deck.push_back(ranks[j] + suits[i]);
}

class Hand{
private:
  std::vector<string> cards;
  bool isDiler = 0;
  unsigned short softness = 0, aces = 0;
public:
  unsigned short value = 0;
  bool defeat = 0;
  bool blackJack = 0;

  void dealer(){
    isDiler = 1;
  }

  void newCard(){
    int r;

    SLEEP;
    srand(time(NULL));

    r = rand() % deck.size();

    cards.push_back(deck[r]);
    deck.erase(deck.begin() + r);
    value += valueMap[cards.back()[0]];
    aces += cards.back()[0] == 'A' ? 1 : 0;

    check();
  }

  string show(){
    string hand;
    int displayValue;

    if(isDiler){
      hand = "Dealer has " + cards[0] + (cards.size() > 1 ? (" and " + to_string(cards.size() - 1) + " other cards") : "");
      displayValue = valueMap[cards[0][0]];
    }
    else{
      displayValue = value;
      hand = "Your hand: ";
      for(int i = 0; i < cards.size(); i++)
        hand += cards[i] + ' ';
    }
    hand += "\nValue of hand - " + to_string(displayValue) + "\n\n";
    return hand;
  }

  string lastShow(){
    string hand = "";

    if(isDiler)
      hand += "Dealer's hand:\n";
    else
      hand += "Your hand:\n";

    for(int i = 0; i < cards.size(); i++){
      hand += cards[i] + ' ';
    }

    hand += "\nValue of hand - " + to_string(value) + '\n';

    return hand;
  }

  void start(){
    newCard();
    newCard();
  }


  void check(){
    if(value > 21){
      if(aces - softness > 0){
        value -= 10;
        softness++;
      }
      else
        defeat = 1;
    }
    else if(value == 21 && cards.size() == 2)
      blackJack = 1;
  }
};

int main(){

  formDeck();

  string answer;

  Hand player, dealer;

  dealer.dealer();

  cout << "Welcome to Black Jack!\n";

  dealer.start();
  player.start();

  cout << dealer.show() << player.show() << '\n';

  while(dealer.value < 16)
    dealer.newCard();

  while(!player.defeat || player.value != 21){

    do {
      cout << "Hit or stand?\n";
      cin >> answer;
    }
    while(answer[0] != 'h' && answer[0] != 's' && !player.defeat);


    if(answer[0] == 'h')
      player.newCard();
    else
      break;
    cout << '\n' << dealer.show() << player.show() << '\n';

  }

  cout << '\n' << dealer.lastShow() << player.lastShow() << '\n';

  if(player.defeat || (player.value < dealer.value && !dealer.defeat))
    cout << "You've last.\n";
  else {
    cout << "You won!\n";
  }

  return 0;
}
