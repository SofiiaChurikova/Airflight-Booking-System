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
            cout << "\033[31m" << "An error occurred while opening the " << userFile << "\033[31m" << endl;
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
private:
    bool continueApp = true;

    bool isNumber(const string &userString) {
        for (char c: userString) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    void errorTooFewArguments(const string &command, const string &expectedArgs) {
        cout << "\033[31m" << "Error: Too few arguments. Command '" << command << "' requires " << expectedArgs <<
                "\033[0m" << endl;
    }

    void errorTooManyArguments(const string &command, const string &expectedArgs) {
        cout << "\033[31m" << "Error: Too many arguments. Command '" << command << "' requires " << expectedArgs <<
                "\033[0m" << endl;
    }

    void changeContinue() {
        continueApp = false;
    }

public:
    void InputReader() {
        while (continueApp) {
            string input;
            cout << "Enter a command ('stop' to end program): ";
            getline(cin, input);
            string command;
            istringstream iss(input);
            iss >> command;

            if (command == "stop") {
                cout << "Thanks for using this program!" << endl;
                changeContinue();
            } else if (command == "check") {
                string date, flightNumber;
                if (!(iss >> date >> flightNumber)) {
                    errorTooFewArguments(command, "2 arguments ('Date' and 'Flight Number')");
                } else {
                    string additionalArgument;
                    if (!(iss >> additionalArgument)) {
                        cout << "Command: " << command << "\nDate: " << date << "\nFlight Number: " << flightNumber <<
                                endl;
                    } else {
                        errorTooManyArguments(command, "2 arguments ('Date' and 'Flight Number')");
                    }
                }
            } else if (command == "book") {
                string date, flightNumber, place, username;
                if (!(iss >> date >> flightNumber >> place >> username)) {
                    errorTooFewArguments(command, "4 arguments ('Date', 'Flight Number', 'Place' and 'Username')");
                } else {
                    string additionalArgument;
                    if (!(iss >> additionalArgument)) {
                        cout << "Command: " << command << "\nDate: " << date << "\nFlight Number: " << flightNumber
                                << "\nPlace: " << place << "\nUsername: " << username << endl;
                    } else {
                        errorTooManyArguments(command, "4 arguments ('Date', 'Flight Number', 'Place' and 'Username')");
                    }
                }
            } else if (command == "return") {
                int id;
                if (!(iss >> id)) {
                    errorTooFewArguments(command, "1 argument ('Id')");
                } else {
                    string additionalArgument;
                    if (!(iss >> additionalArgument)) {
                        cout << "Command: " << command << "\nId: " << id << endl;
                    } else {
                        errorTooManyArguments(command, "1 argument ('Id')");
                    }
                }
            } else if (command == "view") {
                string param;
                if (!(iss >> param)) {
                    errorTooFewArguments(command, "1 argument ('username', 'flight', or 'ID')");
                } else if (param == "username") {
                    string username;
                    if (!(iss >> username)) {
                        errorTooFewArguments(command, "1 argument ('Username')");
                    } else {
                        string additionalArgument;
                        if (!(iss >> additionalArgument)) {
                            cout << "Command: " << command << "\nUsername: " << username << endl;
                        } else {
                            errorTooManyArguments(command, "1 argument ('Username')");
                        }
                    }
                } else if (param == "flight") {
                    string flight, date;
                    if (!(iss >> flight >> date)) {
                        errorTooFewArguments(command, "2 arguments ('Flight' and 'Date')");
                    } else {
                        string additionalArgument;
                        if (!(iss >> additionalArgument)) {
                            cout << "Command: " << command << "\nFlight: " << flight << "\nDate: " << date << endl;
                        } else {
                            errorTooManyArguments(command, "2 arguments ('Flight' and 'Date')");
                        }
                    }
                } else if (isNumber(param)) {
                    int id = stoi(param);
                    string additionalArgument;
                    if (!(iss >> additionalArgument)) {
                        cout << "Command: " << command << "\nID: " << id << endl;
                    } else {
                        errorTooManyArguments(command, "1 argument ('ID')");
                    }
                } else {
                    cout << "\033[31m" <<
                            "Error: Invalid parameter for 'view' command. Expected 'username', 'flight', or an 'ID'." <<
                            "\033[0m" << endl;
                }
            } else {
                cout << "\033[31m" << "Unknown command: " << command << "\nTry again." << "\033[0m" << endl;
            }
        }
    }
};

int main() {
    string file = "/Users/macbook/Documents/Airflight-Booking-System/airplane_text.txt";
    ParseInput parseInput;
    parseInput.InputReader();
    return 0;
}
