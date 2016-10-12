//
// Created by mattijn on 11-10-16.
//

#ifndef JHSOLVER2_DATASTRUCTURES_H
#define JHSOLVER2_DATASTRUCTURES_H
#include <iostream>
#include <vector>

namespace data {

    enum DeelGebied /**< \brief deelgebied kan in ABCDEF gebruikt worden als index \example voorbeeld ABCDEF[Alpha] */
    {
        None = -1,
        Alpha,
        Bravo,
        Charlie,
        Delta,
        Echo,
        Foxtrot
    };

    struct RD /**< \brief een punt volgens het RD stelsel */
    {
        double x; /**< \brief het x coordinaat van het punt */
        double y; /**< \brief het y coordinaat van het punt */
        double nap; /**< \brief de hoogte van het punt maar wordt niet gebruikt */
        std::string name; /**< \brief indien nodig kan er een naam gegeven worden aan het punt */
        data::DeelGebied deelgebied; /**< \brief indien nodig kan het deelgebied gespecificeerd worden */
        int index;
    };

    struct Homebase /**< \brief presenteerd een homebase van een groep die meedoet aan de jotihunt */
    {
        std::string naam; /**< \brief de naam van de groep */
        std::string website; /**< \brief de website van de groep */
        std::string adres; /**< \brief het adres van de groep */
        std::string plaats; /**< \brief de plaats waar de homebase zit */
        RD locatie; /**< \brief de locatie van de homebase */
        bool tegenhunt; /**< \brief true als de vossen hier al langs zijn geweest */
        data::DeelGebied deelgebied; /**< \brief het deelgebied waar de homebase zich in bevindt */
    };

    struct Deelgebied /**<  \brief presenteerd een deelgebied */
    {
        std::string naam; /**< \brief de naam van het deelgebied */
        data::DeelGebied deelgebied; /**< \brief  het deelgebied waarin het deelgebied zich in bevindt (huh?!?)*/
        std::vector<RD> Grens; /**< \brief de grens van het deelgebied */
        std::vector<RD> Route; /**< \brief het pad dat de vossen gelopen hebben */
        std::vector<Homebase> HQs; /**< \brief de homebases die zich bevinden in het deelgebied */
        RD linksonder; /**< \brief defineert samen met rechtboven een rechthoek waarin het deelgebied zich bevindt.  er wordt een foutmarge toegevoegd. */
        RD rechtsboven; /**< \brief defineert samen met rechtboven een rechthoek waarin het deelgebied zich bevindt. er wordt een foutmarge toegevoegd. */
        bool online; /**< \brief true als de vossen in het deelgebied op online staan */
    };

    struct wsg /**< \brief een punt volgens het wsg stelsel in graden*/
    {
        double lo;/**< \brief longitude in graden */
        double la;/**< \brief latitude in graden */
        double al;/**< \brief altidude in meters */
        std::string name; /**< \brief indien nodig kan er een naam gegeven worden aan het punt */
        DeelGebied deelgebied; /**< \brief indien nodig kan het deelgebied gespecificeerd worden */
    };
}
bool operator>>(std::ifstream& s, data::wsg& coords);
#endif //JHSOLVER2_DATASTRUCTURES_H
