#include<bits/stdc++.h>
using namespace std;

string getString(string str, bool flag){
    string s;
    if(flag){
        for(int i=1; i<str.size(); i++){
            s += str[i];
        }
    }else{
        for(int i=2; i<str.size(); i++){
            s += str[i];
        }
    }
    return s;
}
/////////////////////////////////////////////////////////////////
string decode(string s, unordered_map<string, char> &codesR){
    string temp = "";
    string ans;

    for(int i=0; i<s.size(); i++){
        temp += s[i];
        auto it = codesR.find(temp);
        if(it != codesR.end()){
            ans += it->second;
            temp = "";
        }
    }
    return ans;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//--------------------FILE DECOMPRESSOR------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------






int main(){
    ///////////////////////////////take compressed file as input!
    ifstream in;
    string compressedFileName = "compressed.txt";
    cout<< "Enter the name of compressed file to decompressed( try sample: compressed.txt ) : " <<endl;
    cin>>compressedFileName;
    in.open(compressedFileName);
    if(in.fail()){
        cout << "File Not Found :(" <<endl;
        return 0;
    }
    /////////////////////////extract the length of the encodedString
    string lengthStr;
    in>>lengthStr;
    stringstream calclen(lengthStr);
    int length = 0;
    calclen >> length;
    //////////////////extract the codes map  from the compressed file

    unordered_map<string, char> codesR;
    string str;
    while(true){
        in>>str;
        // getline(in, str);
        if(str == "1757051") break;

        if(str[1] == '~'){
            codesR[getString(str, false)] = ' ';
        }else if(str[1] == '!'){
            codesR[getString(str, false)] = '\n';
        }else{
            codesR[getString(str, true)] = str[0];
        }
    }
    // for(auto pair : codesR){
    //     cout<<pair.second << " - "<<pair.first<<endl;
    // }
    ////////////////////extract the encodedString as preOrignalString


    bool flag = false;
    string preOrignalString;
    while(!in.eof()){
        string decodedString;
        getline(in, decodedString);
        //all the project got fucked up when i used in>>decodedstring
        //cause when encrypted string had spaces, it took the string 
        // before it but that space character was lost that was the biggest 
        // flaw
        for(int i=0; i<decodedString.size(); i++){
            bool flag1 = false;
            char c = decodedString[i];
            unsigned char mask = 1;
            for(int j=7; j>=0; j--){
                if((c & (mask << j)) != 0){
                    preOrignalString += "1";
                }else{
                    preOrignalString += "0";
                }
                if(preOrignalString.size() >= length){
                    flag1 = true;
                    break;
                }
            }
            if(flag1){
                flag = true;
                break;
            }
        }
        if(flag){
            break;
        }
    }
    // cout<<preOrignalString <<endl;
    //////////////////construct orignalString from preOrignalString
    string orignalString = decode(preOrignalString, codesR);
    // cout<<orignalString <<endl;
    //////////////////write orignal string to the decompressed file
    ofstream out;
    out.open("decompressed.txt");
    out << orignalString; 

    cout << "\nFile decompressed successfully and saved as decompressed.txt!!!\n" << endl;
    return 0;
}
