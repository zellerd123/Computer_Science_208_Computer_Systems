#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "machine.h"
#include "code.h"
#include <stdbool.h> 


struct machine_t machine;

/*
 * Allocate more space to keep track of values on the simulated stack.
 */
void grow_stack(uint64_t new_sp) {
    // Grow the stack upwards
    if (new_sp < machine.stack_top) {
        // Round down to a multiple of word size
        if (new_sp % WORD_SIZE_BYTES != 0) {
            new_sp -= new_sp % WORD_SIZE_BYTES;
        }

        // Allocate space and copy over old values 
        void *new_stack = malloc(machine.stack_bot - new_sp + 1);
        memset(new_stack, 0, machine.stack_top - new_sp);
        if (machine.stack != NULL) {
            memcpy(new_stack + (machine.stack_top - new_sp), machine.stack, machine.stack_bot - machine.stack_top + 1);
            free(machine.stack);
        }

        // Update machine
        machine.stack = new_stack;
        machine.stack_top = new_sp;
    }
    // Grow the stack downwards
    else if (new_sp > machine.stack_bot) {
        // Round up to a multiple of word size
        if (new_sp % WORD_SIZE_BYTES != 0) {
            new_sp += WORD_SIZE_BYTES - (new_sp % WORD_SIZE_BYTES);
        }
        else {
            new_sp += WORD_SIZE_BYTES;
        }

        // Allocate space and copy over old values 
        void *new_stack = malloc(new_sp - machine.stack_top);
        memset(new_stack + (machine.stack_bot - machine.stack_top), 0, new_sp - machine.stack_bot);
        if (machine.stack != NULL) {
            memcpy(new_stack, machine.stack, machine.stack_bot - machine.stack_top);
            free(machine.stack);
        }

        // Update machine
        machine.stack = new_stack;
        machine.stack_bot = new_sp - 1;
    }
}

/*
 * Initialize the machine
 */
void init_machine(uint64_t sp, uint64_t pc, char *code_filepath) {
    // Populate general purpose registers
    for (int i = 0; i <= 30; i++) {
        machine.registers[i] = REGISTER_NULL;
    }
    
    // Populate special purpose registers
    machine.sp = sp;
    machine.pc = pc;
    
    // Load code
    machine.code = parse_file(code_filepath, &(machine.code_top), &(machine.code_bot));
    /*int i = 0;
    while (machine.code[i].operation.code != OPERATION_NULL) {
        print_instruction(machine.code[i]);
        i++;
    }*/

    // Prepare stack
    machine.stack_top = sp;
    machine.stack_bot = sp + WORD_SIZE_BYTES - 1;
    machine.stack = malloc(WORD_SIZE_BYTES);
    memset(machine.stack, 0, WORD_SIZE_BYTES);

    // Clear all condition codes
    machine.conditions = 0;
}

void print_memory() {
    // Print condition codes
    printf("Condition codes:");
    if (machine.conditions & CONDITION_ZERO) {
        printf(" Z");
    }
    if (machine.conditions & CONDITION_NEGATIVE) {
        printf(" N");
    }
    if (machine.conditions & CONDITION_POSITIVE) {
        printf(" P");
    }
    printf("\n");

    // Print the value of all used registers
    printf("Registers:\n");
    for (int i = 0; i <= 30; i++) {
        if (machine.registers[i] != REGISTER_NULL) {
            printf("\tw/x%d = 0x%lx\n", i, machine.registers[i]);
        }
    }
    printf("\tsp = 0x%lX\n", machine.sp);
    printf("\tpc = 0x%lX\n", machine.pc);

    // If necessary, grow the stack before printing it
    if (machine.sp < machine.stack_top || machine.sp > machine.stack_bot) {
        grow_stack(machine.sp);
    }

    // Print the value of all words on the stack
    printf("Stack:\n");
    unsigned char *stack = machine.stack;
    for (int i = 0; i < (machine.stack_bot - machine.stack_top); i += 8) {
        printf("\t");

        if (machine.sp == i + machine.stack_top) {
            printf("%10s ", "sp->");
        }
        else {
            printf("           ");
        }

        printf("+-------------------------+\n");
        printf("\t0x%08lX | ", i + machine.stack_top);
        for (int j = 0; j < 8; j++) {
            printf("%02X ", stack[i+j]);
        }
        printf("|\n");
    }
    printf("\t           +-------------------------+\n");
}

/*
 * Get the next instruction to execute
 */
struct instruction_t fetch() {
    int index = (machine.pc - machine.code_top) / 4;
    return machine.code[index];
}

/*
 * Get the value associated with a constant or register operand.
 */
uint64_t get_value(struct operand_t operand) {
    assert(operand.type == OPERAND_constant || operand.type == OPERAND_address || operand.type == OPERAND_register);

    switch(operand.type){
        case OPERAND_constant:
            return operand.constant;
        
        case OPERAND_register:
            switch(operand.reg_type){
                case REGISTER_w:
                    return machine.registers[operand.reg_num] & 0xFFFFFFFF;
                
                case REGISTER_x:
                    return machine.registers[operand.reg_num];
                
                case REGISTER_pc:
                    return machine.pc;

                case REGISTER_sp:
                    return machine.sp;
                default:
                    printf("SIMULATION CANNOT HANDLE THAT REGISTER\n");
                    break;
            }
        default:
            printf("SIMULATION CONNAT GET A VALUE FROM THAT SPACE\n");
            break;
    }

    return 0; //if no cases are found return default value of zero

}

/*
 * Put a value in a register specified by an operand.
 */
void put_value(struct operand_t operand, uint64_t value) {
    assert(operand.type == OPERAND_register);
    
    switch(operand.reg_type){
        case REGISTER_w:
            machine.registers[operand.reg_num] = value & 0xFFFFFFFF;
            break;
        
        case REGISTER_x:
            machine.registers[operand.reg_num] = value;
            break;
        
        case REGISTER_pc:
            machine.pc = value;
            break;
        
        case REGISTER_sp:
            machine.sp = value;
            break;

        default:
            printf("THIS SIMULATION HAS NO ABILITY TO PUT SOMETHING IN THAT REGISTER!\n");
            break;
    }
}

/*
 * Get the memory address associated with a memory operand.
 */
uint64_t get_memory_address(struct operand_t operand) {
    assert(operand.type == OPERAND_memory);
    uint64_t baseAddress = 0;
    uint64_t offset = operand.constant;

    switch(operand.reg_type){
        case REGISTER_w:
            baseAddress = machine.registers[operand.reg_num] & 0xFFFFFFFF;
            break;
        
        case REGISTER_x:
            baseAddress = machine.registers[operand.reg_num];
            break;
        
        case REGISTER_sp:
            baseAddress = machine.sp;
            break;
        
        case REGISTER_pc:
            baseAddress = machine.pc;
            break;
        
        default:
            printf("THAT REGISTER DOES NOT EXIST IN THIS SIMULATION!\n");
    }
    return baseAddress + offset;
}

/*
 * Execute an instruction
 */
void execute(struct instruction_t instruction) {
    switch(instruction.operation) {
    
    case OPERATION_nop:
        break;
        
    case OPERATION_add: {
        uint64_t operand1 = get_value(instruction.operands[1]);
        uint64_t operand2 = get_value(instruction.operands[2]);
        uint64_t result = operand1 + operand2;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_sub: {
        uint64_t operand1 = get_value(instruction.operands[1]);
        uint64_t operand2 = get_value(instruction.operands[2]);
        uint64_t result = operand1 - operand2;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_lsl: {
        uint64_t value = get_value(instruction.operands[1]);
        uint64_t shift = get_value(instruction.operands[2]);
        uint64_t result = value << shift;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_lsr: {
        uint64_t value = get_value(instruction.operands[1]);
        uint64_t shift = get_value(instruction.operands[2]);
        uint64_t result = value >> shift;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_and: {
        uint64_t operand1 = get_value(instruction.operands[1]);
        uint64_t operand2 = get_value(instruction.operands[2]);
        uint64_t result = operand1 & operand2;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_orr: {
        uint64_t operand1 = get_value(instruction.operands[1]);
        uint64_t operand2 = get_value(instruction.operands[2]);
        uint64_t result = operand1 | operand2;
        put_value(instruction.operands[0], result);
        break;
    }

    case OPERATION_ldr: {
        uint64_t add1 = get_memory_address(instruction.operands[1]);
        uint64_t add2 = (uint64_t)(machine.stack + (add1 -machine.stack_top));
        uint64_t value = 0;

        if(instruction.operands[0].reg_type == REGISTER_w)
        {
            value = *((uint32_t *)add2);
        }
        else //assume it is register X
        {
            value = *((uint64_t *)add2);
        }

        put_value(instruction.operands[0], value);
        break;
    }  

    case OPERATION_str: {
        uint64_t value = get_value(instruction.operands[0]);
        uint64_t add1 = get_memory_address(instruction.operands[1]);
        uint64_t add2 = (uint64_t)(machine.stack + (add1 - machine.stack_top));

        if(instruction.operands[0].reg_type == REGISTER_w)
        {
            *((uint32_t *) add2) = (uint32_t)value;
        }
        else //assume it is register x
        {
            *((uint64_t *)add2) = value;
        }

        break;
    }

    case OPERATION_mov: {
        uint64_t value = get_value(instruction.operands[1]);
        put_value(instruction.operands[0],value);
        break;
    }

    case OPERATION_b: {
        machine.pc = instruction.operands[0].constant;
        return; //prevent PC increment @ the end of the switch
    }
    case OPERATION_cmp: {
        uint64_t operand1 = get_value(instruction.operands[0]);
        uint64_t operand2 = get_value(instruction.operands[1]);
        uint64_t result = operand1 - operand2;
        if (result < 0){
            machine.conditions = CONDITION_NEGATIVE;
        }
        else if (result == 0){
            machine.conditions = CONDITION_ZERO;
        }
        else{
            machine.conditions = CONDITION_POSITIVE;
        }
        return;
    }
    case OPERATION_bl: {
        machine.registers[30] = machine.pc + 4; //link register
        machine.pc = instruction.operands[0].constant;
        return; //prevent PC increment @ the end of the switch
    }

    case OPERATION_beq:
    case OPERATION_bne:
    case OPERATION_blt:
    case OPERATION_bgt:
    case OPERATION_ble:
    case OPERATION_bge:
    {
        bool shouldBranch = false;

        switch (instruction.operation) {
            case OPERATION_beq:
                shouldBranch = (machine.conditions & CONDITION_ZERO) != 0;
                break;
            case OPERATION_bne:
                shouldBranch = (machine.conditions & CONDITION_ZERO) == 0;
                break;
            case OPERATION_blt:
                shouldBranch = (machine.conditions & CONDITION_NEGATIVE) != 0;
                break;
            case OPERATION_bgt:
                shouldBranch = (machine.conditions & CONDITION_POSITIVE) != 0;
                break;
            case OPERATION_ble:
                shouldBranch = (machine.conditions & (CONDITION_ZERO | CONDITION_NEGATIVE)) != 0;
                break;
            case OPERATION_bge:
                shouldBranch = (machine.conditions & CONDITION_POSITIVE) != 0;
                break;
        }

        if(shouldBranch) {
            machine.pc = instruction.operands[0].constant;
            return;
        }
        break;

    }

    case OPERATION_ret: {
        machine.pc = machine.registers[30]; //return from link register
        return; //prevent PC increment @ the end of the switch
    }

    default:
        printf("!!INSTRUCTION HAS NOT BE IMPLEMENTED!!");
    }

    machine.pc += 4; //update program counter

}