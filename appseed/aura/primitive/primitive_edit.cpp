//#include "framework.h"


base_edit::base_edit(::aura::application * papp) :
   object(papp)
{
}

base_edit::~base_edit()
{
}

void base_edit::apply()
{

   m_spdata->edit(this);
   on_apply();

}

void base_edit::on_apply()
{
}

