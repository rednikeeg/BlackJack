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

std::vector<std::string> deck;
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
  std::vector<std::string> ranks = {
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

  std::vector<std::string> suits = {
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
  std::vector<std::string> cards;
  bool isDiler = 0;
  unsigned short softness = 0, aces = 0;
public:
  unsigned short value = 0;
  bool defeat = 0;
  bool blackjack = 0;

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

  std::string show(){
    std::string hand = "\n";
    int displayValue;

    if(isDiler){
      hand += "Dealer has " + cards[0] + (cards.size() > 1 ? (" and " + std::to_string(cards.size() - 1) + " other cards") : "");
      displayValue = valueMap[cards[0][0]];
    }
    else{
      displayValue = value;
      hand += "Your hand: ";
      for(int i = 0; i < cards.size(); i++)
        hand += cards[i] + ' ';
    }
    hand += "\nValue of hand - " + std::to_string(displayValue) + "\n\n";
    return hand;
  }

  std::string lastShow(){
    std::string hand = "\n";

    if(isDiler)
      hand += "Dealer's hand:\n";
    else
      hand += "Your hand:\n";

    for(int i = 0; i < cards.size(); i++){
      hand += cards[i] + ' ';
    }

    hand += "\nValue of hand - " + std::to_string(value) + '\n';

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
      blackjack = 1;
  }
};

int main(){

  formDeck();

  std::string answer;

  Hand player, dealer;

  dealer.dealer();

  std::cout << "Welcome to Black Jack!\n";

  dealer.start();
  player.start();

  std::cout << dealer.show() << player.show() << '\n';

  //Making final hand for dealer
  while(dealer.value < 16)
    dealer.newCard();

  while(!player.defeat && player.value != 21){
    //Player's move
    do {
      std::cout << "Hit or stand?\n";
      std::cin >> answer;
    }
    while(answer[0] != 'h' && answer[0] != 's');

    if(answer[0] == 'h')
      player.newCard();
    else
      break;
    //Game at the moment
    std::cout << dealer.show() << player.show();

  }

  std::cout << dealer.lastShow() << player.lastShow() << '\n';

  //Defeat
  if(player.defeat || (player.value < dealer.value && !dealer.defeat))
    std::cout << "You've lost.\n";
  //Endgame by blackjack
  else if(player.value == dealer.value) {
    if(!player.blackjack && dealer.blackjack){
      std::cout << "Values of hands are the same, but dealer got the blackjack.\nYou've last.\n";
    }
    else if (player.blackjack && !dealer.blackjack){
      std::cout << "Values of hands are the same, but you got the blackjack.\nYou've won.\n";
    }
    else{
      std::cout << "It's draw.\n";
    }
  }
  //Victory
  else{
    std::cout << "You've won.\n";
  }
  return 0;
}
