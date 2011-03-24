//*****************************************************************************
//
//      Class:          service_status
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Provides default initialization for SERVICE_STATUS.
//
//*****************************************************************************

#pragma once



class CLASS_DECL_ca service_status : public SERVICE_STATUS
{
public:

   service_status()
   {
      ATLASSERT(sizeof (*this) == sizeof (SERVICE_STATUS));

      ::ZeroMemory(this,
         sizeof (SERVICE_STATUS));
   }
};
