﻿#pragma once


namespace lemon
{


   template < class TDST, class TSRC >
   ::count copy(spa(TDST) & dsta, const spa(TSRC) & srca);


   template < class A, class T >
   A make_array(T t1)
   {
      A a;
      a.add(t1);
      return a;
   }

   namespace ptra
   {

      template < class TDST,class TSRC >
      ::count copy(pointer_array < TDST > & dsta, const pointer_array < TSRC > & srca);

      template < class TDST, class TSRC >
      ::count ptrcopy(TDST & dsta, const TSRC & srca);

   }

   namespace array
   {


      template < class TDST,class TSRC >
      ::count copy(TDST & dsta,const TSRC & srca);


      template < class A >
      void randomize(A & a)
      {
         A b;
         while(a.get_count() > 0)
         {
            index i = (index) (rand() % a.get_size());
            b.add(a.element_at(i));
            a.remove_at(i);
         }
         a = b;
      }

      template < class A >
      typename A::BASE_TYPE pick_random(A & a)
      {
         index i = (index)(rand() % a.get_size());
         return a.element_at(i);
      }

      template < class A, class TYPE >
      ::count populate(A & a, TYPE start, TYPE end, TYPE step, bool bIncludeStart = true, bool bIncludeEnd = true, bool bIncludeEndOnStep = true)
      {
         ::count count = 0;
         TYPE cur = start;
         if(bIncludeStart)
         {
            if(start == end)
            {
               if(bIncludeEnd)
               {
                  a.add(cur);
                  return 1;
               }
               else
                  return 0;
            }
            else
            {
               a.add(cur);
               count++;
            }
         }
         if(end > start)
         {
            if(step == 0)
               return count;
            if(step < 0)
               step = -step;
            while(true)
            {
               cur = cur + step;
               if(cur >= end)
                  break;
               a.add(cur);
               count++;
            }
         }
         else
         {
            if(step == 0)
               return count;
            if(step < 0)
               step = -step;
            while(true)
            {
               cur = cur - step;
               if(cur <= end)
                  break;
               a.add(cur);
               count++;
            }
         }
         if(bIncludeEnd)
         {
            if(bIncludeEndOnStep)
            {
               if(cur == end)
               {
                  a.add(cur);
                  return count + 1;
               }
               else
               {
                  return count;
               }
            }
            else
            {
               a.add(cur);
               return count + 1;
            }
         }
         else
         {
            return count;
         }
      }


      template < typename ARRAY >
      index find_first(const ARRAY & a, typename ARRAY::BASE_ARG_TYPE t,index(* lpfnCompare)(typename ARRAY::BASE_ARG_TYPE,typename ARRAY::BASE_ARG_TYPE),index find = 0,index last = -1)
      {
         if(find < 0)
            find += a.get_count();
         if(last < 0)
            last += a.get_count();
         for(; find <= last; find++)
         {
            if(lpfnCompare((typename ARRAY::BASE_ARG_TYPE)a.element_at(find),(typename ARRAY::BASE_ARG_TYPE)t) == 0)
               return find;
         }
         return -1;
      }


      template <class ARRAY >
      inline typename ARRAY::BASE_TYPE pop(ARRAY & a, index n)
      {

         index i = a.get_upper_bound(n);

         typename ARRAY::TYPE t = a.element_at(i);

         a.remove_at(i);

         return t;

      }


      template <class ARRAY >
      inline void pop_back(ARRAY & a,index n)
      {

         a.remove_at(a.get_upper_bound(n));

      }


      template <class ARRAY >
      inline index push(ARRAY & a,typename ARRAY::BASE_ARG_TYPE newElement,index n)
      {

         return a.insert_at(a.get_upper_bound(n),newElement);

      }


      template <class ARRAY >
      inline void push_back(ARRAY & a,typename ARRAY::BASE_ARG_TYPE newElement,index n)
      {

         a.insert_at(a.get_upper_bound(n),newElement);

      }

      template < typename T >
      inline bool are_all_elements_equal(const spa(T) & a1, const spa(T) & a2)
      {

         if (a1.get_count() != a2.get_count())
         {

            return false;

         }

         for (int i = 0; i < a1.get_count(); i++)
         {

            if (a1.at(i) != a2.at(i))
            {

               return false;

            }

         }

         return true;

      }



      namespace every
      {

         template < typename A >
         bool ci_is(const A & a, const typename A::BASE_TYPE & t)
         {

            for (auto & i : a)
            {

               if (::stricmp(i, t))
               {

                  return false;

               }

            }

            return true;

         }

         template < typename A >
         bool is_true(const A & a)
         {

            for (auto & i : a)
            {

               if (!(bool) i)
               {

                  return false;

               }

            }

            return true;

         }

      }


   } // namespace array



   template < class A >
   void array_merge(A & a, const A & a1, const A & a2)
   {
      a = a1 + a2;
   }

   template < class A >
   void array_makecombination(smart_pointer_array < A > & comb, const A & a, A blindprefixa = A(), index idx = 0)
   {
      comb.add(canew(A(blindprefixa)));
      for(::index i = idx; i < a.get_count(); i++)
      {
         array_makecombination(comb, a, blindprefixa + make_array < A > (a[i]), i + 1);
      }
   }

   /*
   function array_arrange($a)
   {
   $comb = array_makecombination($a);
   $ret = array();
   $count = count($comb);
   foreach($comb as $ai)
   {
      if(count($ai) > 0)
   	   $ret = array_merge($ret, array_permute($ai));
   }
   return $ret;
   }*/

   template < class A, class A2D >
   void array_permute(A2D & perm,const A & a);

   template < class A >
   void array_permute(smart_pointer_array < A > & perm,const A & a);

   CLASS_DECL_AURA stringa x1_decode_ida(const char * psz);

   CLASS_DECL_AURA string x1_encode_ida(const stringa & stra);


   template < class A >
   void x1_on_remove(const A & a, typename A::BASE_TYPE & n)
   {
      ::count c = 0;
      for (index i = 0; i < a.get_size(); i++)
      {
         if (a[i] < n)
         {
            c++;
         }
      }
      n -= c;
   }

   /*	function array_remove_by_keys($a, $keys, $bStrict = false)
   {
   	$newa = array();
   	foreach($a as $key=>$value)
   	{
   		if(!in_array($key, $keys, $bStrict))
   		{
   			$newa[$key] = $value;
   		}
   	}
   	return $newa;
   }

   function array_is_equal(&$a1, &$a2)
   {
   	if(!is_array($a1) || !is_array($a2))
   		return false;
   	return array_contains_array($a1, $a2) && array_contains_array($a2, $a1);
   }

   function array_contains_array(&$a1, &$a2)
   {
   	foreach($a2 as $key=>$value)
   	{
   		if(!array_key_exists($key, $a1))
   			return false;
   		if(!gen_is_equal($a1[$key], $value))
   			return false;
   	}
   	return true;
   }

   function array_randomize($a)
   {
   	$count = count($a);
   	$newa = array();
   	while($count > 0)
   	{
   		$i = mt_rand(0, $count - 1);
   		$item = array_splice($a, $i, 1);
   		$newa[] = $item[0];
   		$count--;
   	}
   	return $newa;
   }

   function array_pick_random_item($a)
   {
   	return $a[mt_rand(0, count($a) - 1)];
   }

   function array_remove($item, $a)
   {
   	$reta = array();
   	foreach($a as $key=>$value)
   	{
   		if($value != $item)
   		{
   			$reta[$key] = $value;
   		}
   	}
   	return $reta;
   }
   */


}   // namespace lemon



