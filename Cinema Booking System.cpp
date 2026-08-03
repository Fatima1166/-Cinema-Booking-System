// ============================================================
// CINEMA BOOKING SYSTEM
// A simple console-based application for managing movie tickets
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// ============================================================
// COLOR CODES - Makes output look better
// ============================================================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// ============================================================
// MOVIE CLASS
// Stores information about a single movie
// ============================================================
class Movie {
private:
    int id;
    string title;
    string genre;
    int duration;      // in minutes
    string language;
    double rating;     // out of 10

public:
    // Default constructor
    Movie() {
        id = 0;
        duration = 0;
        rating = 0.0;
    }
    
    // Parameterized constructor
    Movie(int movieId, string movieTitle, string movieGenre, 
          int movieDuration, string movieLanguage, double movieRating) {
        id = movieId;
        title = movieTitle;
        genre = movieGenre;
        duration = movieDuration;
        language = movieLanguage;
        rating = movieRating;
    }

    // Getter methods
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getGenre() const { return genre; }
    int getDuration() const { return duration; }
    string getLanguage() const { return language; }
    double getRating() const { return rating; }

    // Setter methods
    void setTitle(string newTitle) { title = newTitle; }
    void setGenre(string newGenre) { genre = newGenre; }
    void setDuration(int newDuration) { duration = newDuration; }
    void setLanguage(string newLanguage) { language = newLanguage; }
    void setRating(double newRating) { rating = newRating; }

    // Display movie info on screen
    void display() const {
        cout << "  ─────────────────────────\n";
        cout << "  ID: " << id << "\n";
        cout << "  Title: " << title << "\n";
        cout << "  Genre: " << genre << "\n";
        cout << "  Duration: " << duration << " minutes\n";
        cout << "  Language: " << language << "\n";
        cout << "  Rating: " << rating << "/10\n";
    }

    // Convert movie to string for saving to file
    string toFileString() const {
        return to_string(id) + "|" + title + "|" + genre + "|" + 
               to_string(duration) + "|" + language + "|" + to_string(rating);
    }

    // Create movie object from file string
    static Movie fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, title, genre, durationStr, language, ratingStr;
        
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, genre, '|');
        getline(ss, durationStr, '|');
        getline(ss, language, '|');
        getline(ss, ratingStr, '|');
        
        return Movie(stoi(idStr), title, genre, stoi(durationStr), 
                     language, stod(ratingStr));
    }
};

// ============================================================
// SHOW CLASS
// Represents a movie screening with date, time, and seats
// ============================================================
class Show {
private:
    int id;
    int movieId;
    string date;          // Format: DD-MM-YYYY
    string time;          // Format: HH:MM
    int hallNumber;
    double ticketPrice;
    int totalSeats;
    vector<bool> seatBooked;  // true = booked, false = available

public:
    // Default constructor
    Show() {
        id = 0;
        movieId = 0;
        hallNumber = 0;
        ticketPrice = 0.0;
        totalSeats = 0;
    }

    // Parameterized constructor
    Show(int showId, int movieIdVal, string showDate, string showTime, 
         int hall, double price, int seatsCount) {
        id = showId;
        movieId = movieIdVal;
        date = showDate;
        time = showTime;
        hallNumber = hall;
        ticketPrice = price;
        totalSeats = seatsCount;
        
        // Initially all seats are available
        seatBooked.resize(totalSeats, false);
    }

    // Getter methods
    int getId() const { return id; }
    int getMovieId() const { return movieId; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    int getHallNumber() const { return hallNumber; }
    double getPrice() const { return ticketPrice; }
    int getTotalSeats() const { return totalSeats; }

    // Check if a specific seat is available
    bool isSeatAvailable(int seatNumber) const {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            return false;
        }
        return !seatBooked[seatNumber - 1];
    }

    // Book a seat
    bool bookSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            return false;
        }
        if (seatBooked[seatNumber - 1]) {
            return false;  // Already booked
        }
        
        seatBooked[seatNumber - 1] = true;
        return true;
    }

    // Cancel a seat booking
    bool cancelSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            return false;
        }
        if (!seatBooked[seatNumber - 1]) {
            return false;  // Not booked
        }
        
        seatBooked[seatNumber - 1] = false;
        return true;
    }

    // Count available seats
    int countAvailableSeats() const {
        int count = 0;
        for (bool seat : seatBooked) {
            if (!seat) count++;
        }
        return count;
    }

    // Display seating arrangement (visual layout)
    void displaySeats() const {
        cout << CYAN << "\n  ╔═══════════════════════════════════════╗\n";
        cout << "  ║           SCREEN (Front)              ║\n";
        cout << "  ╚═══════════════════════════════════════╝\n" << RESET;
        
        int columns = 6;
        cout << "\n  ";
        
        for (int i = 1; i <= totalSeats; i++) {
            if (seatBooked[i - 1]) {
                cout << RED << "[X]" << RESET << " ";
            } else {
                cout << GREEN << "[" << setw(2) << i << "]" << RESET << " ";
            }
            
            // New line after every 6 seats
            if (i % columns == 0 && i != totalSeats) {
                cout << "\n  ";
            }
        }
        
        cout << "\n\n  " << GREEN << "[Available]" << RESET << "  " 
             << RED << "[X = Booked]" << RESET << "\n";
    }

    // Display show details
    void display() const {
        cout << "  Show ID: " << id << "\n";
        cout << "  Date: " << date << " | Time: " << time << "\n";
        cout << "  Hall: " << hallNumber << " | Price: $" 
             << fixed << setprecision(2) << ticketPrice << "\n";
        cout << "  Available Seats: " << countAvailableSeats() 
             << "/" << totalSeats << "\n";
    }

    // Save show to file
    string toFileString() const {
        stringstream ss;
        ss << id << "|" << movieId << "|" << date << "|" << time << "|"
           << hallNumber << "|" << ticketPrice << "|" << totalSeats << "|";
        
        for (bool seat : seatBooked) {
            ss << (seat ? "1" : "0");
        }
        return ss.str();
    }

    // Load show from file
    static Show fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, movieIdStr, date, time, hallStr, priceStr, seatsStr;
        
        getline(ss, idStr, '|');
        getline(ss, movieIdStr, '|');
        getline(ss, date, '|');
        getline(ss, time, '|');
        getline(ss, hallStr, '|');
        getline(ss, priceStr, '|');
        getline(ss, seatsStr, '|');
        
        Show show(stoi(idStr), stoi(movieIdStr), date, time, 
                  stoi(hallStr), stod(priceStr), seatsStr.length());
        
        // Load seat status
        for (int i = 0; i < seatsStr.length(); i++) {
            show.seatBooked[i] = (seatsStr[i] == '1');
        }
        
        return show;
    }
};

// ============================================================
// BOOKING CLASS
// Stores customer booking information
// ============================================================
class Booking {
private:
    int id;
    int showId;
    string customerName;
    string customerPhone;
    string customerEmail;
    vector<int> seatNumbers;
    double totalAmount;
    time_t bookingTime;

public:
    // Default constructor
    Booking() {
        id = 0;
        showId = 0;
        totalAmount = 0.0;
    }

    // Parameterized constructor
    Booking(int bookingId, int showIdVal, string name, string phone, 
            string email, vector<int> seats, double amount) {
        id = bookingId;
        showId = showIdVal;
        customerName = name;
        customerPhone = phone;
        customerEmail = email;
        seatNumbers = seats;
        totalAmount = amount;
        time(&bookingTime);  // Store current time
    }

    // Getter methods
    int getId() const { return id; }
    int getShowId() const { return showId; }
    string getCustomerName() const { return customerName; }
    string getCustomerPhone() const { return customerPhone; }
    string getCustomerEmail() const { return customerEmail; }
    vector<int> getSeatNumbers() const { return seatNumbers; }
    double getTotalAmount() const { return totalAmount; }

    // Display booking details
    void display() const {
        cout << "\n  " << BOLD << "🎟️ Booking #" << id << RESET << "\n";
        cout << "  Show ID: " << showId << "\n";
        cout << "  Customer: " << customerName << "\n";
        cout << "  Phone: " << customerPhone << "\n";
        cout << "  Email: " << customerEmail << "\n";
        cout << "  Seats: ";
        for (int seat : seatNumbers) {
            cout << seat << " ";
        }
        cout << "\n  Total: $" << fixed << setprecision(2) << totalAmount << "\n";
        
        char* dt = ctime(&bookingTime);
        cout << "  Booked on: " << dt;
    }

    // Save booking to file
    string toFileString() const {
        stringstream ss;
        ss << id << "|" << showId << "|" << customerName << "|"
           << customerPhone << "|" << customerEmail << "|" << totalAmount << "|"
           << bookingTime << "|";
        
        for (int seat : seatNumbers) {
            ss << seat << ",";
        }
        return ss.str();
    }

    // Load booking from file
    static Booking fromFileString(const string& line) {
        stringstream ss(line);
        string idStr, showIdStr, name, phone, email, amountStr, timeStr, seatsStr;
        
        getline(ss, idStr, '|');
        getline(ss, showIdStr, '|');
        getline(ss, name, '|');
        getline(ss, phone, '|');
        getline(ss, email, '|');
        getline(ss, amountStr, '|');
        getline(ss, timeStr, '|');
        getline(ss, seatsStr, '|');
        
        // Parse seat numbers
        vector<int> seats;
        stringstream seatSS(seatsStr);
        string seat;
        while (getline(seatSS, seat, ',')) {
            if (!seat.empty()) {
                seats.push_back(stoi(seat));
            }
        }
        
        Booking booking(stoi(idStr), stoi(showIdStr), name, phone, email, 
                        seats, stod(amountStr));
        booking.bookingTime = stol(timeStr);
        return booking;
    }
};

// ============================================================
// MAIN SYSTEM CLASS
// Manages everything - movies, shows, bookings
// ============================================================
class CinemaSystem {
private:
    vector<Movie> movies;
    vector<Show> shows;
    vector<Booking> bookings;
    
    int nextMovieId;
    int nextShowId;
    int nextBookingId;
    
    // File names for data persistence
    const string MOVIE_FILE = "movies.txt";
    const string SHOW_FILE = "shows.txt";
    const string BOOKING_FILE = "bookings.txt";

public:
    // Constructor - loads data from files
    CinemaSystem() {
        nextMovieId = 1;
        nextShowId = 1;
        nextBookingId = 1;
        loadData();
    }

    // Destructor - saves data to files
    ~CinemaSystem() {
        saveData();
    }

    // ============================================================
    // MAIN MENU - Entry point of the application
    // ============================================================
    void run() {
        int choice;
        
        do {
            displayHeader("🎬 CINEMA BOOKING SYSTEM");
            cout << BLUE << "  [1] " << RESET << "Manage Movies\n";
            cout << BLUE << "  [2] " << RESET << "Manage Shows\n";
            cout << BLUE << "  [3] " << RESET << "Book Tickets\n";
            cout << BLUE << "  [4] " << RESET << "Cancel Booking\n";
            cout << BLUE << "  [5] " << RESET << "View All Bookings\n";
            cout << BLUE << "  [6] " << RESET << "View Available Shows\n";
            cout << BLUE << "  [0] " << RESET << "Exit\n";
            cout << "  ════════════════════════════════════════════\n";
            cout << "  Enter your choice: ";
            cin >> choice;
            
            // Clear input buffer
            cin.ignore();
            
            switch (choice) {
                case 1:
                    manageMovies();
                    break;
                case 2:
                    manageShows();
                    break;
                case 3:
                    bookTickets();
                    break;
                case 4:
                    cancelBooking();
                    break;
                case 5:
                    viewAllBookings();
                    break;
                case 6:
                    viewAvailableShows();
                    break;
                case 0:
                    cout << GREEN << "\n  Thank you for using our system!\n" << RESET;
                    cout << "  Have a great day! 🎬\n\n";
                    break;
                default:
                    cout << RED << "\n  Invalid choice! Please try again.\n" << RESET;
            }
            
        } while (choice != 0);
    }

    // ============================================================
    // MOVIE MANAGEMENT FUNCTIONS
    // ============================================================
    void manageMovies() {
        int choice;
        
        do {
            displayHeader("🎬 MANAGE MOVIES");
            cout << BLUE << "  [1] " << RESET << "Add Movie\n";
            cout << BLUE << "  [2] " << RESET << "View All Movies\n";
            cout << BLUE << "  [3] " << RESET << "Update Movie\n";
            cout << BLUE << "  [4] " << RESET << "Delete Movie\n";
            cout << BLUE << "  [0] " << RESET << "Back to Main Menu\n";
            cout << "  ════════════════════════════════════════════\n";
            cout << "  Enter your choice: ";
            cin >> choice;
            cin.ignore();
            
            switch (choice) {
                case 1:
                    addMovie();
                    break;
                case 2:
                    viewAllMovies();
                    break;
                case 3:
                    updateMovie();
                    break;
                case 4:
                    deleteMovie();
                    break;
                case 0:
                    break;
                default:
                    cout << RED << "\n  ❌ Invalid choice!\n" << RESET;
            }
            
        } while (choice != 0);
    }

    void addMovie() {
        string title, genre, language;
        int duration;
        double rating;
        
        cout << "\n  📝 Enter new movie details:\n";
        cout << "  Title: ";
        getline(cin, title);
        
        cout << "  Genre: ";
        getline(cin, genre);
        
        cout << "  Duration (minutes): ";
        cin >> duration;
        cin.ignore();
        
        cout << "  Language: ";
        getline(cin, language);
        
        cout << "  Rating (0-10): ";
        cin >> rating;
        cin.ignore();
        
        // Create and add movie
        Movie newMovie(nextMovieId++, title, genre, duration, language, rating);
        movies.push_back(newMovie);
        
        cout << GREEN << "\n  ✅ Movie added successfully! (ID: " 
             << newMovie.getId() << ")\n" << RESET;
        saveData();
    }

    void viewAllMovies() const {
        displayHeader("📽️ ALL MOVIES");
        
        if (movies.empty()) {
            cout << YELLOW << "\n  No movies in the system yet.\n" << RESET;
            return;
        }
        
        for (const auto& movie : movies) {
            movie.display();
        }
    }

    Movie* findMovieById(int id) {
        for (auto& movie : movies) {
            if (movie.getId() == id) {
                return &movie;
            }
        }
        return nullptr;
    }

    void updateMovie() {
        int id;
        viewAllMovies();
        
        cout << "\n  Enter Movie ID to update: ";
        cin >> id;
        cin.ignore();
        
        Movie* movie = findMovieById(id);
        if (movie == nullptr) {
            cout << RED << "\n  ❌ Movie not found!\n" << RESET;
            return;
        }
        
        string title, genre, language;
        int duration;
        double rating;
        
        cout << "\n  📝 Enter new details (press Enter to keep current):\n";
        
        cout << "  Title (" << movie->getTitle() << "): ";
        getline(cin, title);
        if (!title.empty()) movie->setTitle(title);
        
        cout << "  Genre (" << movie->getGenre() << "): ";
        getline(cin, genre);
        if (!genre.empty()) movie->setGenre(genre);
        
        cout << "  Duration (" << movie->getDuration() << " min): ";
        string durationStr;
        getline(cin, durationStr);
        if (!durationStr.empty()) {
            movie->setDuration(stoi(durationStr));
        }
        
        cout << "  Language (" << movie->getLanguage() << "): ";
        getline(cin, language);
        if (!language.empty()) movie->setLanguage(language);
        
        cout << "  Rating (" << movie->getRating() << "): ";
        string ratingStr;
        getline(cin, ratingStr);
        if (!ratingStr.empty()) {
            movie->setRating(stod(ratingStr));
        }
        
        cout << GREEN << "\n  ✅ Movie updated successfully!\n" << RESET;
        saveData();
    }

    void deleteMovie() {
        int id;
        viewAllMovies();
        
        cout << "\n  Enter Movie ID to delete: ";
        cin >> id;
        cin.ignore();
        
        auto it = find_if(movies.begin(), movies.end(),
            [id](const Movie& m) { return m.getId() == id; });
        
        if (it == movies.end()) {
            cout << RED << "\n  ❌ Movie not found!\n" << RESET;
            return;
        }
        
        cout << RED << "\n  ⚠️ Are you sure you want to delete this movie? (y/n): " << RESET;
        char confirm;
        cin >> confirm;
        cin.ignore();
        
        if (tolower(confirm) == 'y') {
            movies.erase(it);
            cout << GREEN << "\n  ✅ Movie deleted successfully!\n" << RESET;
            saveData();
        }
    }

    // ============================================================
    // SHOW MANAGEMENT FUNCTIONS
    // ============================================================
    void manageShows() {
        int choice;
        
        do {
            displayHeader("🎭 MANAGE SHOWS");
            cout << BLUE << "  [1] " << RESET << "Add Show\n";
            cout << BLUE << "  [2] " << RESET << "View All Shows\n";
            cout << BLUE << "  [3] " << RESET << "Delete Show\n";
            cout << BLUE << "  [0] " << RESET << "Back to Main Menu\n";
            cout << "  ════════════════════════════════════════════\n";
            cout << "  Enter your choice: ";
            cin >> choice;
            cin.ignore();
            
            switch (choice) {
                case 1:
                    addShow();
                    break;
                case 2:
                    viewAllShows();
                    break;
                case 3:
                    deleteShow();
                    break;
                case 0:
                    break;
                default:
                    cout << RED << "\n  ❌ Invalid choice!\n" << RESET;
            }
            
        } while (choice != 0);
    }

    void addShow() {
        if (movies.empty()) {
            cout << YELLOW << "\n  ⚠️ No movies available. Please add a movie first!\n" << RESET;
            return;
        }
        
        viewAllMovies();
        
        int movieId, hallNumber, totalSeats;
        string date, time;
        double price;
        
        cout << "\n  📝 Enter show details:\n";
        cout << "  Movie ID: ";
        cin >> movieId;
        cin.ignore();
        
        if (findMovieById(movieId) == nullptr) {
            cout << RED << "\n  ❌ Movie not found!\n" << RESET;
            return;
        }
        
        cout << "  Date (DD-MM-YYYY): ";
        getline(cin, date);
        
        cout << "  Time (HH:MM): ";
        getline(cin, time);
        
        cout << "  Hall Number: ";
        cin >> hallNumber;
        cin.ignore();
        
        cout << "  Ticket Price ($): ";
        cin >> price;
        cin.ignore();
        
        cout << "  Total Seats: ";
        cin >> totalSeats;
        cin.ignore();
        
        Show newShow(nextShowId++, movieId, date, time, hallNumber, price, totalSeats);
        shows.push_back(newShow);
        
        cout << GREEN << "\n  ✅ Show added successfully! (ID: " 
             << newShow.getId() << ")\n" << RESET;
        saveData();
    }

    void viewAllShows() const {
        displayHeader("🎭 ALL SHOWS");
        
        if (shows.empty()) {
            cout << YELLOW << "\n  No shows scheduled.\n" << RESET;
            return;
        }
        
        for (const auto& show : shows) {
            show.display();
            cout << "  ─────────────────────────\n";
        }
    }

    Show* findShowById(int id) {
        for (auto& show : shows) {
            if (show.getId() == id) {
                return &show;
            }
        }
        return nullptr;
    }

    void deleteShow() {
        int id;
        viewAllShows();
        
        cout << "\n  Enter Show ID to delete: ";
        cin >> id;
        cin.ignore();
        
        auto it = find_if(shows.begin(), shows.end(),
            [id](const Show& s) { return s.getId() == id; });
        
        if (it == shows.end()) {
            cout << RED << "\n  ❌ Show not found!\n" << RESET;
            return;
        }
        
        cout << RED << "\n  ⚠️ Are you sure you want to delete this show? (y/n): " << RESET;
        char confirm;
        cin >> confirm;
        cin.ignore();
        
        if (tolower(confirm) == 'y') {
            shows.erase(it);
            cout << GREEN << "\n  ✅ Show deleted successfully!\n" << RESET;
            saveData();
        }
    }

    // ============================================================
    // BOOKING FUNCTIONS
    // ============================================================
    void viewAvailableShows() const {
        displayHeader("🎫 AVAILABLE SHOWS");
        
        if (shows.empty()) {
            cout << YELLOW << "\n  No shows available.\n" << RESET;
            return;
        }
        
        bool hasAvailable = false;
        for (const auto& show : shows) {
            if (show.countAvailableSeats() > 0) {
                show.display();
                cout << "  ─────────────────────────\n";
                hasAvailable = true;
            }
        }
        
        if (!hasAvailable) {
            cout << YELLOW << "\n  No shows with available seats.\n" << RESET;
        }
    }

    void bookTickets() {
        if (shows.empty()) {
            cout << YELLOW << "\n  ⚠️ No shows available. Please add a show first!\n" << RESET;
            return;
        }
        
        viewAvailableShows();
        
        int showId;
        cout << "\n  Enter Show ID to book: ";
        cin >> showId;
        cin.ignore();
        
        Show* show = findShowById(showId);
        if (show == nullptr) {
            cout << RED << "\n  ❌ Show not found!\n" << RESET;
            return;
        }
        
        if (show->countAvailableSeats() == 0) {
            cout << RED << "\n  ❌ Sorry, no seats available for this show!\n" << RESET;
            return;
        }
        
        // Display seat layout
        show->displaySeats();
        
        // Get customer details
        string name, phone, email;
        int numSeats;
        vector<int> selectedSeats;
        
        cout << "\n  📝 Enter customer details:\n";
        cout << "  Name: ";
        getline(cin, name);
        
        cout << "  Phone: ";
        getline(cin, phone);
        
        cout << "  Email: ";
        getline(cin, email);
        
        cout << "\n  Number of seats to book: ";
        cin >> numSeats;
        cin.ignore();
        
        // Book each seat
        for (int i = 0; i < numSeats; i++) {
            int seatNum;
            cout << "  Select seat " << i + 1 << ": ";
            cin >> seatNum;
            cin.ignore();
            
            if (show->isSeatAvailable(seatNum)) {
                show->bookSeat(seatNum);
                selectedSeats.push_back(seatNum);
                cout << GREEN << "  ✅ Seat " << seatNum << " booked!\n" << RESET;
            } else {
                cout << RED << "  ❌ Seat " << seatNum << " is not available. Try again.\n" << RESET;
                i--;  // Retry
            }
        }
        
        // Calculate total
        double total = selectedSeats.size() * show->getPrice();
        
        // Create booking
        Booking newBooking(nextBookingId++, showId, name, phone, email, 
                          selectedSeats, total);
        bookings.push_back(newBooking);
        
        cout << GREEN << "\n  ✅ Booking successful!\n" << RESET;
        newBooking.display();
        saveData();
    }

    void cancelBooking() {
        if (bookings.empty()) {
            cout << YELLOW << "\n  ⚠️ No bookings to cancel.\n" << RESET;
            return;
        }
        
        viewAllBookings();
        
        int bookingId;
        cout << "\n  Enter Booking ID to cancel: ";
        cin >> bookingId;
        cin.ignore();
        
        auto it = find_if(bookings.begin(), bookings.end(),
            [bookingId](const Booking& b) { return b.getId() == bookingId; });
        
        if (it == bookings.end()) {
            cout << RED << "\n  ❌ Booking not found!\n" << RESET;
            return;
        }
        
        // Free the seats
        Show* show = findShowById(it->getShowId());
        if (show != nullptr) {
            for (int seat : it->getSeatNumbers()) {
                show->cancelSeat(seat);
            }
        }
        
        cout << RED << "\n  ⚠️ Are you sure you want to cancel this booking? (y/n): " << RESET;
        char confirm;
        cin >> confirm;
        cin.ignore();
        
        if (tolower(confirm) == 'y') {
            bookings.erase(it);
            cout << GREEN << "\n  ✅ Booking cancelled successfully!\n" << RESET;
            saveData();
        }
    }

    void viewAllBookings() const {
        displayHeader("📋 ALL BOOKINGS");
        
        if (bookings.empty()) {
            cout << YELLOW << "\n  No bookings found.\n" << RESET;
            return;
        }
        
        for (const auto& booking : bookings) {
            booking.display();
            cout << "  ════════════════════════════════════════════\n";
        }
    }

    // ============================================================
    // FILE HANDLING - Save and Load Data
    // ============================================================
    void saveData() {
        // Save movies
        ofstream movieFile(MOVIE_FILE);
        if (movieFile.is_open()) {
            for (const auto& movie : movies) {
                movieFile << movie.toFileString() << "\n";
            }
            movieFile.close();
        }
        
        // Save shows
        ofstream showFile(SHOW_FILE);
        if (showFile.is_open()) {
            for (const auto& show : shows) {
                showFile << show.toFileString() << "\n";
            }
            showFile.close();
        }
        
        // Save bookings
        ofstream bookingFile(BOOKING_FILE);
        if (bookingFile.is_open()) {
            for (const auto& booking : bookings) {
                bookingFile << booking.toFileString() << "\n";
            }
            bookingFile.close();
        }
    }

    void loadData() {
        // Load movies
        ifstream movieFile(MOVIE_FILE);
        if (movieFile.is_open()) {
            string line;
            while (getline(movieFile, line)) {
                if (!line.empty()) {
                    Movie m = Movie::fromFileString(line);
                    movies.push_back(m);
                    if (m.getId() >= nextMovieId) {
                        nextMovieId = m.getId() + 1;
                    }
                }
            }
            movieFile.close();
        }
        
        // Load shows
        ifstream showFile(SHOW_FILE);
        if (showFile.is_open()) {
            string line;
            while (getline(showFile, line)) {
                if (!line.empty()) {
                    Show s = Show::fromFileString(line);
                    shows.push_back(s);
                    if (s.getId() >= nextShowId) {
                        nextShowId = s.getId() + 1;
                    }
                }
            }
            showFile.close();
        }
        
        // Load bookings
        ifstream bookingFile(BOOKING_FILE);
        if (bookingFile.is_open()) {
            string line;
            while (getline(bookingFile, line)) {
                if (!line.empty()) {
                    Booking b = Booking::fromFileString(line);
                    bookings.push_back(b);
                    if (b.getId() >= nextBookingId) {
                        nextBookingId = b.getId() + 1;
                    }
                }
            }
            bookingFile.close();
        }
    }

    // ============================================================
    // UTILITY FUNCTIONS
    // ============================================================
    void displayHeader(const string& title) const {
        cout << "\n\n";
        cout << CYAN << "  ╔═══════════════════════════════════════════╗\n";
        cout << "  ║" << BOLD << "  " << title << RESET << CYAN << "  ║\n";
        cout << "  ╚═══════════════════════════════════════════╝\n" << RESET;
    }
};

// ============================================================
// MAIN FUNCTION - Program starts here
// ============================================================
int main() {
    // Create system instance and run
    CinemaSystem system;
    system.run();
    return 0;
}
