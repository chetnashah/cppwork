
Streams are generally associated to a physical source or destination of characters, like a disk file, the keyboard, or the console, so the characters gotten or written to/from our abstraction called stream are physically input/output to the physical device. For example, file streams are C++ objects to manipulate and interact with files; Once a file stream is used to open a file, any input or output operation performed on that stream is physically reflected in the file.

istream - input streams e.g. std::cin
ostream - output streams e.g. std::cout
fstream - input and output stream for files.
stringstream - it is both input stream and outstream. so can be used like one e.g.
``` cpp
int n,k;
string line;
line = getline(cin, line);
stringstream sin;
sin << line; // dump line string contents in sin
while(sin >> k) { // read contents from sin into k
    n++;
}
// now n is the number of integers read from line.
```


The global std::getline() function works with C++ std::string objects (more convinient)

The istream::getline() methods work with "classic" C strings (pointers to char).


### reading input for custom classes

Input streams use the extraction (>>) operator for the standard types. You can write similar extraction operators for your own types; your success depends on using white space precisely.

```cpp
istream& operator>> (istream& is, Date& dt)
{
    is>> dt.mo>> dt.da>> dt.yr;
    return is;
}
```