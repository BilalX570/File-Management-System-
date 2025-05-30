#include<iostream>
#include <algorithm>
#include <map>
#include <limits>

#include<string>
using namespace std;
// File type classification
enum class FileType {
    DOCUMENT, IMAGE, AUDIO, VIDEO, ARCHIVE, OTHER
};

// Mapping of file extensions to their types
map<string, FileType> fileTypeMap = {
    {".txt", FileType::DOCUMENT}, {".pdf", FileType::DOCUMENT},
    {".jpg", FileType::IMAGE}, {".png", FileType::IMAGE},
    {".mp3", FileType::AUDIO}, {".wav", FileType::AUDIO},
    {".mp4", FileType::VIDEO}, {".mov", FileType::VIDEO} 
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

struct FileNode {
    string filename;
    string content;
    size_t size;
    time_t lastModified;
    
    FileNode* prev;
    FileNode* next;
    
    FileNode(const string& name, const string& cont = "") : 
        filename(name), content(cont), prev(nullptr), next(nullptr) {
        
        updateFileStats();
    }
    
    void updateFileStats() {
        size = content.size();
        
    }
};

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
            cout << "File '" << filename << "' already exists in the list.\n";
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
            cout << "File '" << filename << "' already exists in the list.\n";
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
        if (position < 0 || position > size) {
            cout << "Invalid position.\n";
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
        if (position < 0 || position >= size) {
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




void displayMainMenu() {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout << "******************************************************\n";
    cout << "||...          FILE MANAGEMENT SYSTEM            ...||\n";
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
    cout << "Create File Options:\n";
    cout << "1. At the beginning\n";
    cout << "2. At a specific position\n";
    cout << "3. At the end\n";
    cout << "0. Back to main menu\n";
    cout << "......................................................\n";
    cout << "Enter your choice: ";
}

void displayDeleteMenu() {
    cout << "Delete File Options:\n";
    cout << "1. From the beginning\n";
    cout << "2. From a specific position\n";
    cout << "3. From the end\n";
    cout << "4. By filename\n";
    cout << "0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "Enter your choice: ";
}

void displaySortMenu() {
    cout << "Sort Files By:\n";
    cout << "1. Name\n";
    cout << "2. Size\n";
    cout << "3. Date Modified\n";
    cout << "0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "Enter your choice: ";
}

void displayFileOperationsMenu() {
    cout << "File Operations:\n";
    cout << "1. Read File\n";
    cout << "2. Append to File\n";
    cout << "3. Overwrite File\n";
    cout << "4. File Statistics\n";
    cout << "5. Display File Content from Memory\n";
    cout << "6. Search Files by Prefix\n";
    cout << "7. Retrieve File Content\n";
    cout << "8. Rename File\n";
    cout << "9. Update File Metadata\n";
    cout << "0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "Enter your choice: ";
}

void displayAdvancedSearchMenu() {
    cout << "Advanced Search Options:\n";
    cout << "1. Search by Content\n";
    cout << "2. Search by File Type\n";
    cout << "3. Search by Size Range\n";
    cout << "0. Back to main menu\n";
    cout << "------------------------------------------------------\n";
    cout << "Enter your choice: ";
}



int main() {


    
    int choice, subChoice, position;
    string filename, content, oldName, newName;

    while (true) {
        displayMainMenu();
        cin >> choice;
        

        switch (choice) {
            case 1: // Create File
                while (true) {
                    displayCreateMenu();
                    cin >> subChoice;
                   
                    
                    if (subChoice == 0) break;
                    
                    cout << "Enter filename: ";
                    getline(cin, filename);
                    
                    switch (subChoice) {
                        case 1:
                            
                            break;
                        case 2:
                            cout << "Enter position: ";
                            cin >> position;
                            
                            
                            break;
                        case 3:
                          
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 2: // Delete File
                while (true) {
                    displayDeleteMenu();
                    cin >> subChoice;
                    
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                          
                            break;
                        case 2:
                            cout << "Enter position: ";
                            cin >> position;
                            
                         
                            break;
                        case 3:
                           
                            break;
                        case 4:
                            cout << "Enter filename to delete: ";
                            getline(cin, filename);
                           
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 3: // List Files
              
                break;
                
            case 4: // Search File
                cout << "Enter filename to search: ";
                getline(cin, filename);
                
                break;
                
            case 5: // Sort Files
                while (true) {
                    displaySortMenu();
                    cin >> subChoice;
                    
                    
                    if (subChoice == 0) break;
                    
                    
                }
                break;
                
            case 6: // File Operations
                while (true) {
                    displayFileOperationsMenu();
                    cin >> subChoice;
                    
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                            cout << "Enter filename to read: ";
                            getline(cin, filename);
                           
                            break;
                        case 2:
                            cout << "Enter filename to append: ";
                            getline(cin, filename);
                            cout << "Enter content to append: ";
                            getline(cin, content);
                            
                            break;
                        case 3:
                            cout << "Enter filename to overwrite: ";
                            getline(cin, filename);
                            cout << "Enter new content: ";
                            getline(cin, content);
                            
                            break;
                        case 4:
                            cout << "Enter filename for statistics: ";
                            getline(cin, filename);
                         
                            break;
                        case 5:
                            cout << "Enter filename to display from memory: ";
                            getline(cin, filename);
                          
                            break;
                        case 6:
                            
                            break;
                        case 7:
                            cout << "Enter filename to retrieve content: ";
                            getline(cin, filename);
                      
                            break;
                        case 8:
                            cout << "Enter current filename: ";
                            getline(cin, oldName);
                            cout << "Enter new filename: ";
                            getline(cin, newName);
                            
                            break;
                        case 9:
                            cout << "Enter filename to update metadata: ";
                            getline(cin, filename);
                            
                            break;
                        default:
                            cout << "Invalid choice.\n";
                    }
                    cout << "******************************************************\n";
                }
                break;
                
            case 7: // Delete All Files
             
                break;
                
            case 8: // Advanced Search
                while (true) {
                    displayAdvancedSearchMenu();
                    cin >> subChoice;
                    
                    
                    if (subChoice == 0) break;
                    
                    switch (subChoice) {
                        case 1:
                            
                            break;
                        case 2:
                            
                            break;
                        case 3:
                            
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
