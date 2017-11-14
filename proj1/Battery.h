//Battery.h
#ifndef BATTERY_INCLUDED
#define BATTERY_INCLUDED

class Battery
{
public:
	Battery(int initialLevel);
	int level() const;
	void decrease();
private:
	int m_currentlevel;
};
#endif //BATTERY_INCLUDED