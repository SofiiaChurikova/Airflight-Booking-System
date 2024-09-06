#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

class Seat {
    int seatNumber;
    char seatLetter;
    bool isAvailable;
    int price;

public:
    Seat(int seatNumber, char seatLetter, int price,
         bool isAvailable = true) : seatNumber(seatNumber), seatLetter(seatLetter),
                                    price(price), isAvailable(isAvailable) {
    }

    char getRowLetter() const {
        return this->seatLetter;
    }

    int getSeatNumber() const {
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

    int getSeatNumber() const {
        return this->seat.getSeatNumber();
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
        cout << "Seat number: " << seat.getSeatNumber() << "\n";
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
             const vector<Seat> &availableSeat) : date(date), flightNumber(flightNumber), seatsNum(seatsNum),
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

    vector<Seat> getAvailableSeat() const {
        return this->availableSeat;
    }
};

class Parser {
public:
    vector<Airplane> parseData(const string &userFile) {
        vector<Airplane> airplanes;
        ifstream file(userFile);
        string line;
        if (!file.is_open()) {
            cout << "An error occurred while opening the " << userFile << endl;
            return airplanes;
        }

        while (getline(file, line)) {
            istringstream iss(line);
            string date, flightNumber;
            int seatsPerRow;
            iss >> date >> flightNumber >> seatsPerRow;
            vector<Seat> availableSeat;
            while (iss) {
                int startRow, endRow, priceForRow;
                char dollarSign;
                if (iss >> startRow) {
                    iss.ignore(1, '-');
                    if (iss >> endRow >> priceForRow) {
                        for (int row = startRow; row <= endRow; ++row) {
                            for (int i = 0; i < seatsPerRow; ++i) {
                                char seatLetter = 'A' + i;
                                int seatNumber = (row - 1) * seatsPerRow + (i + 1);
                                Seat seat(seatNumber, seatLetter, priceForRow, true);
                                availableSeat.push_back(seat);
                            }
                        }
                        iss >> dollarSign;
                    }
                }
            }

            Airplane airplane(date, flightNumber, seatsPerRow, availableSeat);
            airplanes.push_back(airplane);
        }

        return airplanes;
    }
};

class ParseInput {
public:
};

int main() {
    return 0;
}
