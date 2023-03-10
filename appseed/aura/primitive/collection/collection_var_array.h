#pragma once




class CLASS_DECL_AURA var_array :
   virtual public array < var >
{
public:


   var_array(::aura::application * papp = NULL);
   var_array(const stringa & stra);
   var_array(const int_array & inta);
   var_array(const property_set & propset);
   var_array(const var_array & vara);
   virtual ~var_array();


   index add(var var);
   ::count add(const var_array & vara);
   ::count add_unique(const var_array & vara);

   string implode(const char * pszGlue) const;

   template < typename T, typename... Args >
   void add(const T & t,Args... args)
   {
      add((var) t);
      add(args...);
   }

   // for adding void
   void add(void)
   {
   }

   index find_first_ci(const char * psz, index find = 0, index last = -1) const;
   index find_first(const char * psz, index find = 0, index last = -1) const;
   index find_first(const var & var, index find = 0, index last = -1) const;

   bool contains_ci(const char * lpcsz, index find = 0, index last = -1, ::count countMin = 1, ::count countMax = -1) const;
   bool contains(const char * lpcsz, index find = 0, index last = -1, ::count countMin = 1, ::count countMax = -1) const;
   bool contains(const var & var, index find = 0, index last = -1, ::count countMin = 1, ::count countMax = -1) const;

   ::count remove_first_ci(const char * lpcsz, index find = 0, index last = -1);
   ::count remove_first(const char * lpcsz, index find = 0, index last = -1);
   ::count remove_first(const var & var, index find = 0, index last = -1);

   ::count remove_ci(const char * lpcsz, index find = 0, index last = -1, ::count countMin = 0, ::count countMax = -1);
   ::count remove(const char * lpcsz, index find = 0, index last = -1, ::count countMin = 0, ::count countMax = -1);
   ::count remove(const var & var, index find = 0, index last = -1, ::count countMin = 0, ::count countMax = -1);

   ::count remove(const var_array & vara);

   var_array & operator -=(var var);
   var_array & operator -=(var_array vara);
   var_array operator -(var var) const;
   var_array operator -(var_array vara) const;
   var_array & operator +=(var var);
   var_array & operator +=(var_array vara);
   var_array operator +(var var) const;
   var_array operator +(var_array vara) const;

   var_array & operator = (const stringa & stra);
   var_array & operator = (const int_array & inta);
   var_array & operator = (const property_set & propset);
   var_array & operator = (const var_array & vara);


   void parse_json(const char * & pszJson);
   void parse_json(const char * & pszJson, const char * pszEnd);

   void find_json_child(const char * & pszJson, const char * pszEnd, const var & var);

   static void skip_json(const char * & pszJson);
   static void skip_json(const char * & pszJson, const char * pszEnd);

   string & get_json(string & str, bool bNewLine = true) const;


   var_array array()
   {
      return var_array();
   }

   template < typename... VARS >
   var_array array(VARS&... vars)
   {

      var_array va;
      return va.array(vars...);

   }


   virtual void io(stream & serialize) override;


};






namespace lemon
{

   template < typename... VARS >
   var_array array_merge(VARS&... vars)
   {

      return var_array().array(vars...);


   }


} // namespace lemon
