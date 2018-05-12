#include "stdafx.h"
#include "fuku_obfuscator.h"


fuku_obfuscator::fuku_obfuscator(){
    this->arch = ob_fuku_arch::ob_fuku_arch_x32;

    this->destination_virtual_address = 0;

    this->label_seed = 1;

    this->complexity = 1;
    this->number_of_passes = 1;

    this->association_table = 0; 
    this->relocations       = 0;
    this->ip_relocations    = 0;
}


fuku_obfuscator::~fuku_obfuscator(){
}

void fuku_obfuscator::set_arch(ob_fuku_arch arch) {
    this->arch = arch;
}

void fuku_obfuscator::set_destination_virtual_address(uint64_t destination_virtual_address) {
    this->destination_virtual_address = destination_virtual_address;
}

void fuku_obfuscator::set_complexity(unsigned int complexity) {
    this->complexity = complexity;
}

void fuku_obfuscator::set_number_of_passes(unsigned int number_of_passes) {
    this->number_of_passes = number_of_passes;
}

void fuku_obfuscator::set_association_table(std::vector<ob_fuku_association>*	associations) {
    this->association_table = associations;
}

void fuku_obfuscator::set_relocation_table(std::vector<ob_fuku_relocation>* relocations) {
    this->relocations = relocations;
}

void fuku_obfuscator::set_ip_relocation_table(std::vector<ob_fuku_ip_relocations>* ip_relocations) {
    this->ip_relocations = ip_relocations;
}

ob_fuku_arch   fuku_obfuscator::get_arch() {
    return this->arch;
}

uint64_t     fuku_obfuscator::get_destination_virtual_address() {
    return this->destination_virtual_address;
}

unsigned int fuku_obfuscator::get_complexity() {
    return this->complexity;
}

unsigned int fuku_obfuscator::get_number_of_passes() {
    return this->number_of_passes;
}

std::vector<ob_fuku_association>*    fuku_obfuscator::get_association_table() {
    return this->association_table;
}

std::vector<ob_fuku_relocation>*     fuku_obfuscator::get_relocation_table() {
    return this->relocations;
}

std::vector<ob_fuku_ip_relocations>* fuku_obfuscator::get_ip_relocation_table() {
    return this->ip_relocations;
}


bool fuku_obfuscator::analyze_code(
    uint8_t * src, uint32_t src_len,
    uint64_t virtual_address,
    std::vector<fuku_instruction>&  lines,
    std::vector<ob_fuku_relocation>*	relocations) {

    unsigned int current_len = 0;
    unsigned int line_counter = 0;

    _CodeInfo code_info = { 0,0, src ,src_len ,
        arch == ob_fuku_arch::ob_fuku_arch_x32 ? _DecodeType::Decode32Bits : _DecodeType::Decode64Bits,
        0
    };

    std::vector<_DInst> distorm_instructions;
    unsigned int instructions_number = 0;
    distorm_instructions.resize(src_len);

    _DecodeResult di_result = distorm_decompose64(&code_info, distorm_instructions.data(), src_len, &instructions_number);

    if (di_result == _DecodeResult::DECRES_SUCCESS) {
        distorm_instructions.resize(instructions_number);
        lines.reserve(instructions_number);

        for (const auto &distorm_line : distorm_instructions) {
            fuku_instruction line;

            line.set_source_virtual_address(virtual_address + distorm_line.addr)
                .set_virtual_address(virtual_address + distorm_line.addr)
                .set_op_code(&src[distorm_line.addr], distorm_line.size)
                .set_type((_InstructionType)distorm_line.opcode)
                .set_modified_flags(distorm_line.modifiedFlagsMask)
                .set_tested_flags(distorm_line.testedFlagsMask);

            if (distorm_line.flags&FLAG_RIP_RELATIVE) {
                line.set_flags(line.get_flags() | ob_fuku_instruction_has_ip_relocation)
                    .set_ip_relocation_destination(INSTRUCTION_GET_RIP_TARGET(&distorm_line) + virtual_address)
                    .set_ip_relocation_disp_offset(distorm_line.disp_offset - &src[distorm_line.addr]);
            }

            lines.push_back(line);
        }


        if (relocations) {
            for (auto reloc : *relocations) { //associate relocs

                fuku_instruction * line = this->get_line_by_source_va(lines, reloc.virtual_address);
                if (line) {
                    line->set_flags(line->get_flags() | ob_fuku_instruction_has_relocation);
                    line->set_relocation_id(reloc.relocation_id);
                    line->set_relocation_imm_offset((uint8_t)(reloc.virtual_address - line->get_virtual_address()));
                }
            }
        }


        for (auto &line : lines) {//jmp set labels

            if (line.get_flags()&ob_fuku_instruction_has_ip_relocation) { //disp to local code

                fuku_instruction * dst_line = this->get_line_by_source_va(lines, line.get_ip_relocation_destination());

                if (dst_line) {
                    line.set_link_label_id(set_label(*dst_line));
                }
            }
            else {
                if (line.is_jump()) {
                    uint64_t jmp_dst_va = line.get_virtual_address() +
                        line.get_op_length() +
                        line.get_jump_imm();

                    fuku_instruction * dst_line = get_line_by_source_va(lines, jmp_dst_va);

                    if (dst_line) {
                        line.set_link_label_id(set_label(*dst_line));
                    }
                    else {
                        size_t prefixes_number = line.get_op_pref_size();

                        if (
                            (line.get_op_code()[prefixes_number] == 0x0f &&
                            (line.get_op_code()[prefixes_number + 1] & 0xf0) == 0x80)
                            ) { //far jcc
                            line.set_ip_relocation_disp_offset(prefixes_number + 2);
                        }
                        else if (
                            line.get_op_code()[prefixes_number] == 0xE9 ||
                            line.get_op_code()[prefixes_number] == 0xE8
                            ) {	   //jmp \ call
                            line.set_ip_relocation_disp_offset(prefixes_number + 1);
                        }

                        line.set_flags(line.get_flags() | ob_fuku_instruction_has_ip_relocation);
                        line.set_ip_relocation_destination(jmp_dst_va);
                    }
                }
            }
        }

        handle_jmps(lines);
        return true;
    }
    return false;
}


bool fuku_obfuscator::push_code(
    uint8_t * src, uint32_t src_len,
    uint64_t virtual_address,
    std::vector<ob_fuku_relocation>*	relocations) {

    std::vector<fuku_instruction> new_lines;

    if (analyze_code(src, src_len, virtual_address, new_lines, relocations)) {


        for (auto& stored_line : lines) {  //link stored lines with new lines

            if (!stored_line.get_link_label_id()) {

                if (stored_line.is_jump()) {
                    uint64_t jmp_dst_va = stored_line.get_source_virtual_address() +
                        stored_line.get_op_length() +
                        stored_line.get_jump_imm();

                    fuku_instruction * dst_line = get_line_by_source_va(new_lines, jmp_dst_va);
                    
                    if (dst_line) {
                        stored_line.set_link_label_id(set_label(*dst_line));
                    }
                }
                else if (stored_line.get_flags()&ob_fuku_instruction_has_ip_relocation) {

                    fuku_instruction * dst_line = get_line_by_source_va(new_lines, stored_line.get_ip_relocation_destination());

                    if (dst_line) {
                        stored_line.set_link_label_id(set_label(*dst_line));

                        stored_line.set_ip_relocation_destination(0);
                        stored_line.set_ip_relocation_disp_offset(0);
                        stored_line.set_flags(stored_line.get_flags()& (~uint32_t(ob_fuku_instruction_has_ip_relocation)));
                    }
                }
            }
        }

        for (auto& new_line : new_lines) {//link new lines with stored lines

            if (!new_line.get_link_label_id()) {

                if (new_line.is_jump()) {

                    uint64_t jmp_dst_va = new_line.get_source_virtual_address() +
                        new_line.get_op_length() + 
                        new_line.get_jump_imm();

                    fuku_instruction * dst_line = get_line_by_source_va(lines, jmp_dst_va);

                    if (dst_line) {
                        new_line.set_link_label_id(set_label(*dst_line));
                    }
                }
                else if (new_line.get_flags()&ob_fuku_instruction_has_ip_relocation) {

                    fuku_instruction * dst_line = get_line_by_source_va(lines, new_line.get_ip_relocation_destination());

                    if (dst_line) {
                        new_line.set_link_label_id(set_label(*dst_line));

                        new_line.set_ip_relocation_destination(0);
                        new_line.set_ip_relocation_disp_offset(0);
                        new_line.set_flags(new_line.get_flags()& (~uint32_t(ob_fuku_instruction_has_ip_relocation)));
                    }
                }
            }
        }


        lines.insert(lines.end(), new_lines.begin(), new_lines.end());

        lines_correction(this->lines, this->destination_virtual_address);
    }

    return false;
}

std::vector<uint8_t> fuku_obfuscator::obfuscate_code() {

    fuku_mutation * mutator = (arch == ob_fuku_arch::ob_fuku_arch_x32) ?
        (fuku_mutation*)(new fuku_mutation_x86(this->complexity, this)) : (fuku_mutation*)(new fuku_mutation_x64(this->complexity, this));


    for (unsigned int passes = 0; passes < number_of_passes; passes++) {

        lines_correction(lines, destination_virtual_address);
        mutator->obfuscate(this->lines);

        spagetti_code(lines, this->destination_virtual_address); //mix lines
    }

    delete mutator;

    build_tables(this->lines, this->association_table, this->relocations, this->ip_relocations);
    finalize_jmps(this->lines);

    return lines_to_bin(lines);
}

void fuku_obfuscator::spagetti_code(std::vector<fuku_instruction>& lines, uint64_t virtual_address) {

    lines_correction(lines, virtual_address);

    struct block_lines {
        unsigned int order;
        std::vector<fuku_instruction> lines;
        block_lines::block_lines(unsigned int order, std::vector<fuku_instruction> lines) { this->order = order; this->lines = lines; }
    };
    std::vector<block_lines> line_blocks;


    //generate blocks of lines
    for (unsigned int line_idx = 0; line_idx < lines.size(); ) {
        std::vector<fuku_instruction> line_block;


        for (unsigned int block_lines_counter = 0; line_idx < lines.size(); line_idx++, block_lines_counter++) {
            if (FUKU_GET_CHANCE(FUKU_GENERATE_NEW_BLOCK_CHANCE)) {
                fuku_instruction jmp_instruction;
                fuku_asm_x86_jmp(jmp_instruction,0);                
                jmp_instruction.set_link_label_id(set_label(lines[line_idx]));//add jmp to next instruction
                jmp_instruction.set_tested_flags(lines[line_idx].get_tested_flags());
                line_block.push_back(jmp_instruction);
                break;
            }

            line_block.push_back(lines[line_idx]); //push line
        }


        line_blocks.push_back(block_lines(line_blocks.size(), line_block)); //push block of lines
    }

    //rand blocks without first block
    if (line_blocks.size() > 2) {
        for (unsigned int r_block = 0; r_block < line_blocks.size(); r_block++) {
            std::swap(
                line_blocks[((rand() % (line_blocks.size() - 1)) + 1)],
                line_blocks[((rand() % (line_blocks.size() - 1)) + 1)]
            );
        }
    }


    lines.clear();

    //push lines
    for (unsigned int r_block = 0; r_block < line_blocks.size(); r_block++) {
        for (unsigned int r_block_line = 0; r_block_line < line_blocks[r_block].lines.size(); r_block_line++) {
            lines.push_back(line_blocks[r_block].lines[r_block_line]);
        }
    }
}

void fuku_obfuscator::lines_correction(std::vector<fuku_instruction>& lines, uint64_t virtual_address) {
    uint64_t _virtual_address = virtual_address;
    this->labels_cache.clear();
    this->labels_cache.resize(label_seed - 1);

    for (auto &line : lines) {
        line.set_virtual_address(_virtual_address);
        _virtual_address += line.get_op_length();

        if (line.get_label_id()) { labels_cache[line.get_label_id() - 1] = &line; }
    }
}

fuku_instruction * fuku_obfuscator::get_line_by_source_va(std::vector<fuku_instruction>& lines, uint64_t virtual_address) {

    size_t left = 0;
    size_t right = lines.size();
    size_t mid = 0;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (lines[mid].get_source_virtual_address() == virtual_address ) {

            return &lines[mid];
        }
        else if (lines[mid].get_source_virtual_address() > virtual_address) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }

    return 0;
}

fuku_instruction * fuku_obfuscator::get_line_by_va(std::vector<fuku_instruction>& lines, uint64_t virtual_address) {

    size_t left = 0;
    size_t right = lines.size();
    size_t mid = 0;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (lines[mid].get_virtual_address() == virtual_address) {
            return &lines[mid];
        }
        else if (lines[mid].get_virtual_address() > virtual_address) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }

    return 0;
}

fuku_instruction * fuku_obfuscator::get_line_by_label_id(unsigned int label_id) {

    if (this->labels_cache.size()) {
        if (label_id > 0 && label_id <= this->labels_cache.size()) {
            return this->labels_cache[label_id - 1];
        }
    }

    return 0;
}


void fuku_obfuscator::build_tables(
    std::vector<fuku_instruction>& lines,
    std::vector<ob_fuku_association>* association,
    std::vector<ob_fuku_relocation>*	relocations,
    std::vector<ob_fuku_ip_relocations>*		ip_relocations
) {
    lines_correction(lines, destination_virtual_address);



    if (association) { association->clear(); }
    if (relocations) { relocations->clear(); }
    if (ip_relocations) { ip_relocations->clear(); }


    for (auto &line : lines) {
        if (association) {																		//association
            if (line.get_source_virtual_address() != -1) {
                association->push_back({ line.get_source_virtual_address(), line.get_virtual_address() });
            }
        }

        if (relocations) {																		//relocations
            if (line.get_flags()&ob_fuku_instruction_has_relocation) {
                if (line.get_relocation_label_id()) {//need fix it 
                    uint8_t op_code[16];
                    memcpy(op_code, line.get_op_code(),16);

                    *(uint32_t*)&op_code[line.get_relocation_imm_offset()] =
                        get_line_by_label_id(line.get_relocation_label_id())->get_virtual_address();

                    line.set_op_code(op_code, line.get_op_length());
                }

                relocations->push_back({ (line.get_virtual_address() + line.get_relocation_imm_offset()),line.get_relocation_id() });
            }
        }
        if (ip_relocations) {
            if (line.get_flags()&ob_fuku_instruction_has_ip_relocation && !(line.get_link_label_id())) {				//callouts
                ob_fuku_ip_relocations ip_reloc;

                ip_reloc.destination_virtual_address = line.get_ip_relocation_destination();
                ip_reloc.virtual_address  = line.get_virtual_address();
                ip_reloc.instruction_size = line.get_op_length();
                ip_reloc.disp_relocation_offset = line.get_ip_relocation_disp_offset();

                ip_relocations->push_back(ip_reloc);
            }
        }
    }
}

void fuku_obfuscator::handle_jmps(std::vector<fuku_instruction>& lines) {

    for (size_t line_idx = 0; line_idx < lines.size(); line_idx++) {

        auto& line = lines[line_idx];

        if (line.is_jump()) {
            unsigned int prefixes_number = line.get_op_pref_size();

            uint8_t op_code[16];
            memset(op_code, 0, sizeof(op_code));

            switch (line.get_op_code()[prefixes_number]) {

                //near jmp
            case 0xEB: {

                if (prefixes_number) {
                    op_code[0] = line.get_op_code()[prefixes_number - 1];
                    op_code[1] = 0xE9;
                    *(int32_t*)&op_code[2] = line.get_jump_imm() - 3;

                    line.set_op_code(op_code, 6);
                }
                else {
                    op_code[0] = 0xE9;
                    *(int32_t*)&op_code[1] = line.get_jump_imm() - 3;

                    line.set_op_code(op_code, 5);
                }

                break;
            }

                       //near jcc
            case 0x70:case 0x71:case 0x72:case 0x73:case 0x74:case 0x75:case 0x76:case 0x77:
            case 0x78:case 0x79:case 0x7A:case 0x7B:case 0x7C:case 0x7D:case 0x7E:case 0x7F: {


                if (prefixes_number) {
                    op_code[0] = line.get_op_code()[prefixes_number - 1];
                    op_code[1] = 0x0F;
                    op_code[2] = 0x80 + (line.get_op_code()[prefixes_number]&0xF);
                    *(int32_t*)&op_code[3] = line.get_jump_imm();

                    line.set_op_code(op_code, 7);
                }
                else {
                    op_code[0] = 0x0F;
                    op_code[1] = 0x80 + (line.get_op_code()[prefixes_number]&0xF);
                    *(int32_t*)&op_code[2] = line.get_jump_imm();

                    line.set_op_code(op_code, 6);
                }

                break;
            }

            //loopnz
            case 0xE0: {

                uint8_t new_line_opcode = 0x49;//dec ecx
                fuku_instruction new_line;
                new_line.set_source_virtual_address(line.get_source_virtual_address());
                new_line.set_virtual_address(line.get_virtual_address());
                new_line.set_op_code(&new_line_opcode, 1);

                op_code[0] = 0x74;//loopnz to je
                op_code[1] = line.get_jump_imm();
                line.set_op_code(op_code, 2);
                line.set_source_virtual_address(line.get_source_virtual_address() + 1);
                line.set_virtual_address(line.get_virtual_address() + 1);

                lines.insert(lines.begin() + line_idx, line);
                break;
            }

            //loopz
            case 0xE1: {

                uint8_t new_line_opcode = 0x49;//dec ecx
                fuku_instruction new_line;
                new_line.set_source_virtual_address(line.get_source_virtual_address());
                new_line.set_virtual_address(line.get_virtual_address());
                new_line.set_op_code(&new_line_opcode, 1);

                op_code[0] = 0x75;//loopz to jne
                op_code[1] = line.get_jump_imm();
                line.set_op_code(op_code, 2);
                line.set_source_virtual_address(line.get_source_virtual_address() + 1);
                line.set_virtual_address(line.get_virtual_address() + 1);

                lines.insert(lines.begin() + line_idx, line);

                break;
            }

            //loop
            case 0xE2: {

                uint8_t new_line_opcode = 0x49;//dec ecx
                fuku_instruction new_line;
                new_line.set_source_virtual_address(line.get_source_virtual_address());
                new_line.set_virtual_address(line.get_virtual_address());
                new_line.set_op_code(&new_line_opcode, 1);

                op_code[0] = 0x75;//loop to jne
                op_code[1] = line.get_jump_imm();
                line.set_op_code(op_code, 2);
                line.set_source_virtual_address(line.get_source_virtual_address() + 1);
                line.set_virtual_address(line.get_virtual_address() + 1);

                lines.insert(lines.begin() + line_idx, line);

                break;
            }

            //jcxz
            case 0xE3: {
                
                uint8_t new_line_opcode[2];//or ecx,ecx
                fuku_instruction new_line;
                new_line_opcode[0] = 0x09;
                new_line_opcode[1] = 0xC9;

                new_line.set_source_virtual_address(line.get_source_virtual_address());
                new_line.set_virtual_address(line.get_virtual_address());
                new_line.set_op_code(new_line_opcode, 2);

                op_code[0] = 0x74;//jcxz to jz
                op_code[1] = line.get_jump_imm();
                line.set_op_code(op_code, 2);

                line.set_source_virtual_address(line.get_source_virtual_address() + 1);
                line.set_virtual_address(line.get_virtual_address() + 1);

                lines.insert(lines.begin() + line_idx, line);

                break;
            }

            default: {break; }
            }
        }
    }
}

void fuku_obfuscator::finalize_jmps(std::vector<fuku_instruction>& lines) {

    for (auto &line : lines) {

        if (!(line.get_flags()&ob_fuku_instruction_has_relocation)) {

            if (line.get_link_label_id()) {

                if (line.get_flags()&ob_fuku_instruction_has_ip_relocation) {

                    uint8_t op_code[16];
                    memcpy(op_code, line.get_op_code(), 16);

                    if (line.get_link_label_id()) {
                        fuku_instruction * line_destination = get_line_by_label_id(line.get_link_label_id());

                        if (line_destination) {
                            *(uint32_t*)&op_code[line.get_ip_relocation_disp_offset()] =
                                (line_destination->get_virtual_address() - line.get_virtual_address() - line.get_op_length());
                        }
                    }
                    else {
                        *(uint32_t*)&op_code[line.get_ip_relocation_disp_offset()] =
                            (line.get_ip_relocation_destination() - line.get_virtual_address() - line.get_op_length());
                    }
                }
                else {
                    fuku_instruction * line_destination = get_line_by_label_id(line.get_link_label_id());

                    if (line_destination) {
                        line.set_jump_imm(line_destination->get_virtual_address());
                    }
                }
            }
        }
    }
}

std::vector<uint8_t>  fuku_obfuscator::lines_to_bin(std::vector<fuku_instruction>&  lines) {

    std::vector<uint8_t> lines_dump;
    size_t dump_size = 0;

    for (size_t line_idx = 0; line_idx < lines.size(); line_idx++) { dump_size += lines[line_idx].get_op_length(); }
    lines_dump.resize(dump_size);

    size_t opcode_caret = 0;
    for (auto &line : lines) {
        memcpy(&lines_dump.data()[opcode_caret], line.get_op_code(), line.get_op_length());
        opcode_caret += line.get_op_length();
    }

    return lines_dump;
}

uint32_t fuku_obfuscator::set_label(fuku_instruction& line) {
    if (!line.get_label_id()) {
        line.set_label_id(this->label_seed);
        this->label_seed++;
    }
    return line.get_label_id();
}

uint32_t fuku_obfuscator::get_maxlabel() {
    return this->label_seed;
}