#include<bits/stdc++.h>
#include<fstream>
#include<limits>


using namespace std;

ifstream& GotoLine(ifstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}

string DecToHex(int n){
    string HexNum = "00";
    int i = 0;
    while(n!=0){
        int temp = 0;
        temp = n % 16;
        if (temp < 10) {
            HexNum[i] = temp + 48;
            i++;
        }
        else {
            HexNum[i] = temp + 55;
            i++;
        }
        n = n / 16;
    }
    swap(HexNum[0],HexNum[1]);

    HexNum[2]= '\0';
    return HexNum;
}



int main(){
	ifstream ftempin("input.txt");
	ofstream ftemp("temp_input.txt");
	ofstream fout("instruction_set_hex_code");
	
	string opcode, word, line;
	
	while(!ftempin.eof()){
        ftempin >> opcode;
		if(opcode[opcode.size() - 1] == ':'){
			//label line
			ftemp << opcode << endl;
			continue;
		}

        if(opcode == "push"){
            //push
			ftempin >> word;
            //mixed
			if(word[word.length() -1] == ')'){
				//push 3($t0)
				ftemp << "subi $sp, $sp, 2" << endl;
                ftemp << "sw $t0, 0($sp)" << endl;
                ftemp << "lw $t0, " << word << endl;
                ftemp << "sw $t0, 1($sp)" << endl;
                ftemp << "lw $t0, 0($sp)" << endl;
                ftemp << "addi $sp, $sp, 1" << endl;
			}

            else{
                ftemp << "subi $sp, $sp, 1" << endl;
				ftemp << "sw " << word << ", 0($sp)" << endl;
            }
			continue;
        }
        else if(opcode == "pop"){
            //pop
			ftempin >> word;
            ftemp << "lw " << word << ", 0($sp)" << endl;
            ftemp << "addi $sp, $sp, 1" << endl;
			continue;
        }
        else{
			getline(ftempin, line);
			ftemp << opcode << line << endl;
		}
    }
	ftemp.close();
	ftempin.close();
	ifstream ftemp2in("temp_input.txt");

    char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string Instructions[] = { "add", "nor", "and", "ori", "j", "subi", "andi", "bneq", "addi", "sub", "beq", "sw", "or", "lw", "srl", "sll"};
    string Register[] = { "$zero", "$t0", "$t1", "$t2", "$t3", "$t4", "$sp"};

    map<string, char> hexVlaue;
    map<string, int> label;

    for(int i=0;i<16;i++)
        hexVlaue.insert(pair<string, char>(Instructions[i], hex[i]));
    for(int i=0;i<7;i++)
        hexVlaue.insert(pair<string, char>(Register[i], hex[i]));

    //fout<<hexVlaue.at("add")<<endl;

    string ch, temp2;
    int temp;
    unsigned int instruction_count = 0;

    while(!ftemp2in.eof()){
        getline(ftemp2in, ch);
        if(ch[ch.size() - 1] == ':'){
            ch.resize(ch.size() - 1);
            label.insert(pair<string, int>( ch, instruction_count));
            instruction_count--;
        }
        instruction_count++;
    }

    //GotoLine(ftemp2in, 1);
	ftemp2in.close();
	ifstream fin("temp_input.txt");
	
    string out = "00000";
	
	fout<<"v2.0 raw\n"<<endl;
	
	int pc = 0;
    while(!fin.eof()){
        out = "00000";
        fin>>ch;
		pc++;
		//cout<<"temp file while loop : pc " << pc << " word read : \"" << ch << "\"" <<endl;
        if(ch=="add"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            out[2] = hexVlaue.at(ch);

            fout<<out<<endl;

        }
        else if(ch=="nor"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            out[2] = hexVlaue.at(ch);

            fout<<out<<endl;

        }
        else if(ch=="and"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            out[2] = hexVlaue.at(ch);

            fout<<out<<endl;

        }
        else if(ch=="ori"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>temp;
			if(temp < 0){
				temp += 256;
			}
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;
        }
        else if(ch=="j"){ ///J
            //opcode
            out[0] = hexVlaue.at(ch);
            //address
            fin>>ch;
            out[1] = DecToHex(label.at(ch))[0];
            out[2] = DecToHex(label.at(ch))[1];
			
			//cout<<ch<<": pc = "<<label.at(ch)<<endl;

            fout<<out<<endl;
        }
        else if(ch=="subi"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>temp;
			if(temp < 0){
				temp += 256;
			}
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;

        }
        else if(ch=="andi"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>temp;
			if(temp < 0){
				temp += 256;
			}
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;

        }
        else if(ch=="bneq"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>ch;
			int pc_related = label.at(ch) - pc;
			if(pc_related < 0){
				pc_related += 256;
			}
            out[3] = DecToHex(pc_related)[0];
            out[4] = DecToHex(pc_related)[1];
			
			//cout<<ch<<": pc = "<<label.at(ch)<<", pc related = "<<pc_related<<endl;

            fout<<out<<endl;

        }
        else if(ch=="addi"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>temp;
			if(temp < 0){
				temp += 256;
			}
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;

        }
        else if(ch=="sub"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            out[2] = hexVlaue.at(ch);

            fout<<out<<endl;

        }
        else if(ch=="beq"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //immed
            fin>>ch;
            int pc_related = label.at(ch) - pc;
			if(pc_related < 0){
				pc_related += 256;
			}
            out[3] = DecToHex(pc_related)[0];
            out[4] = DecToHex(pc_related)[1];
			
			//cout<<ch<<": pc = "<<label.at(ch)<<", pc related = "<<pc_related<<endl;

            fout<<out<<endl;

        }
        else if(ch=="sw"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
			//cout<<"sw : "<<ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //mixed
            fin>>ch;
			//cout<<" "<<ch;
            ch.resize(ch.size() - 1);
            temp2 = ch.substr(0, ch.find("("));
            temp = 0;
            int i = 0;
            while(temp2[i]!='\0'){
				if(temp2[i] == '-'){
					continue;
				}
                temp = temp * 10 + (temp2[i] - '0');
                i++;
            }
			if(temp2[0] == '-'){
				temp = 256 - temp;
			}
            //offset
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];
            //immed
            i = ch.find("(");
            temp2 = ch.substr(i+1);
            out[1] = hexVlaue.at(temp2);

            fout<<out<<endl;

        }
        else if(ch=="or"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src1
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            out[2] = hexVlaue.at(ch);

            fout<<out<<endl;

        }
        else if(ch=="lw"){ ///I
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[2] = hexVlaue.at(ch);
            //mixed
            fin>>ch;
            ch.resize(ch.size() - 1);
            temp2 = ch.substr(0, ch.find("("));
            temp = 0;
            int i = 0;
            while(temp2[i]!='\0'){
				if(temp2[i] == '-'){
					continue;
				}
                temp = temp * 10 + (temp2[i] - '0');
                i++;
            }
			if(temp2[0] == '-'){
				temp = 256 - temp;
			}
            //offset
            out[3] = DecToHex(temp)[0];
            out[4] = DecToHex(temp)[1];
            //immed
            i = ch.find("(");
            temp2 = ch.substr(i+1);
            out[1] = hexVlaue.at(temp2);

            fout<<out<<endl;

        }
        else if(ch=="srl"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //shft
            fin>>temp;
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;

        }
        else if(ch=="sll"){ ///R
            //opcode
            out[0] = hexVlaue.at(ch);
            //dst
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[3] = hexVlaue.at(ch);
            //src2
            fin>>ch;
            ch.resize(ch.size() - 1);
            out[1] = hexVlaue.at(ch);
            //shft
            fin>>temp;
            out[4] = DecToHex(temp)[1];

            fout<<out<<endl;

        }
        else {
			pc--;

        }
    }

	fin.close();
	fout.close();
    return 0;
}
