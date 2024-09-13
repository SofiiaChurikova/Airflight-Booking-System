#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

using namespace std;

class RAII {
    int fileDescriptor;
    const size_t bufferSize = 1024;
    char buffer[1024];

public:
    RAII(const string &filePath) {
        fileDescriptor = open(filePath.c_str(), O_RDONLY);
        if (fileDescriptor == -1) {
            throw runtime_error("Error opening file");
        }
    }

    ~RAII() {
        if (fileDescriptor != -1) {
            close(fileDescriptor);
        }
    }

    string readFile() {
        string fileContent;
        ssize_t bytesRead;

        while ((bytesRead = read(fileDescriptor, buffer, bufferSize)) > 0) {
            fileContent.append(buffer, bytesRead);
        }

        if (bytesRead == -1) {
            throw runtime_error("Error reading file");
        }

        return fileContent;
    }
};

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

    char getSeatLetter() const {
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
    int price;

public:
    Ticket(const string &passengerName, const Seat &seat, const string &flightNumber, const string &date,
           int ticketId, int price) : passengerName(passengerName), seat(seat), flightNumber(flightNumber),
                                      date(date), ticketId(ticketId), price(price) {
    }

    string getPassengerName() const {
        return this->passengerName;
    }

    int getSeatNumber() const {
        return this->seat.getSeatNumber();
    }

    char getSeatLetter() const {
        return this->seat.getSeatLetter();
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

    int getPrice() const {
        return this->seat.getPrice();
    }

    void PrintTicketInfo() {
        cout << "Ticket info\n";
        cout << "Flight number: " << flightNumber << "\n";
        cout << "Date: " << date << "\n";
        cout << "Seat number: " << seat.getSeatNumber() << seat.getSeatLetter() << "\n";
        cout << "Price: " << seat.getPrice() << "$" << "\n";
        cout << "Passenger name: " << passengerName << "\n";
    }
};

class Passenger {
    string username;
    vector<Ticket> tickets;

public:
    Passenger(const string &username) : username(username) {
    }

    string getUsername() const {
        return this->username;
    }

    vector<Ticket> getTickets() const {
        return this->tickets;
    }

    void addTicket(const Ticket &ticket) {
        this->tickets.push_back(ticket);
    }

    void returnTicket(int ticketId) {
        for (size_t i = 0; i < tickets.size(); ++i) {
            if (tickets[i].getTicketId() == ticketId) {
                cout << "Confirmed " << tickets[i].getPrice() << "$ refund for "
                        << tickets[i].getPassengerName() << endl;
                tickets.erase(tickets.begin() + i);
                return;
            }
        }
    }
};


class Airplane {
    string date;
    string flightNumber;
    vector<Seat> availableSeat;

public:
    Airplane(const string &date, const string &flightNumber,
             const vector<Seat> &availableSeat)
        : date(date), flightNumber(flightNumber),
          availableSeat(availableSeat) {
    }

    string getDate() const {
        return this->date;
    }

    string getFlightNumber() const {
        return this->flightNumber;
    }

    vector<Seat> &getAvailableSeat() {
        return this->availableSeat;
    }
};


class DataParser {
public:
    vector<Airplane> parseData(const string &userFile) {
        vector<Airplane> airplanes;
        RAII file(userFile);
        string fileContent = file.readFile();
        istringstream fileStream(fileContent);
        string line;
        while (getline(fileStream, line)) {
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

            Airplane airplane(date, flightNumber, availableSeat);
            airplanes.push_back(airplane);
        }

        return airplanes;
    }
};

class Config {
public:
    string filePath = "/Users/macbook/Documents/Airflight-Booking-System/airplane_text.txt";
};


class CommandExecutor {
public:
    string file;

    CommandExecutor(const Config &config) : file(config.filePath) {
        airplanes = parseFile.parseData(file);
    }

    DataParser parseFile;
    vector<Airplane> airplanes = parseFile.parseData(file);
    vector<Ticket> tickets;
    vector<Seat> seats;
    vector<Passenger> passengers;
    int ticketId = 1;

public:
    Airplane *FindFlight(const string &date, const string &flightNumber) {
        for (auto &airplane: this->airplanes) {
            if (airplane.getDate() == date && airplane.getFlightNumber() == flightNumber) {
                return &airplane;
            }
        }
        return nullptr;
    }

    void CheckSeats(const string &date, const string &flightNumber) {
        Airplane *airplane = FindFlight(date, flightNumber);
        if (airplane != nullptr) {
            for (const auto &seat: airplane->getAvailableSeat()) {
                if (seat.getAvailable()) {
                    cout << seat.getSeatNumber() << seat.getSeatLetter() << " " << seat.getPrice() << "$\n";
                }
            }
        } else {
            cout << "\033[31m" << "We couldn't find the flight with the given date and number." << "\033[0m" << endl;
        }
    }

    void BookSeat(const string &date, const string &flightNumber, const string &place, const string &username) {
        Airplane *airplane = FindFlight(date, flightNumber);
        if (airplane != nullptr) {
            int seatNumber = stoi(place.substr(0, place.size() - 1));
            char seatLetter = place.back();
            bool seatExists = false;
            for (Seat &seat: airplane->getAvailableSeat()) {
                if (seat.getSeatNumber() == seatNumber && seat.getSeatLetter() == seatLetter) {
                    seatExists = true;
                    if (!seat.getAvailable()) {
                        cout << "\033[31m" << "The seat " << seatNumber << seatLetter << " is already booked." <<
                                "\033[0m" << endl;
                        return;
                    }
                    seat.seatNotAvailable();
                    Ticket newTicket(username, seat, flightNumber, date, ticketId, seat.getPrice());
                    tickets.push_back(newTicket);
                    ticketId++;
                    bool passengerExists = false;
                    for (auto &passenger: passengers) {
                        if (passenger.getUsername() == username) {
                            passenger.addTicket(newTicket);
                            passengerExists = true;
                            break;
                        }
                    }

                    if (!passengerExists) {
                        Passenger newPassenger(username);
                        newPassenger.addTicket(newTicket);
                        passengers.push_back(newPassenger);
                    }

                    cout << "Confirmed with ID " << newTicket.getTicketId() << ".\n";
                    return;
                }
            }
            if (!seatExists) {
                cout << "\033[31m" << "The seat " << seatNumber << seatLetter << " does not exist on this flight." <<
                        "\033[0m" << endl;
            }
        } else {
            cout << "\033[31m" << "We couldn't find the flight with the given date and number." << "\033[0m" << endl;
        }
    }

    void ReturnSeat(const int &id) {
        Ticket *ticketToReturn = nullptr;
        for (auto &ticket: this->tickets) {
            if (ticket.getTicketId() == id) {
                ticketToReturn = &ticket;
                break;
            }
        }
        if (!ticketToReturn) {
            cout << "\033[31m" << "Ticket ID " << id << " not found." << "\033[0m" << endl;
            return;
        }

        for (auto &airplane: this->airplanes) {
            if (airplane.getDate() == ticketToReturn->getDate() && airplane.getFlightNumber() == ticketToReturn->
                getFlightNumber()) {
                for (auto &seat: airplane.getAvailableSeat()) {
                    if (seat.getSeatNumber() == ticketToReturn->getSeatNumber() && seat.getSeatLetter() ==
                        ticketToReturn->getSeatLetter()) {
                        for (auto &passenger: this->passengers) {
                            if (passenger.getUsername() == ticketToReturn->getPassengerName()) {
                                seat.seatAvailable();
                                passenger.returnTicket(id);
                                for (size_t i = 0; i < this->tickets.size(); ++i) {
                                    if (this->tickets[i].getTicketId() == id) {
                                        this->tickets.erase(this->tickets.begin() + i);
                                        break;
                                    }
                                }
                                return;
                            }
                        }
                    }
                }
            }
        }
    }


    void ViewByID(const int &id) {
        bool found = false;
        for (auto &ticket: this->tickets) {
            if (ticket.getTicketId() == id) {
                ticket.PrintTicketInfo();
                cout << "---------------------------------" << endl;
                found = true;
                return;
            }
        }
        if (!found) {
            cout << "\033[31m" << "Ticket ID " << id << " not found" << "\033[0m" << endl;
        }
    }

    void ViewUsername(const string &username) {
        bool found = false;
        for (auto &passenger: this->passengers) {
            if (passenger.getUsername() == username)
                for (auto &ticket: this->tickets) {
                    if (ticket.getPassengerName() == username) {
                        ticket.PrintTicketInfo();
                        cout << "---------------------------------" << endl;
                        found = true;
                    }
                }
        }
        if (!found) {
            cout << "\033[31m" << "Username " << username << " not found" << "\033[0m" << endl;
        }
    }

    void ViewByFlight(const string &flight, const string &date) {
        bool flightExists = false;
        bool ticketsFound = false;
        for (const auto &airplane: airplanes) {
            if (airplane.getFlightNumber() == flight && airplane.getDate() == date) {
                flightExists = true;
                for (const auto &ticket: tickets) {
                    if (ticket.getFlightNumber() == flight && ticket.getDate() == date) {
                        cout << ticket.getSeatNumber() << ticket.getSeatLetter() << " " << ticket.getPassengerName() <<
                                " " << ticket.getPrice() << "$" << endl;
                        ticketsFound = true;
                    }
                }
                if (!ticketsFound) {
                    cout << "\033[31m" << "No booked tickets found for flight " << flight << " on date " << date << "\033[0m" << endl;
                }
                return;
            }
        }
        if (!flightExists) {
            cout << "\033[31m" << "The flight with number " << flight << " on date " << date << " does not exist." << "\033[0m" << endl;
        }
    }
};

class ParseInput {
private:
    bool continueApp = true;
    CommandExecutor commandExecutor;

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
    ParseInput(const Config &config) : commandExecutor(config) {
    }

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
                        commandExecutor.CheckSeats(date, flightNumber);
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
                        commandExecutor.BookSeat(date, flightNumber, place, username);
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
                        commandExecutor.ReturnSeat(id);
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
                            commandExecutor.ViewUsername(username);
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
                            commandExecutor.ViewByFlight(flight, date);
                        } else {
                            errorTooManyArguments(command, "2 arguments ('Flight' and 'Date')");
                        }
                    }
                } else if (isNumber(param)) {
                    int id = stoi(param);
                    string additionalArgument;
                    if (!(iss >> additionalArgument)) {
                        commandExecutor.ViewByID(id);
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
    Config config;
    ParseInput parseInput(config);
    parseInput.InputReader();
    return 0;
}
