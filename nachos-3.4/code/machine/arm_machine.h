/*
	nachos 的 ARM 体系结构模拟的头文件
*/

#ifndef ARM_MACHINE_H
#define ARM_MACHINE_H

#include "copyright.h"
#include "utility.h"
#include "translate.h"
#include "disk.h"


#define PageSize 	SectorSize 	// set the page size equal to
					// the disk sector size, for
					// simplicity

#define NumPhysPages    32
#define MemorySize 	(NumPhysPages * PageSize)
#define TLBSize		4		// if there is a TLB, make it small

enum ExceptionType { 
			 
			 NoException,           // Everything ok!
		     SyscallException,      // A program executed a system call.
		     PageFaultException,    // No valid translation found
		     ReadOnlyException,     // Write attempted to page marked 
					    // "read-only"
		     BusErrorException,     // Translation resulted in an 
					    // invalid physical address
		     AddressErrorException, // Unaligned reference or one that
					    // was beyond the end of the
					    // address space
		     OverflowException,     // Integer overflow in add or sub.
		     IllegalInstrException, // Unimplemented or reserved instr.
		     
		     NumExceptionTypes
};


// User program CPU state.  The full set of MIPS registers, plus a few
// more because we need to be able to start/stop a user program between
// any two instructions (thus we need to keep track of things like load
// delay slots, etc.)

#define StackPointer 13
#define LinkRegister 14
#define ProgramCounter 15
#define CPSR	16


#define NumTotalRegs 	37

// The following class defines an instruction, represented in both
// 	undecoded binary form
//      decoded to identify
//	    operation to do
//	    registers to act on
//	    any immediate operand value

class ArmInstruction {
  public:
    void Decode();	// decode the binary representation of the instruction
    unsigned int value; // binary representation of the instruction
    char cond;
    char rn;
    char rd;
    char rm;
    char rs;

    int immediate;
    int rotate;

};


class ARMRegister{

	char name[10];
	int number;


};
/*arm
ARM7 处理器的运行模式：

缩写		名字

USR 	用户模式
FIQ		快速中断模式
IRQ		外部中断模式
SVC		特权模式
MON     管理者模式
ABT		终止模式
HYP		虚拟机模式	
UND     未定义模式
SYS     系统模式

*/
class Mode {
public:
	char modeName[4];
	int mod;
	int value;

	Mode(){}
	Mode(const char * c, int _mod, int _value){
		strcpy(modeName,c);
		mod = _mod;
		value = _value;
	}

};


class ARMMachine {
  public:
    ARMMachine(bool debug);	// Initialize the simulation of the hardware
				// for running user programs
    ~ARMMachine();			// De-allocate the data structures

    void Run();	 		// Run a user program

    int ReadRegister(int num);

    void WriteRegister(int num, int value);
    void OneInstruction(ArmInstruction *instr); 	
    void DelayedLoad(int nextReg, int nextVal);  	
    bool ReadMem(int addr, int size, int* value);
    bool WriteMem(int addr, int size, int value);
    
    ExceptionType Translate(int virtAddr, int* physAddr, int size,bool writing);

    void RaiseException(ExceptionType which, int badVAddr);
				// Trap to the Nachos kernel, because of a
				// system call or other exception.  

    void Debugger();		// invoke the user program debugger
    void DumpState();		// print the user CPU and memory state 

    char *mainMemory;		// physical memory to store user program,
				// code and data, while executing
    int registers[NumTotalRegs]; // CPU registers, for executing user programs

    Mode mod; // ARM 处理器的模式

    TranslationEntry *tlb;
    TranslationEntry *pageTable;
    unsigned int pageTableSize;

  private:
    bool singleStep;
    int runUntilTime;		
};

extern void ExceptionHandler(ExceptionType which);

unsigned int WordToHost(unsigned int word);
unsigned short ShortToHost(unsigned short shortword);
unsigned int WordToMachine(unsigned int word);
unsigned short ShortToMachine(unsigned short shortword);

#endif
