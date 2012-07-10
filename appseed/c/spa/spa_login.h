#pragma once


class CLASS_DECL_c spa_login :
   public simple_ui
{

public:


   enum e_result
   {
      result_ok,
      result_wrong_password,
   };


   simple_label         m_labelUser;
   simple_edit_box      m_editUser;
   simple_label         m_labelPassword;
   simple_password      m_password;
   simple_tap           m_tap;

   vsstring             m_strPasshash;
   
   spa_login(simple_ui * puiParent);
   virtual ~spa_login();


   virtual void on_action(const char * pszId);


   void start_login();

   static DWORD WINAPI thread_proc_login(LPVOID lpParam);

   e_result login();


   virtual void login_result(e_result eresult);




};