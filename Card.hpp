#include <iostream>
#include <unordered_map>
#include "Exceptions.hpp"

enum class Value{
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

enum class Suit{
    Clubs,
    Spades,
    Hearts,
    Diamonds
};

std::unordered_map<std::string, Value> StringToVal = {
    {"Two", Value::Two}, {"Three", Value::Three}, {"Four", Value::Four},
    {"Five", Value::Five}, {"Six", Value::Six}, {"Seven", Value::Seven},
    {"Eight", Value::Eight}, {"Nine", Value::Nine}, {"Ten", Value::Ten},
    {"Jack", Value::Jack}, {"Queen", Value::Queen}, {"King", Value::King}, 
    {"Ace", Value::Ace},
};

std::unordered_map<std::string, Suit> StringToSuit = {
    {"Clubs", Suit::Clubs}, {"Spades", Suit::Spades}, 
    {"Hearts", Suit::Hearts}, {"Diamonds", Suit::Diamonds}
};



class Card{
    friend class Deck;
    friend class WinCheck;

    Value val;
    Suit suit;

    public:
    Card(){}
    Card(Value v, Suit s) : val(v), suit(s) {}

    const Value& get_Val() const{return val;}

    const Suit& get_Suit() const{return suit;}

    friend std::istream& operator>>(std::istream& is, Card &card);
    friend std::ostream& operator<<(std::ostream& os, const Card& card);

    bool operator<(const Card& b) {return get_Val() < b.get_Val();}
    bool operator>(const Card& b) {return get_Val() > b.get_Val();}
    bool operator==(const Card& b) {return get_Val() == b.get_Val();}
    bool operator<=(const Card& b) {return !(*this > b);} 
    bool operator>=(const Card& b) {return !(*this < b);}
    bool operator!=(const Card& b) {return !(*this == b);}

    static Value nextRank(Value val){
        if(val == Value::Ace) return Value::Ace;
        return (Value) (((int) val) + 1);
    }
};




std::istream& operator>>(std::istream& is, Card &card){
    std::string a, b;
    is >> a >> b >> b;

    if(StringToVal.find(a) == StringToVal.end() 
    || StringToSuit.find(b) == StringToSuit.end()
    ) throw InvalidInput();

    Card temp(StringToVal[a], StringToSuit[b]);
    std::swap(temp.val, card.val);
    std::swap(temp.suit, card.suit);

    return is;
}

std::ostream& operator<<(std::ostream& os, const Card &card) {
    switch(card.get_Val()){

        case Value::Two:
            os << "Two";
            break;
        case Value::Three:
            os << "Three";
            break;
        case Value::Four:
            os << "Four";
            break;
        case Value::Five:
            os << "Five";
            break;
        case Value::Six:
            os << "Six";
            break;
        case Value::Seven:
            os << "Seven";
            break;
        case Value::Eight:
            os << "Eight";
            break;
        case Value::Nine:
            os << "Nine";
            break;
        case Value::Ten:
            os << "Ten";
            break;
        case Value::Jack:
            os << "Jack";
            break;
        case Value::Queen:
            os << "Queen";
            break;
        case Value::King:
            os << "King";
            break;
        case Value::Ace:
            os << "Ace";
            break;
    }

    os << " of ";

    switch(card.get_Suit()){

        case Suit::Clubs:
            os << "Clubs";
            break;
        case Suit::Spades:
            os << "Spades";
            break;
        case Suit::Hearts:
            os << "Hearts";
            break;
        case Suit::Diamonds:
            os << "Diamonds";
            break;
    }

    os << "\n";

    return os;
}

