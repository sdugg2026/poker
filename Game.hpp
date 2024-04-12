#include "Card.hpp"
#include <utility>
using playerHand = std::pair< Card, Card >;

class Game{
    uint NUM_SIMULATIONS;
    private:

    class Deck{
        friend class Game;
        private:

        static const uint DECK_SIZE = 52;
        std::vector<Card> deck;

        Deck(){
            deck.reserve(52);
            for(auto ValEntry: StringToVal){ for(auto SuitEntry: StringToSuit){
                    deck.emplace_back(ValEntry.second, SuitEntry.second);
            }}
        }

        void shuffle();
        void deal();
        void reset();
    };


    Deck GameDeck;
    std::vector< playerHand > PLAYERS_HANDS;
    std::vector< Card > CommunityCards;
    unsigned int NUM_PLAYERS;
    public:

    Game();

    void simulate();
    bool UserWon();
};




//********** MESSY IMPLEMENTATIONS THAT NOONE CARES ABOUT **********//