#include "FleetValidator.h"
#include <algorithm> // Поддержка "find"
#include <stack>

FleetValidator::FleetValidator(const ARRAY_TABLE& arrTbl)
 : m_arrTbl(arrTbl)
{   
}

void FleetValidator::PrepareMetaData()
{
	int n_prev_x = -1;
	VECT_INT vect_current;
	int index = -1;
    for (int row = 0; row < 10; ++row)
    {
	    for (int col = 0; col < 10; ++col)
        {
		    index = row * 10 + col;
			int n_out = m_arrTbl[index];
			if (n_out > 0)
			{
				if (-1 == n_prev_x)
				{
					vect_current.clear();
				}
				vect_current.push_back(index);				
				n_prev_x = index;				
			}
			else
			{
				if (-1 != n_prev_x)
				{
					if (!vect_current.empty())
					{
						BYTE b = static_cast<BYTE>(vect_current[0]);
						m_mapMetaData[b] = vect_current;
						vect_current.clear();
					}
				}
				n_prev_x = -1;				
			}
		}
		if (-1 != n_prev_x)
		{
			if (!vect_current.empty())
			{
				BYTE b = static_cast<BYTE>(vect_current[0]);
				m_mapMetaData[b] = vect_current;
				vect_current.clear();
			}
		}
		n_prev_x = -1;
	}
	PrepareYMetaData();
}

int FleetValidator::ProcessIndexForYMetaData(VECT_INT& vect, int n_inp_index)
{
	int n_base_index = -1;
	std::stack<int> st;
	for (int n_index = n_inp_index; n_index >= 0; n_index = n_index - 10)	
	{
		auto it = m_mapMetaData.find(n_index);
		if (it != m_mapMetaData.end())
		{
			auto pair = *it;
			VECT_INT v = pair.second;
			if (1 == v.size())
			{	
				st.push(v[0]);
				n_base_index = n_index;
			}
			else
				break;
		}
		else
			break;
	}
	while (!st.empty())
	{
		int val = st.top();
		st.pop();
		vect.push_back(val);
	}
	return n_base_index;
}

void FleetValidator::PrepareYMetaData()
{
	MAP_FLEET map;
	for (int row = 0; row < 10; ++row)	
	{
		for (int col = 0; col < 10; ++col)		
		{
			int index = row * 10 + col;
			VECT_INT vect;
			int n_baseIndex = ProcessIndexForYMetaData(vect, index);
			if (n_baseIndex > 0)
				map[n_baseIndex] = vect;
			else // if (n_baseIndex < 0)
				continue;			
		}
	}
	EraseExtraData(map);
	AddingNewData(map);
}

void FleetValidator::EraseExtraData(const MAP_FLEET& map)
{
	for (auto it = map.begin(); it != map.end(); it++)
	{
		auto pairErase = *it;
		VECT_INT vectErase = pairErase.second;
		for (const int valErase : vectErase)
		{
			auto itErase = m_mapMetaData.find(valErase);
			m_mapMetaData.erase(itErase);
		}
	}
}

void FleetValidator::AddingNewData(const MAP_FLEET& map)
{
	for (auto it = map.begin(); it != map.end(); it++)
	{
		auto pair = *it;
		VECT_INT vectAdd = pair.second;
		if (vectAdd.size() > 0)
		{
			BYTE bt_key = static_cast<BYTE>(vectAdd[0]);
			m_mapMetaData[bt_key] = vectAdd;
		}
	}
}

void FleetValidator::PrintVector(const VECT_INT& vect)
{
	for (const int var : vect)
		std::cout << var << " ";

	std::cout << '\n';
}

void FleetValidator::ShowFleet()
{
	MAP_FLEET map = m_mapMetaData;
	for (const auto pair : map)
	{
		std::cout << static_cast<int>(pair.first) << " :" << '\n';
		auto vect = pair.second;
		FleetValidator::PrintVector(vect);
	}
}

bool FleetValidator::Validating()
{
	int n_counter_4x_ships = 1; // Один 4-х палубный
	int n_counter_3x_ships = 2; // Два 3-х палубных
	int n_counter_2x_ships = 3; // Три 2-х палубных
	int n_counter_1x_ships = 4; // Четырк однопалубных

	MAP_FLEET mapExtra;
	for (auto it = m_mapMetaData.begin(); it != m_mapMetaData.end(); it++)
	{
		auto pair = *it;
		BYTE bt_key = pair.first;
		VECT_INT vect = pair.second;

		if (vect.size() > 4)
		{
			mapExtra[bt_key] = vect;
		}

		if (vect.size() == 4)
		{
			if (!n_counter_4x_ships)
				mapExtra[bt_key] = vect;

			if (n_counter_4x_ships)
				n_counter_4x_ships--;
		}

		if (vect.size() == 3)
		{
			if (!n_counter_3x_ships)
				mapExtra[bt_key] = vect;

			if (n_counter_3x_ships)
				n_counter_3x_ships--;
		}

		if (vect.size() == 2)
		{
			if (!n_counter_2x_ships)
				mapExtra[bt_key] = vect;

			if (n_counter_2x_ships)
				n_counter_2x_ships--;
		}

		if (vect.size() == 1)
		{
			if (!n_counter_1x_ships)
				mapExtra[bt_key] = vect;

			if (n_counter_1x_ships)
				n_counter_1x_ships--;
		}
	}

	if ((!n_counter_4x_ships) &&
		(!n_counter_3x_ships) &&
		(!n_counter_2x_ships) &&
		(!n_counter_1x_ships))
	{
		RemoveInvalidShips(mapExtra);
		return true;
	}
	return false;
}

MAP_FLEET FleetValidator::GetMetaData() const
{
	return m_mapMetaData;
}

void FleetValidator::RemoveInvalidShips(const MAP_FLEET& mapExtra)
{
	for (auto it = mapExtra.begin(); it != mapExtra.end(); it++)
	{
		auto pair = *it;
		BYTE b_key = pair.first;

		auto itRemove = m_mapMetaData.find(b_key);
		m_mapMetaData.erase(itRemove);
	}
}