//
// Created by mattijn on 11-10-16.
//

#include "kml_handling.h"
#include "./rapidxml-1.13/rapidxml.hpp"
#include "./rapidxml-1.13/rapidxml_utils.hpp"
#include "rd/rdnaptrans2008.2.cpp" // voor converteren van wsg naar rd en andersom
#include <algorithm>
#include <vector>
#include "global.h"
void remove_dubl_endl(std::string& str);
void readKML(rapidxml::xml_document<>& doc,std::string filename);
rapidxml::xml_node<> *get_groepen(rapidxml::xml_document<>& doc);
void  tags_to_tabs(std::string& str);
data::RD wsgtord(data::wsg ws);
data::Deelgebied createDeelgebied(string naam, std::vector<data::RD>& Grens, std::vector<data::Homebase>& HQs);
int find_lowest_x(std::vector<data::RD>& db);
int find_lowest_y(std::vector<data::RD>& db);
int find_higest_x(std::vector<data::RD>& db);
int find_highest_y(std::vector<data::RD>& db);
std::vector<data::RD> read_deelgebied_rd(ifstream& f);
/** \brief
 *
 * \param vector< vector< RD > >& dgngrens
 * \param HQs vector< Homebase>&
 * \return vector<Deelgebied>
 *
 */
std::vector<data::Deelgebied> kml_h::create_deelgebieden(std::vector< std::vector< data::RD > >& dgngrens, std::vector< data::Homebase >& HQs)
{
    std::vector<data::Deelgebied> dgn;
    for (unsigned int i = 0; i  < dgngrens.size(); i++)
        dgn.push_back(createDeelgebied(dgngrens[i][0].name,dgngrens[i],HQs));
    return dgn;
}


/** \brief
 *
 * \param HQs vector<Homebase>
 * \return vector <vector < Homebase>>
 *
 */
std::vector <std::vector < data::Homebase> > kml_h::sort_hb(std::vector<data::Homebase> HQs)
{
    std::vector <std::vector <data::Homebase> > db;
    vector <data::Homebase> A;
    vector <data::Homebase> B;
    vector <data::Homebase> C;
    vector <data::Homebase> D;
    vector <data::Homebase> E;
    vector <data::Homebase> F;
    for (unsigned int i = 0; i < HQs.size(); i++)
    {
        cout <<i << '\t'<<HQs[i].deelgebied << endl;
        if (HQs[i].deelgebied == data::Alpha) A.push_back(HQs[i]);
        if (HQs[i].deelgebied == data::Bravo) B.push_back(HQs[i]);
        if (HQs[i].deelgebied == data::Charlie) C.push_back(HQs[i]);
        if (HQs[i].deelgebied == data::Delta) D.push_back(HQs[i]);
        if (HQs[i].deelgebied == data::Echo) E.push_back(HQs[i]);
        if (HQs[i].deelgebied == data::Foxtrot) F.push_back(HQs[i]);
    }
    db.push_back(A);
    db.push_back(B);
    db.push_back(C);
    db.push_back(D);
    db.push_back(E);
    db.push_back(F);
    for (int i = 0; i < NDG;i++)
        ABCDEF[i].HQs=db[i];
    return db;
}
/** \brief
 *
 * \param f ifstream&
 * \return vector< vector< RD >>
 *
 */
std::vector< std::vector< data::RD > > kml_h::read_deelgebieden_rd(ifstream& f)
{
    std::vector<std::vector< data::RD > > dgn;
    for (int i = 0; i  < 6; i++) dgn.push_back(read_deelgebied_rd(f));
    return dgn;
}
void remove_dubl_endl(std::string& str)
{// str shouldd not contain tabs the will be removed from the string and thrown in oblivion for noone to find!!
    for (int i = 1; i<str.size();i++)
    {
        if (str[i] == '\n' && str[i-1]=='\n')
            str[i] = '	';
    }
    str.erase (std::remove(str.begin(), str.end(), '	'), str.end());
}

void readKML(rapidxml::xml_document<>& doc,std::string filename)
{
    rapidxml::file<> data (filename.c_str());
    doc.parse<0>(data.data());    // 0 means default parse flags
}

rapidxml::xml_node<> *get_groepen(rapidxml::xml_document<>& doc)
{
    rapidxml::xml_node<> *node = doc.first_node();
    rapidxml::xml_node<> *node2 = node->first_node();
    rapidxml::xml_node<> *node3 = node2->first_node();
    rapidxml::xml_node<> *node4 = node3->next_sibling();
    rapidxml::xml_node<> *node5 = node4->next_sibling();
    return node5;
    /*rapidxml::xml_node<> *n = doc.first_node();
    rapidxml::xml_node<> *folder = n->first_node("Document");

    for (rapidxml::xml_node<> *placemark = folder->first_node("Folder"); placemark; placemark = placemark->next_sibling("Folder"))
        if ((placemark->first_node("name"))->value() == "Groepen") return placemark;
    return NULL;*/
}

void kml_h::create_homebase_file(std::string filename,std::string kmlfilename)
{
    rapidxml::xml_document<> doc;
    readKML(doc,kmlfilename);
    rapidxml::xml_node<> *folder  = get_groepen(doc);
    std::cout << folder->name() << '\n';
    //wait("86");
    create_homebase_file(filename, folder);
}

/** \brief
 *
 * \param f ifstream&
 * \return vector<wsg>
 *
 */
std::vector<data::wsg> read_deelgebied_wsg(std::ifstream& f)
{
    std::string name;
    f >> name;
    data::wsg c;
    if (name == "Alpha") c.deelgebied = data::Alpha;
    if (name == "Bra         vo") c.deelgebied = data::Bravo;
    if (name == "Charlie") c.deelgebied = data::Charlie;
    if (name == "Delta") c.deelgebied = data::Delta;
    if (name == "Echo") c.deelgebied = data::Echo;
    if (name == "Foxtrot") c.deelgebied = data::Foxtrot;
    char ch;
    f.get(ch);
    std::vector<data::wsg> cs;
    while (f >> c)
    {
        c.name = name;
        cs.push_back(c);
    }
    c.name = name;
    cs.push_back(c);
    return cs;
}
void kml_h::create_homebase_file(std::string filename, rapidxml::xml_node<> *folder)
{
    std::ofstream out (filename.c_str());
    for (rapidxml::xml_node<> *placemark = folder->first_node("Placemark"); placemark; placemark = placemark->next_sibling("Placemark"))
    {
        rapidxml::xml_node<> *temp = placemark->first_node("name");
        rapidxml::xml_node<> *name  = temp->first_node();
        temp = placemark->first_node("ExtendedData");
        rapidxml::xml_node<> *extendeddata  = temp->first_node();
        temp = placemark->first_node("Point");
        rapidxml::xml_node<> *location  = temp->first_node();
        std::string naam =  name->value();
        std::string coordinaten = location->value();
        std::string ExtendedData = extendeddata->value();
        tags_to_tabs(ExtendedData);
        ExtendedData.erase (std::remove(ExtendedData.begin(), ExtendedData.end(),'	'), ExtendedData.end());
        remove_dubl_endl( ExtendedData);
        out <<  naam <<'\n'<<coordinaten <<  ExtendedData << '\n';
    }
    out.close();
}

void  tags_to_tabs(std::string& str)
{
    bool del = false;
    for (int i = 0; i<str.size();i++)
    {
        if (str[i]=='<') del = true;
        if (del)
        {
            if (str[i]=='>') del = false;
            str[i] = '	';
        }
    }
}

data::RD wsgtord(data::wsg ws)
{
    data::RD rd;
    int error  = etrs2rdnap(ws.lo , ws.la, ws.al, rd.x,rd.y,rd.nap);
    if (error != 0)
    {
        std::cout << error  << std::endl;
        std::cout << "name " << ws.name << std::endl;
        std::cout << "lo " << ws.lo << std::endl;
        std::cout << "la " << ws.la << std::endl;
        std::cout << "al " << ws.al << std::endl;
    }

    rd.name = ws.name;
    rd.deelgebied = ws.deelgebied;
    if (rd.name == "Alpha" || rd.name == "Bravo")
        return rd;
    return rd;
}


/** \brief
 *
 * \param f ifstream&
 * \return vector<Homebase>
 *
 */
std::vector<data::Homebase> kml_h::read_hombases(std::ifstream& f) {
    /* vb
    Rhedense Pioniers
    6.027932,52.00621,0.0
    Rheden
    http://www.rhedensepioniers.nl
    Arnhemseweg 30
    6991 DP Rheden
    B

    */

    std::string website;
    std::string adres1;
    std::string adres2;
    std::string plaats;
    std::string dg;
    std::string name;
    data::RD locatie;
    data::DeelGebied deelgebied;
    data::wsg c;
    data::Homebase hb;
    std::vector<data::Homebase> db;
    char chara;
    int i = 0;
    f.get(chara);
    while (f) {
        std::string str;
        getline(f, str);
        name = " " + str;
        name[0] = chara;
        f >> c;
        c.name = name;
        getline(f, plaats);
        getline(f, website);
        getline(f, adres1);
        getline(f, adres2);
        getline(f, dg);
        locatie = wsgtord(c);
        cout << dg[0] << endl;
        if (dg[0] == 'A') deelgebied = data::Alpha;
        if (dg[0] == 'B') deelgebied = data::Bravo;
        if (dg[0] == 'C') deelgebied = data::Charlie;
        if (dg[0] == 'D') deelgebied = data::Delta;
        if (dg[0] == 'E') deelgebied = data::Echo;
        if (dg[0] == 'F') deelgebied = data::Foxtrot;
        locatie.deelgebied = deelgebied;
        hb.locatie = locatie;
        hb.naam = name;
        hb.website = website;
        hb.adres = adres1 + ' ' + adres2;
        hb.plaats = plaats;
        hb.deelgebied = deelgebied;
        hb.tegenhunt = false;
        db.push_back(hb);
        f.get(chara);
        f.get(chara);
        i++;
        cout << i << '\t' << hb.naam << endl;
    }
    f.close();
    return db;
}

/** \brief
 *
 * \param f ifstream&
 * \return vector<RD>
 *
 */
std::vector<data::RD> read_deelgebied_rd(ifstream& f)
{
    string name;
    f >> name;
    char ch;
    f.get(ch);
    data::wsg c;
    std::vector<data::RD> cs;
    while (f >> c)
    {
        c.name = name;
        cs.push_back(wsgtord(c));
    }
    c.name = name;
    cs.push_back(wsgtord(c));
    return cs;
}
/** \brief maakt een deelgebied aan en zet het deelgebied op de juiste plek in ABCDEF
 *
 * \param naam string de naam van het deelgebied
 * \param Grens vector<RD>& de grens van het deelgebied. dit is gegeven in het kml bestand.
 * \param HQs vector<Homebase>& een lijst met alle hombases die meedoen tijdens de jotihunt. dit is gegeven in het kml bestand.
 * \return Deelgebied het deelgebied dat gecreerd is. en op de juiste locatie in ABCDEF is geplaats.
 *
 */
data::Deelgebied createDeelgebied(string naam, std::vector<data::RD>& Grens, std::vector<data::Homebase>& HQs)
{
    double foutmarge = 100.0;
    data::Deelgebied dg;
    dg.naam = naam;
    if (naam == "Alpha") dg.deelgebied = data::Alpha;
    if (naam == "Bravo") dg.deelgebied = data::Bravo;
    if (naam == "Charlie") dg.deelgebied = data::Charlie;
    if (naam == "Delta") dg.deelgebied = data::Delta;
    if (naam == "Echo") dg.deelgebied = data::Echo;
    if (naam == "Foxtrot") dg.deelgebied = data::Foxtrot;
    dg.Grens = Grens;
    for (unsigned int i = 0; i < HQs.size();i++) /**< zoekt welke homebases in het deelgebied zitten */
        if (HQs[i].deelgebied == dg.deelgebied)
            dg.HQs.push_back(HQs[i]);
    dg.online = true;
    dg.linksonder.name = "linksonder";
    dg.rechtsboven.name = "rechtsboven";
    dg.linksonder.nap = 0.0;
    dg.rechtsboven.nap = 0.0;
    int index = find_lowest_x(Grens);
    dg.linksonder.x = Grens [index].x - foutmarge;
    index = find_lowest_y(Grens);
    dg.linksonder.y = Grens [index].y - foutmarge;
    index = find_highest_y(Grens);
    dg.rechtsboven.y = Grens [index].y  + foutmarge;
    index = find_higest_x(Grens);
    dg.rechtsboven.x = Grens[index].x + foutmarge;
    ABCDEF[dg.deelgebied] = dg;
    return dg;
}
/** \brief vind het grootste x coordinaat uit een vector met punten. nodig om een gebied af te bakenen waarin het deelgebied zich bevindt
 *
 * \param db vector<RD>& De locaties waarin gezocht wordt. dit is waarschijnlijk de grens van een deelgebied.
 * \return int de index in db waar de hoogste x zich bevindt.
 *
 * \author Mattijn
 */
int find_higest_x(std::vector<data::RD>& db)
{
    double highest = db[0].x;
    int index = 0;
    for (unsigned int i = 1; i <db.size(); i++)
    {
        if (db[i].x > highest)
        {
            index = i;
            highest = db[i].x;
        }
    }
    return index;
}
double calc_dist(data::RD a, data::RD b){
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dxdx = dx * dx;
    double dydy = dy * dy;
    double dxdx_plus_dydy = dxdx + dydy;
    return sqrt(dxdx_plus_dydy);
}
/** \brief vind het grootste y coordinaat uit een vector met punten. nodig om een gebied af te bakenen waarin het deelgebied zich bevindt
 *
 * \param db vector<RD>& De locaties waarin gezocht wordt. dit is waarschijnlijk de grens van een deelgebied.
 * \return int de index in db waar de hoogste y zich bevindt.
 *
 */
int find_highest_y(std::vector<data::RD>& db)
{
    double highest = db[0].y;
    int index = 0;
    for (unsigned int i = 1; i <db.size(); i++)
    {
        if (db[i].y > highest)
        {
            index = i;
            highest = db[i].y;
        }
    }
    return index;
}

/** \brief vind het kleinste x coordinaat uit een vector met punten. nodig om een gebied af te bakenen waarin het deelgebied zich bevindt
 *
 * \param db vector<RD>& De locaties waarin gezocht wordt. dit is waarschijnlijk de grens van een deelgebied.
 * \return int de index in db waar de kleinste x zich bevindt.
 *
 */
int find_lowest_x(std::vector<data::RD>& db)
{
    double lowest = db[0].x;
    int index = 0;
    for (unsigned int i = 1; i <db.size(); i++)
    {
        if (db[i].x < lowest)
        {
            index = i;
            lowest = db[i].x;
        }
    }
    return index;
}

/** \brief vind het kleinste y coordinaat uit een vector met punten. nodig om een gebied af te bakenen waarin het deelgebied zich bevindt
 *
 * \param db vector<RD>& De locaties waarin gezocht wordt. dit is waarschijnlijk de grens van een deelgebied.
 * \return int de index in db waar de kleinste y zich bevindt.
 *
 */
int find_lowest_y(std::vector<data::RD>& db)
{
    double lowest = db[0].y;
    int index = 0;
    for (unsigned int i = 1; i <db.size(); i++)
    {
        if (db[i].y < lowest)
        {
            index = i;
            lowest = db[i].y;
        }
    }
    return index;
}