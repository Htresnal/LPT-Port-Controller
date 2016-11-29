#pragma once

//Functions exported from DLL.
//For easy inclusion is user projects.
//Original InpOut32 function support
void	__stdcall Out32(short PortAddress, short data);
short	__stdcall Inp32(short PortAddress);

//My extra functions for making life easy
BOOL	__stdcall IsInpOutDriverOpen();  //Returns TRUE if the InpOut driver was opened successfully
BOOL	__stdcall IsXP64Bit();			//Returns TRUE if the OS is 64bit (x64) Windows.

//DLLPortIO function support
UCHAR   __stdcall DlPortReadPortUchar (USHORT port);
void    __stdcall DlPortWritePortUchar(USHORT port, UCHAR Value);

USHORT  __stdcall DlPortReadPortUshort (USHORT port);
void    __stdcall DlPortWritePortUshort(USHORT port, USHORT Value);

ULONG	__stdcall DlPortReadPortUlong(ULONG port);
void	__stdcall DlPortWritePortUlong(ULONG port, ULONG Value);

//WinIO function support (Untested and probably does NOT work - esp. on x64!)
PBYTE	__stdcall MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
BOOL	__stdcall UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
BOOL	__stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
BOOL	__stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);




