
#include "stdafx.h"

/*
uint8_t pcode[] = { vm_opcode_86_pure, 0x5, 0x0, 0xb8 ,0 ,0 ,0 ,0  };

int main() {
    uint32_t ddd = 0x1337;
    std::vector<uint8_t> p_code;

    p_code.push_back(vm_opcode_86_operand_create);  //set operand eax
    p_code.push_back(vm_opcode_86_operand_set_base_link_reg); p_code.push_back(r_86::r_eax);

  //  p_code.push_back(vm_opcode_86_operand_create);  //set operand ecx
   // p_code.push_back(vm_opcode_86_operand_set_base_link_reg); p_code.push_back(r_86::r_ecx);

    
    p_code.push_back(vm_opcode_86_operand_create);  //set operand eax
    p_code.push_back(vm_opcode_86_operand_set_index_scale); p_code.push_back(r_86::r_eax); p_code.push_back(2);

    p_code.push_back(vm_opcode_86_operand_set_disp);
    p_code.resize(p_code.size() + 4); *(uint32_t*)&p_code.data()[p_code.size() - 4] = (uint32_t)ddd;
    
//    p_code.push_back(vm_opcode_86_mov); p_code.push_back(vm_ops_ex_code(1, 0, 4).ex_code); //mov op 4 bytes
    
        //vm_double_ops_ex_code
    void *ptr_code = p_code.data();

    __asm {
        push ptr_code
        jmp fuku_vm_entry
    }
    
    return 0;
}
//*/

BOOL APIENTRY main_dll( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call) {

    case DLL_PROCESS_ATTACH: {

        break;
    }

    case DLL_THREAD_ATTACH: {

        break;
    }

    case DLL_THREAD_DETACH: {

        break;
    }

    case DLL_PROCESS_DETACH: {

        break;
    }


    default:
        break;
    }
    return TRUE;
}

//*/