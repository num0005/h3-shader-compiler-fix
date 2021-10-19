// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <mutex>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


enum e_platform : int {
    dx11_px,
    durango
};

typedef void __fastcall pc_shader_compiler__set_pdb_path(const char*);

typedef bool __fastcall pc_shader_compiler__init(void* thisptr);

typedef intptr_t __fastcall pc_shader_compiler__clear_external_parameters_t(void* thisptr);

struct Compiler {

    static Compiler& getCompiler() {
        static Compiler compiler;
        return compiler;
    }

    void* get_procedure(const char* name) {
        if (lib)
            return GetProcAddress(lib, name);
        return nullptr;
    }

    Compiler(Compiler&) = delete;

private:

    Compiler() {
        lib = LoadLibraryA("pc_shader_compiler_imp.dll");
        if (lib) {
            auto init_func = reinterpret_cast<pc_shader_compiler__init*>(get_procedure("?init@pc_shader_compiler@@YA_NXZ"));
            bool success = init_func(nullptr);
            auto set_pdb = reinterpret_cast<pc_shader_compiler__set_pdb_path*>(get_procedure("?set_pdb_path@pc_shader_compiler@@YAXPEBD@Z"));
            set_pdb("shader_pdbs\\");
        }
    }
    ~Compiler() {
        if (lib)
            FreeLibrary(lib);
    }
    HMODULE lib = NULL;
};

static inline e_platform remap_platform(e_platform requested)
{
    return e_platform::dx11_px;
}

extern "C" {
    /* doesn't actually need to be detoured cause it doesn't use platform/compiler
    typedef intptr_t __fastcall pc_shader_compiler__add_external_parameter_t(const char* a1, const char* a2, const char* a3);
    intptr_t __fastcall pc_shader_compiler__add_external_parameter(const char* a1, const char* a2, const char* a3) {
        auto hooked = reinterpret_cast<pc_shader_compiler__add_external_parameter_t*>(Compiler::getCompiler().get_procedure("?add_external_parameter@pc_shader_compiler@@YAXPEBD@Z"));
        return hooked(a1, a2, a3);
    }

    intptr_t __fastcall compiler_clear_external_parameters(void* thisptr) {
        auto hooked = reinterpret_cast<pc_shader_compiler__clear_external_parameters_t*>(Compiler::getCompiler().get_procedure("?clear_external_parameters@pc_shader_compiler@@YAXXZ"));
        return hooked(thisptr);
    }
    */

    typedef __int64 __fastcall pc_shader_compiler__compile_shader_t(e_platform platform, __int64 a2, const char* a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7, int a8, void* a9, void* a10);
    
    __int64 __fastcall pc_shader_compiler__compile_shader(e_platform platform, __int64 a2, const char* a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7, int a8, void* a9, void* a10)
    {
        auto hooked = reinterpret_cast<pc_shader_compiler__compile_shader_t*>(Compiler::getCompiler().get_procedure("?compile_shader@pc_shader_compiler@@YAJW4e_platform@1@PEBDIPEBUshader_define@@PEAVc_shader_include_handler@@11IPEAPEAVc_shader_buffer@@4@Z"));
        return hooked(remap_platform(platform), a2, a3, a4, a5, a6, a7, a8, a9, a10);
    }

    typedef __int64 __fastcall pc_shader_compiler__enum_shader_constants_t(e_platform platform, __int64 a2, unsigned int a3, DWORD* a4, __int64* a5);
    __int64 __fastcall pc_shader_compiler__enum_shader_constants(e_platform platform, __int64 a2, unsigned int a3, DWORD* a4, __int64* a5)
    {
        auto hooked = reinterpret_cast<pc_shader_compiler__enum_shader_constants_t*>(Compiler::getCompiler().get_procedure("?enum_shader_constants@pc_shader_compiler@@YAJW4e_platform@1@PEBXIPEAHPEAPEBUshader_constant@@@Z"));
        return hooked(remap_platform(platform), a2, a3, a4, a5);
    }
    typedef signed __int64 __fastcall pc_shader_compiler__get_constant_buffer_size_t(e_platform platform, __int64 a2, unsigned int a3, __int64 a4, DWORD* a5);
    signed __int64 __fastcall pc_shader_compiler__get_constant_buffer_size(e_platform platform, __int64 a2, unsigned int a3, __int64 a4, DWORD* a5)
    {
        auto hooked = reinterpret_cast<pc_shader_compiler__get_constant_buffer_size_t*>(Compiler::getCompiler().get_procedure("?get_constant_buffer_size@pc_shader_compiler@@YAJW4e_platform@1@PEBXIPEBDPEAI@Z"));
        return hooked(remap_platform(platform), a2, a3, a4, a5);
    }
}
