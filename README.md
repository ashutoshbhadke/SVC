# SVC
Simple version control implemented in C++

// Features:
//	* No limit on the line length and the size of the text file
//	* Edit/Delete/Insert anywhere in the file and it will be versioned.
//	* Revert to any previous version of the file.
//	* Only the diff file will be stored as a version. Saves disk space!
//
// Limitations:
//	* File should have a 'txt' extension
//
// Usage:
// svc <filename> [V | version] [rev]
//    filename - A text file to commit
//    V - Specifies the version number of the file to display
//    version - Display the latest version-no and size of the text file
//    rev - Revert the given file to the specified version
// e.g.
//  "svc t1.txt"   -- commit the file
//  "svc t1.txt 2" -- displays the 2nd version of the file
//  "svc t1.txt version -- displays the latest version-no and size of the text file
//  "svc t1.txt 1 rev" -- reverts to the 1st version of the file
//

