#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <sys/stat.h>
#include <map>
#include <limits>
#include <vector>

using namespace std;

// File type classification
enum class FileType {
    DOCUMENT, IMAGE, AUDIO, VIDEO, ARCHIVE, OTHER
};

// Mapping of file extensions to their types
map<string, FileType> fileTypeMap = {
    {".txt", FileType::DOCUMENT}, {".pdf", FileType::DOCUMENT},
    {".doc", FileType::DOCUMENT}, {".docx", FileType::DOCUMENT},
    {".jpg", FileType::IMAGE}, {".png", FileType::IMAGE},
    {".gif", FileType::IMAGE}, {".bmp", FileType::IMAGE},
    {".mp3", FileType::AUDIO}, {".wav", FileType::AUDIO},
    {".ogg", FileType::AUDIO}, {".flac", FileType::AUDIO},
    {".mp4", FileType::VIDEO}, {".avi", FileType::VIDEO},
    {".mov", FileType::VIDEO}, {".mkv", FileType::VIDEO},
    {".zip", FileType::ARCHIVE}, {".rar", FileType::ARCHIVE},
    {".7z", FileType::ARCHIVE}, {".tar", FileType::ARCHIVE}
};

// Determine file type based on extension
FileType getFileType(const string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos) {
        string ext = filename.substr(dotPos);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        auto it = fileTypeMap.find(ext);
        if (it != fileTypeMap.end()) return it->second;
    }
    return FileType::OTHER;
}

// Convert FileType enum to string for display
string fileTypeToString(FileType type) {
    switch(type) {
        case FileType::DOCUMENT: return "Document";
        case FileType::IMAGE:    return "Image";
        case FileType::AUDIO:    return "Audio";
        case FileType::VIDEO:    return "Video";
        case FileType::ARCHIVE:  return "Archive";
        default:                 return "Other";
    }
}

// Node structure for doubly linked list
struct FileNode {
    string filename;
    string content;
    size_t size;
    time_t lastModified;
    FileType type;
    FileNode* prev;
    FileNode* next;
    
    FileNode(const string& name, const string& cont = "") : 
        filename(name), content(cont), prev(nullptr), next(nullptr) {
        type = getFileType(filename);
        updateFileStats();
    }
    
    void updateFileStats() {
        size = content.size();
        lastModified = time(nullptr);
    }
};

// Doubly linked list class to manage files
struct FileList {
    FileNode* head;
    FileNode* tail;
    int size;

    FileList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~FileList() {
        clear();
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getSize() const {
        return size;
    }

    bool contains(const string& filename) const {
        FileNode* current = head;
        while (current) {
            if (current->filename == filename) return true;
            current = current->next;
        }
        return false;
    }

    void addFileAtBeginning(const string& filename, const string& content) {
        if (contains(filename)) {
            cout << "File '" << filename << "       ' already exists in the list.\n";
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void addFileAtEnd(const string& filename, const string& content) {
        if (contains(filename)) {
            cout << "File '" << filename << "       ' already exists in the list.\n";
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

void addFileAtPosition(const string& filename, int position, const string& content) {
        if (position < 0  position > size) {
            cout << "       Invalid position.\n";
            return;
        }

        if (position == 0) {
            addFileAtBeginning(filename, content);
            return;
        }

        if (position == size) {
            addFileAtEnd(filename, content);
            return;
        }

        FileNode* newNode = new FileNode(filename, content);
        
        FileNode* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }
        
        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;
        
        size++;
    }

    void removeFileFromBeginning() {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        FileNode* temp = head;
        
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        
        cout << "File '" << temp->filename << "' removed from beginning of list.\n";
        delete temp;
        size--;
    }

    void removeFileFromEnd() {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        FileNode* temp = tail;
        
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        
        cout << "File '" << temp->filename << "' removed from end of list.\n";
        delete temp;
        size--;
    }

    void removeFileFromPosition(int position) {
        if (position < 0  position >= size) {
            cout << "Invalid position.\n";
            return;
        }

        if (position == 0) {
            removeFileFromBeginning();
            return;
        }

        if (position == size - 1) {
            removeFileFromEnd();
            return;
        }

        FileNode* current = head;
        
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
        
        current->prev->next = current->next;
        current->next->prev = current->prev;
        
        cout << "File '" << current->filename << "' removed from position " << position << ".\n";
        delete current;
        size--;
    }

    void removeFile(const string& filename) {
        if (isEmpty()) {
            cout << "List is empty.\n";
            return;
        }

        if (head->filename == filename) {
            removeFileFromBeginning();
            return;
        }

        if (tail->filename == filename) {
            removeFileFromEnd();
            return;
        }

        FileNode* current = head->next;
        while (current && current->filename != filename) {
            current = current->next;
        }

        if (current) {
            current->prev->next = current->next;
            if (current->next) {
                current->next->prev = current->prev;
            }
            cout << "File '" << current->filename << "' removed.\n";
            delete current;
            size--;
        } else {
            cout << "File '" << filename << "' not found.\n";
        }
    }

    // Bubble sort implementation for sorting by name
    void sortByName() {
        if (size <= 1) return;

        bool swapped;
        do {
            swapped = false;
            FileNode* current = head;
            FileNode* nextNode = head->next;
            FileNode* last = nullptr;


while (nextNode != nullptr) {
                if (current->filename > nextNode->filename) {
                    // Swap the nodes
                    if (current->prev) {
                        current->prev->next = nextNode;
                    } else {
                        head = nextNode;
                    }
                    
                    if (nextNode->next) {
                        nextNode->next->prev = current;
                    } else {
                        tail = current;
                    }
                    
                    current->next = nextNode->next;
                    nextNode->prev = current->prev;
                    current->prev = nextNode;
                    nextNode->next = current;
                    
                    swapped = true;
                    nextNode = current->next;
                } else {
                    current = nextNode;
                    nextNode = nextNode->next;
                }
            }
            last = current;
        } while (swapped);
    }

    // Bubble sort implementation for sorting by size
    void sortBySize() {
        if (size <= 1) return;

        bool swapped;
        do {
            swapped = false;
            FileNode* current = head;
            FileNode* nextNode = head->next;
            FileNode* last = nullptr;

            while (nextNode != nullptr) {
                if (current->size > nextNode->size) {
                    // Swap the nodes
                    if (current->prev) {
                        current->prev->next = nextNode;
                    } else {
                        head = nextNode;
                    }
                    
                    if (nextNode->next) {
                        nextNode->next->prev = current;
                    } else {
                        tail = current;
                    }
                    
                    current->next = nextNode->next;
                    nextNode->prev = current->prev;
                    current->prev = nextNode;
                    nextNode->next = current;
                    
                    swapped = true;
                    nextNode = current->next;
                } else {
                    current = nextNode;
                    nextNode = nextNode->next;
                }
            }
            last = current;
        } while (swapped);
    }

    // Bubble sort implementation for sorting by modification date
    void sortByModifiedDate() {
        if (size <= 1) return;

        bool swapped;
        do {
            swapped = false;
            FileNode* current = head;
            FileNode* nextNode = head->next;
            FileNode* last = nullptr;

            while (nextNode != nullptr) {
                if (current->lastModified > nextNode->lastModified) {
                    // Swap the nodes
                    if (current->prev) {
                        current->prev->next = nextNode;
                    } else {
                        head = nextNode;
                    }
                    
                    if (nextNode->next) {
                        nextNode->next->prev = current;
                    } else {
                        tail = current;
                    }
                    
                    current->next = nextNode->next;
                    nextNode->prev = current->prev;
                    current->prev = nextNode;
                    nextNode->next = current;
                    
                    swapped = true;
                    nextNode = current->next;
                } else {
                    current = nextNode;
                    nextNode = nextNode->next;
                }
            }
            last = current;
        } while (swapped);
    }

    void printFiles() const {
        if (isEmpty()) {
            cout << "No files in the list.\n";
            return;
        }

FileNode* current = head;
        int count = 1;
        
        while (current) {
            //cout << "------------------------------------------------------\n";
            cout << count++ << ". " << current->filename <<endl;
            cout << "------------------------------------------------------\n"
                 << ":  File type: " << fileTypeToString(current->type) << ", "
                 << ":  Size: " << current->size << " bytes, "
                 << ":  Modified: " << ctime(&current->lastModified);
            current = current->next;
        }
    }

    void clear() {
        while (head) {
            FileNode* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        size = 0;
    }

    FileNode* getFileNode(const string& filename) const {
        FileNode* current = head;
        while (current) {
            if (current->filename == filename) return current;
            current = current->next;
        }
        return nullptr;
    }

    FileNode* getFileNode(int index) const {
        if (index < 0  index >= size) return nullptr;

        FileNode* current = head;
        for (int i = 0; i < index && current; i++) {
            current = current->next;
        }
        return current;
    }

    void addFile(const string& filename, const string& content = "", int position = -1) {
        if (position == -1) {
            addFileAtEnd(filename, content);
        } else if (position == 0) {
            addFileAtBeginning(filename, content);
        } else {
            addFileAtPosition(filename, position, content);
        }
       
    }
  

    void removeFile(int position = -1) {
        if (position == -1) {
            removeFileFromEnd();
        } else if (position == 0) {
            removeFileFromBeginning();
        } else {
            removeFileFromPosition(position);
        }
    }

    void searchByPrefix(const string& prefix) const {
        FileNode* current = head;
        bool found = false;
        int count = 1;
        
        while (current) {
            if (current->filename.find(prefix) == 0) {
                cout << count++ << ". " << current->filename 
                     << " (" << fileTypeToString(current->type) << ", "
                     << "Size: " << current->size << " bytes, "
                     << "Modified: " << ctime(&current->lastModified);
                found = true;
            }
            current = current->next;
        }
        
        if (!found) {
            cout << "No files found with prefix '" << prefix << "'.\n";
        }
    }

    void updateFileContent(const string& filename, const string& content) {
        FileNode* fileNode = getFileNode(filename);
        if (fileNode) {
            fileNode->content = content;
            fileNode->updateFileStats();
        }
    }

    string getFileContent(const string& filename) const {
        FileNode* fileNode = getFileNode(filename);
        return fileNode ? fileNode->content : "";
    }

    void sortFiles(int criteria) {
        switch (criteria) {
            case 1: sortByName(); break;
            case 2: sortBySize(); break;
            case 3: sortByModifiedDate(); break;
            default: cout << "Invalid sorting criteria.\n";
        }
    }

    // Enhanced Search Operations
    vector<FileNode*> searchByContent(const string& keyword) const {
        vector<FileNode*> results;
        FileNode* current = head;
        while (current) {
            if (current->content.find(keyword) != string::npos) {
                results.push_back(current);
            }
            current = current->next;
        }
        return results;
    }

    vector<FileNode*> searchByType(FileType type) const {
        vector<FileNode*> results;
        FileNode* current = head;
        while (current) {
            if (current->type == type) {
                results.push_back(current);
            }
            current = current->next;
        }
        return results;
    }

    vector<FileNode*> searchBySizeRange(size_t minSize, size_t maxSize) const {
        vector<FileNode*> results;
        FileNode* current = head;
        while (current) {
            if (current->size >= minSize && current->size <= maxSize) {
                results.push_back(current);
            }
            current = current->next;
        }
        return results;
    }
}; 

struct FileManager {
    FileList fileList;

    string readFileContent(const string& filename) {
        ifstream file(filename);
        string content, line;
        if (file.is_open()) {
            while (getline(file, line)) {
                content += line + "\n";
            }
            file.close();
        }
        return content;
    }

    void displayFileStats(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            
            cout << "File: " << filename << "\n";
            cout << "Type: " << fileTypeToString(fileNode->type) << "\n";
            cout << "Size: " << fileNode->size << " bytes\n";
            cout << "Last Modified: " << ctime(&fileNode->lastModified);
            int lineCount = 0;
            for (char c : fileNode->content) {
                if (c == '\n') lineCount++;
            }
            cout << "Lines: " << lineCount << "\n";
        } else {
            cout << "File not found in memory.\n";
        }
    }

    void createFile(const string& filename, int position = -1) {
        ofstream file(filename);
        if (file.is_open()) {
            cout << "       File '" << filename << "created successfully.\n";
            fileList.addFile(filename, "", position);
            file.close();
        } else {
            cout << "       Error creating file '" << filename << "'.\n";
        }
    }

    void readFile(const string& filename) {
        if (!fileList.contains(filename)) {
            cout << "File not found in the managed list.\n";
            return;
        }

        string content = readFileContent(filename);
        if (!content.empty()) {
            cout << "Contents of '" << filename << "':\n";
            cout << content;
            fileList.updateFileContent(filename, content);
        } else {
            cout << "       File is empty or couldn't be read.\n";
        }
    }

    void updateFile(const string& filename, const string& content) {
        if (!fileList.contains(filename)) {
            cout << "       File doesn't exist. Create it first.\n";
            return;
        }

        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << content << '\n';
            cout << "Content appended to '" << filename << "' successfully.\n";
            file.close();
            
            string currentContent = fileList.getFileContent(filename);
            fileList.updateFileContent(filename, currentContent + content + "\n");
        } else {
            cout << "       Error: Unable to open file '" << filename << "'.\n";
        }
    }

    void overwriteFile(const string& filename, const string& content) {
        if (!fileList.contains(filename)) {
            cout << "File doesn't exist. Create it first.\n";
            return;
        }

        ofstream file(filename);
        if (file.is_open()) {
            file << content;
            cout << "File '" << filename << "' overwritten successfully.\n";
            file.close();
            
            fileList.updateFileContent(filename, content);
        } else {
            cout << "Error: Unable to open file '" << filename << "'.\n";
        }
    }

    void deleteFile(int position = -1) {
        if (fileList.getSize() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        FileNode* fileNode;
        if (position == -1) {
            fileNode = fileList.getFileNode(fileList.getSize() - 1);
        } else if (position == 0) {
            fileNode = fileList.getFileNode(0);
        } else {
            fileNode = fileList.getFileNode(position);
        }


if (!fileNode) {
            cout << "Invalid file position.\n";
            return;
        }

        string filename = fileNode->filename;
        if (remove(filename.c_str()) == 0) {
            cout << "File '" << filename << "'        deleted successfully.\n";
            fileList.removeFile(position);
        } else {
            cout << "Error: Unable to delete file '" << filename << "'.\n";
        }
    }

    void deleteFileByName(const string& filename) {
        if (fileList.getSize() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        if (remove(filename.c_str()) == 0) {
            cout << "File '" << filename << "' deleted successfully.\n";
            fileList.removeFile(filename);
        } else {
            cout << "Error: Unable to delete file '" << filename << "'.\n";
        }
    }

    void deleteAllFiles() {
        if (fileList.getSize() == 0) {
            cout << "No files to delete.\n";
            return;
        }

        FileNode* current = fileList.getFileNode(0);
        while (current) {
            if (remove(current->filename.c_str()) != 0) {
                cout << "Warning: Could not delete file '" << current->filename << "' from disk.\n";
            }
            current = current->next;
        }
        
        fileList.clear();
        cout << "All files deleted.\n";
    }

    void listFiles() {
        cout << "\nManaged Files (" << fileList.getSize() << "):\n";
        fileList.printFiles();
    }

    void searchFile(const string& filename) {
        if (fileList.contains(filename)) {
            cout << "File found: " << filename << endl;
            displayFileStats(filename);
        } else {
            cout << "File not found.\n";
        }
    }

    void searchFilesByPrefix() {
        string prefix;
        cout << "Enter filename prefix to search: ";
        getline(cin, prefix);
        fileList.searchByPrefix(prefix);
    }

    void fileStatistics(const string& filename) {
        if (fileList.contains(filename)) {
            displayFileStats(filename);
        } else {
            cout << "File not found in managed list.\n";
        }
    }

    void displayFileContent(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            cout << "Content of " << filename << " from memory:\n";
            cout << fileNode->content;
        } else {
            cout << "File not found in memory.\n";
        }
    }

    void sortFiles(int criteria) {
        fileList.sortFiles(criteria);
        cout << "Files sorted successfully.\n";
    }

    // Enhanced Retrieve Operations
    void retrieveFileContent(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            cout << "Retrieved content of " << filename << ":\n";
            cout << fileNode->content;
        } else {
            cout << "File not found.\n";
        }
    }

    /*void retrieveFileMetadata(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            cout << "Metadata for " << filename << ":\n";
            cout << "Type: " << fileTypeToString(fileNode->type) << "\n";
            cout << "Size: " << fileNode->size << " bytes\n";
            cout << "Last Modified: " << ctime(&fileNode->lastModified);
        } else {
            cout << "File not found.\n";
        }
    }*/


// Enhanced Update Operations
    void updateFileName(const string& oldName, const string& newName) {
        FileNode* fileNode = fileList.getFileNode(oldName);
        if (fileNode) {
            if (fileList.contains(newName)) {
                cout << "A file with name '" << newName << "' already exists.\n";
                return;
            }
            
            if (rename(oldName.c_str(), newName.c_str()) == 0) {
                fileNode->filename = newName;
                fileNode->type = getFileType(newName);
                cout << "File renamed from '" << oldName << "' to '" << newName << "' successfully.\n";
            } else {
                cout << "Error renaming file on disk.\n";
            }
        } else {
            cout << "File not found.\n";
        }
    }

    void updateFileMetadata(const string& filename) {
        FileNode* fileNode = fileList.getFileNode(filename);
        if (fileNode) {
            fileNode->updateFileStats();
            cout << "Metadata updated for " << filename << ".\n";
        } else {
            cout << "File not found.\n";
        }
    }

    // Enhanced Search Operations
    void searchFilesByContent() {
        string keyword;
        cout << "Enter content keyword to search: ";
        getline(cin, keyword);
        
        vector<FileNode*> results = fileList.searchByContent(keyword);
        if (results.empty()) {
            cout << "No files found containing '" << keyword << "'.\n";
        } else {
            cout << "Files containing '" << keyword << "':\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << " (" 
                     << fileTypeToString(results[i]->type) << ")\n";
            }
        }
    }

    void searchFilesByType() {
        cout << "      Select file type to search:\n";
        cout << "       1. Document\n";
        cout << "       2. Image\n";
        cout << "       3. Audio\n";
        cout << "       4. Video\n";
        cout << "       5. Archive\n";
        cout << "       6. Other\n";
        
        cout << "Enter choice: ";
        
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        FileType type;
        switch (choice) {
            case 1: type = FileType::DOCUMENT; break;
            case 2: type = FileType::IMAGE; break;
            case 3: type = FileType::AUDIO; break;
            case 4: type = FileType::VIDEO; break;
            case 5: type = FileType::ARCHIVE; break;
            case 6: type = FileType::OTHER; break;
            default: 
            
                cout << "Invalid choice.\n";
                return;
        }
        
        vector<FileNode*> results = fileList.searchByType(type);
        if (results.empty()) {
            cout << "No files found of type " << fileTypeToString(type) << ".\n";
        } else {
            cout << "Files of type " << fileTypeToString(type) << ":\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << "\n";
            }
        }

    void loadfiles() {
    ifstream file("files.txt");
    if (!file) {
        cout << "Error opening file for loading cars.\n";
        return;
    }
        numCars++;
       
    }
    file.close();
}

void saveCars() {
    ofstream file("files.txt", ios::out);  // Open file for writing (overwrite existing file)
    if (!file) {
        cout << "Error opening file for saving data.\n";
        return;
    }
    }
 
        

    }


void searchFilesBySizeRange() {
        size_t minSize, maxSize;
        cout << "Enter minimum size (bytes): ";
        cin >> minSize;
        cout << "Enter maximum size (bytes): ";
        cin >> maxSize;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (minSize > maxSize) {
            cout << "Invalid range (min > max).\n";
            return;
        }
        
        vector<FileNode*> results = fileList.searchBySizeRange(minSize, maxSize);
        if (results.empty()) {
            cout << "No files found in size range " << minSize << "-" << maxSize << " bytes.\n";
        } else {
            cout << "Files in size range " << minSize << "-" << maxSize << " bytes:\n";
            for (size_t i = 0; i < results.size(); i++) {
                cout << i+1 << ". " << results[i]->filename << " (" 
                     << results[i]->size << " bytes)\n";
            }
        }
    }
};

void displayMainMenu() {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout << "******************************************************\n";
    cout << "...          FILE MANAGEMENT SYSTEM            ...||\n";
    cout << "======================================================\n";
    cout << "1. Create File                                       *\n";
    cout << "2. Delete File                                       *\n";
    cout << "3. List Files                                        *\n";
    cout << "4. Search File                                       *\n";
    cout << "5. Sort Files                                        *\n";
    cout << "6. File Operations                                   *\n";
    cout << "7. Delete All Files                                  *\n";
    cout << "8. Advanced Search                                   *\n";
    cout << "0. Exit                                              *\n";
    cout << "======================================================\n";
    cout << "Enter your choice: ";
   
}

void displayCreateMenu() {
    cout << "      Create File Options:\n";
    cout << "       1. At the beginning\n";
    cout << "       2. At a specific position\n";
    cout << "       3. At the end\n";
    cout << "       0. Back to main menu\n";
    //cout << "------------------------------------------------------\n";
    cout << "......................................................\n";
    cout << "      Enter your choice: ";
}

void displayDeleteMenu() {
    cout << "      Delete File Options:\n";
    cout << "       1. From the beginning\n";
    cout << "       2. From a specific position\n";
    cout << "       3. From the end\n";
    cout << "       4. By filename\n";
    cout << "       0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "      Enter your choice: ";
}

void displaySortMenu() {
    cout << "      Sort Files By:\n";
    cout << "       1. Name\n";
    cout << "       2. Size\n";
    cout << "       3. Date Modified\n";
    cout << "       0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "      Enter your choice: ";
}

void displayFileOperationsMenu() {
    cout << "      File Operations:\n";
    cout << "       1. Read File\n";
    cout << "       2. Append to File\n";
    cout << "       3. Overwrite File\n";
    cout << "       4. File Statistics\n";
    cout << "       5. Display File Content from Memory\n";
    cout << "       6. Search Files by Prefix\n";
    cout << "       7. Retrieve File Content\n";
    cout << "       8. Rename File\n";
    cout << "       9. Update File Metadata\n";
    cout << "       0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "      Enter your choice: ";
}

void displayAdvancedSearchMenu() {
    cout << "      Advanced Search Options:\n";
    cout << "       1. Search by Content\n";
    cout << "       2. Search by File Type\n";
    cout << "       3. Search by Size Range\n";
    cout << "       0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "      Enter your choice: ";
}

int main() {
    FileManager fileManager;
    int choice, subChoice, position;
    string filename, content, oldName, newName;

    while (true) {
        displayMainMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        

        switch (choice) {
            case 1: // Create File
                while (true) {
                    displayCreateMenu();
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (subChoice == 0) break;
                    
                    cout << "       Enter filename: ";
                    getline(cin, filename);
                    
                    switch (subChoice) {
                        case 1:
                            fileManager.createFile(filename, 0);
                            break;
                        case 2:
                            cout << "       Enter position: ";
                            cin >> position;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            fileManager.createFile(filename, position);
                            break;
                        case 3:
                            fileManager.createFile(filename);
                            break;
                        default:
                            cout << "       Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 2: // Delete File
                while (true) {
                    displayDeleteMenu();
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                            fileManager.deleteFile(0);
                            break;
                        case 2:
                            cout << "Enter position: ";
                            cin >> position;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            fileManager.deleteFile(position);
                            break;
                        case 3:
                            fileManager.deleteFile();
                            break;
                        case 4:
                            cout << "Enter filename to delete: ";
                            getline(cin, filename);
                            fileManager.deleteFileByName(filename);
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 3: // List Files
                fileManager.listFiles();
                
                break;
                
            case 4: // Search File
                cout << "Enter filename to search: ";
                getline(cin, filename);
                fileManager.searchFile(filename);
            
                break;
                
            case 5: // Sort Files
                while (true) {
                    displaySortMenu();
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (subChoice == 0) break;

fileManager.sortFiles(subChoice);
                }
                break;
                
            case 6: // File Operations
                while (true) {
                    displayFileOperationsMenu();
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                            cout << "Enter filename to read: ";
                            getline(cin, filename);
                            fileManager.readFile(filename);
                            break;
                        case 2:
                            cout << "Enter filename to append: ";
                            getline(cin, filename);
                            cout << "Enter content to append: ";
                            getline(cin, content);
                            fileManager.updateFile(filename, content);
                            break;
                        case 3:
                            cout << "Enter filename to overwrite: ";
                            getline(cin, filename);
                            cout << "Enter new content: ";
                            getline(cin, content);
                            fileManager.overwriteFile(filename, content);
                            break;
                        case 4:
                            cout << "Enter filename for statistics: ";
                            getline(cin, filename);
                            fileManager.fileStatistics(filename);
                            break;
                        case 5:
                            cout << "Enter filename to display from memory: ";
                            getline(cin, filename);
                            fileManager.displayFileContent(filename);
                            break;
                        case 6:
                            fileManager.searchFilesByPrefix();
                            break;
                        case 7:
                            cout << "Enter filename to retrieve content: ";
                            getline(cin, filename);
                            fileManager.retrieveFileContent(filename);
                            break;
                        case 8:
                            cout << "Enter current filename: ";
                            getline(cin, oldName);
                            cout << "Enter new filename: ";
                            getline(cin, newName);
                            fileManager.updateFileName(oldName, newName);
                            break;
                        case 9:
                            cout << "Enter filename to update metadata: ";
                            getline(cin, filename);
                            fileManager.updateFileMetadata(filename);
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 7: // Delete All Files
                fileManager.deleteAllFiles();
                break;
                
            case 8: // Advanced Search
                while (true) {
                    displayAdvancedSearchMenu();
                    cin >> subChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                            fileManager.searchFilesByContent();
                            break;
                        case 2:
                            fileManager.searchFilesByType();
                            break;
                        case 3:
                            fileManager.searchFilesBySizeRange();
break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 0:
                cout << "Exiting program.\n";
                return 0;
                
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}