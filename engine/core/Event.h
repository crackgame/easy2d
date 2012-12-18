#ifndef _EVENT_H_
#define _EVENT_H_
#include <string>
using namespace std;

namespace easy2d {

class Event
{
public:
	Event(const char* type);
	virtual ~Event();
		
protected:
	string mType;

private:
};

}	// namespace

#endif