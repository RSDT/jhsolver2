//
// Created by mattijn on 11-10-16.
//

#ifndef JHSOLVER2_KML_HANDLING_H
#define JHSOLVER2_KML_HANDLING_H

#include <iostream>
#include <vector>
#include <fstream>
#include "datastructures.h"
#include "./rapidxml-1.13/rapidxml.hpp"
#include "./rapidxml-1.13/rapidxml_utils.hpp"

namespace kml_h{
    void  create_homebase_file( std::string hqfilename, std::string kmlfilename);

    std::vector<data::Homebase> read_hombases(std::ifstream& ifstream);

    void create_homebase_file(std::string filename, rapidxml::xml_node<> *folder);

    std::vector<std::vector<data::Homebase>> sort_hb(std::vector<data::Homebase> HQs);

    std::vector<data::Deelgebied>
    create_deelgebieden(std::vector<std::vector<data::RD>> &dgngrens, std::vector<data::Homebase> &HQs);

    std::vector<std::vector<data::RD>> read_deelgebieden_rd(std::ifstream &f);
}

#endif //JHSOLVER2_KML_HANDLING_H
