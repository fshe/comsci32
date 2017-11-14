#include "Battery.h"

Battery::Battery(int initialLevel)
	:m_currentlevel(initialLevel)
{}

void Battery::decrease()
{
	if(m_currentlevel>0)
		m_currentlevel--;
}

int Battery::level() const
{
	return m_currentlevel;
}