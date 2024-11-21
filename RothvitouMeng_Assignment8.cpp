#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <list>  // For std::list
#include <iomanip>  // For formatting timestamps

using namespace std;

// Function to draw a triangle with the given number of lines
void drawTriangle(int lines) {
    cout << "Drawing a triangle with " << lines << " lines:" << endl;
    for (int i = 1; i <= lines; ++i) {
        for (int j = 1; j <= i; ++j) {
            cout << "*";
            usleep(200000);  // Sleep to make the drawing more visible
        }
        cout << endl;
        usleep(200000);  // Sleep after each line for better visibility
    }
    cout << endl; // Adds extra space after the triangle for clarity
}

// Function to create a job file with the number of lines (triangle height)
void createJobFile(int lines) {
    time_t rawTime;
    struct tm *timeinfo;
    char fileName[15];  // Filename will be based on timestamp
    time(&rawTime);
    timeinfo = localtime(&rawTime);
    strftime(fileName, 15, "%Y%m%d%H%M%S", timeinfo);

    string fullFileName = "triangle" + string(fileName) + ".txt";  // Create job file name

    ofstream writeFile(fullFileName);
    if (!writeFile) {
        cerr << "Error creating job file!" << endl;
        exit(1);
    }
    writeFile << lines;  // Write the number of lines (triangle height) to the file
    writeFile.close();
    cout << "Created job file: " << fullFileName << endl;
}

// Function to log a timestamp, action, and triangle height to a log file
void logTimestamp(const string& action, const string& fileName, int lines) {
    ofstream logFile("jobLog.txt", ios::app);
    if (!logFile) {
        cerr << "Error opening log file!" << endl;
        return;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);
    logFile << "" << 1900 + ltm->tm_year << "/" 
            << setw(2) << setfill('0') << 1 + ltm->tm_mon << "/" 
            << setw(2) << setfill('0') << ltm->tm_mday << " " 
            << setw(2) << setfill('0') << ltm->tm_hour << ":"
            << setw(2) << setfill('0') << ltm->tm_min << ":"
            << setw(2) << setfill('0') << ltm->tm_sec << " "
            << action << ": "<< lines << " " << fileName  << endl;

    logFile.close();
}

// Program 1: Create job files for triangles
// Program 1: Create job files for triangles
void program1() {
    int choice = 0;
    int lines = 0;
    int fileCount = 0;  // To count the number of files created

    while (choice != 99) {
        cout << "CS22 ASSIGNMENT 8: CREATE TRIANGLE JOBS" << endl;
        cout << "=======================================" << endl;
        cout << "[1] Create files to draw triangles" << endl;
        cout << "[99] Quit" << endl;
        cout << "ENTER A CHOICE ABOVE: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
        case 1:
            // Check if 8 files are already created
            if (fileCount >= 8) {
                cout << "Error: You can only create up to 8 triangle job files. Maximum reached." << endl;
                break;  // Stop file creation if 8 files are already created
            }

            // Check if lines input is valid (less than 11)
            do {
                cout << "Enter the number of lines for the triangle (less than 11): ";
                cin >> lines;
                if (lines >= 11) {
                    cout << "Error: The number of lines must be less than 11." << endl;
                }
            } while (lines >= 11);  // Keep asking until the input is valid

            createJobFile(lines);  // Create the job file with the valid triangle height
            fileCount++;  // Increment the file count after creating a file
            break;

        case 99:
            cout << "Exiting program." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        usleep(1000000);  // Sleep for 1 second
    }
}


// Program 2: Process job queue and draw triangles using std::list (manual queue)
void program2() {
    string dirCmd = "ls triangle*.txt > Assignment8jobQueue.txt";  // List all triangle*.txt files
    ifstream jobFile;
    string fileName;
    list<string> jobQueue;  // Using std::list for the queue (doubly linked list)
    int jobCount = 0;  // To count the total number of processed jobs

    ofstream clearLog("jobLog.txt", ios::trunc);  // Clear the log file before starting
    clearLog.close();

    while (true) {
        system(dirCmd.c_str());  // Execute system command to find triangle job files

        jobFile.open("Assignment8jobQueue.txt");
        if (!jobFile) {
            cerr << "Failed to open job queue file." << endl;
            break;
        }

        while (getline(jobFile, fileName)) {
            cout << "Found job file: " << fileName << endl;  // Debugging: Check filenames
            jobQueue.push_back(fileName);  // Enqueue operation
        }
        jobFile.close();

        // If no job files are found, break out of the loop and end the program
        if (jobQueue.empty()) {
            cout << "No more files found. Ending program." << endl;
            break;
        }

        while (!jobQueue.empty()) {
            string currentFile = jobQueue.front();
            jobQueue.pop_front();  // Dequeue operation

            ifstream triangleFile(currentFile);
            int lines;
            triangleFile >> lines;  // Read the number of lines (triangle height)
            triangleFile.close();

            logTimestamp("Processing job file", currentFile, lines);  // Log the action

            cout << "Processing file: " << currentFile << " with " << lines << " lines" << endl;

            drawTriangle(lines);  // Draw the triangle

            string deleteCmd = "rm " + currentFile;  // Remove the file after processing
            system(deleteCmd.c_str());

            usleep(1000000);  // Sleep for 1 second between triangle drawings

            jobCount++;  // Increment the job count
        }

        cout << "Queue flushed. Waiting for new job files..." << endl;
        usleep(2000000);  // Sleep for 2 seconds after each queue flush
    }

    logTimestamp("Total processed jobs", "N/A", jobCount);  // Log the total number of processed jobs
    cout << "Total processed jobs: " << jobCount << endl;
}

int main() {
    int choice;

    while (true) {
        cout << "CS22 ASSIGNMENT 8: JOB QUEUE PROCESSING" << endl;
        cout << "========================================" << endl;
        cout << "[1] Program 1: Create Triangle Job Files" << endl;
        cout << "[2] Program 2: Process Job Queue" << endl;
        cout << "[99] Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
        case 1:
            program1();  // Run Program 1 (Job creation)
            break;
        case 2:
            program2();  // Run Program 2 (Job queue processing)
            break;
        case 99:
            cout << "Exited" << endl;
            exit(0);
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}
