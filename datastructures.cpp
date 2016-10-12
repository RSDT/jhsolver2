//
// Created by mattijn on 11-10-16.
//

#include "datastructures.h"
#include <fstream>
#include <math.h>
#include <sstream>
/** \brief
 *
 * \param ca[] char
 * \param c char
 * \return int
 *
 */
int find(char ca[],char c)
{
    int i = 0;
    while (ca[i] != c && i<20)  i++;
    if (i == 20) return -1;
    else return i;
}

/** \brief
 *
 * \param ca[] char
 * \return double
 *
 */
double char_to_double(char ca[])
{
    double f = 0.0;
    int dot =  find (ca,'.');
    double d = 1.0 ;
    for (int i = 1; i < dot; i++)
        d *= 10 ;
    bool done = false;
    for (int i = 0; i < 20; i++)
    {
        if (ca[i] == '\0') done = true;
        if (!done)
        {
            if (ca[i] != '.')
            {
                f += static_cast<double>(ca[i] - '0') * d;
                d /= 10.0;
            }
        }
    }
//    cout << endl;
    return f;
}

/** \brief
*
* \param s ifstream&
* \param coords wsg&
* \return bool
*
*/
bool operator>>(std::ifstream& s, data::wsg& coords)
{

    char coord[20];
    int d = 0; // dimensie
    bool nendofl = true;
    while (d<3)
    {
        bool done = false;
        char c;
        int i = 0;
        while (!done)
        {
            s.get(c);
            coord[i] = c;
            coord[++i] = '\0';
            if (c == ',' ||c == ' '||c=='\n')
            {
                d++;
                coord[i-1] = '\0';
                done = true;
                if (c == '\n') nendofl = false;
            }
        }
        if (d== 1)
            coords.la = char_to_double(coord);
        if (d== 2)
            coords.lo = char_to_double(coord);
        if (d== 3)
            coords.al = char_to_double(coord);
    }
    //cout << endl << endofl <<endl;
    return nendofl;
}