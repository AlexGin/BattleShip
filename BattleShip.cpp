// BattleShip.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// #define DIAGNOST
#include <iostream>
#include <vector>
#include <array>
#include <algorithm> // Поддержка find
#include "PRNG.h"
#include "FleetValidator.h"

#pragma warning (disable : 4129)

using BYTE = unsigned char;
using ARRAY_TABLE = std::array<BYTE, 100>;
using VECT_INT = std::vector<int>;

PRNG g_gen;

enum class VALID_CORNER_MATRIX
{
    UPPER_LEFT = -11,
    UPPER_RIGHT = -9,
    LOWER_LEFT = 9,
    LOWER_RIGHT = 11
};

int GenerateRandom()
{    
    return g_gen.getRandom();
}

void ClearTable(ARRAY_TABLE& a)
{
    for (int i = 0; i < 100; ++i)
        a[i] = 0;
}

void PrintTable(const ARRAY_TABLE& arrTbl)
{
    for (int i = 0; i < 12; ++i)
    {
        if ((!i) || (11 == i))
        {            
            std::cout << "_0123456789_" << '\n';            
        }
        else
        {
            for (int j = 0; j < 12; ++j)
            {
                if (!j)
                {
                    std::cout << static_cast<char>(0x40 + i);
                }
                else if ((j > 0) && (j < 11))
                {
                    int row = i - 1;
                    int col = j - 1;
                    int index = row * 10 + col;
                    int n_out = arrTbl[index];
                    if (!n_out)
                        std::cout << '\ ';
                    else
                        std::cout << '\Z';
                }
                else if (11 == j)
                {
                    std::cout << static_cast<char>(0x40 + i) << '\n';
                }
            }
        }
    }
}

bool IsValid(const VECT_INT& vect, int n_val_to_add, VALID_CORNER_MATRIX corner)
{
    int value = n_val_to_add + static_cast<int>(corner);
    auto it = find(vect.begin(), vect.end(), value);
    if (it != vect.end()) // Число найдено - валидность нарушена
        return false;

    return true;
}

bool PreValidation(const VECT_INT& vect, int n_val_to_add)
{    
    bool valid_UL = IsValid(vect, n_val_to_add, VALID_CORNER_MATRIX::UPPER_LEFT);
    if (!valid_UL) // Число найдено
        return false;

    bool valid_UR = IsValid(vect, n_val_to_add, VALID_CORNER_MATRIX::UPPER_RIGHT);
    if (!valid_UR) // Число найдено
        return false;

    bool valid_LL = IsValid(vect, n_val_to_add, VALID_CORNER_MATRIX::LOWER_LEFT);
    if (!valid_LL) // Число найдено
        return false;

    bool valid_LR = IsValid(vect, n_val_to_add, VALID_CORNER_MATRIX::LOWER_RIGHT);
    if (!valid_LR) // Число найдено
        return false;

    return true;
}

void PrepareVectPref(VECT_INT& vect_pref)
{
    for (int i = 0; i < 10; ++i)
        vect_pref.push_back(i);

    for (int j = 10; j < 90; j = j + 10)
        vect_pref.push_back(j);

    for (int k = 19; k < 90; k = k + 10)
        vect_pref.push_back(k);

    for (int l = 90; l < 100; ++l)
        vect_pref.push_back(l);
}

bool Prefetcher(const VECT_INT& vect_pref, int n_val)
{
    auto it = find(vect_pref.begin(), vect_pref.end(), n_val);
    if (it == vect_pref.end()) // Число не найдено - запрет
        return false;

    return true; // Предвыборка - OK
}

int main()
{
    bool b_valid = false;
    ARRAY_TABLE a;
    do
    {
        ClearTable(a);
        VECT_INT vect; // , vect_pref;
        // PrepareVectPref(vect_pref);

        for (int i = 0; i < 30 /*260*/; i++)
        {
            int val = GenerateRandom();
            // int n_5 = val % 4;
            // bool prefetched = Prefetcher(vect_pref, val);
            if(1) // ((0 == n_5) || prefetched)
            {
                auto it = find(vect.begin(), vect.end(), val);
                bool is_exist = it != vect.end();
                int n_val_to_add = is_exist ? n_val_to_add = GenerateRandom() : val;
                bool is_valid = PreValidation(vect, n_val_to_add);
                if (!is_valid)
                    continue;

                vect.push_back(n_val_to_add);
                a[n_val_to_add] = 1;
            }
        }
#ifdef DIAGNOST
        PrintTable(a);
#endif
        FleetValidator fv(a);
        fv.PrepareMetaData();
#ifdef DIAGNOST
        fv.ShowFleet();
#endif
        b_valid = fv.Validating();
        if (b_valid)
        {
            ClearTable(a);
            MAP_FLEET mflt = fv.GetMetaData();
            for (const auto pair : mflt)
            {
                VECT_INT vect = pair.second;
                for (const int index : vect)
                    a[index] = 1;
            }
            PrintTable(a);
        }
        // FleetValidator::PrintVector(vect);
    } while (!b_valid);

    std::cout << "Нажмите любую клавишу для выхода" << std::endl;
    std::cin.seekg(0, std::ios::end);
    std::cin.clear();
    ::getchar();
}