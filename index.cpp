#include <iostream>         // el classico
#include <fstream>          // ifstream, ofstream
#include <string>           // for String - Related Business (Getline and any string variables)
#include <cstdlib>          // for stoi and stof command,
#include <iomanip>          // for Design Purposes (setw commands)
#include <conio.h>          // For _getch()
#include <regex>            // For regex
#include <stack>            // For stack

using namespace std;

// Variables Zone
struct Movie {
  string name;
  string year;
  string genre;
  string origin;
  string director;
  string cast;
  float price;
  int premiere_count; // Same as movie_count but for time and inside of a struct
  string showtime[10]; // Maximum 10 Premiere Times per Show
  int seats_available[10];
};
Movie movies[100];
int movie_count = 0; // movie counter

string id1, id2; // Add movie id

char choice; // main menu shenanigans
bool see = false; // main menu shenanigans(1)
bool use = false;

// Function prototypes
void load_movies();
void save_movies();
void book();
void view();
void add();
void update();
void erase();
void seat();
void save_billing(const string & customerName,
  const string & contactInfo,
    const string & movieName,
      const string & showtime, int ticketCount, double totalPrice);
void bill_history();

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

// Banner hihe
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
  while (true) {
    title();
    cout << "[1] Book a Reservation" << endl;
    cout << "[2] View Movies" << endl;
    cout << "[3] Add Movie" << endl;
    cout << "[4] Update Movie" << endl;
    cout << "[5] Delete Movie" << endl;
    cout << "[6] View Seatings" << endl;
    cout << "[7] Bill History" << endl;
    cout << "[8] Exit \n\n" << endl;
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
      seat();
    } else if (choice == '7') {
      bill_history();
    } else if (choice == '8') {
      exit(0);
    } else {
      system("cls");
      cout << "Invalid input\n\n" << endl;
    }
  }
}

// Function to save movies to a file 
void save_movies() {
  ofstream write("cineseats.txt"); // Open the file for writing
  if (!write.is_open()) {
    cerr << "Error: Could not open file for writing." << endl;
    return;
  }

  // Write the number of movies
  write << movie_count << endl;

  // Iterate through each movie and write its details
    for (int i = 0; i < movie_count; i++) {
    write << movies[i].name << "|" << movies[i].year << "|" << movies[i].genre << "|"
          << movies[i].origin << "|" << movies[i].director << "|" << movies[i].cast << "|"
          << movies[i].price << "|" << movies[i].premiere_count << "|" << endl;

    // Write showtimes
    for (int j = 0; j < movies[i].premiere_count; j++) {
      write << movies[i].showtime[j] << "|";
    }
    write << endl;
  }

  write.close(); // Close the file
}

// Function to load movies from a file
void load_movies() {
  ifstream read("cineseats.txt"); // Open the file for reading
  if (!read.is_open()) {
    cerr << "Error: Could not open file." << endl;
    return;
  }

  // Read the number of movies
  if (!(read >> movie_count)) {
    cerr << "Error: Could not read count from file." << endl;
    read.close();
    return;
  }
  read.ignore(); // Ignore the newline character after count

  // Iterate through each movie and read its details
  for (int i = 0; i < movie_count; i++) {
    if (!getline(read, movies[i].name, '|') ||
      !getline(read, movies[i].year, '|') ||
      !getline(read, movies[i].genre, '|') ||
      !getline(read, movies[i].origin, '|') ||
      !getline(read, movies[i].director, '|') ||
      !getline(read, movies[i].cast, '|')) {
      cerr << "Error: Could not read movie details for movie " << i + 1 << endl;
      break;
    }

    string priceStr;
    if (!getline(read, priceStr, '|')) {
      cerr << "Error: Could not read price for movie " << i + 1 << endl;
      break;
    }
    try {
      movies[i].price = stof(priceStr);
    } catch (...) {
      cerr << "Error: Invalid price format for movie " << i + 1 << endl;
      continue;
    }

    // Read premiere count and cinema
    if (!(read >> movies[i].premiere_count)) {
      cerr << "Error: Could not read premiere count for movie " << i + 1 << endl;
      break;
    }
    read.ignore(); // Ignore the newline character after premiere_count

    // Read showtimes
    for (int j = 0; j < movies[i].premiere_count; j++) {
      if (!getline(read, movies[i].showtime[j], '|')) {
        cerr << "Error: Could not read showtime " << j + 1 << " for movie " << i + 1 << endl;
        break;
      }
    }
    read.ignore(); // Ignore the newline character after showtimes
  }

  read.close(); // Close the file
}

// Function to book a reservation
void book() {
  string customerName, contactInfo;
  use = true;
  view(); //view() displays available movies
  use = false;
  // Prompt for customer information
  cout << "\nEnter Customer Name: ";
  getline(cin, customerName);
  cout << "Enter Customer Contact (email or phone number): ";
  getline(cin, contactInfo);
  int id;
  cout << "\nEnter Movie ID to book: ";
  cin >> id;
  if (id < 1 || id > movie_count) {
    cout << "Invalid Movie ID!" << endl;
    return;
  }
  Movie selected_movie = movies[id - 1];
  cout << "Premiere Times for " << selected_movie.name << ":" << endl;
  for (int i = 0; i < selected_movie.premiere_count; i++) {
    cout << i + 1 << ". " << selected_movie.showtime[i] << endl;
  }
  int timechoice;
  cout << "Enter the number of the premiere time to book: ";
  cin >> timechoice;
  if (timechoice < 1 || timechoice > selected_movie.premiere_count) {
    cout << "Invalid premiere time selection!" << endl;
    return;
  }

  int ticketCount;
  cout << "Enter the number of tickets to buy: ";
  cin >> ticketCount;

  if (ticketCount < 1) {
    cout << "Invalid number of tickets!" << endl;
    return;
  }

  double totalPrice = ticketCount * selected_movie.price;

  // Confirm booking
  cout << "\nYou have successfully booked " << ticketCount << " tickets for " << selected_movie.name <<
    " at " << selected_movie.showtime[timechoice - 1] << endl;
  cout << "Total Price: Php" << fixed << setprecision(2) << totalPrice << endl;

  // Save billing information
  save_billing(customerName, contactInfo, selected_movie.name, selected_movie.showtime[timechoice - 1], ticketCount, totalPrice);

  cout << "\n\nPress Any Key to Continue:";
  _getch();
  system("cls");
}

// Function to save billing information to a file
void save_billing(const string & customerName,
  const string & contactInfo,
    const string & movieName,
      const string & showtime, int ticketCount, double totalPrice) {
  ofstream billingsFile("billings.txt", ios_base::app); // Append mode

  if (!billingsFile.is_open()) {
    cerr << "Error: Could not open billings file for writing." << endl;
    return;
  }

  // Write customer and booking details to file
  billingsFile << "Customer Name: " << customerName << endl;
  billingsFile << "Contact Info: " << contactInfo << endl;
  billingsFile << "Movie Name: " << movieName << endl;
  billingsFile << "Showtime: " << showtime << endl;
  billingsFile << "Number of Tickets: " << ticketCount << endl;
  billingsFile << "Total Price: Php " << fixed << setprecision(2) << totalPrice << endl << endl;

  cout << "Billing information saved successfully.\n";
  billingsFile.close();
}

void view() {
    system("cls"); // Clear screen
    title(); // Print title banner
    load_movies(); // Load movies from file

    cout << "\n\nAvailable Movies:\n" << string(140, '-') << "\n\n";

    // Header row
    cout << left << setw(3) << "ID" <<
        setw(35) << "Name" <<
        setw(18) << "Year" <<
        setw(35) << "Genre" <<
        setw(25) << "Origin" <<
        setw(30) << "Director" <<
        setw(30) << "Cast" <<
        setw(18) << "Price" <<
        endl;
    cout << string(140, '-') << endl;

    // Display each movie
    for (int i = 0; i < movie_count; i++) {
        cout << left << setw(3) << i + 1 <<
            setw(35) << movies[i].name <<
            setw(18) << movies[i].year <<
            setw(35) << movies[i].genre <<
            setw(25) << movies[i].origin <<
            setw(30) << movies[i].director <<
            setw(30) << movies[i].cast <<
            "Php " << fixed << setprecision(2) << movies[i].price <<
            endl;

        // Premiere times
        cout << setw(3) << "" << "Premiere Times: ";
        for (int j = 0; j < movies[i].premiere_count; j++) {
            if (j > 0) cout << ", ";
            cout << movies[i].showtime[j];
        }
        cout << endl;
        cout << string(140, '-') << endl;
    }

    if (!use) {
        cout << "\n\nPress Any Key to Continue:";
        _getch();
        system("cls");
    }
}


void add() {
  system("cls");
  title();
  see = true;

  if (movie_count >= 100) {
    cout << "Error: Maximum number of movies reached." << endl;
    return;
  }

  Movie new_movie;
  cout << "\n\nAdding a New Movie:\n\n";
  cout << "Enter movie name: ";
  getline(cin >> ws, new_movie.name);
  if (new_movie.name == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  cout << "Enter movie year: ";
  getline(cin >> ws, new_movie.year);
  if (new_movie.year == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }
  while (!validate_year(new_movie.year)) {
    cout << "Invalid year format. Please enter a valid year: ";
    getline(cin >> ws, new_movie.year);
  }

  cout << "Enter movie genre: ";
  getline(cin >> ws, new_movie.genre);
  if (new_movie.genre == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  cout << "Enter movie language: ";
  getline(cin >> ws, new_movie.origin);
  if (new_movie.origin == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  cout << "Enter movie director: ";
  getline(cin >> ws, new_movie.director);
  if (new_movie.director == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  cout << "Enter main cast: ";
  getline(cin >> ws, new_movie.cast);
  if (new_movie.cast == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  cout << "Enter ticket price: ";
  string price_str;

  getline(cin >> ws, price_str);
  if (price_str == "cancel") {
    cout << "\nAdding new movie cancelled.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }
  while (!validate_price(price_str)) {
    cout << "Invalid price format. Please enter a valid price: ";
    getline(cin >> ws, price_str);
  }
  new_movie.price = stof(price_str);

  cout << "Enter the number of showtimes (1-10): ";
  cin >> new_movie.premiere_count;
  while (new_movie.premiere_count < 1 || new_movie.premiere_count > 10) {
    cout << "Invalid number of showtimes. Please enter a number between 1 and 10: ";
    cin >> new_movie.premiere_count;
  }

  cin.ignore(); // Ignore remaining newline character

  for (int i = 0; i < new_movie.premiere_count; i++) {
    cout << "Enter showtime " << i + 1 << " (format: h:mm XM): ";
    getline(cin >> ws, new_movie.showtime[i]);
    while (!validate_time_format(new_movie.showtime[i])) {
      cout << "Invalid time format. Please enter showtime " << i + 1 << " (format: h:mm XM): ";
      getline(cin >> ws, new_movie.showtime[i]);
    }
  }


  movies[movie_count] = new_movie;
  movie_count++;

  save_movies();

  cout << "\n\nMovie Added Successfully!\n\nPress Any Key to Continue:";
  _getch();
  system("cls");
}

void update() {
  system("cls");
  title();
  use = true;
  view();
  use = false;
  see = true;

  cout << "\n\nUpdating an Existing Movie:\n\n";
  cout << "Enter the ID of the movie you want to update: ";
  int id;
  cin >> id;
  cin.ignore(); // Ignore the newline character after the ID input

  if (id < 1 || id > movie_count) {
    cout << "Invalid movie ID.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  Movie & movie = movies[id - 1];
  cout << "Updating details for: " << movie.name << "\n\n";

  string input;

  // Movie name
  cout << "Enter new movie name (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    movie.name = input;
  }

  // Movie year
  cout << "Enter new movie year (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    while (!validate_year(input)) {
      cout << "Invalid year format. Please enter a valid year: ";
      getline(cin, input);
    }
    movie.year = input;
  }

  // Movie genre
  cout << "Enter new movie genre (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    movie.genre = input;
  }

  // Movie language
  cout << "Enter new movie language (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    movie.origin = input;
  }

  // Movie director
  cout << "Enter new movie director (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    movie.director = input;
  }

  // Main cast
  cout << "Enter new main cast (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    movie.cast = input;
  }

  // Ticket price
  cout << "Enter new ticket price (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    while (!validate_price(input)) {
      cout << "Invalid price format. Please enter a valid price: ";
      getline(cin, input);
    }
    movie.price = stof(input);
  }

  // Number of showtimes
  cout << "Enter the new number of showtimes (leave blank to keep current): ";
  getline(cin, input);
  if (!input.empty()) {
    int new_premiere_count = stoi(input);
    while (new_premiere_count < 1 || new_premiere_count > 10) {
      cout << "Invalid number of showtimes. Please enter a number between 1 and 10: ";
      getline(cin, input);
      new_premiere_count = stoi(input);
    }
    movie.premiere_count = new_premiere_count;

    for (int i = 0; i < new_premiere_count; i++) {
      cout << "Enter new showtime " << i + 1 << " (leave blank to keep current, format: h:mm XM): ";
      getline(cin, input);
      if (!input.empty()) {
        while (!validate_time_format(input)) {
          cout << "Invalid time format. Please enter showtime " << i + 1 << " (format: h:mm XM): ";
          getline(cin, input);
        }
        movie.showtime[i] = input;
      }
    }
  }

  

  save_movies();

  cout << "\n\nMovie Updated Successfully!\n\nPress Any Key to Continue:";
  _getch();
  system("cls");
}

void erase() {
  system("cls");
  title();
  use = true;
  view();
  use = false;
  see = true;

  cout << "\n\nDeleting a Movie:\n\n";
  cout << "Enter the ID of the movie you want to delete: ";
  int id;
  cin >> id;

  if (id < 1 || id > movie_count) {
    cout << "Invalid movie ID.\n\nPress Any Key to Continue:";
    _getch();
    system("cls");
    return;
  }

  for (int i = id - 1; i < movie_count - 1; i++) {
    movies[i] = movies[i + 1];
  }
  movie_count--;

  save_movies();

  cout << "\n\nMovie Deleted Successfully!\n\nPress Any Key to Continue:";
  _getch();
  system("cls");
}

void seat() {
  system("cls");
  cout << "6" << endl;
}

void bill_history() {
  system("cls");
  title();

  ifstream billingsFile("billings.txt");
  if (!billingsFile.is_open()) {
    cerr << "Error: Could not open billings file." << endl;
    return;
  }

  cout << "\n\nBill History:\n" << string(80, '=') << "\n\n";

  string line;
  while (getline(billingsFile, line)) {
    cout << line << endl;
  }

  billingsFile.close();

  cout << "\n\nPress Any Key to Continue:";
  _getch();
  system("cls");
}
