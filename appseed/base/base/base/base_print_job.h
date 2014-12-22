#pragma once


class CLASS_DECL_BASE print_job:
   virtual public ::user::job
{
public:


   int32_t      m_iPageStart;
   int32_t      m_iPageCount;
   int32_t      m_iPrintingPage;


   print_job(::aura::application * papp);
   virtual ~print_job();


   virtual int32_t run() = 0;



};





