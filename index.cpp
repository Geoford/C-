#include <iostream>			// el classico
#include <fstream>			// ifstream, ofstream
#include <string>			// for String - Related Business (Getline and any string variables)
#include <cstdlib>			// for stoi and stof command, 
#include <iomanip> 			// for Design Purposes (setw commands)
#include <conio.h>			// For _getch()
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
    int premiere_count;		// Same as movie_count but for time and inside of a struct
    string showtime[10];	// Maximum 10 Premiere Times per Show
};
Movie movies[100];
int movie_count = 0;		// movie counter

string id1, id2;			// Add movie id

char choice;				// main menu shinanigans
bool see = false;			// main menu shinanigans(1)

// Function prototypes
void load_movies();
void save_movies();
void book();
void view();
void add();
void update();
void erase();
void seat();



//regex zone

bool validate_time_format(const string& time) {
    // Regular expression for validating the time format (h:mm XM)
    regex time_pattern("^(1[0-2]|[1-9]):[0-5][0-9] (AM|PM)$");
    return regex_match(time, time_pattern);
}

bool validate_year(const string& year) {
    // Check if year is a number and within a reasonable range
    regex year_pattern("^[0-9]{4}$");
    return regex_match(year, year_pattern);
}

bool validate_price(const string& price) {
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
        if (!see) {
            title();
            see = false;
        }
        cout << "[1] Book a Reservation" << endl;
        cout << "[2] View Movies" << endl;
        cout << "[3] Add Movie" << endl;
        cout << "[4] Update Movie" << endl;
        cout << "[5] Delete Movie" << endl;
        cout << "[6] View Seatings" << endl;
        cout << "[7] Exit \n\n" << endl;
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
            exit(0);
        } else {
            system("cls");
            cout << "Invalid input\n\n" << endl;
        }
    }
}

// Function to save movies to a file 
void save_movies() {
    ofstream write("cineseats.txt"); // file name (FFS, don't run the program without checking the file and its name)
    if (!write.is_open()) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }
    write << movie_count << endl;
    for (int i = 0; i < movie_count; i++) {
        write << movies[i].name << "|" << movies[i].year << "|" << movies[i].genre << "|"
              << movies[i].origin << "|" << movies[i].director << "|" << movies[i].cast << "|"
              << movies[i].price << "|" << movies[i].premiere_count << endl;
        for (int j = 0; j < movies[i].premiere_count; j++) {
            write << movies[i].showtime[j] << "|";
        }
        write << endl;
    }
    write.close();
}

// Function to load movies from a file
void load_movies() {
    ifstream read("cineseats.txt");
    if (!read.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }
    if (!(read >> movie_count)) {
        cerr << "Error: Could not read count from file." << endl;
        read.close();
        return;
    }
    read.ignore(); // Ignore the newline character after count
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
        if (!(read >> movies[i].premiere_count)) {
            cerr << "Error: Could not read premiere count for movie " << i + 1 << endl;
            break;
        }
        read.ignore(); // Ignore the newline character after premiere_count
        for (int j = 0; j < movies[i].premiere_count; j++) {
            if (!getline(read, movies[i].showtime[j], '|')) {
                cerr << "Error: Could not read showtime " << j + 1 << " for movie " << i + 1 << endl;
                break;
            }
        }
        read.ignore(); // Ignore the newline character after showtime
    }
    read.close();
}

void book(){
    system("cls");
    cout<<"1"<<endl;
}

void view(){
    system("cls");
    title();
    load_movies(); // read
    see = true;
    cout << "\n\nAvailable Movies:" << endl << endl;
    cout << setw(3) << "ID" << setw(20) << "Name" << setw(20) << "Year" << setw(20) << "Genre"
         << setw(20) << "Language" << setw(20) << "Director" << setw(20) << "Cast"
         << setw(20) << "Price" << endl;
    for (int i = 0; i < movie_count; i++) {
        cout << setw(3) << i + 1 << ". "
             << setw(20) << movies[i].name
             << setw(20) << movies[i].year
             << setw(20) << movies[i].genre
             << setw(20) << movies[i].origin
             << setw(20) << movies[i].director
             << setw(20) << movies[i].cast
             << setw(20) << movies[i].price << endl;
        cout << "Premiere Times: \n";
        for (int j = 0; j < movies[i].premiere_count; j++) {
            cout << movies[i].showtime[j] << endl;
        }
        cout << "\n";
    }
    cout << endl << endl << endl;
}

void add() {
	system("cls");
    cout << "Enter Movie name (or type \"cancel\" to go back): ";
    getline(cin, movies[movie_count].name);
    if (movies[movie_count].name == "cancel") {
        return;
    }

    while (true) {
        cout << "Enter year (or type \"cancel\" to go back): ";
        getline(cin, movies[movie_count].year);
        if (movies[movie_count].year == "cancel") {
            return;
        }
        if (validate_year(movies[movie_count].year)) {
            break;
        } else {
            cout << "Invalid year format. Please enter a 4-digit year." << endl;
        }
    }

    cout << "Enter Movie genre (or type \"cancel\" to go back): ";
    getline(cin, movies[movie_count].genre);
    if (movies[movie_count].genre == "cancel") {
        return;
    }

    cout << "Enter Movie Language (or type \"cancel\" to go back): ";
    getline(cin, movies[movie_count].origin);
    if (movies[movie_count].origin == "cancel") {
        return;
    }

    cout << "Enter Movie Director (or type \"cancel\" to go back): ";
    getline(cin, movies[movie_count].director);
    if (movies[movie_count].director == "cancel") {
        return;
    }

    cout << "Enter Movie cast (or type \"cancel\" to go back): ";
    getline(cin, movies[movie_count].cast);
    if (movies[movie_count].cast == "cancel") {
        return;
    }

    cout << "Enter how many times movie will premiere? (or type \"cancel\" to go back): ";
    string temp_input;
    getline(cin, temp_input);
    if (temp_input == "cancel") {
        return;
    }
    movies[movie_count].premiere_count = stoi(temp_input);

    for (int i = 0; i < movies[movie_count].premiere_count; i++) {
        while (true) {
            cout << "Input time " << i + 1 << " (or type \"cancel\" to go back): ";
            getline(cin, movies[movie_count].showtime[i]);
            if (movies[movie_count].showtime[i] == "cancel") {
                return;
            }
            if (validate_time_format(movies[movie_count].showtime[i])) {
                break;
            } else {
                cout << "Invalid time format. Please enter time in the format h:mm XM (e.g., 1:00 AM, 2:00 PM)." << endl;
            }
        }
    }
    string price;
    while (true) {
        cout << "Enter Price (or type \"cancel\" to go back): ";
        getline(cin, price);
        if (price == "cancel") {
            return;
        }
        if (validate_price(price)) {
            movies[movie_count].price = stof(price);
            break;
        } else {
            cout << "Invalid price format. Please enter a valid price (e.g., 10.00)." << endl;
        }
    }
    cout << "\nSuccessfully Registered!\n" << endl;
    movie_count++;
    save_movies();  
}

void update(){
	system("cls");
    string id1;
    view();
    cout << "Enter ID to be updated (Type \"cancel\" to go back to the main menu): ";
    cin >> id1;
    if (id1 == "cancel") {
        return; // Go back to main menu
    }
    try {
        int id = stoi(id1);
        if (id < 1 || id > movie_count) {
            cout << "Invalid ID!" << endl;
            return;
        }
        int index = id - 1; // Adjust index since ID is 1-based
        for (int i = 0; i < movie_count; i++) {
            if (i + 1 == id) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            cin.ignore();
            cout << "Enter updated Movie name (or type \"cancel\" to go back): ";
            getline(cin, movies[index].name);
            if (movies[index].name == "cancel") {
                return;
            }

            while (true) {
                cout << "Enter updated year (or type \"cancel\" to go back): ";
                getline(cin, movies[index].year);
                if (movies[index].year == "cancel") {
                    return;
                }
                if (validate_year(movies[index].year)) {
                    break;
                } else {
                    cout << "Invalid year format. Please enter a 4-digit year." << endl;
                }
            }

            cout << "Enter updated Movie genre (or type \"cancel\" to go back): ";
            getline(cin, movies[index].genre);
            if (movies[index].genre == "cancel") {
                return;
            }

            cout << "Enter updated Movie Language (or type \"cancel\" to go back): ";
            getline(cin, movies[index].origin);
            if (movies[index].origin == "cancel") {
                return;
            }

            cout << "Enter updated Movie Director (or type \"cancel\" to go back): ";
            getline(cin, movies[index].director);
            if (movies[index].director == "cancel") {
                return;
            }

            cout << "Enter updated Movie cast (or type \"cancel\" to go back): ";
            getline(cin, movies[index].cast);
            if (movies[index].cast == "cancel") {
                return;
            }

            cout << "Enter how many times movie will premiere? (or type \"cancel\" to go back): ";
            string temp_input;
            getline(cin, temp_input);
            if (temp_input == "cancel") {
                return;
            }
            movies[index].premiere_count = stoi(temp_input);

            for (int i = 0; i < movies[index].premiere_count; i++) {
                while (true) {
                    cout << "Input updated time " << i + 1 << " (or type \"cancel\" to go back): ";
                    getline(cin, movies[index].showtime[i]);
                    if (movies[index].showtime[i] == "cancel") {
                        return;
                    }
                    if (validate_time_format(movies[index].showtime[i])) {
                        break;
                    } else {
                        cout << "Invalid time format. Please enter time in the format h:mm XM (e.g., 1:00 AM, 2:00 PM)." << endl;
                    }
                }
            }

            while (true) {
            	string price;
                cout << "Enter updated Price (or type \"cancel\" to go back): ";
                getline(cin, price);
                if (price == "cancel") {
                    return;
                }
                if (validate_price(price)) {
                    movies[index].price = stof(price);
                    break;
                } else {
                    cout << "Invalid price format. Please enter a valid price (e.g., 10.00)." << endl;
                }
            }

            cout << "\nUpdate Successfully!\n" << endl;
            save_movies();
        }
    } catch (...) {
        cout << "Invalid input. Please try again!\n";
    }
}
void erase() {
    view();
    string id1;
    cout << "Enter ID to be deleted (Type \"cancel\" to go back to the main menu): ";
    cin >> id1;
    if (id1 == "cancel") {
        return; // Go back to main menu
    }
    try {
        int id = stoi(id1);
        if (id < 1 || id > movie_count) {
            cout << "Invalid ID!" << endl;
            return;
        }
        int index = id - 1; // Adjust index since ID is 1-based

        for (int i = index; i < movie_count - 1; i++) {
            movies[i] = movies[i + 1]; // Shift movies down
        }
        movie_count--; // Decrease the count of movies

        cout << "Record deleted successfully!" << endl;
        save_movies(); // Save changes to file
    } catch (...) {
        cout << "Invalid input. Please try again!" << endl;
    }
}


void seat(){
    system("cls");
    cout<<"6"<<endl;
}
