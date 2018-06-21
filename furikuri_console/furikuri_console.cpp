#include "stdafx.h"




#pragma comment(lib,"..\\..\\LZO\\lzo2_d_86.lib")
#include "..\..\LZO\lzo1z.h"

unsigned char lzo_depack_32[] = {//0xCC,
    0x55, 0x8B, 0xEC, 0x8B, 0x55, 0x08, 0x8B, 0x45, 0x14, 0x01, 0x55, 0x0C, 0x53, 0x33, 0xDB, 0x56,
    0x57, 0x8B, 0x7D, 0x10, 0x89, 0x18, 0x8A, 0x02, 0x89, 0x5D, 0x08, 0x6A, 0x04, 0x5B, 0x3C, 0x11,
    0x76, 0x1C, 0x0F, 0xB6, 0xC8, 0x83, 0xE9, 0x11, 0x42, 0x3B, 0xCB, 0x0F, 0x82, 0xF6, 0x01, 0x00,
    0x00, 0x8A, 0x02, 0x88, 0x07, 0x47, 0x42, 0x83, 0xE9, 0x01, 0x75, 0xF5, 0xEB, 0x63, 0x0F, 0xB6,
    0x32, 0x42, 0x83, 0xFE, 0x10, 0x0F, 0x83, 0x90, 0x00, 0x00, 0x00, 0x85, 0xF6, 0x75, 0x17, 0xEB,
    0x07, 0x81, 0xC6, 0xFF, 0x00, 0x00, 0x00, 0x42, 0x80, 0x3A, 0x00, 0x74, 0xF4, 0x0F, 0xB6, 0x02,
    0x83, 0xC6, 0x0F, 0x03, 0xF0, 0x42, 0x8B, 0x02, 0x03, 0xD3, 0x89, 0x07, 0x03, 0xFB, 0x83, 0xEE,
    0x01, 0x74, 0x2E, 0x3B, 0xF3, 0x72, 0x1F, 0x8B, 0x02, 0x2B, 0xF3, 0x89, 0x07, 0x03, 0xD3, 0x03,
    0xFB, 0x3B, 0xF3, 0x73, 0xF2, 0x85, 0xF6, 0x74, 0x18, 0x8A, 0x02, 0x88, 0x07, 0x47, 0x42, 0x83,
    0xEE, 0x01, 0x75, 0xF5, 0xEB, 0x0B, 0x8A, 0x02, 0x88, 0x07, 0x47, 0x42, 0x83, 0xEE, 0x01, 0x75,
    0xF5, 0x0F, 0xB6, 0x32, 0x42, 0x83, 0xFE, 0x10, 0x73, 0x31, 0x0F, 0xB6, 0x02, 0x8B, 0xCF, 0xC1,
    0xE6, 0x06, 0xC1, 0xE8, 0x02, 0x8D, 0x9E, 0x01, 0x07, 0x00, 0x00, 0x03, 0xD8, 0x42, 0x2B, 0xCB,
    0x89, 0x5D, 0x08, 0x8A, 0x01, 0x88, 0x07, 0x8A, 0x41, 0x01, 0x88, 0x47, 0x01, 0x8A, 0x41, 0x02,
    0x88, 0x47, 0x02, 0x83, 0xC7, 0x03, 0xE9, 0x43, 0x01, 0x00, 0x00, 0x83, 0xFE, 0x40, 0x72, 0x47,
    0x8B, 0xCE, 0x8B, 0xDF, 0x83, 0xE1, 0x1F, 0x83, 0xF9, 0x1C, 0x72, 0x05, 0x2B, 0x5D, 0x08, 0xEB,
    0x12, 0x0F, 0xB6, 0x02, 0xC1, 0xE8, 0x02, 0xC1, 0xE1, 0x06, 0x40, 0x03, 0xC1, 0x42, 0x89, 0x45,
    0x08, 0x2B, 0xD8, 0xC1, 0xEE, 0x05, 0x4E, 0x8A, 0x03, 0x88, 0x07, 0x8A, 0x43, 0x01, 0x88, 0x47,
    0x01, 0x83, 0xC7, 0x02, 0x83, 0xC3, 0x02, 0x8A, 0x03, 0x88, 0x07, 0x47, 0x43, 0x83, 0xEE, 0x01,
    0x75, 0xF5, 0xE9, 0xF7, 0x00, 0x00, 0x00, 0x83, 0xFE, 0x20, 0x72, 0x35, 0x83, 0xE6, 0x1F, 0x75,
    0x17, 0xEB, 0x07, 0x81, 0xC6, 0xFF, 0x00, 0x00, 0x00, 0x42, 0x80, 0x3A, 0x00, 0x74, 0xF4, 0x0F,
    0xB6, 0x02, 0x83, 0xC6, 0x1F, 0x03, 0xF0, 0x42, 0x0F, 0xB6, 0x02, 0x8B, 0xDF, 0x0F, 0xB6, 0x4A,
    0x01, 0xC1, 0xE0, 0x06, 0xC1, 0xE9, 0x02, 0x40, 0x03, 0xC1, 0x2B, 0xD8, 0x83, 0xC2, 0x02, 0xEB,
    0x57, 0x83, 0xFE, 0x10, 0x0F, 0x82, 0x93, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0x8B, 0xDF, 0x83, 0xE0,
    0x08, 0xC1, 0xE0, 0x0B, 0x2B, 0xD8, 0x83, 0xE6, 0x07, 0x75, 0x17, 0xEB, 0x07, 0x81, 0xC6, 0xFF,
    0x00, 0x00, 0x00, 0x42, 0x80, 0x3A, 0x00, 0x74, 0xF4, 0x0F, 0xB6, 0x02, 0x83, 0xC6, 0x07, 0x03,
    0xF0, 0x42, 0x0F, 0xB6, 0x4A, 0x01, 0x0F, 0xB6, 0x02, 0x83, 0xC2, 0x02, 0xC1, 0xE9, 0x02, 0xC1,
    0xE0, 0x06, 0x03, 0xC8, 0x2B, 0xD9, 0x3B, 0xDF, 0x0F, 0x84, 0xA6, 0x00, 0x00, 0x00, 0x81, 0xEB,
    0x00, 0x40, 0x00, 0x00, 0x8B, 0xC7, 0x2B, 0xC3, 0x89, 0x45, 0x08, 0x83, 0xFE, 0x06, 0x0F, 0x82,
    0x43, 0xFF, 0xFF, 0xFF, 0x8B, 0xC7, 0x6A, 0x04, 0x2B, 0xC3, 0x59, 0x3B, 0xC1, 0x0F, 0x8C, 0x34,
    0xFF, 0xFF, 0xFF, 0x8B, 0x03, 0x03, 0xD9, 0x89, 0x07, 0x03, 0xF9, 0x83, 0xEE, 0x02, 0x8B, 0x03,
    0x2B, 0xF1, 0x89, 0x07, 0x03, 0xD9, 0x03, 0xF9, 0x3B, 0xF1, 0x73, 0xF2, 0x85, 0xF6, 0x74, 0x2E,
    0x8A, 0x03, 0x88, 0x07, 0x47, 0x43, 0x83, 0xEE, 0x01, 0x75, 0xF5, 0xEB, 0x21, 0x0F, 0xB6, 0x02,
    0x8B, 0xCF, 0xC1, 0xE8, 0x02, 0x40, 0xC1, 0xE6, 0x06, 0x03, 0xC6, 0x42, 0x2B, 0xC8, 0x89, 0x45,
    0x08, 0x8A, 0x01, 0x88, 0x07, 0x8A, 0x41, 0x01, 0x88, 0x47, 0x01, 0x83, 0xC7, 0x02, 0x0F, 0xB6,
    0x4A, 0xFF, 0x83, 0xE1, 0x03, 0x74, 0x25, 0x8A, 0x02, 0x88, 0x07, 0x47, 0x42, 0x83, 0xF9, 0x01,
    0x76, 0x11, 0x8A, 0x02, 0x88, 0x07, 0x47, 0x42, 0x83, 0xF9, 0x02, 0x76, 0x06, 0x8A, 0x02, 0x88,
    0x07, 0x47, 0x42, 0x0F, 0xB6, 0x32, 0x42, 0xE9, 0x8F, 0xFE, 0xFF, 0xFF, 0x6A, 0x04, 0x5B, 0xE9,
    0xEA, 0xFD, 0xFF, 0xFF, 0x2B, 0x7D, 0x10, 0x8B, 0x45, 0x14, 0x3B, 0x55, 0x0C, 0x89, 0x38, 0x1B,
    0xC0, 0x83, 0xE0, 0xFC, 0x33, 0xF6, 0x83, 0xC0, 0xFC, 0x3B, 0x55, 0x0C, 0x5F, 0x0F, 0x44, 0xC6,
    0x5E, 0x5B, 0x5D, 0xC3
};

int main(){

    /*
    shibari_module module(std::string("..\\..\\app for test\\linked_programm.exe"));

    furikuri fuku;

    if (fuku.set_main_module(&module)) {
        std::vector<uint8_t> out_image;
        

        auto& symbols_code = module.get_code_symbols();
        symbols_code.push_back({ 0x10F9 , 0x139 });

        if (fuku.fuku_protect({ 1,2,90.f,90.f,90.f }, out_image)) {
            FILE* hTargetFile;
            fopen_s(&hTargetFile, "..\\..\\app for test\\fuku_test.exe", "wb");

            if (hTargetFile) {
                fwrite(out_image.data(), out_image.size(), 1, hTargetFile);
                fclose(hTargetFile);
            }
        }
    }

    //*/
   




    
    uint8_t * data_ = new uint8_t[0x1000];
    uint8_t * data_1 = new uint8_t[0x1000];
    for (unsigned int i = 0; i < 0x1000 / 4; i += 4) {
        (*(uint32_t*)&data_[i]) = rand();
    }

    uint8_t * compressed_buf = new uint8_t[0x1000 + (0x1000 / 16) + 64 + 3];
    void * work_mem = new uint8_t[LZO1Z_999_MEM_COMPRESS];

    lzo_uint packed_size = 0;

    if (lzo1z_999_compress(data_, 0x1000, compressed_buf, &packed_size, work_mem) != LZO_E_OK) {
        delete[] work_mem;
        delete[] compressed_buf;
        return 0;
    }

    delete[] work_mem;

    typedef int(__cdecl * _depack_algo)(const unsigned char * src, unsigned long  src_len, unsigned char * dst, unsigned long * dst_len, void * wrkmem);
    for (unsigned int i = 0; i < 1000; i++) {

        fuku_obfuscator obfuscator;
        std::vector<ob_fuku_relocation> relocations;

        obfuscator.set_arch(ob_fuku_arch::ob_fuku_arch_x32);
        obfuscator.set_destination_virtual_address(0);
        obfuscator.set_settings({ 1,2,90.f,00.f,100.f });
        obfuscator.set_relocation_table(&relocations);

                
        unsigned int s_time = GetTickCount();

        obfuscator.push_code(lzo_depack_32, sizeof(lzo_depack_32), 0, 0);
        std::vector<uint8_t> __obf_unpacker = obfuscator.obfuscate_code();
        printf("obfuscated in %.4f sec | size scale %.2f |", (GetTickCount() - s_time) / 1000.f, (float)__obf_unpacker.size() / sizeof(lzo_depack_32));


        uint8_t * __obf_unpacker_ = new uint8_t[__obf_unpacker.size()];

        memcpy(__obf_unpacker_, __obf_unpacker.data(), __obf_unpacker.size());
        DWORD old_p;
        VirtualProtect(__obf_unpacker_, __obf_unpacker.size(), PAGE_EXECUTE_READWRITE, &old_p);

        _depack_algo depack = (_depack_algo)__obf_unpacker_;

        unsigned long unpack_size = 0x1000;

        for (auto &rel : relocations) { //fix reloc
            *(DWORD*)&__obf_unpacker_[rel.virtual_address] += (DWORD)__obf_unpacker_;
        }


        depack(compressed_buf, packed_size, data_1, &unpack_size, 0);

        for (unsigned int i = 0; i < 0x1000; i++) {
            if (data_[i] != data_1[i]) { printf("error!"); Sleep(100000); }
        }

        printf("good!\n");
        delete[] __obf_unpacker_;
    }
    //*/
    return 0;
}

