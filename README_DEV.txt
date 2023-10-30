In the required folder are the programs CMake and minGW

1. Install CMake
2. Place minGW contents in a folder you won't accidentally remove
3. Add the location of the bin file in minGW to the path variable
4. In CMake select the contents of the project folder
5. For build folder you can make a new folder
6. Select minGW makefile
7. Press configure and generate
8. If everything works you can compile in cmd with "mingw32-make -f MakeFile"
	Still have to look into if 64 works.
9. Run the exe but preferably use cmd with "Warehouse.exe" as input
10. Profit!?