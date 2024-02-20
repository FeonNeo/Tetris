This code uses non-standard C library (ncurses) merely for the non-blocking getch() function.
An alternate way is to use the kbhit() function presented in another non-standard C library (conio), but it only works if you're using Windows.
