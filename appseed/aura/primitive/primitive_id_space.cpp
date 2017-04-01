//#include "framework.h"


//id_space * create_id_space();
class ::id_space * id_space::s_pidspace = NULL;


id_space::id_space()
{
   m_pcs = new critical_section();
   m_map.InitHashTable(8192);
}

// id_space is static, it goes aways only and with the application
// so avoid freeing errors when even crash translators does not exist.

id_space::~id_space()
{

   free_all();

   ::aura::del(m_pcs);

}


void id_space::free_all()
{

   cslock sl(m_pcs);

   try
   {
      for (auto p : m_map)
      {
      //for (index i = 0; i < m_psza.get_count(); i++)
      //{
         try
         {

//            char * psz = (char *)m_psza.element_at(i);

            ::memory_free(p.m_element2);

         }
         catch (...)
         {

         }

      }

   }
   catch (...)
   {

   }

   //m_psza.remove_all();

}


id id_space::operator()(const char * psz)
{

   cslock sl(m_pcs);

   index iIndex = 0;

   auto * p = m_map.get_assoc(psz);

   if(p->m_element2 != NULL)
   {

      return id(p->m_element2, this);

   }

   char * pszNew = (char *)memory_alloc(strlen(psz) + 1);

   if(pszNew == NULL)
      throw memory_exception(::get_thread_app());

   strcpy(pszNew,psz);

   //m_psza.insert_at(iIndex,pszNew);

   p->m_element2 = pszNew;

   return id(pszNew, this);

}

id id_space::operator()(int64_t i)
{

   return (int_ptr) i;

}


//void id_space::sort()
//{
//
//   if(m_psza.m_nSize <= 1)
//      return;
//
//   stackLowerBound.allocate(0,1024);
//   stackUpperBound.allocate(0,1024);
//
//   const char * pid1 = NULL;
//   const char * pid2 = NULL;
//
//   const char ** pida = m_psza.get_data();
//
//
//   index iLowerBound;
//   index iUpperBound;
//   index iLPos,iUPos,iMPos;
//   //   index i;
//   stackLowerBound.push(0);
//   stackUpperBound.push(m_psza.m_nSize - 1);
//   while(true)
//   {
//      iLowerBound = stackLowerBound.pop();
//      iUpperBound = stackUpperBound.pop();
//      iLPos = iLowerBound;
//      iMPos = iLowerBound;
//      iUPos = iUpperBound;
//      while(true)
//      {
//         if(iMPos != iUPos)
//         {
//            pid1 = pida[iMPos];
//         }
//         while(true)
//         {
//            if(iMPos == iUPos)
//               break;
//            // REFERENCE
//            //if(id_strcmp(&m_ida.m_pData[m_iaStr.m_pData[iMPos]], &m_ida.m_pData[m_iaStr.m_pData[iUPos]]) <= 0)
//            // iUPos--;
//            iU = pia[iUPos];
//            pid2 = pida[iU];
//            if(strcmp(pid1,pid2) > 0)
//               iUPos--;
//            else
//            {
//               pia[iMPos] = iU;
//               pia[iUPos] = iM;
//               break;
//            }
//         }
//         if(iMPos == iUPos)
//            break;
//         iMPos = iUPos;
//         if(iMPos != iLPos)
//         {
//            iM = pia[iMPos];
//            pid2 = pida[iM];
//         }
//         while(true)
//         {
//            if(iMPos == iLPos)
//               break;
//            iL = pia[iLPos];
//            pid1 = pida[iL];
//            if(strcmp(pid1,pid2) > 0)
//               iLPos++;
//            else
//            {
//               pia[iLPos] = iM;
//               pia[iMPos] = iL;
//               break;
//            }
//         }
//         if(iMPos == iLPos)
//            break;
//         iMPos = iLPos;
//      }
//      if(iLowerBound < iMPos - 1)
//      {
//         stackLowerBound.push(iLowerBound);
//         stackUpperBound.push(iMPos - 1);
//      }
//      if(iMPos + 1 < iUpperBound)
//      {
//         stackLowerBound.push(iMPos + 1);
//         stackUpperBound.push(iUpperBound);
//      }
//      if(stackLowerBound.get_size() == 0)
//         break;
//   }
//
//}


//bool id_space::find(const char * pszFind,index & iIndex)
//{
//   
//   if(m_psza.m_nSize == 0)
//   {
//
//      iIndex = 0;
//
//      return false;
//
//   }
//
//   index iLowerBound = 0;
//
//   index iUpperBound = m_psza.get_upper_bound();
//
//   int64_t iCompare;
//   
//   const char * psz;
//
//   while(iUpperBound - iLowerBound >= 0)
//   {
//
//      iIndex = (iUpperBound + iLowerBound) / 2;
//
//      psz =  m_psza[iIndex];
//
//      iCompare = strcmp(pszFind,psz);
//
//      if(iCompare == 0)
//      {
//
//         return true;
//
//      }
//      else if(iCompare > 0)
//      {
//
//         if(iUpperBound == iLowerBound)
//         {
//
//            break;
//
//         }
//
//         iLowerBound = iIndex + 1;
//
//      }
//      else
//      {
//
//         if(iUpperBound == iLowerBound)
//         {
//
//            break;
//
//         }
//
//         iUpperBound = iIndex - 1;
//
//      }
//
//   }
//
//   if(iCompare > 0)
//   {
//
//      iIndex++;
//
//   }
//
//   return false;
//
//}




strid_array::strid_array(bool bSynch)
{

   if(bSynch)
   {

      m_pmutex = canew(mutex(get_thread_app()));

   }

}

// id_space is static, it goes aways only and with the application
// so avoid freeing errors when even crash translators does not exist.

strid_array::~strid_array()
{

   /*try
   {
   if(m_pmutex != NULL)
   {
   delete m_pmutex;
   }
   }
   catch(...)
   {
   }

   try
   {
   for(index i = 0; i < this->get_count(); i++)
   {
   try
   {
   if(this->element_at(i).is_text())
   {
   free((void *) this->element_at(i).m_psz);
   }
   }
   catch(...)
   {
   }
   }
   }
   catch(...)
   {
   }*/
}
void strid_array::sort()
{
   single_lock sl(m_pmutex,TRUE);
   if(m_idptra.m_nSize <= 1)
      return;

   stackLowerBound.allocate(0,1024);
   stackUpperBound.allocate(0,1024);

   index iLowerBound;
   index iUpperBound;
   index iLPos,iUPos,iMPos;
   index i;
   stackLowerBound.push(0);
   stackUpperBound.push(m_idptra.m_nSize - 1);
   while(true)
   {
      iLowerBound = stackLowerBound.pop();
      iUpperBound = stackUpperBound.pop();
      iLPos = iLowerBound;
      iMPos = iLowerBound;
      iUPos = iUpperBound;
      while(true)
      {
         while(true)
         {
            if(iMPos == iUPos)
               break;
            if(m_idptra.get_data()[m_iaId.get_data()[iMPos]] <= m_idptra.get_data()[m_iaId.get_data()[iUPos]])
               iUPos--;
            else
            {
               i = m_iaId.get_data()[iMPos];
               m_iaId.get_data()[iMPos] = m_iaId.get_data()[iUPos];
               m_iaId.get_data()[iUPos] = i;
               break;
            }
         }
         if(iMPos == iUPos)
            break;
         iMPos = iUPos;
         while(true)
         {
            if(iMPos == iLPos)
               break;
            if(m_idptra.get_data()[m_iaId.get_data()[iLPos]] <= m_idptra.get_data()[m_iaId.get_data()[iMPos]])
               iLPos++;
            else
            {
               i = m_iaId.get_data()[iMPos];
               m_iaId.get_data()[iMPos] = m_iaId.get_data()[iUPos];
               m_iaId.get_data()[iUPos] = i;
               break;
            }
         }
         if(iMPos == iLPos)
            break;
         iMPos = iLPos;
      }
      if(iLowerBound < iMPos - 1)
      {
         stackLowerBound.push(iLowerBound);
         stackUpperBound.push(iMPos - 1);
      }
      if(iMPos + 1 < iUpperBound)
      {
         stackLowerBound.push(iMPos + 1);
         stackUpperBound.push(iUpperBound);
      }
      if(stackLowerBound.get_size() == 0)
         break;
   }


}



bool strid_array::find(const char * psz,index & iIndex) const
{
   single_lock sl(m_pmutex,TRUE);
   if(m_idptra.m_nSize == 0)
   {
      iIndex = 0;
      return false;
   }

   index iLowerBound = 0;
   index iMaxBound = m_idptra.m_nSize - 1;
   index iUpperBound = iMaxBound;
   index iCompare;
   // do binary search
   iIndex = (iUpperBound + iLowerBound) / 2;
   while(iUpperBound - iLowerBound >= 8)
   {
      iCompare = m_idptra.get_data()[m_iaId.get_data()[iIndex]] - psz;
      if(iCompare == 0)
      {
         return true;
      }
      else if(iCompare > 0)
      {
         iUpperBound = iIndex - 1;
         if(iUpperBound < 0)
         {
            iIndex = 0;
            break;
         }
      }
      else
      {
         iLowerBound = iIndex + 1;
         if(iLowerBound > iMaxBound)
         {
            iIndex = iMaxBound + 1;
            break;
         }
      }
      iIndex = (iUpperBound + iLowerBound) / 2;
   }
   // do sequential search
   while(iIndex < m_idptra.m_nSize)
   {
      iCompare = m_idptra.get_data()[m_iaId.get_data()[iIndex]] - psz;
      if(iCompare == 0)
         return true;
      else if(iCompare < 0)
         iIndex++;
      else
         break;
   }
   if(iIndex >= m_idptra.m_nSize)
      return false;
   while(iIndex >= 0)
   {
      iCompare = m_idptra.get_data()[m_iaId.get_data()[iIndex]] - psz;
      if(iCompare == 0)
         return true;
      else if(iCompare > 0)
         iIndex--;
      else
         break;
   }
   iIndex++;
   return false;

}


void strid_array::add(const id & id)
{

   index iIndex;

   if(find(id,iIndex))
      return;

   m_idptra.add((char * const &) id.m_psz);
   m_iaId.insert_at(iIndex,m_idptra.get_upper_bound());
   //m_iaId.add(m_idptra.get_upper_bound());
   //sort();

}

bool strid_array::find(const id & id,index & iIndex) const
{


   if(!find((const char *)id,iIndex))
      return false;

   iIndex = m_iaId[iIndex];

   return true;


}




::id_space & get_id_space()
{
   //if(::id_space::s_pidspace == NULL)
   //{
   //   ::id_space::s_pidspace = create_id_space();
   //}
   return *::id_space::s_pidspace;
}
