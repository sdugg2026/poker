#include "Card.hpp"
#include <utility>
#include <random>
#include <functional>
#include <queue>
#include <algorithm>
#include <chrono>
using playerHand = std::pair< Card, Card >;


std::unordered_map<int, std::string> HANDS = {
    {0, "High Card"}, {1, "Pair"}, {2, "Two Pair"},
    {3, "Three of a Kind"}, {4, "Straight"}, {5, "Flush"},
    {6, "Full house"}, {7, "Four of a Kind"}, {8, "Straight Flush"},
    {9, "Royal Flush"}
};

class RandomNumberGenerator{
    std::random_device rd;
    int x, y;
    std::uniform_int_distribution<int> dist;
    public:
    RandomNumberGenerator(int x1, int y1): x(x1), y(y1), dist(x,y) {}
    int operator()(){
        return dist(rd);
    }
};
RandomNumberGenerator rng(0, 51);

class CheckWin{

    std::vector<Card> AllCards;
    std::unordered_map< Value, int> ofAKind;
    uint numSpades = 0, numClubs = 0, numHearts = 0, numDiamonds = 0;

    public:
    CheckWin(std::vector<Card> communityCards){ 
        AllCards.reserve(7);
        sort(communityCards.begin(), communityCards.end(), [](Card&a, Card&b){ return a < b;});
        for(auto &a: communityCards) add(a);
    }

    void add(Card& a){
        if(a.get_Suit() == Suit::Spades) numSpades++;
        else if(a.get_Suit() == Suit::Diamonds) numDiamonds++;
        else if(a.get_Suit() == Suit::Hearts) numHearts++;
        else numClubs++;

        ofAKind[a.get_Val()]++;
        AllCards.push_back(a);
    }

    bool RoyalFlush();
    bool StraightFlush(Value &beginningValue);
    bool FourofAKind(); // done
    bool FullHouse(); // done
    bool Flush(); // done
    bool Straight(uint start, uint end, Value &beginningValue);
    bool ThreeofAKind(); // done
    bool TwoPair(); // done
    bool Pair(); // done

    int bestPossibleHand();
};

class Game{
    int NUM_SIMULATIONS;
    uint NUM_OPPONENTS;
    
    double numWins = 0, totalSimulations = 0;
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

        Deck(const Deck& other){ for(const auto &a: other.deck) deck.push_back(a) ;}

        const Card deal();
        void reset();
        void shuffle();
    };


    Deck GameDeck;
    std::vector< playerHand > OPPONENTS_HANDS;
    std::vector< Card > CommunityCards;
    playerHand USER_HAND;

    public:

    Game(playerHand in, int numSim, uint numOpps = 1): 
    NUM_SIMULATIONS(numSim), 
    NUM_OPPONENTS(numOpps) 
    {
        USER_HAND = {std::move(in.first), std::move(in.second)};
    }
    

    void simulate(){
        std::cout << std::endl;
        while(NUM_SIMULATIONS--){
            GameDeck.shuffle();
            for(uint i = 0; i < NUM_OPPONENTS; i++) OPPONENTS_HANDS.push_back({GameDeck.deal(), GameDeck.deal()});
            while(CommunityCards.size() != 5) CommunityCards.push_back(GameDeck.deal());

            std::vector<Card> temp = CommunityCards;
            temp.push_back(USER_HAND.first);
            temp.push_back(USER_HAND.second);
            CheckWin user(temp);
            int userScore = user.bestPossibleHand(),
            BestOpponent = -1;

            //for(auto a: temp) std::cout << a;
            //std::cout << std::endl;

            for(uint i = 0; i < NUM_OPPONENTS; i++){
                temp.pop_back();
                temp.pop_back();
                temp.push_back(OPPONENTS_HANDS[i].first);
                temp.push_back(OPPONENTS_HANDS[i].second);
                CheckWin oppScore(temp);
                BestOpponent = std::max(BestOpponent, oppScore.bestPossibleHand());

                //for(auto a: temp) std::cout << a;
                //std::cout << std::endl;
            }

            //std::cout << HANDS[userScore] << " " << HANDS[BestOpponent] << std::endl;
            if(userScore > BestOpponent){
                numWins++;
                //std::cout << "Win\n";
            } 
            /*else std::cout << "Lose\n";

            std::cout << "////////////////////////////////////////\n";*/

            

            OPPONENTS_HANDS.clear();
            GameDeck.reset();
            CommunityCards.clear();
            totalSimulations++;
        }
    }

    void printResults(){
        std::cout << "\nUser Hand:\n" << USER_HAND.first << USER_HAND.second;
        std::cout << numWins/totalSimulations << std::endl;
    }
};






//********** MESSY IMPLEMENTATIONS THAT NOONE CARES ABOUT **********//
void Game::Deck::reset(){
    Deck temp;
    std::swap(temp.deck, this->deck);
}

const Card Game::Deck::deal(){
    Card top = deck.back();
    deck.pop_back();

    return top;
}

void Game::Deck::shuffle(){
    
    for (int i=0; i<(int)deck.size() ;i++)
    {
        int r = rng();
        std::swap(deck[(uint)i], deck[(uint)r]);
    }
}

bool CheckWin::RoyalFlush(){
    Value v;
    bool ans = StraightFlush(v);
    return ans && v == Value::Ten;
}
bool CheckWin::StraightFlush(Value &beginningValue){
    if(!Flush()) return false;
    Suit suit;
    if(numClubs > 4) suit = Suit::Clubs;
    else if(numSpades > 4) suit = Suit::Spades;
    else if(numDiamonds > 4) suit = Suit::Diamonds;
    else if(numHearts > 4) suit = Suit::Hearts;

    std::vector<Card> temp = AllCards;
    std::stable_partition(AllCards.begin(), AllCards.end(), [&](Card& a) { return (int)a.get_Suit() == (int)suit;} );   


    bool ans = Straight(0, 5, beginningValue);
    AllCards = temp;
    return ans;
}
bool CheckWin::FourofAKind(){
    for(auto a: ofAKind) if(a.second > 3) return true;
    return false;
}
bool CheckWin::FullHouse(){
    bool three = false, two = false;
    for(auto a: ofAKind){
        if(!three && a.second > 2) three = true;
        else if(a.second > 1) two = true;
    }
    return three && two;
}
bool CheckWin::Flush(){
    return numSpades > 4 || numClubs > 4 || numDiamonds > 4 || numHearts > 4;
}
bool CheckWin::Straight(uint start, uint end, Value &beginningValue){
    for(uint i = start; i < end; i++){
        if(AllCards[i].get_Val() > Value::Ten) continue;
        Value second = Card::nextRank(AllCards[i].get_Val()), third = Card::nextRank(second),
        fourth = Card::nextRank(third), fifth = Card::nextRank(fourth);

        if(ofAKind[second] 
        && ofAKind[third] 
        && ofAKind[fourth] 
        && ofAKind[fifth]){
            beginningValue = AllCards[i].get_Val();
            return true;
        }
    }

    if(ofAKind[Value::Ace] 
    && ofAKind[Value::Two] 
    && ofAKind[Value::Three] 
    && ofAKind[Value::Four] 
    && ofAKind[Value::Five]) return true; // exception
    return false;
}
bool CheckWin::ThreeofAKind(){
    for(auto a: ofAKind) if(a.second > 2) return true;
    return false;
}
bool CheckWin::TwoPair(){
    int count = 0;
    for(auto a: ofAKind) if(a.second > 1) count++;
    return count > 1;
}
bool CheckWin::Pair(){
    for(auto a: ofAKind) if(a.second > 1) return true;
    return false;
}


int CheckWin::bestPossibleHand(){
    uint start = 0, end = (uint)AllCards.size();

    Value dummy;
    if(RoyalFlush()) return 9;
    else if(StraightFlush(dummy)) return 8;
    else if(FourofAKind()) return 7;
    else if(FullHouse()) return 6;
    else if(Flush()) return 5;
    else if(Straight(start, end, dummy)) return 4;
    else if(ThreeofAKind()) return 3;
    else if(TwoPair()) return 2;
    else if(Pair()) return 1;

    return 0;
}

