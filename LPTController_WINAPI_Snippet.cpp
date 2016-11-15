// TAKE ¹1
HANDLE portHwd = CreateFile( _T("\\\\.\\LPT1" ),
                        GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
      if (portHwd)
      {
          char outBuffer[] = _T("This is a test\r\n");
          int sz_buffer = strlen(outBuffer);

            DWORD bytes_written;
            if (!WriteFile( portHwd,
                  outBuffer,
                  sz_buffer ,
                  &bytes_written,
                  NULL))
            {
                  CloseHandle(portHwd);
                  GetLastError();
                  return 1;
            }

            CloseHandle(portHwd);
      }

// TAKE ¹2
HANDLE parallelPort = CreateFile("LPT1", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
if(parallelPort == INVALID_HANDLE_VALUE)
{
    // handle error
}
...
// Write the string "foobar" (and its null terminator) to the parallel port.
// Error checking omitted for expository purposes.
const char *data = "foobar";
WriteFile(parallelPort, data, strlen(data)+1, NULL, NULL);
...
CloseHandle(parallelPort);
