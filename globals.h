//
// Created by mattijn on 12-10-16.
//

#ifndef JHSOLVER2_GLOBALS_H
#define JHSOLVER2_GLOBALS_H
#include "datastructures.h"
namespace global {
    const int NDG =  6; /**< het aantal deelgebieden */
    data::Deelgebied ABCDEF[NDG]; /**< de database waar alle info over deelgebieden instaat. A = [0] b =[1] etc. dit is te combineren met een enum DeelGebieden */

}
#endif //JHSOLVER2_GLOBALS_H
