#include <iostream>
#include "datastructures.h"
#include <vector>
#include <fstream>
#include "kml_handling.h"
int main()
{

    std::string  hqfilename = "homebases.jh";
    std::string kmlfilename = "jotihunt2014.kml";
    kml_h::create_homebase_file( hqfilename, kmlfilename);
    std::string s;
    std::ifstream  f ("deelgebieden.jh");
    std::ifstream  fh ("homebases.jh") ;
    std::vector<std::vector< data::RD > > dgn = kml_h::read_deelgebieden_rd(f);
    std::vector< data::Homebase >  HQs = kml_h::read_hombases(fh);
    kml_h::sort_hb(HQs);
    std::vector<data::Deelgebied>  dg = kml_h::create_deelgebieden(dgn,HQs);
  //  data::RD linksonder = get_linksonder();
  //  data::RD rechtsboven = get_rechtsboven();
    /*vector<Hint> hints = test(dg);

    for (auto i = 0 ; i < hints.size(); i++)
    {
        read_sol(hints[i]);
    }
    vector<vector <int> > sol = find_sols(hints);
    for (auto i = 0 ; i < hints.size(); i++)
    {
        clean_sol(hints[i],sol);
    }
    ofstream  out ("out.txt");
    for (auto i = 0 ; i < hints.size(); i++)
    {
        show_sol(hints[i]);
        cout <<  ABCDEF[hints[i].deelgebied].Route[ABCDEF[hints[i].deelgebied].Route.size()-1]<< endl;
        cout << ABCDEF[hints[i].deelgebied].HQs.size()<< endl;
        cout << hints[i].SolRD.size() << endl;
        out << endl<<create_url(hints[i].SolRD,ABCDEF[hints[i].deelgebied].Route[ABCDEF[hints[i].deelgebied].Route.size()-1],ABCDEF[hints[i].deelgebied].HQs)<< endl;
    }
    for (auto i = 0; i < 1; i++)
    {

        cout << ABCDEF[i].naam  << endl;
        cout << "naam\tP_gehad\tP_volgend\n";
        for (auto j = 0; j < ABCDEF[i].HQs.size();j++)
            cout << ABCDEF[i].HQs[j].naam
            <<'\t'<<
            '%' <<'\t'<<
            '%'<<endl;
        cout << endl;
    }*/
  //  fill_database(linksonder,rechtsboven); // 7050s 1,5GB
    return 0;
}