#include "copyright.h"

#include "arm_machine.h"
#include "armsim.h"
#include "system.h"

int getBetween(int num, int a, int b){
	unsigned int n = (unsigned int) num;
	n <<= 31 - a;
	n >>= 31 - a + b;
	return n;
}

void ARMMachine::Run(){
	ArmInstruction *instr = new ArmInstruction;
	interrupt->setStatus(UserMode);

	for (;;) {
        OneInstruction(instr);

		interrupt->OneTick();
		
		if (singleStep && (runUntilTime <= stats->totalTicks))
	  	Debugger();
    }
}

void ARMMachine::OneInstruction(ArmInstruction *instr){

	int raw;
	if (!machine->ReadMem(registers[ProgramCounter], 4, &raw))
	return;			// exception occurred
    
    instr->value = raw;
    instr->Decode();


    if (getBetween(raw,27,25) == 5)
    {
    	/* code */
    	if(raw & (1<<24)){
			printf("bl %d\n", getBetween(raw,23,0));
    	}
    	else{
    		printf("b %d\n", getBetween(raw,23,0));
    	}
    }


    registers[ProgramCounter] += 4;

}

void ARMMachine::DelayedLoad(int nextReg, int nextValue){}

void ArmInstruction::Decode(){

	printf("ArmInstruction : ");
	int i;
	for(i=31;i>=0;i--){
		printf("%d",(bool) ( (1<<i) & value) );
	}
	printf("\n");


	cond = value >> 28;

}