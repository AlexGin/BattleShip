#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <array>

using BYTE = unsigned char;
using VECT_INT = std::vector<int>;
using MAP_FLEET = std::map<BYTE, std::vector<int>>;
using ARRAY_TABLE = std::array<BYTE, 100>;

class FleetValidator
{
public:
    FleetValidator(const ARRAY_TABLE& arrTbl);    
	void PrepareMetaData();
    void ShowFleet();
    bool Validating();
    MAP_FLEET GetMetaData() const;
private:    
    void PrepareYMetaData();
    void RemoveInvalidShips(const MAP_FLEET& mapExtra);
    int ProcessIndexForYMetaData(VECT_INT& vect, int n_inp_index);
    void EraseExtraData(const MAP_FLEET& map);
    void AddingNewData(const MAP_FLEET& map);
private:
    ARRAY_TABLE  m_arrTbl;
    MAP_FLEET m_mapMetaData;   
public:
    static void PrintVector(const VECT_INT& vect);
};