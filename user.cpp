#include"global.h"
#include"user.h"
User::User(string name, int year, string hostel, int phonenumber, string b) {
    user.name = name;
    user.year = year;
    user.hostel = hostel;
    user.branch = b;
    number = phonenumber;
    followers = 0;
    following = 0;

    StudentList.push_back(name);
    FriendGraph.push_back(vector<int>());
    users.push_back(*this);

    int BranchIdx = branch_index[user.branch];
    if (BranchIdx >= 0 && BranchIdx < BranchList.size()) {
        BranchList[BranchIdx].push_back(user);
    } else {
        attron(A_BOLD);
        printw("Invalid branch for user: %s\n", user.name.c_str());
        attroff(A_BOLD);
    }

    int hostelIdx = hostel_index[user.hostel];
    if (hostelIdx >= 0 && hostelIdx < HostelList.size()) {
        HostelList[hostelIdx].push_back(user);
    } else {
        attron(A_BOLD);
        printw("Invalid hostel index for user: %s\n", user.name.c_str());
        attroff(A_BOLD);
    }

    refresh(); // Refresh to show any updates on the screen
}

void User::print() {
    GetFollowersCount();
    GetFollowingCount();

    // Display the user information using ncurses
    printw("Name: %s\n", user.name.c_str());
    printw("Grad Year: %d\n", user.year);
    printw("Hostel: %s\n", user.hostel.c_str());
    printw("Branch: %s\n", user.branch.c_str());
    printw("Phone Number: %d\n", number);
    printw("Followers: %d\n", followers);
    printw("Following: %d\n", following);
    
    refresh(); // Refresh the screen to display the updated content
   
}

void User::PostDisplay() {
    if (user.posts.empty()) {
        printw("Nothing to display\n");
        refresh();
        return;
    }

    for (size_t i = 0; i < user.posts.size(); i++) {
        printw("Post %zu:\n", i + 1);
        printw("   Content: %s\n", user.posts[i].post.c_str());
        printw("   Date: %s\n", user.posts[i].dateTime.c_str());

        // Display comments if any
        if (user.posts[i].comment.empty()) {
            printw("   No comments.\n");
        } else {
            // Option to view comments or go back to the post list
            printw("   Do you want to view comments for this post? (y/n): ");
            char choice;
            scanw("%c", &choice); // Use scanw for single character input
            
            if (choice == 'y' || choice == 'Y') {
                // Display comments for this post
                if (user.posts[i].comment.empty()) {
                    printw("   No comments on this post.\n");
                } else {
                    printw("   Comments for Post %zu:\n", i + 1);
                    for (size_t j = 0; j < user.posts[i].comment.size(); j++) {
                        printw("     %zu. %s by %s at %s\n", j + 1, user.posts[i].comment[j].text.c_str(), user.posts[i].comment[j].name.c_str(), user.posts[i].comment[j].dateTime.c_str());
                    }
                }
            }
        }
        attron(A_BOLD);
        printw("---------------------------\n");
        attroff(A_BOLD);
    }

    refresh(); // Refresh the screen to display the updated content
    
}



// void CommentDisplay(User& user) {
//     auto it = find(StudentList.begin(), StudentList.end(), user.getName());
//     if (it != StudentList.end()) {
//         int userIndex = distance(StudentList.begin(), it);
//         User& user = users[userIndex];  

//         if (user.user.posts.empty()) {
//             cout << "No posts to display!\n";
//             return;
//         }

//         for (int i = 0; i < user.user.posts.size(); i++) {
//             cout << "Post " << i + 1 << " : " << user.user.posts[i].post << endl;
//         }
        
//         cout << "Enter the post number to view comments: ";
//         int n;
//         cin >> n;
//         n--;  // Adjusting to zero-based indexing

//         if (n >= 0 && n < user.user.posts.size()) {
//             if (user.user.posts[n].comment.empty()) {
//                 cout << "No comments to display on this post!\n";
//                 return;
//             }

//             for (int i = 0; i < user.user.posts[n].comment.size(); i++) {
//                 cout << i + 1 << "th comment: " << user.user.posts[n].comment[i].text << endl;
//             }
//         } else {
//             cout << "Invalid post number.\n";
//         }
//     } else {
//         cout << "User not found.\n";
//     }
// }
void User::addComment(string username, int postIndex,const std:: string& commentText) {
    auto it = find(StudentList.begin(), StudentList.end(), username);
    if (it != StudentList.end()) {
        int userIndex = distance(StudentList.begin(), it);
        User& u = users[userIndex];

        if (postIndex > 0 && postIndex <= u.user.posts.size()) {
            postIndex--;
            Comment newComment;
            newComment.text = commentText;
            newComment.name = user.name;
            newComment.dateTime = getCurrentDateTime();
            u.user.posts[postIndex].comment.push_back(newComment);
            attron(A_BOLD);
            printw("Comment added successfully on %s.\n", newComment.dateTime.c_str());
            attroff(A_BOLD);
        } else {
        attron(A_BOLD);
            printw("Invalid post index. Please enter a valid post number.\n");
            attroff(A_BOLD);
        }
    } else {
    attron(A_BOLD);
        printw("User not found.\n");
        attroff(A_BOLD);
    }

    refresh(); // Refresh the screen to display the updated content
    wgetch(stdscr); // Wait for user input before returning
}


void User::sendMessage(string recipientName, const std::string& message) {
    auto it = find(StudentList.begin(), StudentList.end(), recipientName);
    if (it != StudentList.end()) {
        int userIndex = distance(StudentList.begin(), it);
        User& recipient = users[userIndex];
        string messageTime = getCurrentDateTime();

        recipient.inbox.push_back(make_pair(user.name, message + " (" + messageTime + ")")); // Append the time
        printw("Message sent to %s's inbox on %s.\n", recipientName.c_str(), messageTime.c_str());
    } else {
        attron(A_BOLD);
        printw("User not found.\n");
        attroff(A_BOLD);
    }

    refresh(); // Refresh the screen to display the updated content
    wgetch(stdscr); // Wait for user input before returning
}


void User::viewInbox() {
    
    if (inbox.empty()) {
        attron(A_BOLD);
        printw("Inbox is empty.\n");
        attroff(A_BOLD);
    } else {
        for (int i = 0; i < inbox.size(); i++) {
            printw("%dth message from %s: %s\n", 
                   i + 1, 
                   inbox[i].first.c_str(), 
                   inbox[i].second.c_str());
        }
    }
    refresh(); // Refresh the screen to display the content
    getch();   // Wait for the user to acknowledge
   
}



void User:: createPost(const std::string& content) {
    Post newPost;
    newPost.post = content;
    newPost.dateTime = getCurrentDateTime();
    user.posts.push_back(newPost);
    attron(A_BOLD);
    printw("New post created successfully on %s.\n", newPost.dateTime.c_str());
    attroff(A_BOLD);
    refresh();

}

void User::followUser(string FollowersName){
        int userIndex=-1,FollowersIndex=-1;
        for(int i=0;i<StudentList.size();i++){
            if(StudentList[i]==user.name){
                userIndex=i;
            }
            if(StudentList[i]==FollowersName){
                FollowersIndex=i;
            }
        }
        if(FollowersIndex==-1){
            attron(A_BOLD);
            printw("User does not found\n");
            attroff(A_BOLD);
            refresh();
        }
        else{
            auto it = std::find(FriendGraph[userIndex].begin(), FriendGraph[userIndex].end(),FollowersIndex);
            if(it!=FriendGraph[userIndex].end()){
                attron(A_BOLD);
                printw("Already following\n");
                attroff(A_BOLD);
                
            }
            else{
            FriendGraph[userIndex].push_back(FollowersIndex);
            attron(A_BOLD);
            printw("Now successfully following\n");
            attroff(A_BOLD);
            }
        }
        refresh();
        }
    bool isAlreadyFollowing(int userIndex, const string& potentialUserName) {
    for (int followedIndex : FriendGraph[userIndex]) {
        if (StudentList[followedIndex] == potentialUserName) {
            return true;
        }
    }
    return false;
}    
void User::suggestUsers() {
    int userIndex = -1;
    for (int i = 0; i < StudentList.size(); i++) {
        if (StudentList[i] == user.name) {
            userIndex = i;
            break;
        }
    }
    if (userIndex == -1) {
        printw("Error: User not found in StudentList.\n");
        refresh();
        return;
    }

    int currentBranchIndex = branch_index[user.branch];
    vector<int> distances = dijkstra(currentBranchIndex, distanceMatrix);

    // Debug distances
   // printw("Distances from branch '%s':\n", user.branch.c_str());
    //for (int i = 0; i < distances.size(); ++i) {
   //     printw("Branch %d: Distance %d\n", i, distances[i]);
  //  }
  //  refresh();

    vector<pair<int, int>> branchDistances;
    for (int i = 0; i < distances.size(); ++i) {
        if (i != currentBranchIndex && distances[i] != INF) {
            branchDistances.push_back({distances[i], i});
        }
    }
    sort(branchDistances.begin(), branchDistances.end());

    vector<Name> suggestions;

    // Step 1: Same branch users
    for (const Name& potentialUser : BranchList[currentBranchIndex]) {
        if (potentialUser.name != user.name && !isAlreadyFollowing(userIndex, potentialUser.name)) {
            suggestions.push_back(potentialUser);
            if (suggestions.size() == 6) break;
        }
    }

    // Step 2: Nearby branch users
    for (const auto& [dist, branchIndex] : branchDistances) {
        if (suggestions.size() >= 6) break;
        for (const Name& potentialUser : BranchList[branchIndex]) {
            if (potentialUser.name != user.name && !isAlreadyFollowing(userIndex, potentialUser.name)) {
                suggestions.push_back(potentialUser);
                if (suggestions.size() == 6) break;
            }
        }
    }

    // Output the suggestions
    if (suggestions.empty()) {
        attron(A_BOLD);
        printw("No suggestions available.\n");
        attroff(A_BOLD);
    } else {
        attron(A_BOLD);
        printw("Suggested users:\n\n");
        attroff(A_BOLD);
        for (const Name& suggestedUser : suggestions) {
            printw("Name: %s, Year: %d, Hostel: %s, Branch: %s\n", 
                   suggestedUser.name.c_str(), 
                   suggestedUser.year, 
                   suggestedUser.hostel.c_str(), 
                   suggestedUser.branch.c_str());
        }
    }
    refresh();
}



void User::GetFollowersCount(){
    int userIndex=-1;
     for(int i=0;i<StudentList.size();i++){
            if(StudentList[i]==user.name){
                userIndex=i;
                break;
            }
        }
    int count = 0;
    vector<string>names;
        for(int i=0;i<FriendGraph.size();i++){
                for(int j=0;j<FriendGraph[i].size();j++) {
                    if(FriendGraph[i][j]==userIndex){
                        names.push_back(StudentList[i]);
                        count++;
                    }
                }
            }
        followers = count;
        return;
}

void User::GetFollowers(){
    int userIndex=-1;
     for(int i=0;i<StudentList.size();i++){
            if(StudentList[i]==user.name){
                userIndex=i;
                break;
            }
        }
    int count = 0;
    vector<string>names;
        for(int i=0;i<FriendGraph.size();i++){
                for(int j=0;j<FriendGraph[i].size();j++) {
                    if(FriendGraph[i][j]==userIndex){
                        names.push_back(StudentList[i]);
                        count++;
                    }
                }
            }
        followers = count;
        attron(A_BOLD);
            printw("Number of followers : ");
            
           printw("%d\n", count);
      attroff(A_BOLD);
           for (auto it : names) {
    printw("%s\n", it.c_str()); // Print each string in the list
}
printw("\n"); // Add an extra newline at the end
refresh(); // Refresh the screen to display the output

    
}
void User:: GetFollowingCount(){
     int index;
    for(int i=0;i<StudentList.size();i++){
        if(StudentList[i]==user.name){
            index=i;
            break;
        }
    }
    following=FriendGraph[index].size();
    return;
}

void User::GetFollowing(){
    attron(A_BOLD);
    printw("Number of users following is  :");
    attroff(A_BOLD);
    int index;
    for(int i=0;i<StudentList.size();i++){
        if(StudentList[i]==user.name){
            index=i;
            break;
        }
    }
    printw("%ld\n",FriendGraph[index].size());
    following=FriendGraph[index].size();
    for(auto it:FriendGraph[index]){
       printw("%s\n", StudentList[it].c_str());

    }
    printw("\n");

}
string User:: getName(){
        return user.name;
    }
int User:: getNum()
{
    return number;
} 
void User:: unfollow(string username){
    int userIndex=-1,unfollowIndex=-1;
    for(int i=0;i<StudentList.size();i++){
        if(StudentList[i]==user.name){
            userIndex=i;
        }
        if(StudentList[i]==username){
            unfollowIndex=i;
        }
    }
        if(unfollowIndex==-1){
        attron(A_BOLD);
            printw("User not found!\n");
            attroff(A_BOLD);
            return;
     }
      else{
        auto it=find(FriendGraph[userIndex].begin(),FriendGraph[userIndex].end(),unfollowIndex);
        if (it != FriendGraph[userIndex].end()) {
            FriendGraph[userIndex].erase(it);
            following--;
            attron(A_BOLD);
            printw("Successfully unfollowed %s.\n", username.c_str());
            attroff(A_BOLD);

        } 
        else {
        attron(A_BOLD);
            printw("You are not following %s.\n", username.c_str());
            attroff(A_BOLD);

        }
        return;
    }
}
void User:: EditProfile() {
    clear();
    refresh();
    noecho();
    curs_set(1);

    while (true) {
        clear();
        attron(A_BOLD);
        printw("=== Edit Profile ===\n");
        attroff(A_BOLD);
        printw("1. Edit Name\n");
        printw("2. Edit Graduation Year\n");
        printw("3. Edit Hostel\n");
        printw("4. Edit Phone Number\n");
        printw("5. Edit Branch\n");
        printw("6. Go Back\n");
        printw("Please enter your choice: ");
        refresh();

        int choice;
        echo();
        scanw("%d", &choice);  // Use scanw for numeric input
        noecho();
        switch (choice) {
            case 1: {
            attron(A_BOLD);
    printw("Enter new name: ");
    attroff(A_BOLD);
    refresh();

    char newName[100];
    echo();
    getstr(newName);  // Use getstr for string input
    noecho();

    std::string newNameStr(newName);

    // Check if the new name is already taken
    auto t = find(StudentList.begin(), StudentList.end(), newNameStr);
    if (t != StudentList.end()) {
        printw("Username already taken. Kindly try a different name.\n");
      
    } else {
        user.name = newNameStr;
        attron(A_BOLD);
        printw("Name updated successfully.\n");
        attroff(A_BOLD);
    }
    break;
}

            case 2: {
            attron(A_BOLD);
                printw("Enter new graduation year: ");
                attroff(A_BOLD);
                refresh();
                int newYear;
                  echo();
                scanw("%d", &newYear);
                  noecho();
                user.year = newYear;
                attron(A_BOLD);
                printw("Graduation year updated successfully.\n");
                attroff(A_BOLD);
                break;
            }
            case 3: {
            attron(A_BOLD);
                printw("Enter new hostel name: ");
                attroff(A_BOLD);
                refresh();
                char newHostel[100];
                  echo();
                getstr(newHostel);
                  noecho();
                std::string hostelStr(newHostel);

                if (hostel_index.find(hostelStr) != hostel_index.end()) {
                    int oldHostelIdx = hostel_index[user.hostel];
                    int newHostelIdx = hostel_index[hostelStr];

                    auto it = std::find_if(HostelList[oldHostelIdx].begin(), HostelList[oldHostelIdx].end(),
                                           [&](const Name& n) { return n.name == user.name; });
                    if (it != HostelList[oldHostelIdx].end()) {
                        HostelList[oldHostelIdx].erase(it);
                    }

                    user.hostel = hostelStr;
                    HostelList[newHostelIdx].push_back(user);
                    attron(A_BOLD);
                    printw("Hostel updated successfully.\n");
                    attroff(A_BOLD);
                } else {
                attron(A_BOLD);
                    printw("Invalid hostel name.\n");
                    attroff(A_BOLD);
                }
                break;
            }
            case 4: {
            attron(A_BOLD);
                printw("Enter new phone number: ");
                attroff(A_BOLD);
                refresh();
                long newNumber;
                  echo();
                scanw("%ld", &newNumber);
                  noecho();
               number = newNumber;  // Update the correct member
               attron(A_BOLD);
                printw("Phone number updated successfully.\n");
                attroff(A_BOLD);
                break;
            }
            case 5: {
            attron(A_BOLD);
                printw("Enter new branch: ");
                attroff(A_BOLD);
                refresh();
                char branch[100];
                  echo();
                getstr(branch);
                  noecho();
                user.branch = std::string(branch);
                attron(A_BOLD);
                printw("Branch updated successfully.\n");
                attroff(A_BOLD);
                break;
            }
            case 6:
                return;  // Exit the function to go back
            default:
            attron(A_BOLD);
                printw("Invalid choice. Please try again.\n");
                attroff(A_BOLD);
                break;
        }
        attron(A_BOLD);
        printw("Press any key to continue...\n");
        attroff(A_BOLD);
        refresh();
        getch();  // Wait for user input to continue
    }
}


  
void User::DisplayMenu() {
    clear();
    refresh();
    noecho();
    curs_set(1);
    //title();
    //menu();

    while (true) {
    clear();
    title();
    menu();
        int choice;
        char input[10];
        echo();
        getstr(input); // ncurses way to read input
        noecho();

        try {
            choice = std::stoi(input); // Convert input to integer
        } catch (...) {
        attron(A_BOLD);
            printw("Invalid choice. Please try again.\n");
            attroff(A_BOLD);
            continue;
        }

        switch (choice) {
            case 1: {
                clear();
                title();
                print();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 2: {
                clear();
                title();
                attron(A_BOLD);
                printw("Enter the content for the post: ");
                attroff(A_BOLD);
                char content[256];
                echo();
                getstr(content); // Read post content
                noecho();
                createPost(content);
                attron(A_BOLD);
                printw("Post created successfully!\n");
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 3: {
                clear();
                title();
                PostDisplay();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
case 4: {
    clear();
    title();
    attron(A_BOLD);
    printw("Enter name of the person whose post you want to comment on: ");
    attroff(A_BOLD);
    char username[256];
    echo();
    getstr(username);  // Use getstr for input
    noecho();

    auto it = find(StudentList.begin(), StudentList.end(), std::string(username));
    if (it != StudentList.end()) {
        int userIndex = std::distance(StudentList.begin(), it);
        User& user = users[userIndex];

        if (user.user.posts.empty()) {  // Check if the user has any posts
        attron(A_BOLD);
            printw("No posts to display!\n");
            printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            break;
        }

        // Display all posts by the user
        for (size_t i = 0; i < user.user.posts.size(); ++i) {
            printw("Post %zu: %s\n", i + 1, user.user.posts[i].post.c_str());
        }
        attron(A_BOLD);
        printw("Enter the post number to comment on: ");
        attroff(A_BOLD);
        int postIndex;
        echo();
        scanw("%d", &postIndex);  // Use scanw for numeric input
        noecho();
        if (postIndex < 1 || postIndex > static_cast<int>(user.user.posts.size())) {
        attron(A_BOLD);
            printw("Invalid post number.\n");
            printw("Press any key to return back.\n");
            attroff(A_BOLD);
            getch();
            break;
        }

        // Input comment
        attron(A_BOLD);
        printw("Enter your comment: ");
        attroff(A_BOLD);
        char comment[256];
        echo();
        getstr(comment);
        noecho();

        // Add comment to the post
        addComment(username, postIndex, comment);  // Ensure addComment works as intended
        attron(A_BOLD);
        printw("Press any key to return back.\n");
        attroff(A_BOLD);
        getch();
    } else {
    attron(A_BOLD);
        printw("User not found.\n");
        printw("Press any key to return back.\n");
        attroff(A_BOLD);
        getch();
    }
    break;
}

            case 5: {
                clear();
                title();
                attron(A_BOLD);
                printw("Enter the username of the recipient: ");
                attroff(A_BOLD);
                char recipientName[256];
                echo();
                getstr(recipientName);
                noecho();
                
                attron(A_BOLD);
                printw("Enter your message: ");
                attroff(A_BOLD);
                char message[256];
                echo();
                getstr(message);
                noecho();

                sendMessage(recipientName, message);
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 6: {
                clear();
                title();
                viewInbox();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 7: {
                clear();
                title();
                attron(A_BOLD);
                printw("Enter the name of the user to follow: ");
                attroff(A_BOLD);
                char followerName[256];
                echo();
                getstr(followerName);
                noecho();

                followUser(followerName);
                attron(A_BOLD);
                printw("Followed successfully!\n");
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 8: {
                clear();
                title();
                suggestUsers();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 9: {
                clear();
                title();
                GetFollowers();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 10: {
                clear();
                title();
                GetFollowing();
                attron(A_BOLD);
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 11: {
                clear();
                title();
                EditProfile();
                attron(A_BOLD);
                printw("Profile edited successfully!\n");
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 12: {
                clear();
                title();
                attron(A_BOLD);
                printw("Enter username of user you want to unfollow: ");
                attroff(A_BOLD);
                char unfollowName[256];
                echo();
                getstr(unfollowName);
                noecho();

                unfollow(unfollowName);
                attron(A_BOLD);
                printw("Unfollowed successfully!\n");
                printw("Press any key to return back.\n");
                attroff(A_BOLD);
                getch();
                break;
            }
            case 13: {
                clear();
                title();
                attron(A_BOLD);
    printw("Enter the name of the user to search: ");
    attroff(A_BOLD);
    char searchName[256];
    echo();
    getstr(searchName);
    noecho();

    std::string searchNameStr(searchName);

    // Search for the user in the list
    auto it = std::find(StudentList.begin(), StudentList.end(), searchNameStr);
    if (it != StudentList.end()) {
        int userIndex = std::distance(StudentList.begin(), it);
        User& foundUser = users[userIndex];

        // Display profile details
        clear();
        attron(A_BOLD);  // Enable bold text
printw("=== Profile Found ===\n");
attroff(A_BOLD); // Disable bold text
        printw("Name: %s\n", foundUser.user.name.c_str());
        printw("Graduation Year: %d\n", foundUser.user.year);
        printw("Hostel: %s\n", foundUser.user.hostel.c_str());
        printw("Phone Number: %d\n", foundUser.number);  // Assuming `phoneNumber` is of type long
        printw("Branch: %s\n", foundUser.user.branch.c_str());
        printw("Number of Posts: %zu\n", foundUser.user.posts.size());
        
        // Optional: Display the first few posts (or all, if desired)
        if (!foundUser.user.posts.empty()) {
            printw("Recent Posts:\n");
            for (size_t i = 0; i < foundUser.user.posts.size(); ++i) {
                printw("Post %zu: %s\n", i + 1, foundUser.user.posts[i].post.c_str());
                if (i == 4) {  // Limit to first 5 posts
                    printw("...\n");
                    break;
                }
            }
        } else {
        attron(A_BOLD);
            printw("No posts available.\n");
            attroff(A_BOLD);
        }
    } else {
    attron(A_BOLD);
        printw("User not found.\n");
        attroff(A_BOLD);
    }
 
    attron(A_BOLD);
    printw("Press any key to return back.\n");
    attroff(A_BOLD);
    getch();
    break;
            }
            case 14:{
            title();
            attron(A_BOLD);
            printw("Logging out...\n");
            attroff(A_BOLD);
                Login();
                return;
                }
            default: {
            attron(A_BOLD);
                printw("Invalid choice. Please try again.\n");
                attroff(A_BOLD);
            }
        }
    }
}

