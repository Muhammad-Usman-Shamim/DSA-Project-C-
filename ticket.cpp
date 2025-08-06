#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // For std::sort
#include <limits>   // For numeric_limits
using namespace std;

struct Train {
    string name;
    string route;
    int price;
    int availableSeats;
    vector<int> seatNumbers;
};

struct Reservation {
    int seatNumber;
    string trainName;
    string passengerName;
    string fatherName;
    string phoneNumber;
    string cnic;
    string fromCity;
    string toCity;
    string travelDate;
};

vector<Train> trains = {
    {"Greenline", "Karachi to Islamabad", 8000, 50, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
    {"Awami Express", "Karachi to Rawalpindi", 8000, 50, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
    {"Tezz Gam", "Karachi to Rawalpindi", 8000, 50, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
    {"Karakaram Express", "Karachi to Lahore", 5000, 50, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
    {"Karachi Express", "Karachi to Lahore", 5000, 50, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}}
};

map<int, Reservation> reservations;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool login() {
    string email, password;
    int attempts = 3;
    
    while (attempts > 0) {
        cout << "Enter your email: ";
        getline(cin, email);
        cout << "Enter your password: ";
        getline(cin, password);

        if (email == "usman" && password == "12345") {
            cout << "Login successful!\n";
            return true;
        } else {
            attempts--;
            if (attempts > 0) {
                cout << "Invalid email or password. " << attempts << " attempts remaining.\n";
            } else {
                cout << "Too many failed attempts. Access denied.\n";
                return false;
            }
        }
    }
    return false;
}

void showTrains() {
    cout << "\nAvailable Trains:\n";
    cout << "==========================================================================\n";
    cout << "No. Train Name          Route                       Price      Seats\n";
    cout << "==========================================================================\n";
    
    for (size_t i = 0; i < trains.size(); ++i) {
        printf("%-4d%-20s%-30s%-10d%d\n", 
            i + 1, 
            trains[i].name.c_str(), 
            trains[i].route.c_str(), 
            trains[i].price,
            trains[i].availableSeats);
    }
}

bool isSeatAvailable(const Train& train, int seatNumber) {
    return find(train.seatNumbers.begin(), train.seatNumbers.end(), seatNumber) != train.seatNumbers.end();
}

void makeReservation() {
    cout << "\nWelcome to TrackLine Train Booking Service\n";

    cout << "Enter your Departure city: ";
    string fromCity;
    getline(cin, fromCity);

    cout << "Enter your destination city: ";
    string toCity;
    getline(cin, toCity);

    cout << "Enter your travel date (DD/MM/YYYY): ";
    string travelDate;
    getline(cin, travelDate);

    showTrains();
    
    int choice;
    while (true) {
        cout << "\nSelect a train (1-" << trains.size() << "): ";
        if (!(cin >> choice) || choice < 1 || choice > trains.size()) {
            cout << "Invalid choice! Please enter a number between 1 and " << trains.size() << ".\n";
            clearInputBuffer();
        } else {
            break;
        }
    }

    Train &selectedTrain = trains[choice - 1];

    if (selectedTrain.availableSeats <= 0) {
        cout << "Sorry, no seats available on this train.\n";
        return;
    }

    clearInputBuffer();

    cout << "Enter your name: ";
    string name;
    getline(cin, name);

    cout << "Enter your father's name: ";
    string fatherName;
    getline(cin, fatherName);

    cout << "Enter your phone number: ";
    string phoneNumber;
    getline(cin, phoneNumber);

    cout << "Enter your CNIC: ";
    string cnic;
    getline(cin, cnic);

    cout << "\nAvailable seat numbers: ";
    for (int seat : selectedTrain.seatNumbers) {
        cout << seat << " ";
    }

    int seatNumber;
    while (true) {
        cout << "\nEnter seat number: ";
        if (!(cin >> seatNumber)) {
            cout << "Invalid input! Please enter a number.\n";
            clearInputBuffer();
            continue;
        }

        if (!isSeatAvailable(selectedTrain, seatNumber)) {
            cout << "Seat not available or invalid. Please choose from the available seats.\n";
        } else {
            break;
        }
    }

    // Remove the seat from available seats
    auto it = find(selectedTrain.seatNumbers.begin(), selectedTrain.seatNumbers.end(), seatNumber);
    if (it != selectedTrain.seatNumbers.end()) {
        selectedTrain.seatNumbers.erase(it);
    }
    selectedTrain.availableSeats--;

    reservations[seatNumber] = {
        seatNumber,
        selectedTrain.name,
        name,
        fatherName,
        phoneNumber,
        cnic,
        fromCity,
        toCity,
        travelDate
    };

    cout << "\n\n*************** Ticket Details ***************\n";
    cout << "Train: " << selectedTrain.name << " (" << selectedTrain.route << ")\n";
    cout << "Passenger: " << name << "\n";
    cout << "Father's Name: " << fatherName << "\n";
    cout << "Contact: " << phoneNumber << " (CNIC: " << cnic << ")\n";
    cout << "Journey: " << fromCity << " to " << toCity << "\n";
    cout << "Date: " << travelDate << "\n";
    cout << "Seat: " << seatNumber << "\n";
    cout << "Price: Rs." << selectedTrain.price << "\n";
    cout << "******************************************\n";
}

void cancelReservation() {
    cout << "\nTicket Cancellation\n";

    cout << "Enter your name: ";
    string name;
    getline(cin, name);

    cout << "Enter train name: ";
    string trainName;
    getline(cin, trainName);

    int seatNumber;
    while (true) {
        cout << "Enter seat number: ";
        if (!(cin >> seatNumber)) {
            cout << "Invalid input! Please enter a number.\n";
            clearInputBuffer();
        } else {
            break;
        }
    }

    clearInputBuffer();

    if (reservations.find(seatNumber) != reservations.end() &&
        reservations[seatNumber].passengerName == name &&
        reservations[seatNumber].trainName == trainName) {

        Reservation res = reservations[seatNumber];

        for (auto &train : trains) {
            if (train.name == res.trainName) {
                train.seatNumbers.push_back(seatNumber);
                sort(train.seatNumbers.begin(), train.seatNumbers.end());
                train.availableSeats++;
                break;
            }
        }

        reservations.erase(seatNumber);
        cout << "\nCancellation successful! Your ticket for " << trainName << " has been cancelled.\n";
    } else {
        cout << "\nCancellation failed. No matching reservation found.\n";
    }
}

void showMenu() {
    cout << "\n==========================================\n";
    cout << "   TrackLine Train Reservation System\n";
    cout << "==========================================\n";
    cout << "1. Reserve a Seat\n";
    cout << "2. Cancel Reservation\n";
    cout << "3. View Available Trains\n";
    cout << "4. Exit\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    cout << "Welcome to TrackLine Train Booking Service\n";
    
    if (!login()) {
        return 0;
    }

    int choice;
    while (true) {
        showMenu();
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        switch (choice) {
            case 1:
                makeReservation();
                break;
            case 2:
                cancelReservation();
                break;
            case 3:
                showTrains();
                break;
            case 4:
                cout << "\nThank you for using TrackLine. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
