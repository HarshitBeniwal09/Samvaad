#include"global.h"

#include"user.h"
int main() {

    initscr();            // Initialize ncurses
    nearestHostel();
    HostelList.resize(hostel_index.size()); 
    BranchList.resize(branch_index.size()); 
    User user1("Harshit", 2024, "lohit",1234,"DSAI");
    User user2("Samvid", 2023, "kapili",122466,"CSE");
    User user3("Avneesh", 2025, "kameng",6456,"Mnc");
    User user4("Raghav", 2024, "bhrahmaputra",2347,"EEE");
    User user5("anant", 2023, "dihing",15678,"BSBE");
    User user6("aryan", 2022, "umiam",987321,"Civil");
    User user7("vaishnavi", 2024, "dhanstri",81234,"ECE");
    User user8("diksha", 2023, "disang",123789,"Mech");
    User user9("srijan", 2025, "manas",23678,"CST");
    User user10("yash", 2024, "siang",12345,"Chemical");
    User user11("daivik", 2023, "barak",2367,"EP");
    title();
    //printw("hjhg");
    Login();
    clear();refresh();
    endwin(); 
    return 0;
}
