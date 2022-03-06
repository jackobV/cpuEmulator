#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <bitset>
#include <math.h>
#include <fstream>
#include <map>
#include <sstream>
#include<iterator>
#include <vector>

using namespace std;
using Byte = unsigned char; // 8bit
using u32 = unsigned int; // 32 bit
u32 myPow(u32 x, u32 p)
{
    if (p == 0) return 1;
    if (p == 1) return x;

    u32 tmp = myPow(x, p/2);
    if (p%2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}
struct Process {
    Byte PCHIS;
    Byte PC;
    Byte SP;
    bool D; // status flag Dead
    u32 stack[16];
    u32 fetched;
    u32 opcode;
    u32 imm;
    u32 diedon;
    bool bombFlag;
    bool teleportFlag;
    vector<string> opHistory;
    vector<string> oplog;
    void Reset(){
        SP = 0;
        D = false;
        teleportFlag = false;
        bombFlag = false;
        for (int i = 0; i < 16; i++){
            stack[i] = 0;
        }
    }
    void Fetch(u32 (&HP)[256]){
        fetched = HP[PC];
        PCHIS = PC;
        if(D == false){
            PC = (PC+1);
        }

    }
    void Die(){
        D = true;
        PC = PCHIS;
    }
    void Decode(){
        bitset<32> bs1(fetched);
        string bitstring = bs1.to_string<char,std::string::traits_type,std::string::allocator_type>();
        string lasteight = bitstring.substr(24,8);
        string middlesixteen = bitstring.substr(8,16);
        opcode = stol(lasteight, 0, 2);
        opHistory.push_back("OPCODE : " + to_string(opcode) + "   |  IMM  :  " + to_string(imm) + " |  stackPointer :  " +
                                    to_string(SP) + "   |   PC  :  " + to_string(PC) + "  | ins  :  " + to_string(fetched));
        imm = stol(middlesixteen, 0, 2);
        fetched = 0;
    }
    void Push(u32 toPush){
        if (SP > 15){
            Die();
            cout << "died pushin";
        }
        else {
            stack[SP] = toPush;
            SP ++;
        }
    }
    u32 Pop(){
        if (SP < 1){
            Die();
            cout<< "died popin";
        }
        else{
            u32 temp = stack[SP-1];
            stack[SP-1] = 0;
            SP --;
            return temp;
        }
    }
    u32 MemoryFetch(u32 toFetch, u32 (&HP)[256]){
        u32 temp = toFetch%256;
        if(toFetch == 666){
            Die();
        }
        else{
            return HP[temp];
        }
    }
    void MemoryPut(u32 toPut,u32 location, u32 (&HP)[256]){
        u32 temp = location%256;
        if(location == 666){
            Die();
        }
        else if(temp != 0){
            HP[temp] = toPut;
        }
    }
    void Jantar(u32 (&HP)[256]){
        int base = PCHIS;
        int m1 = (PCHIS-2)%256;
        int m2 = (PCHIS-4)%256;
        int m3 = (PCHIS-8)%256;
        int p1 = (PCHIS+2)%256;
        int p2 = (PCHIS+4)%256;
        int p3 = (PCHIS+8)%256;
        MemoryPut(19,m1,HP);
        MemoryPut(19,m2,HP);
        MemoryPut(19,m3,HP);
        MemoryPut(19,p1,HP);
        MemoryPut(19,p2,HP);
        MemoryPut(19,p3,HP);

    }
    void Execute(u32 (&HP)[256]){
        switch (opcode) {
            case 0:
                cout << 0;
                break;
            case 1:
                cout << 1;
                Push(PCHIS);
                break;
            case 2:
                cout << 2;
                Push(imm);
                break;
            case 3:
                cout << 3;
                Pop();
                break;
            case 4: {
                cout << 4;
                u32 temp1 = Pop();
                u32 temp2 = Pop();
                Push(temp1);
                Push(temp2);
                break;
            }
            case 5:{
                cout << 5;
                u32 temp = Pop();
                Push(temp);
                Push(temp);
                break;
            }
            case 6:
                cout << 6;
                Push(SP);
                break;
            case 7: {
                cout << 7;
                auto temp = Pop();
                u32 fetchedMemory = MemoryFetch(temp,HP);
                Push(fetchedMemory);
                break;
            }
            case 8: {
                cout << 8;
                auto index = Pop();
                auto temp = Pop();
                MemoryPut(temp,index,HP);
                oplog.push_back("changing memory on index " + to_string(index) + " To a val "+ to_string(temp));
                break;
            }
            case 9: {
                auto val1 = Pop();
                auto val2 = Pop();
                u32 val3 = val1 + val2;
                Push(val3);
                cout << 9;
                break;
            }
            case 10: {
                u32 val1 = Pop();
                u32 val2 = Pop();
                u32 val3 = (val1 - val2)%UINT32_MAX;
                Push(val3);
                cout << "a";
                break;
            }
            case 11: {
                auto val1 = Pop();
                auto val2 = Pop();
                if(val2 != 0){
                    u32 val3 = val1 / val2;
                    Push(val3);
                }
                else{
                    Die();
                }
                break;
            }
            case 12: {
                auto val1 = Pop();
                auto val2 = Pop();
                u32 val3;
                if(val1 == 0 && val2==0){
                    val3 = 1;
                }
                else{
                    val3 = myPow(val1,val2);

                }
                Push(val3);
                break;
            }
            case 13: {
                auto val1 = Pop();
                if (val1 == 0){
                    PC = (PC + imm);
                }
                break;
            }
            case 14: {
                auto val1 = Pop();
                if (val1 == 3){
                    PC = (PC + imm);
                }
                break;
            }
            case 15: {
                auto val1 = Pop();
                if (val1 == 7){
                    PC = (PC + imm);
                }
                cout << "FFFFF";
                break;

            }
            case 16: {
                auto val1 = Pop();
                auto val2 = Pop();
                if (val1 >= val2){
                    PC = (PC + imm);
                }
                break;
            }
            case 17: {
                PC = (1 + imm);
                break;
            }
            case 18: {
                Die();
                break;
            }
            case 19: {
                MemoryPut(18,PCHIS,HP);
                break;
            }
            case 20: {
                teleportFlag = true;
                PC = PCHIS;
                break;
            }
            case 21: {
                Jantar(HP);
                break;
            }
            default : {
                Die();
                break;
            }
        }
    }
 };
struct TestCase{
    u32 HP[256];
    vector<Process> processes;
    map<u32,Byte> withTF;
    std::vector<int> key, value;
    u32 totalPC = 0;
    bool teleported = false;
    void addNewProcess(string Test){
        stringstream ss(Test);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> tempArr(begin, end);
        Byte processOffSet = stol(tempArr[0]);
        processes.push_back({PC:processOffSet});
        tempArr.erase(tempArr.begin());
        tempArr.erase(tempArr.begin());

        for(int i = 0; i< tempArr.size(); i++){
            u32 temp = (processOffSet+i)%256;
            if (temp != 0){
            HP[temp] = stol(tempArr[i]);
        }}
    }
    void Execute(){
        for(int i = 0; i<processes.size(); i++){
            processes[i].Reset();
        }
        for(int i = 0; i<5000;i++){
            for (int y = 0; y<processes.size(); y++) {
                if (processes[y].D == false) {
                    if(processes[y].teleportFlag == false){
                    processes[y].Fetch(HP);
                    processes[y].Decode();
                    processes[y].Execute(HP);}
                    if (processes[y].teleportFlag == true) {
                        teleported = true;
                        withTF.insert(pair<u32,Byte> (y,processes[y].PCHIS));
                    }
                }
            }
            if (withTF.size()>1){

                for (std::map<u32,Byte>::iterator it=withTF.begin(); it!=withTF.end(); ++it){
                    key.push_back(it->first);
                    value.push_back(it->second);
                }
                Byte temp = value[0];
                value.erase(value.begin());
                value.push_back(temp);
                for(int i = 0; i<key.size();i++){
                    processes[key[i]].PC = (value[i]+1)%256;
                    processes[key[i]].teleportFlag = false;
                }
                key.clear();
                value.clear();
                withTF.clear();
            }
        }

    }
    void Write(){
        for(int i =0; i<processes.size();i++){
            totalPC = totalPC + processes[i].PC;

        }
        ofstream output;
        output.open("output.txt",std::ios_base::app);
        output << HP[42] << " " << totalPC << "\n";
        output.close();
    }
};
int main() {
    std::ifstream infile("inputs/input.txt");
    string line;
    u32 currentLine = 0;
    u32 nextTest = 1;
    u32 currentTestNo = 0;
    u32 maxTests;
    vector<TestCase> allTests;
    vector<string> processes;
    while (getline(infile, line)){
        if(currentLine == 0){
            cout << "hi";
        }
        else if(currentLine == nextTest){
            u32 temp = stol(line);
            if(!processes.empty()) {
                allTests.push_back({});
                for (int i = 0; i < processes.size(); i++) {
                    allTests[currentTestNo].addNewProcess(processes[i]);
                }
                processes.clear();
                currentTestNo++;
            }
            nextTest = nextTest+temp+1;
        }
        else{
            processes.push_back(line);
        }
        currentLine ++;
    }
    allTests.push_back({});
    for (int i = 0; i < processes.size(); i++) {
        allTests[currentTestNo].addNewProcess(processes[i]);
    }
    processes.clear();
    infile.close();



    for (int i = 0; i< allTests.size(); i++){
        allTests[i].Execute();
    }
    for (int i = 0; i< allTests.size(); i++){
        allTests[i].Write();
    }
    return 0;
}