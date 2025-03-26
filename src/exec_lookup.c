/*
 * exec_lookup.c - Static analysis and binary inspection
 *
 * liblookup - a platform-independent runtime and static lookup library
 *
 * Copyright (c) 2025 Impact Tiling Group Pty Ltd.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <lookup/exec_lookup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef PLATFORM_MACHO
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#else
#include <elf.h>
#endif /* PLATFORM_MACHO */

/* Linux and FreeBSD (ELF format) */
#ifdef PLATFORM_ELF
static char* __read_string_table(int fd, Elf64_Shdr* shdr)
{
    char* strtab = malloc(shdr->sh_size);
    if (!strtab) {
        return NULL;
    }
    
    if (lseek(fd, shdr->sh_offset, SEEK_SET) == -1) {
        free(strtab);
        return NULL;
    }
    
    if (read(fd, strtab, shdr->sh_size) != shdr->sh_size) {
        free(strtab);
        return NULL;
    }
    
    return strtab;
}

int find_symbol_in_executable(const char* filename, const char* symbol)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    
    Elf64_Ehdr header;
    if (read(fd, &header, sizeof(header)) != sizeof(header)) {
        close(fd);
        return -1;
    }
    
    /* verify file format */
    if (memcmp(header.e_ident, ELFMAG, SELFMAG) != 0) {
        close(fd);
        return -1;
    }
    
    /* File is valid, let's parse it and find the symbol. */
    int result = parse_exec_and_find_symbol(fd, symbol);
    close(fd);
    return result;
}

int parse_exec_and_find_symbol(int fd, const char* symbol)
{
    Elf64_Ehdr ehdr;
    
    /* Go back to start of file. */
    if (lseek(fd, 0, SEEK_SET) == -1) {
        return -1;
    }
    
    /* Read ELF header and section headers. */
    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
        return -1;
    }
    
    Elf64_Shdr* section_headers = malloc(sizeof(Elf64_Shdr) * ehdr.e_shnum);
    if (!section_headers) {
        return -1;
    }
    if (lseek(fd, ehdr.e_shoff, SEEK_SET) == -1) {
        free(section_headers);
        return -1;
    }
    if (read(fd, section_headers, sizeof(Elf64_Shdr) * ehdr.e_shnum)
        != sizeof(Elf64_Shdr) * ehdr.e_shnum) {
        free(section_headers);
        return -1;
    }
    
    /* Retrieve section header string table. */
    Elf64_Shdr* sh_strtab = &section_headers[ehdr.e_shstrndx];
    char* sh_strtab_data = __read_string_table(fd, sh_strtab);
    if (!sh_strtab_data) {
        free(section_headers);
        return -1;
    }
    
    /* Retrieve the symbol table and its associated string table. */
    Elf64_Shdr* symtab_hdr = NULL;
    Elf64_Shdr* strtab_hdr = NULL;
    
    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char* section_name = sh_strtab_data + section_headers[i].sh_name;
        
        if (section_headers[i].sh_type == SHT_SYMTAB) {
            symtab_hdr = &section_headers[i];
        } else if (strcmp(section_name, ".strtab") == 0) {
            strtab_hdr = &section_headers[i];
        }
    }
    
    if (!symtab_hdr || !strtab_hdr) {
        free(sh_strtab_data);
        free(section_headers);
        return -1;
    }
    
    /* We have the tables, let's read and process 'em. */
    char* strtab = __read_string_table(fd, strtab_hdr);
    if (!strtab) {
        free(sh_strtab_data);
        free(section_headers);
        return -1;
    }

    if (lseek(fd, symtab_hdr->sh_offset, SEEK_SET) == -1) {
        free(strtab);
        free(sh_strtab_data);
        free(section_headers);
        return -1;
    }
    
    int found = -1;
    Elf64_Sym sym;
    size_t num_symbols = symtab_hdr->sh_size / sizeof(Elf64_Sym);
    
    for (size_t i = 0; i < num_symbols; i++) {
        if (read(fd, &sym, sizeof(sym)) != sizeof(sym)) {
            break;
        }
        
        /* Filter out empty symbols */
        if (sym.st_name == 0) {
            continue;
        }
        
        const char* sym_name = strtab + sym.st_name;
        if (strcmp(sym_name, symbol) == 0) {
            /* Check if the symbol we have is valid */
            unsigned char type = ELF64_ST_TYPE(sym.st_info);
            if (type == STT_FUNC || type == STT_OBJECT) {
                found = 1;
                break;
            }
        }
    }
    
    free(strtab);
    free(sh_strtab_data);
    free(section_headers);
    return found;
}
#endif  /* PLATFORM_ELF */

/* macOS (Mach-O format) */
#ifdef PLATFORM_MACHO
int find_symbol_in_executable(const char *filename, const char *symbol)
{
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {
       return -1;
   }

   struct mach_header_64 header;
   if (read(fd, &header, sizeof(header)) != sizeof(header)) {
       close(fd);
       return -1;
   }

   /* verify file format */
   if (header.magic != MH_MAGIC_64) {
       close(fd);
       return -1;
   }

   /* File is valid, let's parse it and find the symbol. */
   int result = parse_exec_and_find_symbol(fd, symbol);
   close(fd);
   return result;
}

int parse_exec_and_find_symbol(int fd, const char *symbol)
{
   struct load_command load_cmd;
   struct symtab_command symtab_cmd;
   struct nlist_64 symbol_entry;

   /* Find the symbol table. */
   off_t offset = sizeof(struct mach_header_64);
   int num_load_cmds = 0;
   if (read(fd, &num_load_cmds, sizeof(num_load_cmds)) != sizeof(num_load_cmds)) {
       return -1;
   }

   /* Read the load commands and check for 'LC_SYMTAB'. */
   for (int i = 0; i < num_load_cmds; i++) {
       if (read(fd, &load_cmd, sizeof(load_cmd)) != sizeof(load_cmd)) {
           return -1;
       }

       if (load_cmd.cmd == LC_SYMTAB) {
           /* We've found the symbol table */
           if (read(fd, &symtab_cmd, sizeof(symtab_cmd)) != sizeof(symtab_cmd)) {
               return -1;
           }

           /* We have the symbol table */
           off_t symtab_offset = symtab_cmd.symoff;
           int num_symbols = symtab_cmd.nsyms;

           /* Now, search the symbol table for the symbol. */
           lseek(fd, symtab_offset, SEEK_SET);
           for (int i = 0; i < num_symbols; i++) {
               if (read(fd, &symbol_entry, sizeof(symbol_entry)) != sizeof(symbol_entry)) {
                   return -1;
               }

               /* Filter out invalid or undefined symbols. */
               if (symbol_entry.n_un.n_strx == 0) {
                   continue;
               }

               /* Read the symbol string from the string table. */
               off_t str_offset = symtab_cmd.stroff + symbol_entry.n_un.n_strx;
               char symbol_name[512];
               lseek(fd, str_offset, SEEK_SET);
               if (read(fd, symbol_name, sizeof(symbol_name)) <= 0) {
                   return -1;
               }

               if (strcmp(symbol_name, symbol) == 0) {
                   return 1;  /* We've found the symbol. */
               }
           }
       } else {
           lseek(fd, load_cmd.cmdsize - sizeof(load_cmd), SEEK_CUR);
       }
   }

   return -1;  /* Unable to find the symbol. */
}
#endif  /* PLATFORM_MACHO */

