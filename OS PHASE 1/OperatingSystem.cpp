#include<iostream>
#include<string.h>
#include<sstream>
#include<stdlib.h>
#include<queue>
#include<fstream>
#define NUM_REGS 4
using namespace std;

class VirtualMachine
{
private:
     char Memory[10][10][4]; // Memory
    unsigned char reg[NUM_REGS]; // Register
    unsigned char IR[NUM_REGS]; // Instruction Registers
    unsigned int IC[2];
    unsigned int C;
    int BT_flag = 0;
    queue<string> instruction_queue;
    queue<string> data_queue;
    queue<string> processd_data;
    int gd_data_block,gd_intruction_block = 0,pd_block,no_of_inst;
    int reg1=0, reg2=0, reg3=0, reg4=0;
    int IR1=0, IR2=0, IR3=0, IR4=0;
    int IC1=0, IC2=0;
    int data_length;
    string s;
    string Instructions [7] = {"GD","PD","LR","SR","CR","BT","H"};

public:
    VirtualMachine(/* args */);
    ~VirtualMachine();
    void ShowContent();
    void INIT(string file);
    void LOAD();
    string fetch(int, int);
    void Decode(string inst);
    void Execute();
    void GD(int address);
    void PD(int address);
    void CR(int address);
    int Verify_Block(int block);
    void H();
    void LR(int);
    void SR(int);
    void BT(int);
};
int main()
{
    unsigned x;
    int a;
    VirtualMachine vm;
    cout<<endl;
      cout<<"\t\t\t\t========================================================================="<<endl;
      cout<<"\t\t\t\t-------------------------------------------------------------------------"<<endl;
      cout<<"\t\t\t\t****************  O P E R A T I N G   S Y S T E M    ********************"<<endl;
      cout<<"\t\t\t\t-------------------------------------------------------------------------"<<endl;
      cout<<"\t\t\t\tPROJECT BUILT BY : "<<endl;
      cout<<"\t\t\t\t 				 VIT Pune SY-CS B Students 				      "<<endl;
      cout<<endl;
      cout<<"\t\t\t\t========================================================================="<<endl;


    vm.INIT("Input.txt");
    vm.Execute();
    return 0;
}
VirtualMachine::VirtualMachine()
{
    reg[0] = (char)reg1;
    reg[1] = (char)reg2;
    reg[2] = (char)reg3;
    reg[3] = (char)reg4;
    IR[0] = (char)IR1;
    IR[1] = (char)IR2;
    IR[2] = (char)IR3;
    IR[3] = (char)IR4;
    IC[0] = IC1;
    IC[1] = IC2;
    C = 0;


}

VirtualMachine::~VirtualMachine()
{
}
void VirtualMachine :: INIT(string file)
{
    fstream File(file);
    string text,sub;

    while (getline(File, text))
    {
        sub = text.substr(0,4);

        if (sub.compare("$AMJ")==0)
        {

            sub = text.substr(8,4);
           // cout<<sub;
            stringstream intru(sub);
            int no_of_inst = 0;
            intru >> no_of_inst;
            getline(File,text);
            int j=0;
            string temp;
            for (int i = 0; i < no_of_inst; i++)
            {
                if (i > 9 && i % 10 == 0)
                {
                  getline(File,text);
                  j = 0;
                }

                if(i == (no_of_inst-1))
                {
                   temp = text.substr(j);

                }
                else
                {
                   temp = text.substr(j,4);

                }


                instruction_queue.push(temp);
                j = j+4;
            }

        }
        if (sub.compare("$DTA")==0)
        {
            while (getline(File,text))
            {

                if (text.compare("$END")==0)
                {
                    break;
                }
                data_queue.push(text);

            }


        }


    }

    LOAD();
}
void VirtualMachine :: LOAD()
{
    string pointer;
    int i,j = 0,k=0;
    no_of_inst = 0;
    printf("\n\t\t\t\t***************************  LOADED INSTRUCTIONS  ************************\n");
    while (!instruction_queue.empty())
    {
        pointer = instruction_queue.front();
        cout<<"\n\t\t\t\t\t\t"<<j<<k<<"\t\t\t\t\t"<<pointer<< "\t";
        k++;
        if(k == 10)
        {
          j++;
          k= 0;
        }
        for ( i = 0; i < 4; i++)
        {
            Memory[0][no_of_inst][i] = pointer[i];
            if (pointer[i] == 'H')
            {
              break;
            }

        }
        no_of_inst++;
        instruction_queue.pop();

    }
    printf("\n\t\t\t\t***************************  BEFORE EXCECUTION  *****************************\n");
    ShowContent();
    printf("\n\t\t\t\t***************************  EXCECUTION STARTED  ****************************\n");
}
void VirtualMachine :: ShowContent()
{
  int i;
  printf("\n \tRegisters \t\t\t Instruction Registers \t\t\t Instruction Counter \t\t\t Toggle");
  printf("\n");
  for( i=0; i<NUM_REGS; i++ )
    printf( "  %04X ", reg[ i ] );
    printf("\t\t");
  for( i=0; i<NUM_REGS; i++ )
    printf( " %04X ", IR[ i ] );
    printf("\t\t\t");
    printf( "%d %d", IC[ 0 ],IC[1] );
    printf("\t\t\t\t");
    printf("   %d",C);
}
string VirtualMachine :: fetch(int code_block, int row)
{

    string instruction="";
    int i,j;
      //IC[0] = code_block;
      //IC[1] = IC2;
    for ( i = 0; i < 4; i++)
    {
        string s(1,Memory[code_block][row][i]);
        IR[i] = Memory[code_block][row][i];
        instruction.append(s);
    }

    return instruction;
}
void VirtualMachine :: Decode(string inst)
{
        char in = inst[0];   // Getting Characters
        int ch = (int)in;    // Converting to ASCII
        string add = inst.substr(2,2);
        stringstream addd(add);
        int no = 0; // Getting Block Address
        addd >> no;
        if (ch != 72)
        {
          char in2 = inst[1];

        int ch2 = (int)in2;
        ch = (ch*100)+ ch2;
        }
        else
        {
          ch = 72;
        }
        int flag = 0;

        switch (ch)
        {
        case 7168:
          GD(no);
          break;
        case 8068:
          PD(no);
        break;
        case 7682:
        LR(no);
        break;
        case 8382:
        SR(no);
        break;
        case 6782:
        CR(no);
        break;
        case 6684:
        BT(no);
        break;
        case 72:
        H();
        break;
        default:
        cout<<"\n Fail";
        break;
        }

}
void VirtualMachine :: Execute()
{
  IC2 = 0;
  for (int i = 0; i < no_of_inst; i++)

  {



    string fetched_instruction = fetch(gd_intruction_block, IC2);
    ShowContent();
    Decode(fetched_instruction);
    ++IC2;

    if (IC2 > 9)
    {
      gd_intruction_block++;
      IC2 = IC2%10;
    }


    IC[0]= gd_intruction_block;
    IC[1] = IC2;

  }

}
int VirtualMachine :: Verify_Block(int block)
{
    if (block != 0 && block%10 == 0)
    {
      return (block/10);
    }
    else
    {
      if (block == 0)
      {
          return 0;
      }

      else
      {
        cout<<"Wrong Block !";
        exit(0);
      }


    }


}
void VirtualMachine :: GD(int address)
{
  int j=0,k=0;
  address = Verify_Block(address);
  gd_data_block = address;
  string insert_data;
  insert_data = data_queue.front();
  data_length = insert_data.length();
  data_queue.pop();
  //cout<<insert_data<<data_length;
  for (int i = 0; i < data_length; i++)
  {
      Memory[gd_data_block][j][k] = insert_data[i];
      k++;
      if (k==4)
      {
        ++j;
        k = 0;
      }

  }
  Memory[gd_data_block][j][k] = '$';

}
void VirtualMachine :: PD(int address)
{
      int j=0,k=0;
      string temp="";
      pd_block = Verify_Block(address);
      for (int i = 0; i < 100; i++)
      {
        if (Memory[pd_block][j][k] == '$')
        {
          break;
        }

        string s(1,Memory[pd_block][j][k]);
        temp.append(s);
        k++;
        if (k==4)
        {
          j++;
          k=0;
        }

      }
      ofstream File2("Output.txt",ios::app);
      File2<<temp<<endl;
      File2.close();
      if ( BT_flag == 1)
    {
      ++IC2;
      BT_flag = 0;
    }
}
void VirtualMachine :: LR(int address)
{
    int block,row;
    if (address > 99 or address < 0)
    {
      printf("\n Wrong Block !");
      exit(0);
    }
    if (address == 0)
    {
        block =0;
        row = 0;
    }
    else{
      block = address/10;
      row = address%10;
    }
    string temp = fetch(block,row);
    for (int i = 0; i < 4; i++)
    {

      reg[i] = temp[i];
    }




}
void VirtualMachine :: SR(int address)
{
  int block,row;
    if (address > 99 or address < 0)
    {
      printf("\n Wrong Block !");
      exit(0);
    }
    if (address == 0)
    {
        block =0;
        row = 0;
    }
    else{
      block = address/10;
      row = address%10;
    }
    for (int i = 0; i < 4; i++)
    {
      Memory[block][row][i] = reg[i];
    }

}
void VirtualMachine :: CR(int address)
{
    int block,row;
    if (address > 99 or address < 0)
    {
      printf("\n Wrong Block !");
      exit(0);
    }
    if (address == 0)
    {
        block =0;
        row = 0;
    }
    else{
      block = address/10;
      row = address%10;
    }
    string data1 = fetch(block,row);
    string data2 = "";
    for (int i = 0; i < 4; i++)
    {
      string s1(1,reg[i]);
      data2.append(s1);
    }
    if (data1.compare(data2)==0)
    {
      C = 1;
      ShowContent();
    }



}


void VirtualMachine:: BT(int address)
{
      if (C == 1)
      {
        gd_intruction_block = address / 10;
        IC2 = address % 10;
        IC2 = IC2 - 2;
        no_of_inst = no_of_inst - 1;
      }
      else
      {
        no_of_inst = no_of_inst - 1;
        BT_flag = 1;
      }




}
void VirtualMachine :: H()
{
    ofstream File2("Output.txt",ios::app);
      File2<<"\r\n \r\n";
      File2.close();

    printf("\n\t\t\t\t***************************  JOB COMPLETE  ****************************\n");


}
