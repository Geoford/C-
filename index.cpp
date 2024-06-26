#include <iostream>
#include <fstream>          // ifstream, ofstream
#include <string>           // for String - Related Business (Getline and any string variables)
#include <cstdlib>          // for stoi and stof command,
#include <iomanip>          // for Design Purposes (setw commands)
#include <conio.h>          // For _getch()
#include <regex>            // For validations
#include <stack>            // For stack
#include <sstream>			// for string conversion
#include <vector>			// for array size (array size being dynamic)
#include <stdexcept>		// for error catching (invalid_argument &e)

using namespace std;

// Variables Zone


//Receipt variables
struct Receipt {
    string customerName;
    string contactInfo;
    string movieName;
    string showtime;
    int ticketCount;		// Same as movie_count but for ticket now and also inside of a struct
    double totalPrice;
    string status;
};

//Movie Variables
struct Movie {
  string name;
  string year;
  string genre;
  string origin;
  string director;
  string cast;
  float price;
  int premiere_count; 		// Same as movie_count but for time and inside of a struct
  string showtime[10]; 		// Maximum 10 Premiere Times per Show
  int seats_available[10];	
};


//Cinema variables
struct Cinema {
    string name;
    int movie_count;		// movie counter
    vector<Movie> movies;	// Struct variable
};

vector<Cinema> cinemas;  		// Vector to store multiple cinemas (to change, follow format to cinecseat.txt:
								// Cinema [number]
								// 0
								
int current_cinema_index = 0;   // Index of the current cinema(Cinema 1)
	
// void add variable
string id1, id2; 
bool ignore = false; // Initialize ignore variable			

// main menu variables
char choice; 				
bool see = false; 			
bool use = false;

// Function prototypes
void load_movies();
void save_movies();
void book();
void view();
void add();
void update();
void status();
void erase();
void save_billing(const string & customerName,
  				  const string & contactInfo,
    			  const string & movieName,
      			  const string & showtime, int ticketCount, double totalPrice);
void bill_history();
void save_receipts(const vector<Receipt>& receipts);
void change_cinema();

//regex zone
bool validate_time_format(const string & time) {
  // Regular expression for validating the time format (h:mm XM)
  regex time_pattern("^(1[0-2]|[1-9]):[0-5][0-9] (AM|PM)$");
  return regex_match(time, time_pattern);
}

bool validate_year(const string & year) {
  // Check if year is a number and within a reasonable range
  regex year_pattern("^[0-9]{4}$");
  return regex_match(year, year_pattern);
}

bool validate_price(const string & price) {
  // Check if price is a valid float
  regex price_pattern("^\\d+(\\.\\d{1,2})?$");
  return regex_match(price, price_pattern);
}

//toupper for c++
void to_upper(string& s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return toupper(c); });
}


//clicky clicky it's time to getch

void clear(){
	if (!see) {
        cout << "\n\nPress Any Key to Continue:";
        _getch();
        system("cls");
    }
}
// Banner
void title() {
  cout << ">>=============================================================================<<" << endl;
  cout << "|| ######  #### ##    ## ########  ######  ########    ###    ########  ###### ||" << endl;
  cout << "||##    ##  ##  ###   ## ##       ##    ## ##         ## ##      ##    ##    ##||" << endl;
  cout << "||##        ##  ####  ## ##       ##       ##        ##   ##     ##    ##      ||" << endl;
  cout << "||##        ##  ## ## ## ######    ######  ######   ##     ##    ##     ###### ||" << endl;
  cout << "||##        ##  ##  #### ##             ## ##       #########    ##          ##||" << endl;
  cout << "||##    ##  ##  ##   ### ##       ##    ## ##       ##     ##    ##    ##    ##||" << endl;
  cout << "|| ######  #### ##    ## ########  ######  ######## ##     ##    ##     ###### ||" << endl;
  cout << ">>=============================================================================<<" << endl;
}

int main() {
	load_movies();
  while (true) {
		title();
    cout << "Current Cinema: " << (cinemas.empty() ? "None" : cinemas[current_cinema_index].name) << endl;
    cout << "[1] Book a Reservation" << endl;
    cout << "[2] View Movies" << endl;
    cout << "[3] Add Movie" << endl;
    cout << "[4] Update Movie" << endl;
    cout << "[5] Delete Movie" << endl;
    cout << "[6] Update status" << endl;
    cout << "[7] Bill History" << endl;
    cout << "[8] Change Cinema" << endl;
    cout << "[9] Exit \n\n" << endl;
    cout << "Enter Your Choice: ";
    choice = _getch(); // Read a character from the keyboard without waiting for Enter
    if (choice == '1') {
      book();
    } else if (choice == '2') {
      view();
    } else if (choice == '3') {
      add();
    } else if (choice == '4') {
      update();
    } else if (choice == '5') {
      erase();
    } else if (choice == '6') {
      status();
    } else if (choice == '7') {
      bill_history();
    } else if (choice == '8') {
      change_cinema();
    } else if (choice == '9') {
      exit(0);
    } else {
      system("cls");
      cout << "Invalid input\n\n" << endl;
    }
  }
}

// Function to save movies to a file 
void save_movies() {
    ofstream write("cineseat.txt", ios::trunc); // Open the file for writing, truncating existing content
    if (!write.is_open()) {
        cerr << "Error: Could not open file for writing." << endl;
        clear();
        return;
    }

    write << cinemas.size() << endl;
    for (const auto& cinema : cinemas) {
        write << cinema.name << endl;
        write << cinema.movie_count << endl;
        for (const auto& movie : cinema.movies) {
            write << movie.name << '|' << movie.year << '|' << movie.genre << '|'
                  << movie.origin << '|' << movie.director << '|' << movie.cast << '|'
                  << movie.price << '|' << movie.premiere_count << '|' << endl;
            for (int i = 0; i < movie.premiere_count; ++i) {
                write << movie.showtime[i] << '|';
            }
            write << endl;
            for (int i = 0; i < movie.premiere_count; ++i) {
                write << movie.seats_available[i] << '|';
            }
            write << endl;
        }
    }
    write.close();
}

// Function to load movies from a file
void load_movies() {
    ifstream read("cineseat.txt"); // Open the file for reading
    if (!read.is_open()) {
        cerr << "Error: Could not open file." << endl;
        clear();
        return;
    }

    int cinema_count;
    if (!(read >> cinema_count)) {
        cerr << "Error: Could not read cinema count from file." << endl;
        read.close();
        clear();
        return;
    }
    read.ignore(); // Ignore the newline character after count

    cinemas.resize(cinema_count);

    for (int i = 0; i < cinema_count; i++) {
        getline(read, cinemas[i].name);
        if (!(read >> cinemas[i].movie_count)) {
            cerr << "Error: Could not read movie count for cinema " << i + 1 << endl;
            read.close();
            clear();
            return;
        }
        read.ignore(); // Ignore the newline character after movie count

        cinemas[i].movies.resize(cinemas[i].movie_count);

        for (int j = 0; j < cinemas[i].movie_count; j++) {
            Movie& movie = cinemas[i].movies[j];
            getline(read, movie.name, '|');
            getline(read, movie.year, '|');
            getline(read, movie.genre, '|');
            getline(read, movie.origin, '|');
            getline(read, movie.director, '|');
            getline(read, movie.cast, '|');

            string temp;
            getline(read, temp, '|');
            movie.price = stof(temp);

            getline(read, temp, '|');
            movie.premiere_count = stoi(temp);

            // Read showtimes
            
            read.ignore(); // Ignore the newline character after showtimes
            
            for (int k = 0; k < movie.premiere_count; k++) {
                getline(read, movie.showtime[k], '|');
            }

            read.ignore(); // Ignore the newline character after showtimes

            // Read seats_available
            for (int k = 0; k < movie.premiere_count; k++) {
                getline(read, temp, '|');
                movie.seats_available[k] = stoi(temp);
            }

            read.ignore(); // Ignore the newline character after seats_available
        }
    }

    read.close(); // Close the file
}



// Function to book a reservation
void book() {
    load_movies(); // Load the movies from the file
    system("cls");

    if (cinemas.empty()) {
        cout << "No cinemas available for booking." << endl;
        clear();
        return;
    }

    cout << "\n  ----- Available Cinemas and Movies for Booking -----" << endl;
    for (int i = 0; i < cinemas.size(); i++) {
        cout << i + 1 << ". " << cinemas[i].name << endl;
        for (int j = 0; j < cinemas[i].movie_count; j++) {
            cout << "  " << j + 1 << ". " << cinemas[i].movies[j].name << endl;
        }
        cout << endl;
    }

    int cinema_choice, movie_choice;
    cout << "Enter the cinema number to book     : ";
    cin >> cinema_choice;

    if (cinema_choice < 1 || cinema_choice > cinemas.size()) {
        system("cls");
        cout << "Invalid cinema number.\n" << endl;
        clear();
        return;
    }

    Cinema &cinema = cinemas[cinema_choice - 1];

    cout << "\n----- Available Movies in " << cinema.name << " for Booking ----- \n " << endl;
    for (int i = 0; i < cinema.movie_count; i++) {
        cout << i + 1 << ". " << cinema.movies[i].name << endl;
    }

    cout << "\nEnter the movie number to book      : ";
    cin >> movie_choice;

    if (movie_choice < 1 || movie_choice > cinema.movie_count) {
        system("cls");
        cout << "Invalid movie number.\n" << endl;
        clear();
        return;
    }

    Movie &movie = cinema.movies[movie_choice - 1];

    cout << "Selected Movie: " << movie.name << endl;
    cout << "\n---------- Available Showtimes ----------\n" << endl;
    for (int i = 0; i < movie.premiere_count; i++) {
        cout << i + 1 << ". " << movie.showtime[i] << " (" << movie.seats_available[i] << " seats available)" << endl;
    }

    int showtime_choice;
    cout << "\nEnter the showtime number to book   : ";
    cin >> showtime_choice;

    if (showtime_choice < 1 || showtime_choice > movie.premiere_count) {
        cout << "Invalid showtime number." << endl;
        clear();
        return;
    }

    int &seats_available = movie.seats_available[showtime_choice - 1];

    if (seats_available == 0) {
        cout << "Sorry, no seats available for this showtime." << endl;
        clear();
        return;
    }

    int ticket_count;
    cout << "Enter the number of tickets to book : ";
    cin >> ticket_count;

    if (ticket_count < 1 || ticket_count > seats_available) {
        cout << "\n  Invalid number of tickets." << endl;
        clear();
        return;
    }

    // Get customer details
    string customer_name, contact_info;
    cin.ignore(); // Ignore the newline character left in the buffer
    cout << "Enter your name                     : ";
    getline(cin, customer_name);
    cout << "Enter your contact information      : ";
    getline(cin, contact_info);

    double total_price = ticket_count * movie.price;
    seats_available -= ticket_count;

    // Save the updated movie information
    save_movies();

    // Save the billing information (you need to implement this function)
    save_billing(customer_name, contact_info, movie.name, movie.showtime[showtime_choice - 1], ticket_count, total_price);

    cout << "\n-------- Booking successful! --------" << endl;
    cout << "Total Price: Php " << fixed << setprecision(2) << total_price << "\n " << endl;
}


// Function to save billing information to a file
void save_billing(const string & customerName,
    const string & contactInfo,
    const string & movieName,
    const string & showtime, int ticketCount, double totalPrice) {
    ofstream write("billing.txt", ios::app); // Open the file for appending
    if (!write.is_open()) {
        cerr << "Error: Could not open billing file for writing." << endl;
        clear();
        return;
    }

    write << customerName << "|" << contactInfo << "|" << movieName << "|"
        << showtime << "|" << ticketCount << "|" << totalPrice << "|Pending|" << endl;
    write.close(); // Close the file
}

// Function to load receipts from a file
vector<Receipt> load_receipts() {
    vector<Receipt> receipts;
    ifstream read("billing.txt"); // Open the file for reading
    if (!read.is_open()) {
        cerr << "Error: Could not open billing file." << endl;
        clear();
        return receipts;
    }

    // Read the receipts from the file
    int i = 0;
    while (true) {
    	if ( i != 0){
	    read.ignore();
		}
		i++;
        Receipt receipt;
        string temp;
        
        if (!getline(read, receipt.customerName, '|')) break;
        if (!getline(read, receipt.contactInfo, '|')) break;
        if (!getline(read, receipt.movieName, '|')) break;
        if (!getline(read, receipt.showtime, '|')) break;
        
        if (!getline(read, temp, '|')) break;
        try {
            receipt.ticketCount = stoi(temp);
        } catch (const invalid_argument &e) {
            cerr << "Error: Invalid ticket count in billing file." << endl;
            clear();
            continue; // Skip this receipt and move to the next
        }
        
        if (!getline(read, temp, '|')) break;
        try {
            receipt.totalPrice = stod(temp);
        } catch (const invalid_argument &e) {
            cerr << "Error: Invalid total price in billing file." << endl;
            clear();
            continue; // Skip this receipt and move to the next
        }
        
        if (!getline(read, receipt.status, '|')) break;

        receipts.push_back(receipt);
    }

    read.close(); // Close the file
    return receipts;
}

// Function to save receipts to a file
void save_receipts(const vector < Receipt > & receipts) {
    ofstream write("billing.txt"); // Open the file for writing
    if (!write.is_open()) {
        cerr << "Error: Could not open billing file for writing." << endl;
        clear();
        return;
    }

    // Write the receipts to the file
    for (const auto & receipt: receipts) {
        write << receipt.customerName << "|" << receipt.contactInfo << "|" << receipt.movieName << "|"
            << receipt.showtime << "|" << receipt.ticketCount << "|" << receipt.totalPrice << "|" << receipt.status << "|" << endl;
    }

    write.close(); // Close the file
}

void view() {
    load_movies();
    system("cls");
    title();
    for (const Cinema& cinema : cinemas) {
        cout << endl << cinema.name << endl;
        int i = 0;
        for (const Movie& movie : cinema.movies) {
        	cout << "ID             : " << i + 1 << endl ;
            cout << "Movie          : " << movie.name << endl;
            cout << "Year           : " << movie.year << endl;
            cout << "Genre          : " << movie.genre << endl;
            cout << "Origin         : "<< movie.origin << endl;
            cout << "Director       : " << movie.director << endl;
            cout << "Cast           : " << movie.cast << endl;
            cout << "Price          : "  << movie.price << endl;
            for (int i = 0; i < movie.premiere_count; i++) {
                cout << "Showtime " << i + 1 << ": " << movie.showtime[i] << endl;
                cout << "Seats Available: " << movie.seats_available[i] << endl;
            }
            cout << endl;
            i++;
        }
    }
    if (!see){
      clear();
    }
}


// Function to add a movie

void add() {
    
    system("cls");
    title();
    if (cinemas.empty()) {
        cout << "No cinemas available to add a movie." << endl;
        clear();
        return;
    }
    
    if (cinemas[current_cinema_index].movies.size() >= 100) {
        cout << "Cannot add more movies, cinema is full." << endl;
        clear();
        return;
    }

    Movie movie;
    cout << "Enter Movie Name     : ";
    cin.ignore();
    getline(cin, movie.name);
    cout << "Enter Year of Release: ";
    cin >> movie.year;
    if (!validate_year(movie.year)) {
        system("cls");
        cout << "Invalid year format." << endl;
        clear();
        return;
    }
    cin.ignore();
    cout << "Enter Genre          : ";
    getline(cin, movie.genre);
    cout << "Enter Origin         : ";
    getline(cin, movie.origin);
    cout << "Enter Director       : ";
    getline(cin, movie.director);
    cout << "Enter Cast           : ";
    getline(cin, movie.cast);
    cout << "Enter Price          : ";
    string price;
    getline(cin, price);
    if (!validate_price(price)) {
        system("cls");
        cout << "Invalid price format." << endl;
        clear();
        return;
    }
    movie.price = stof(price);
    cout << "Enter Number of Showtimes      : ";
    cin >> movie.premiere_count;
    if (movie.premiere_count < 1 || movie.premiere_count > 10) {
        system("cls");
        cout << "Invalid number of showtimes (must be between 1 and 10)." << endl;
        clear();
        return;
    }
    cin.ignore();
    for (int i = 0; i < movie.premiere_count; i++) {
        cout << "Enter Showtime " << i + 1 << " (HH:MM PM/AM ): ";
        getline(cin, movie.showtime[i]);
        
        // Convert showtime to uppercase
        to_upper(movie.showtime[i]);

        if (!validate_time_format(movie.showtime[i])) {
            system("cls");
            cout << "Invalid time format." << endl;
            clear();
            return;
        }

        // Check for duplicate showtimes within the same movie
        for (int j = 0; j < i; j++) {
            if (movie.showtime[i] == movie.showtime[j]) {
                cout << "This showtime already exists within the same movie. Please enter a different showtime." << endl;
                clear();
                return;
            }
        }

        // Check for duplicate showtimes with other movies
        bool duplicate_showtime = false;
        for (const auto& existing_movie : cinemas[current_cinema_index].movies) {
            for (int k = 0; k < existing_movie.premiere_count; k++) {
                if (existing_movie.showtime[k] == movie.showtime[i]) {
                    cout << "This showtime already exists for another movie. Please enter a different showtime." << endl;
                    clear();
                    duplicate_showtime = true;
                    break;
                }
            }
            if (duplicate_showtime) {
                break;
            }
        }
        if (duplicate_showtime) {
            return;
        }

        cout << "Enter Number of Available Seats for Showtime " << i + 1 << ": ";
        string seats;
        getline(cin, seats);
        if (!regex_match(seats, regex("\\d+"))) {
            system("cls");
            cout << "Invalid number of seats." << endl;
            clear();
            return;
        }
        movie.seats_available[i] = stoi(seats);
    }
    cinemas[current_cinema_index].movies.push_back(movie);
    cinemas[current_cinema_index].movie_count++;
    save_movies();
    cout << "Movie added successfully!\n" << endl;
    cout << "Press Any Key to Continue:";
    _getch();
    system("cls");
}


// Function to erase a movie
void erase() {
    system("cls");
    title();
	see = true;
    view();
	see = false;

    cout << "\n\nDeleting a Movie:\n\n";
    cout << "Enter the cinema number: ";
    int cinema_choice;
    cin >> cinema_choice;

    if (cinema_choice < 1 || cinema_choice > cinemas.size()) {
        cout << "Invalid cinema number.\n\nPress Any Key to Continue:";
        _getch();
        system("cls");
        return;
    }

    Cinema &cinema = cinemas[cinema_choice - 1];

    cout << "Enter the movie number you want to delete: ";
    int movie_choice;
    cin >> movie_choice;

    if (movie_choice < 1 || movie_choice > cinema.movie_count) {
        cout << "Invalid movie ID.\n\nPress Any Key to Continue:";
        _getch();
        system("cls");
        return;
    }

    cinema.movies.erase(cinema.movies.begin() + (movie_choice - 1));
    cinema.movie_count--;

    save_movies();

    cout << "\n\nMovie Deleted Successfully!\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
}

// Function to update a movie
void update() {
    load_movies(); // Load the movies from the file
    see = true;
    view();
	see = false;
    int cinema_choice;
    cout << "\n\n\nEnter the cinema number to update the movie in: ";
    cin >> cinema_choice;

    if (cinema_choice < 1 || cinema_choice > cinemas.size()) {
        cout << "Invalid cinema number." << endl;
        clear();
        return;
    }

    Cinema &cinema = cinemas[cinema_choice - 1];

    if (cinema.movie_count == 0) {
        cout << "No movies available to update in this cinema." << endl;
        clear();
        return;
    }

    cout << "Available Movies to Update:" << endl;
    for (int i = 0; i < cinema.movie_count; i++) {
        cout << i + 1 << ". " << cinema.movies[i].name << endl << endl;
    }

    int movie_choice;
    cout << "Enter the movie number to update: ";
    cin >> movie_choice;

    if (movie_choice < 1 || movie_choice > cinema.movie_count) {
        cout << "Invalid movie number." << endl;
        clear();
        return;
    }

    Movie &movie = cinema.movies[movie_choice - 1];
    cout << "Updating Movie: " << movie.name << endl;

    cout << "Enter new movie name (or press Enter to keep current): ";
    cin.ignore();
    string input;
    getline(cin, input);
    if (!input.empty()) movie.name = input;

    cout << "Enter new movie year (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) {
        if (!validate_year(input)) {
            cout << "Invalid year format." << endl;
            return;
        }
        movie.year = input;
    }

    cout << "Enter new movie genre (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) movie.genre = input;

    cout << "Enter new movie origin (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) movie.origin = input;

    cout << "Enter new movie director (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) movie.director = input;

    cout << "Enter new movie cast (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) movie.cast = input;

    cout << "Enter new movie price (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) {
        if (!validate_price(input)) {
            cout << "Invalid price format." << endl;
            clear();
            return;
        }
        movie.price = stof(input);
    }

    cout << "Enter new number of premiere times (or press Enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) movie.premiere_count = stoi(input);

    for (int i = 0; i < movie.premiere_count; i++) {
        cout << "Enter new showtime " << i + 1 << " (or press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) {
            if (!validate_time_format(input)) {
                cout << "Invalid time format." << endl;
                clear();
                return;
            }
            movie.showtime[i] = input;
        }
        cout << "Enter new seats available for showtime " << i + 1 << " (or press Enter to keep current): ";
        getline(cin, input);
        if (!input.empty()) movie.seats_available[i] = stoi(input);
    }

    save_movies(); // Save the movies to the file

    cout << "Movie updated successfully!" << endl;
}


// Function to update booking status
void status() {
    vector<Receipt> receipts = load_receipts();
    system("cls");

    if (receipts.empty()) {
        cout << "No booking records available to update." << endl;
        return;
    }

    cout << "Available Booking Records to Update Status:" << endl;
    vector<int> pending_indices;
    for (size_t i = 0; i < receipts.size(); i++) {
        if (receipts[i].status == "Pending") {
            cout << pending_indices.size() + 1 << ". " << receipts[i].customerName << " - " << receipts[i].movieName << " - " << receipts[i].showtime << " - " << receipts[i].status << endl;
            pending_indices.push_back(i);
        }
    }

    if (pending_indices.empty()) {
        cout << "No pending booking records available to update." << endl;
        clear();
        return;
    }

    int record_choice;
    cout << "Enter the booking record number to update status: ";
    cin >> record_choice;

    // Validate the record choice
    if (record_choice < 1 || record_choice > static_cast<int>(pending_indices.size())) {
        cout << "Invalid booking record number." << endl;
        clear();
        return;
    }

    // Adjust for 0-based index using pending_indices
    Receipt &receipt = receipts[pending_indices[record_choice - 1]];

    cout << "Updating Status for Booking Record: " << receipt.customerName << " - " << receipt.movieName << " - " << receipt.showtime << endl;
    cout << "Current Status: " << receipt.status << endl;
    cout << "Enter new status ([1] Booked | [2] Canceled): ";
    cin.ignore();
    string new_status;
    getline(cin, new_status);

    if (new_status != "1" && new_status != "2") {
        cout << "Invalid status." << endl;
        clear();
        return;
    }
	if (new_status == "1"){
		new_status = "Booked";
	} else if (new_status == "2"){
		new_status = "Booked";
	}
    receipt.status = new_status;
    save_receipts(receipts); // Save the updated receipts to the file

    cout << "Booking status updated successfully!" << endl;
}


void bill_history() {
	system("cls");
	title();
    vector<Receipt> receipts = load_receipts();
    if (receipts.empty()) {
        cout << "No billing records available." << endl;
        clear();
        return;
    }

    int choice;
    cout << "Select status to view:" << endl;
    cout << "1. Pending" << endl;
    cout << "2. Booked" << endl;
    cout << "3. Canceled" << endl;
    cout << "4. All" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    string status_filter;
    
    switch (choice) {
        case 1:
            status_filter = "Pending";
            break;
        case 2:
            status_filter = "Booked";
            break;
        case 3:
            status_filter = "Canceled";
            break;
        case 4:
            break;
        default:
            cout << "Invalid choice." << endl;
            return;
    }
	int total = 0;
    cout << "Billing History:" << endl;
    for (const auto &rec : receipts) {
        if (status_filter.empty() || rec.status == status_filter) {
            cout << "Customer Name: " << rec.customerName << endl;
            cout << "Contact Info: " << rec.contactInfo << endl;
            cout << "Movie Name: " << rec.movieName << endl;
            cout << "Showtime: " << rec.showtime << endl;
            cout << "Ticket Count: " << rec.ticketCount << endl;
            cout << "Total Price: Php " << fixed << setprecision(2) << rec.totalPrice << endl;
            cout << "Status: " << rec.status << endl;
            cout << "-----------------------------" << endl;
            total += rec.totalPrice;
        }
    }
    cout << "\n\n Total: Php " << fixed << setprecision(2) << total << endl;
    total = 0;
	clear();
}

void change_cinema() {
    system("cls");
    title();
    if (cinemas.empty()) {
        cout << "No cinemas available to switch." << endl;
        clear();
        return;
    }
    cout << "Available Cinemas:" << endl;
    for (int i = 0; i < cinemas.size(); i++) {
        cout << i + 1 << ". " << cinemas[i].name << endl;
    }
    int cinema_choice;
    cout << "Enter the cinema number to switch: ";
    cin >> cinema_choice;
    if (cinema_choice < 1 || cinema_choice > cinemas.size()) {
        system("cls");
        cout << "Invalid cinema number.\n" << endl;
        clear();
        return;
    }
    current_cinema_index = cinema_choice - 1;
    cout << "Switched to " << cinemas[current_cinema_index].name << " cinema.\n" << endl;
    clear();
}
