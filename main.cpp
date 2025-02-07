#include <bits/stdc++.h>
using namespace std;

map <char, int> hex_value_of_dec (void)
{
    // Map the values to decimal values
    map <char, int> m{ { '0', 0 }, { '1', 1 },
                      { '2', 2 }, { '3', 3 },
                      { '4', 4 }, { '5', 5 },
                      { '6', 6 }, { '7', 7 },
                      { '8', 8 }, { '9', 9 },
                      { 'A', 10 }, { 'B', 11 },
                      { 'C', 12 }, { 'D', 13 },
                      { 'E', 14 }, { 'F', 15 } };

    return m;
}

// Map for converting decimal values
// to hexadecimal
map<int, char> dec_value_of_hex(void)
{
    // Map the values to the
    // hexadecimal values
    map<int, char> m{ { 0, '0' }, { 1, '1' },
                      { 2, '2' }, { 3, '3' },
                      { 4, '4' }, { 5, '5' },
                      { 6, '6' }, { 7, '7' },
                      { 8, '8' }, { 9, '9' },
                      { 10, 'A' }, { 11, 'B' },
                      { 12, 'C' }, { 13, 'D' },
                      { 14, 'E' }, { 15, 'F' } };

    return m;
}

// Function to add the two hexadecimal numbers
string Add_Hex(string a, string b)
{
    map<char, int> m = hex_value_of_dec();
    map<int, char> k = dec_value_of_hex();

    // Check if length of string first is
    // greater than or equal to string second
    if (a.length() < b.length())
        swap(a, b);

    // Store length of both strings
    int l1 = a.length(), l2 = b.length();

    string ans = "";

    // Initialize carry as zero
    int carry = 0, i, j;

    // Traverse till second string
    // get traversal completely
    for (i = l1 - 1, j = l2 - 1;
         j >= 0; i--, j--) {

        // Decimal value of element at a[i]
        // Decimal value of element at b[i]
        int sum = m[a[i]] + m[b[j]] + carry;

        // Hexadecimal value of sum%16
        // to get addition bit
        int addition_bit = k[sum % 16];

        // Add addition_bit to answer
        ans.push_back(addition_bit);

        // Update carry
        carry = sum / 16;
    }

    // Traverse remaining element
    // of string a
    while (i >= 0) {

        // Decimal value of element
        // at a[i]
        int sum = m[a[i]] + carry;

        // Hexadecimal value of sum%16
        // to get addition bit
        int addition_bit = k[sum % 16];

        // Add addition_bit to answer
        ans.push_back(addition_bit);

        // Update carry
        carry = sum / 16;
        i--;
    }

    // Check if still carry remains
    if (carry) {
        ans.push_back(k[carry]);
    }

    // Reverse the final string
    // for desired output
    reverse(ans.begin(), ans.end());

    // Return the answer
    return ans;
}


vector <string> addresses;
string hexaDigits = "0123456789ABCDEF";
void prepareAdress (){
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j) {
            string address = string(1, hexaDigits[i]) + string (1,hexaDigits[j]);
            addresses.push_back(address);
        }
}


class Memory{
    map<string ,string> cells;
public:

    Memory() {
        for (int i = 0; i < addresses.size(); ++i)
            cells[addresses[i]] = "00";
    }

    string getCell (string address){
        return cells[address];
    }

    void setCell (string address, string value){
        cells [address] = value;
    }

    void loadFile (string fileName){

        fstream instructions; string line; int i=0;
        instructions.open(fileName+".txt");

        while (getline(instructions, line)){
            cells[addresses[i++]] = string (1,line[2]) + string(1,line[6]);
            cells[addresses[i++]] = string (1,line[10]) + string (1,line[11]);
        }
    }

    void printCells(){
        for (auto cell : cells)
            cout << cell.first << ": " << cell.second << '\n';
    }
};


class Ram{
    map <string, string> registers;
public:
    Ram(){
        for (int i=0; i < 16; ++i)
            registers["R" + string(1,hexaDigits[i])] = "00";
    }

    void setRegister(char registerNumber, string value){
        registers["R" + string(1,registerNumber)] = value;
    }

    string getRegister(char registerNumber){
        return registers["R" + string(1,registerNumber)];
    }

    void printRegisters(){
        for (auto registr : registers)
            cout << registr.first << ": " << registr.second << '\n';
    }
};



class Machine {
public:
    Memory main_memory;
    Ram buff_memory;
    string instructionRegister;
    string programCounter = "00";
    string screen;
    bool running;

    void setCounter(string counter){
        programCounter = counter;
    }

    void incrementCounter(){
        for (int i = 0; i < addresses.size()-1; ++i)
            if (addresses[i] == programCounter ){
                programCounter = addresses[i+1]; break;
            }
    }

    void setScreen(string value){
        screen = value;
    }

    void fetch(){
        instructionRegister += main_memory.getCell(programCounter);
        incrementCounter();
    }

    void execute (string instruction){

        if (instruction[0] == '1'){
            char registerNumber = instruction[1];
            string address = string(1,instruction[2]) + string(1,instruction [3]);
            buff_memory.setRegister(registerNumber, main_memory.getCell(address));
        }

        else if (instruction[0] == '2'){
            char registerNumber = instruction[1];
            string value = string(1,instruction[2]) + string(1, instruction [3]);
            buff_memory.setRegister(registerNumber, value);
        }

        else if (instruction[0] == '3'){
            string address = instruction.substr(2,2);
            if(address == "00")
                setScreen(buff_memory.getRegister(instruction[1]));
            else main_memory.setCell(address, buff_memory.getRegister(instruction[1]));
        }

        else if (instruction[0] == '4'){
            if (instruction[1] == '0')
                buff_memory.setRegister(instruction[3], buff_memory.getRegister(instruction[2]));
        }

        else if (instruction[0] == '5'){
            string operand1 = buff_memory.getRegister(instruction[2]);
            string operand2 = buff_memory.getRegister(instruction[3]);
            char registerNumber = instruction[1];
            buff_memory.setRegister( registerNumber,Add_Hex(operand1,operand2));

        }

        else if (instruction[0] == 'B'){
            if (buff_memory.getRegister('0') == buff_memory.getRegister(instruction[1]))
                setCounter(string(1,instruction[2]) + string(1,instruction[3]));
        }

        else if (instruction[0] == 'C'){
            if (instruction == "C000")
                running = false;
        }

        else {
            instructionRegister = instructionRegister.substr(2,2);
        }
    }

    void decode(){
        if (instructionRegister.size() == 2)
            fetch();
        if (instructionRegister.size() == 4){
            execute(instructionRegister);
            instructionRegister = "";
        }
    }

    void displayStatus(){
        cout << "Counter: "<<programCounter;
        cout << "\nInstruction Register: " << instructionRegister;
        cout << "\nScreen: " << screen;
        cout << "\nRegisters:- \n";
        buff_memory.printRegisters();
        cout << "Memory:- \n";
        main_memory.printCells();
    }

    void cycle(string fileName){
        main_memory.loadFile(fileName);
        running = true;
        while (running){
            fetch();
            decode();
        }
        displayStatus();
    }
};


int main() {
    prepareAdress();
    Machine simulator;
    cout << "Enter the input file name: ";
    string fileName; cin >> fileName;
    simulator.cycle(fileName);
}