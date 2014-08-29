#include "Imf.h"



//-----------------------------------------------------------------------------
//
//	class Channel
//	class ChannelList
//
//-----------------------------------------------------------------------------

//#include <ImfChannelList.h>
//#include <Iex.h>


//using std::string;
//using std::set;

namespace Imf {


Channel::Channel (PixelType t, int xs, int ys, bool pl):
    type (t),
    xSampling (xs),
    ySampling (ys),
    pLinear (pl)
{
    // empty
}


bool	
Channel::operator == (const Channel &other) const
{
    return type == other.type &&
	   xSampling == other.xSampling &&
	   ySampling == other.ySampling &&
	   pLinear == other.pLinear;
}


void	
ChannelList::insert (const char name[], const Channel &channel)
{
    if (name[0] == 0)
	THROW (Iex::ArgExc, "Image channel name cannot be an empty string.");

    _map[name] = channel;
}


void	
ChannelList::insert (const string &name, const Channel &channel)
{
    insert (name.c_str(), channel);
}


Channel &
ChannelList::operator [] (const char name[])
{
    ChannelMap::iterator i = _map.find (name);

    if (i == _map.end())
	THROW (Iex::ArgExc, "Cannot find image channel \"" << name << "\".");

    return i->m_element2;
}


const Channel &
ChannelList::operator [] (const char name[]) const
{
    ChannelMap::const_iterator i = _map.find (name);

    if (i == _map.end())
	THROW (Iex::ArgExc, "Cannot find image channel \"" << name << "\".");

    return i->m_element2;
}


Channel &
ChannelList::operator [] (const string &name)
{
    return this->operator[] (name.c_str());
}


const Channel &
ChannelList::operator [] (const string &name) const
{
    return this->operator[] (name.c_str());
}


Channel *
ChannelList::findChannel (const char name[])
{
    ChannelMap::iterator i = _map.find (name);
    return (i == _map.end())? 0: &i->m_element2;
}


const Channel *
ChannelList::findChannel (const char name[]) const
{
    ChannelMap::const_iterator i = _map.find (name);
    return (i == _map.end())? 0: &i->m_element2;
}


Channel *
ChannelList::findChannel (const string &name)
{
    return findChannel (name.c_str());
}


const Channel *
ChannelList::findChannel (const string &name) const
{
    return findChannel (name.c_str());
}


ChannelList::Iterator		
ChannelList::begin ()
{
    return _map.begin();
}


ChannelList::ConstIterator	
ChannelList::begin () const
{
    return _map.begin();
}


ChannelList::Iterator
ChannelList::end ()
{
    return _map.end();
}


ChannelList::ConstIterator	
ChannelList::end () const
{
    return _map.end();
}


ChannelList::Iterator
ChannelList::find (const char name[])
{
    return _map.find (name);
}


ChannelList::ConstIterator
ChannelList::find (const char name[]) const
{
    return _map.find (name);
}


ChannelList::Iterator
ChannelList::find (const string &name)
{
    return find (name.c_str());
}


ChannelList::ConstIterator
ChannelList::find (const string &name) const
{
    return find (name.c_str());
}


void
ChannelList::layers (set <string> &layerNames) const
{
    layerNames.clear();

    for (ConstIterator i = begin(); i != end(); ++i)
    {
	string layerName = i.name();
	size_t pos = layerName.rfind ('.');

	if (pos != string::npos && pos != 0 && pos + 1 < layerName.size())
	{
	    layerName.erase (pos);
	    layerNames.insert (layerName);
	}
    }
}


void
ChannelList::channelsInLayer (const string &layerName,
			      Iterator &m_element1,
			      Iterator &last)
{
    channelsWithPrefix (layerName + '.', m_element1, last);
}


void
ChannelList::channelsInLayer (const string &layerName,
			      ConstIterator &m_element1,
			      ConstIterator &last) const
{
    channelsWithPrefix (layerName + '.', m_element1, last);
}


void		
ChannelList::channelsWithPrefix (const char prefix[],
				 Iterator &m_element1,
				 Iterator &last)
{
    m_element1 = last = _map.lower_bound (prefix);
    int n = strlen (prefix);

    while (last != Iterator (_map.end()) &&
	   strncmp (last.name(), prefix, n) <= 0)
    {
	++last;
    }
}


void
ChannelList::channelsWithPrefix (const char prefix[],
				 ConstIterator &m_element1,
				 ConstIterator &last) const
{
    m_element1 = last = _map.lower_bound (prefix);
    int n = strlen (prefix);

    while (last != ConstIterator (_map.end()) &&
	   strncmp (last.name(), prefix, n) <= 0)
    {
	++last;
    }
}


void		
ChannelList::channelsWithPrefix (const string &prefix,
				 Iterator &m_element1,
				 Iterator &last)
{
    return channelsWithPrefix (prefix.c_str(), m_element1, last);
}


void
ChannelList::channelsWithPrefix (const string &prefix,
				 ConstIterator &m_element1,
				 ConstIterator &last) const
{
    return channelsWithPrefix (prefix.c_str(), m_element1, last);
}


bool		
ChannelList::operator == (const ChannelList &other) const
{
    ConstIterator i = begin();
    ConstIterator j = other.begin();

    while (i != end() && j != other.end())
    {
	if (!(i.channel() == j.channel()))
	    return false;

	++i;
	++j;
    }

    return i == end() && j == other.end();
}


} // namespace Imf
