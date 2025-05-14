#include"global.h"
#include"user.h"
void getTerminalSize(int& rows, int& cols) {
      getmaxyx(stdscr, rows, cols);
}

void printBorder(int cols) {
    for (int i = 0; i < cols; i++) {
        addch('=' | A_BOLD);  // Use '=' for the border and make it bold
    }
    printw("\n");
}

void title() {
    setlocale(LC_ALL, "");  // Ensure Unicode support
    clear();
    refresh();
    noecho();
    curs_set(0);

    int rows, cols;
    getTerminalSize(rows, cols);  // Get terminal size dynamically

    // Print the top border with decorative characters
    printBorder(cols);

    // Define the title "Samvad" and subtitle "Aao Baatcheet Karein"
    std::string titleMessage = "SAMVAD";  // Bold title
    std::string subTitleMessage = "Aao Baatcheet Karein";  // Italic-style subheading

    // Enable bold attribute and text color for "SAMVAD"
    attron(A_BOLD);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Set color pair for title (cyan text on black background)
    attron(COLOR_PAIR(1));

    // Print the bold title centered with padding
    printw("|%*s%-*s|\n", static_cast<int>((cols - 4 - titleMessage.size()) / 2), "", 
           static_cast<int>(cols - 4 - (cols - 4 - titleMessage.size()) / 2 - titleMessage.size()), 
           titleMessage.c_str());

    // Disable bold and color after printing the title
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));

    // Simulate italics for the subtitle by slanting the text
    attron(A_BOLD);  // Emphasize the subtitle, simulate italics by making it bold

    // Print the italicized subheading centered with padding
    printw("|%*s%-*s|\n", static_cast<int>((cols - 4 - subTitleMessage.size()) / 2), "", 
           static_cast<int>(cols - 4 - (cols - 4 - subTitleMessage.size()) / 2 - subTitleMessage.size()), 
           subTitleMessage.c_str());

    // Disable bold after printing the message
    attroff(A_BOLD);

    // Print the bottom border with a decorative pattern
    printBorder(cols);

    refresh();  // Update the screen with the title
}



void Login() {
      setlocale(LC_ALL, "");
    clear();
    refresh();
    noecho();         // Disable automatic echoing by default
    curs_set(1);      // Show the cursor for input

    title();          // Display the title
    attron(A_BOLD);
    printw("If existing user, Press 1 to proceed to Login\n");
    printw("If new user, Press 2 to sign up\n");
    attroff(A_BOLD);
    refresh();

    int choice;
    echo();           // Enable echoing so the user can see their input
    scanw("%d", &choice);  // Capture numeric input
    noecho();         // Disable echoing again after input

    if (choice == 1) {
    attron(A_BOLD);
        printw("Enter your Name:\n");
        attroff(A_BOLD);
        refresh();
        char name[100];         // Use a character array for name input
        echo();                 // Enable echoing for string input
        getstr(name);           // Capture string input
        noecho();               // Disable echoing after input
        std::string userName(name);  // Convert to std::string

        auto it = std::find(StudentList.begin(), StudentList.end(), userName);
        if (it != StudentList.end()) {
        attron(A_BOLD);
            printw("User found!\nEnter your Phone Number: ");
            attroff(A_BOLD);
            refresh();
            int userIndex = distance(StudentList.begin(), it);
            User& currentUser = users[userIndex];
            int num;
            echo();             // Enable echoing for number input
            scanw("%d", &num);  // Capture phone number
            noecho();           // Disable echoing after input

            if (currentUser.getNum() == num) {
            attron(A_BOLD);
                printw("Loading profile.....\n");
                attroff(A_BOLD);
                refresh();
                currentUser.DisplayMenu();
            } else {
            attron(A_BOLD);
                printw("Incorrect number. Please try again.\n");
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                Login();
            }
        } else {
        attron(A_BOLD);
            printw("User not found. Kindly sign up.\n");
            printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            Login();
        }
    } else if (choice == 2) {
    attron(A_BOLD);
        printw("Enter your Name: ");
        attroff(A_BOLD);
        refresh();
        char name[100];
        echo();                 // Enable echoing for name input
        getstr(name);
        noecho();               // Disable echoing after input
        std::string userName(name);

        auto t = find(StudentList.begin(), StudentList.end(), userName);
        if (t != StudentList.end()) {
        attron(A_BOLD);
            printw("Username already taken. Kindly try a different name.\n");
            printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            Login();
        } else {
        attron(A_BOLD);
            printw("Enter your Grad Year: ");
            attroff(A_BOLD);
            refresh();
            int year;
            echo();             // Enable echoing for numeric input
            scanw("%d", &year);
            noecho();
           attron(A_BOLD);
            printw("Enter your hostel name: ");

            attroff(A_BOLD);
            printw("\n");
            refresh();
            
            char hostel[100];
            DisplayHostelName();
            echo();
            getstr(hostel);
            noecho();
            std::string userHostel(hostel);
            if(hostel_index.find(hostel)==hostel_index.end()){
            attron(A_BOLD);
            printw("Invalid hostel name.Please refer above list and try signing again\n");
             printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            Login();
            
            }
            attron(A_BOLD);
            printw("Enter your phone number: ");
            attroff(A_BOLD);
            refresh();
            int num;
            echo();
            scanw("%d", &num);
            noecho();
attron(A_BOLD);
            printw("Enter your branch: ");
            attroff(A_BOLD);
            printw("\n");
            refresh();
            char branch[100];
            DisplayBranch();
            echo();
            getstr(branch);
            if(branch_index.find(branch)==branch_index.end()){
            attron(A_BOLD);
            printw("Invalid branch name.Please refer above list and try signing again\n");
             printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            Login();
            }
            noecho();
            std::string userBranch(branch);

            User user(userName, year, userHostel, num, userBranch);  // Create user
            attron(A_BOLD);
            printw("Profile created successfully. Press any key to continue.\n");
            attroff(A_BOLD);
            getch();

            auto it = std::find(StudentList.begin(), StudentList.end(), userName);
            if (it != StudentList.end()) {
                int userIndex = distance(StudentList.begin(), it);
                User& currentUser = users[userIndex];
                currentUser.DisplayMenu();
            }
        }
    }
}


string getCurrentDateTime() {
    time_t now = std::time(nullptr);
    tm* localTime = std::localtime(&now);
    ostringstream oss;
    oss <<put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}


void menu() {
    int rows, cols;
    //getTerminalSize(rows, cols);  // Assuming this function is already working
   // printBorder(cols);            // Assuming you have a printBorder function
    attron(A_BOLD);
    printw("\n=== User Menu ===\n\n");
    attroff(A_BOLD);
    printw("1. View Profile\t\t2. Create a Post\t3. View Posts\n");
    printw("4. Comment on a Post\t5. Send a Message\t6. View Inbox\n");
    printw("7. Follow a User\t8. Suggest Users\t9. Followers Count\n");
    printw("10. Following Count\t11. Edit Profile\t12. Unfollow\n");
    printw("13. Search Profile\t14.Logout\n");
    attron(A_BOLD);
    printw("Please enter your choice: ");
    attroff(A_BOLD);
    refresh(); // Update the screen with the printed content
}
void nearestHostel() {
   branch_index["CSE"] = 0;
    branch_index["DSAI"] = 1;
    branch_index["ECE"] = 2;
    branch_index["EEE"] = 3;
    branch_index["Mech"] = 4;
    branch_index["Civil"] = 5;
    branch_index["EP"] = 6;
    branch_index["BSBE"] = 7;
    branch_index["Design"] = 8;
    branch_index["CST"] = 9;
    branch_index["Energy"] = 10;
    branch_index["Chemical"] = 11;
    branch_index["Mnc"] = 12;

    hostel_index["lohit"] = 0;
    hostel_index["kapili"] = 1;
    hostel_index["kameng"] = 2;
    hostel_index["bhrahmaputra"] = 3;
    hostel_index["gaurang"] = 4;
    hostel_index["umiam"] = 5;
    hostel_index["barak"] = 6;
    hostel_index["manas"] = 7;
    hostel_index["dihing"] = 8;
    hostel_index["disang"] = 9;
    hostel_index["dhanstri"] = 10;
    hostel_index["subhanstri"] = 11;
    hostel_index["siang"] = 12;


 distanceMatrix = {
    // CSE, DSAI, ECE, EEE, Mech, Civil, EP, BSBE, Design, CST, Energy, Chemical, Mnc
    {0, 2, 8, 12, 16, 18, 20, 22, 35, 34, 30, 45, 2},    // CSE
    {2, 0, 3, 5, 6, 8, 9, 11, 14, 4, 5, 4, 2},     // DSAI
    {3, 3, 0, 4, 6, 7, 8, 10, 13, 6, 7, 5, 8},     // ECE
    {6, 5, 4, 0, 5, 6, 7, 9, 11, 8, 9, 7, 10},     // EEE
    {7, 6, 6, 5, 0, 4, 6, 8, 10, 5, 6, 5, 7},      // Mech
    {9, 8, 7, 6, 4, 0, 5, 7, 9, 7, 8, 6, 9},      // Civil
    {10, 9, 8, 7, 6, 5, 0, 6, 8, 8, 7, 6, 8},      // EP
    {22, 21, 19, 19, 10, 7, 6, 0, 3, 10, 12, 9, 21},  // BSBE
    {15, 14, 13, 11, 10, 9, 8, 3, 0, 13, 14, 11, 13}, // Design
    {4, 4, 6, 8, 5, 7, 8, 10, 13, 0, 4, 6, 7},     // CST
    {6, 5, 7, 9, 6, 8, 7, 12, 14, 4, 0, 5, 7},     // Energy
    {10, 9, 3, 4, 5, 6, 6, 7, 11, 2, 5, 0, 8},      // Chemical
    {2, 2, 3, 5, 6, 8, 9, 11, 14, 4, 5, 4, 0}      // Mnc
};

}



vector<int> dijkstra(int start, const vector<vector<int>>& distanceMatrix) {
    int n = distanceMatrix.size();
    vector<int> distances(n, INF);  
    distances[start] = 0; 

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start});  

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currentDistance > distances[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (u != v) {  // No need to re-process self-loops
                int weight = distanceMatrix[u][v];  // weight between u and v

                if (weight != INF && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    pq.push({distances[v], v});
                }
            }
        }
    }

    return distances;
}


void DisplayHostelName() {
attron(A_BOLD);
    printw("Hostel List :\n");
    attroff(A_BOLD);
    printw("lohit      kapili     kameng\n");
    printw("bhrahmaputra gaurang   uiam\n");
    printw("barak      manas      dihing\n");
    printw("disang     dhanstri   subhanstri\n");
    printw("siang\n");
}
void DisplayBranch(){
attron(A_BOLD);
    printw("Branch List\n");
    attroff(A_BOLD);
    printw("CSE    DSAI    ECE\n");
    printw("EEE    Mech    Civil\n");
    printw("EP     BSBE    Design\n");
    printw("CST    Energy  Chemical\n");
     printw("Mnc\n");
}

