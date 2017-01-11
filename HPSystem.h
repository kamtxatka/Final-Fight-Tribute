#ifndef __HP_SYSTEM__
#define __HP_SYSTEM__

class HPSystem
{
public:
	int maxHP, currentHP = 0;
	bool dmged, dead = false;


public:

	HPSystem(int maxHP): maxHP(maxHP)
	{}

	void Start()
	{
		currentHP = maxHP;
		dead = false;
	}

	void Respawn()
	{
		Start();
	}

	bool TakeDamage(int amount)
	{
		currentHP -= amount;
		if (currentHP <= 0)
		{
			dead = true;
		}

		return dead;
	}


};










#endif