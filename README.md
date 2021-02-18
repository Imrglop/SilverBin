# Silver

A simple compressed binary file read/writer for data storage for Windows C++.

# Usage

- Import DLL `Silver.dll` and the class `SilverBinaryFile` (refer  to SilverBinFile.h in the source code)
- Use it in code, example:
```cpp
int main() {
    SilverBinFile myBin("binFile.bin"); // create/load the binary file
    bool success = true;
    int myInt = myBin.next<int>(&success); // read the int
    if (!success)
    {
        // int isn't stored
        myBin.clearPosition(); // go back
        myBin.insert(1000); // put 1000
    } else {
        printf("Number stored: %i\n", myInt);
    }
}
```

# Dependencies

Microsoft Visual C++ 2019