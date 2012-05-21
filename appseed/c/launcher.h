#pragma once


class CLASS_DECL_c launcher
{
public:


   virtual bool ensure_executable() = 0;

   virtual vsstring get_executable_path() = 0;

   virtual bool start();

};


class CLASS_DECL_c simple_shell_launcher
{
public:


#ifdef WINDOWS

   HINSTANCE   m_h;

#else

   bool        m_bOk;
   int         m_pid;

#endif


   simple_shell_launcher(HWND hwnd, const char * lpoperation, const char * lpfile, const char * lpparameters, const char * lpdirectory, int nshowcmd);


   void execute();


};


