#include<stdio.h>
#include<elf.h>

#define FILE_PATH "/home/nadahamed/task1/a.out"

/*argv[0] > application name*/
int main( )
{

FILE* elfFile= fopen(FILE_PATH,"r");

Elf64_Ehdr elfHeader; /*struct el fyh data bta3t elf header*/ 
		      
fread(&elfHeader,sizeof(elfHeader),1,elfFile);

printf("ELF Header:\n");
printf(" Magic:%x %X %X %X ",elfHeader.e_ident[EI_MAG0],elfHeader.e_ident[EI_MAG1],elfHeader.e_ident[EI_MAG2]
		,elfHeader.e_ident[EI_MAG3]);

printf("%x %x %x %x %x %x \n",elfHeader.e_ident[EI_CLASS],elfHeader.e_ident[EI_DATA],elfHeader.e_ident[EI_VERSION],
		elfHeader.e_ident[EI_OSABI], elfHeader.e_ident[EI_ABIVERSION], elfHeader.e_ident[EI_PAD]);
switch(elfHeader.e_ident[EI_CLASS])
{

	case 1: printf(" Class: ELF32\n");
		break;
	case 2: printf(" Class: ELF64\n");
	        break;	
}
switch(elfHeader.e_ident[EI_DATA])
{

	case 1: printf(" Data:  2's complement, little endian\n");
                break;
        case 2: printf(" Data:  2's complement, big endian\n");
                break;

}
printf(" Version:  %d (current)\n",elfHeader.e_ident[EI_VERSION]);

switch(elfHeader.e_ident[EI_OSABI])
{

        case 0: printf(" OS/ABI: System V \n");
                break;
        case 1: printf(" OS/ABI: HP-UX \n");
                break;
	case 2: printf(" OS/ABI: NetBSD \n");
                break;
        case 3: printf(" OS/ABI: Linux \n");
                break;
        case 4: printf(" OS/ABI: GNU Hurd \n");
                break;
        case 6: printf(" OS/ABI: Solaris \n");
                break;
        case 7: printf(" OS/ABI: AIX \n");
                break;
        case 8: printf(" OS/ABI: IRIX \n");
                break;
        case 9: printf(" OS/ABI: FreeBSD \n");
                break;
        case 0x0A: printf(" OS/ABI: Tru64 \n");
                break;
        case 0x0B: printf(" OS/ABI: Novell Modesto \n");
                break;
        case 0x0C: printf(" OS/ABI: OpenBSD \n");
                break;
        case 0x0D: printf(" OS/ABI: OpenVMS \n");
                break;
        case 0x0E: printf(" OS/ABI: NonStop Kernel \n");
                break;
        case 0x0F: printf(" OS/ABI: AROS \n");
                break;
        case 0x10: printf(" OS/ABI: FenixOS \n");
                break;
        case 0x11: printf(" OS/ABI: Nuxi CloudABI \n");
                break;
        case 0x12: printf(" OS/ABI: Stratus Tech OpenVOS \n");
                break;

}
printf(" ABI Version:  %d\n",elfHeader.e_ident[EI_ABIVERSION]);

switch(elfHeader.e_type)
{
	case 0: printf(" TypE: None\n");
		break;
	case 1:  printf(" Type: REL\n");
                break;

	case 2:  printf(" Type: EXEC\n");
                break;

	case 3:
		printf(" Type: DYN SHARED\n");
                break;
      
	case 4: printf(" Type: CORE FILE\n");
                break;	
	default:
	printf("error its =%d\n",elfHeader.e_type);
break;	
}
switch(elfHeader.e_machine)
{
        case 3: printf(" Machine: x86\n");
                break;
        case 6: printf(" Machine: Intel MCU\n");
                break;

        case 7:  printf(" Machine: Intel80860\n");
                break;

        case 13: 
                printf(" Machine: Intel80960\n");
                break;
      
        case 28: printf(" Machine: ARM\n");
                break;
        case 0x3E: printf(" Machine: AMD x86-64\n");
                break;
		 
	default:
	printf(" not indicated machine type!!!its=%x \n",elfHeader.e_machine);
	break;	
}

printf(" VERSION:%d\n",elfHeader.e_version);

printf(" Entry point address:0x%ld\n",elfHeader.e_entry);
printf(" Start of program header:%ld (bytes in file)\n",elfHeader.e_phoff);
printf(" Start of section header%ld (bytes in file)\n",elfHeader.e_shoff);
printf(" Flags:%d\n",elfHeader.e_flags);
printf(" Size of this header:%d (bytes)\n",elfHeader.e_ehsize);
printf(" Size of program header:%d (bytes)\n",elfHeader.e_phentsize);
printf(" Number of program headers:%d\n",elfHeader.e_phnum);
printf(" Size of section headers:%d (bytes)\n",elfHeader.e_shentsize);
printf(" Number of section headers:%d\n",elfHeader.e_shnum);
printf(" Section header string table index:%d\n",elfHeader.e_shstrndx);


fclose(elfFile);

return 0;
}

