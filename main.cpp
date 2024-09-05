#include <iostream>
#include <vector>

using namespace std;

class Seat {
    string seatNumber;
    bool isAvailable;
    int price;

public:
    Seat(const string &seatNumber, int price, bool isAvailable = true) : seatNumber(seatNumber),
                                                                         price(price), isAvailable(isAvailable) {
    }

    string getSeat() const {
        return this->seatNumber;
    }

    int getPrice() const {
        return this->price;
    }

    bool getAvailable() const {
        return this->isAvailable;
    }

    void seatAvailable() {
        this->isAvailable = true;
    }

    void seatNotAvailable() {
        this->isAvailable = false;
    }
};

class Ticket {
    string passengerName;
    Seat seat;
    string flightNumber;
    string date;
    int ticketId;

public:
    Ticket(const string &passengerName, const Seat &seat, const string &flightNumber, const string &date,
           int ticketId) : passengerName(passengerName), seat(seat), flightNumber(flightNumber), date(date),
                           ticketId(ticketId) {
    }

    string getPassengerName() const {
        return this->passengerName;
    }

    string getSeatNumber() const {
        return this->seat.getSeat();
    }

    string getFlightNumber() const {
        return this->flightNumber;
    }

    string getDate() const {
        return this->date;
    }

    int getTicketId() const {
        return this->ticketId;
    }
    void PrintTicketInfo() {
        cout << "Ticket info\n";
        cout << "Flight number: " << flightNumber << "\n";
        cout << "Date: " << date << "\n";
        cout << "Passenger name: " << passengerName << "\n";
        cout << "Seat number: " << seat.getSeat() << "\n";
        cout << "Ticket ID: " << ticketId << endl;
    }
};

class Airplane {
    string date;
    string flightNumber;
    int seatsNum;
    vector<Seat> availableSeat;

public:
    Airplane(const string &date, const string &flightNumber, int seatsNum,
             const vector<Seat> &availableSeat): date(date), flightNumber(flightNumber), seatsNum(seatsNum),
                                                 availableSeat(availableSeat) {
    }

    string getDate() const {
        return this->date;
    }

    string getFlightNumber() const {
        return this->flightNumber;
    }

    int getSeatsNum() const {
        return this->seatsNum;
    }

    vector<Seat> getAvailableSeat() {
        return this->availableSeat;
    }
};


int main() {
    return 0;
}
