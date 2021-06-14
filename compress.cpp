#include<bits/stdc++.h>
using namespace std;

struct Node{
    char ch;
    int f;
    Node* left;
    Node* right;
};

class compare{
    public:
    bool operator()(Node* n1, Node* n2){
        return n1->f > n2->f; 
        //note the the highest priority element will have low freq
    }
};
///////////////////////////////////////////////////////////////////
void inOrder(Node* root, unordered_map<char, string> &code, string s){
    if(root == NULL){
        return;
    }
    if(root->left == NULL && root->right == NULL){
        code[root->ch] = s;
        return;
    }
    inOrder(root->left, code, s+"0");
    inOrder(root->right, code, s+"1");
}
//////////////////////////////////////////////////////////////////
Node* createNode(char c, int f){
    Node* nn = new Node();
    nn->ch = c;
    nn->f = f;
    nn->left = nn->right = NULL;
    return nn;
}
//////////////////////////////////////////////////////////////////
Node* buildHTree(vector<Node*> &v){
    priority_queue<Node*, vector<Node*>, compare> pq;
    for(auto it : v){
        pq.push(it);
    }


    while(pq.size() != 1){
        
        Node* nn = new Node();
        Node* min1 = pq.top();pq.pop();
        Node* min2 = pq.top();pq.pop();
        nn->left = min1;
        nn->right = min2;
        nn->ch = '~';
        nn->f = min1->f + min2->f;
        // cout<<min1->ch <<" "<<min2->ch <<endl;
        pq.push(nn);
    }

    Node* root = pq.top();

    return root;
}
//////////////////////////////////////////////////////////////////
string encode(string s, unordered_map<char, string> &code){
    string ans;
    
    for(int i=0; i<s.size(); i++){
        ans += code[s[i]];
    }
    return ans;
}
///////////////////////////////////////////////////////////////////
void flush(char &buf, int &tBits){
    buf = 0;
    buf = buf&0;
    tBits = 0;
}
///////////////////////////////////////////////////////////////////
void putInbuf(char c, char &buf, int &tBits){
    unsigned char mask = 1;
    if(c == '1'){
        mask = mask << (7 - tBits);
		buf = buf | mask;
    }
    if(c == '0'){
        mask = mask << (7 - tBits);
		mask = ~mask;
		buf = buf & mask;
    }
    tBits++;
}

///////////////////////////////////////////////////////////////////
bool checkFileName(string inputFileName){
    int n = inputFileName.size();
    if(inputFileName[n-1] != 't' || inputFileName[n-2] != 'x' || inputFileName[n-3] != 't' || inputFileName[n-4] != '.'){
        cout<<"Input file MUST be a text(.txt) file!" <<endl;
        return false;
    }
    return true;
}
void print(char c){
    unsigned char mask = 1;
    for(int j=7; j>=0; j--){
        if((c & (mask << j)) != 0){
            cout<<"1";
        }else{
            cout<<"0";
        }  
    }
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//--------------------FILE COMPRESSOR------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------






int main(){
    /////////////////////////////accept file name as input
    ifstream in;
    string inputFileName = "input.txt";
    cout<< "Enter the name of text file to be compressed( try sample: input.txt ) : " <<endl;
    cin>>inputFileName;
    if(!checkFileName(inputFileName)){
        return 0;
    }
    //////////////////////check if file is not present in folder
    in.open(inputFileName);
    if(in.fail()){
        cout << "File Not Found :(" <<endl;
        return 0;
    }
    ////////////////////read input file and construct as string
    string inputString;
    while(!in.eof()){
        string temp;
        getline(in, temp);
        inputString += temp;
        inputString += '\n';
    }
    inputString.pop_back();
    // cout<< inputString <<endl; 
    in.close();
    //////////////////////////////////////////////////////////
    double inputFileSize = inputString.size();
    int compressedFileSize1 = 0, compressedFileSize2 = 0;
    //////////////////////////////create frequency map & a node for HM tree

    unordered_map<char, int> freq;
    for(int i=0; i<inputString.size(); i++){
        freq[inputString[i]]++;
    }

    vector<Node*> v;
    for(auto it : freq){
        Node* nn = createNode(it.first, it.second);
        v.push_back(nn);
    }
    ////////////////////////////////build the HM tree with the nodes array
    Node* root = buildHTree(v);
    ////////////////////////////////create the map of char and its HM codes
    unordered_map<char, string> codes;
    inOrder(root, codes, "");
    // for(auto pair : codes){
    //     cout<< pair.first <<" - "<< pair.second <<endl;
    // }
    ///////////////////encode the inputString using above map
    string encodedString = encode(inputString, codes);
    cout<<encodedString<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    compressedFileSize1 += encodedString.size();
    ///write the codes map and the size of encoded string to the file first

    ofstream out("compressed.txt");

    out<<encodedString.size();
    out<<"\n";

    for(auto pair : codes){
        string str;
        if(pair.first == ' '){
            str += '~';
            str += '~';
        }
        else if(pair.first == '\n'){
            str += '!';
            str += '!';
        }else{
            str += pair.first;
        }
        str += pair.second;
        str += "\n";
        compressedFileSize2 += str.size();
        out<<str;
    }
    out<<"1757051\n";
    compressedFileSize2 += 7;

    //create the characters from encodedString and put in compressed file
    int tBits = 0;
    char buf = 0;
    buf = buf&0;
    for(int i=0; i<encodedString.size(); i++){
        putInbuf(encodedString[i], buf, tBits);

        if(tBits == 8){
            out<<buf;
            print(buf);
            flush(buf, tBits);
            // out.flush();
        }
    }
    out<<buf;
    print(buf);
    flush(buf, tBits);
    // out.flush();
    out.close();

    cout << "\nFile compressed successfully and saved as compressed.txt !!!" <<endl;
    ///////////////////////////compute the % compression//////////////
    // cout << "Input File size : " << inputFileSize << " bytes" <<endl;
    // cout << "Compressed File size : " << compressedFileSize1/8 + compressedFileSize2 << " bytes" <<endl;
    double compressedFileSize = compressedFileSize1/8 + compressedFileSize2;
    double percentage = 100 - (compressedFileSize/inputFileSize)*100;
    cout << (int)percentage <<"%" << " compression Achieved!!!\n" <<endl;
    ////////////////////////////////////////////////////////////////////

    return 0;
}