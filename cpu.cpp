#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <bitset>
using namespace std;
using Byte = unsigned char; // 8bit
using u32 = unsigned long int; // 32 bit

struct Process {
    Byte PC;
    Byte SP;
    Byte D : 1; // status flag Dead
    u32 stack[16];
    u32 fetched;
    u32 opcode;
    u32 imm;
    void Reset(u32 (&HP)[256]){
        PC = 0;
        SP = 0;
        D = 0;
        for (int i = 0; i < 16; i++){
            stack[i] = 0;
        }
        for (int i = 0; i < 16; i++) {
            HP[i + PC] = i;
        }
    }
    void Fetch(u32 (&HP)[256]){
        fetched = HP[PC];
        PC++;
        cout << "fetched code " << fetched;
    }
    void Die(){
        D = 1;
    }
    void Decode(){
        bitset<32> bs1(fetched);
        string bitstring = bs1.to_string<char,std::string::traits_type,std::string::allocator_type>();
        string lasteight = bitstring.substr(24,8);
        string middlesixteen = bitstring.substr(8,16);
        opcode = stoi(lasteight, 0, 2);
        imm = stoi(middlesixteen, 0, 2);
        fetched = 0;
    }
    void Execute(u32 (&HP)[256]){
        switch (opcode) {
            case 0:
                cout << 0;
                break;
            case 1:
                cout << 1;
                stack[SP] = PC; //PC is already incremented, dk if I should add the incremented value or the process PC
                SP ++;
                break;
            case 2:
                cout << 2;
                stack[SP] = imm;
                SP ++;
                break;
            case 3:
                cout << 3;
                stack[SP-1] = 0;
                SP --;
                break;
            case 4:
                cout << 4;
                break;
            case 5:
                cout << 5;
                break;
            case 6:
                cout << 6;
                break;
            case 7:
                cout << 7;
                break;
            case 8:
                cout << 8;
                break;
            case 9:
                cout << 9;
                break;
        }
    }
 };
int main() {
    u32 HP[256];
    for (int i = 0; i < 256; i++){
        HP[i] = 0;
    } //INIT memory HP
    struct Process process1;
    process1.Reset(HP);
    process1.Fetch(HP);
    process1.Fetch(HP);
    process1.Fetch(HP);

    return 0;
}

