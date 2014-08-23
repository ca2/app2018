#include "framework.h"


print_job::print_job(sp(::axis::application) papp) :
   element(papp),
   ::job(papp),
   ::user::job(papp)
{

   m_iPageStart = -1;
   m_iPageCount = -1;
   m_iPrintingPage = 0;

}


print_job::~print_job()
{

}



