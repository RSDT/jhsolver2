/*
**--------------------------------------------------------------
**    RDNAPTRANS(TM)2008
**
**    Authors: Jochem Lesparre, Joop van Buren, Marc Crombaghs, Frank Dentz, Arnoud Pol, Sander Oude Elberink
**             http://www.rdnap.nl
**    Based on RDNAPTRANS2004
**    Main changes:
**    - 7 similarity transformation parameters
**    - 0.0088 offset in the transformation between ellipsoidal height (h) and orthometric heights (NAP)
**    - coordinates are computed also outside the validity regions of the grid files x2c.grd, y2c.grd and nlgeo04.grd
**--------------------------------------------------------------
*/
/** \todo (Mattijn#1#): comments omzetten naa doxyblocks */

/*
**--------------------------------------------------------------
**    Include statements and namespace
**--------------------------------------------------------------
*/
#ifndef RDNAP
#define RDNAP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

/*
**--------------------------------------------------------------
**    Static data declarations
**    Mathematical constant pi = 3.14...
**--------------------------------------------------------------
*/
const double PI = 2.0 * asin(1.0);
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Geographic NL-Bessel coordinates of Amersfoort (pivot point and projection base point)
**        phi     latitude in decimal degrees
**        lambda  longitude in decimal degrees
**        h       ellipsoidal height in meters
**    Source of constants:
**        Hk.J. Heuvelink, "De stereografische kaartprojectie in hare toepassing bij de Rijksdriehoeksmeting". Delft: Rijkscommissie voor Graadmeting en Waterpassing, 1918.
**        HTW, "Handleiding voor de Technische Werkzaamheden van het Kadaster". Apeldoorn: Kadaster, 1996.
**--------------------------------------------------------------
*/
const double PHI_AMERSFOORT_BESSEL    = 52.0+ 9.0/60.0+22.178/3600.0;
const double LAMBDA_AMERSFOORT_BESSEL =  5.0+23.0/60.0+15.500/3600.0;
const double H_AMERSFOORT_BESSEL      =  0.0;
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Parameters of ellipsoids Bessel1841 and GRS80
**        a      half major axis in meters
**        inv_f  inverse flattening
**    Source of constants: HTW, "Handleiding voor de Technische Werkzaamheden van het Kadaster". Apeldoorn: Kadaster, 1996.
**--------------------------------------------------------------
*/
const double A_BESSEL     = 6377397.155;
const double INV_F_BESSEL = 299.1528128;
const double A_ETRS       = 6378137;
const double INV_F_ETRS   = 298.257222101;
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Transformation parameters relative to pivot point Amersfoort. Note: Do NOT confuse with parameters for the center of the ellipsoid!
**        tx     translation in direction of x axis in meters
**        ty     translation in direction of y axis in meters
**        tz     translation in direction of z axis in meters
**        alpha  rotation around x axis in radials
**        beta   rotation around y axis in radials
**        gamma  rotation around z axis in radials
**        delta  scale parameter (scale = 1 + delta)
**    Source of constants: A. de Bruijne, J. van Buren, A. K�sters and H. van der Marel, "De geodetische referentiestelsels van Nederland; Definitie en vastlegging van ETRS89, RD en NAP en hun onderlinge relatie". Delft: Nederlandse Commissie voor Geodesie (NCG), to be published in 2005.
**--------------------------------------------------------------
*/
const double TX_BESSEL_ETRS    =  593.0248;
const double TY_BESSEL_ETRS    =   25.9984;
const double TZ_BESSEL_ETRS    =  478.7459;
const double ALPHA_BESSEL_ETRS =    1.9342e-6;
const double BETA_BESSEL_ETRS  =   -1.6677e-6;
const double GAMMA_BESSEL_ETRS =    9.1019e-6;
const double DELTA_BESSEL_ETRS =    4.0725e-6;

const double TX_ETRS_BESSEL    = -593.0248;
const double TY_ETRS_BESSEL    =  -25.9984;
const double TZ_ETRS_BESSEL    = -478.7459;
const double ALPHA_ETRS_BESSEL =   -1.9342e-6;
const double BETA_ETRS_BESSEL  =    1.6677e-6;
const double GAMMA_ETRS_BESSEL =   -9.1019e-6;
const double DELTA_ETRS_BESSEL =   -4.0725e-6;
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Parameters of RD projection
**        scale         scale factor (k in some notations)
**                      this factor was first defined by Hk.J. Heuvelink as pow(10,-400e-7), nowadays we define it as exactly 0.9999079
**        x_amersfoort  false Easting
**        y_amersfoort  false Northing
**    Source of constants:
**        G. Bakker, J.C. de Munck and G.L. Strang van Hees, "Radio Positioning at Sea". Delft University of Technology, 1995.
**        G. Strang van Hees, "Globale en lokale geodetische systemen". Delft: Nederlandse Commissie voor Geodesie (NCG), 1997.
**--------------------------------------------------------------
*/
const double SCALE_RD = 0.9999079;
const double X_AMERSFOORT_RD = 155000;
const double Y_AMERSFOORT_RD = 463000;
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Names of grd files
**
**    Grd files are binary grid files in the format of the program Surfer(R)
**    The header contains information on the number of grid points, bounding box and extreme values.
**
**    RD-corrections in x and y
**
**          -8000 meters < RD Easting  (stepsize 1 km) < 301000 meters
**         288000 meters < RD Northing (stepsize 1 km) < 630000 meters
**
**    Geoid model NLGEO2004
**
**        50.525   degrees < ETRS89 latitude  (stepsize 0.050000 degrees) < 53.675 degrees
**         3.20833 degrees < ETRS89 longitude (stepsize 0.083333 degrees) <  7.45833 degrees
**
**        Alternative notation:
**        50� 31' 30" < ETRS89_latitude  (stepsize 0� 3' 0") < 53� 40' 30"
**         3� 12' 30" < ETRS89_longitude (stepsize 0� 5' 0") <  7� 27' 30"
**
**        The stepsizes correspond to about 5,5 km x 5,5 km in the Netherlands.
**--------------------------------------------------------------
*/
const string GRID_FILE_DX    = "x2c.grd";
const string GRID_FILE_DY    = "y2c.grd";
const string GRID_FILE_GEOID = "nlgeo04.grd";
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Precision parameters for iterations (respectively in meters and degrees)
**--------------------------------------------------------------
*/
const double PRECISION     = 0.0001;
const double DEG_PRECISION = PRECISION/40e6*360;
/*
**--------------------------------------------------------------
**    Continuation of static data declarations
**    Mean difference between NAP and ellipsoidal Bessel height. This is only used for getting from x, y in RD to phi, lambda in ETRS89.
**--------------------------------------------------------------
*/
const double MEAN_GEOID_HEIGHT_BESSEL = 0.0;

/*
**--------------------------------------------------------------
**    Functions
**--------------------------------------------------------------
*/

/*
**--------------------------------------------------------------
**    Function name: deg_sin
**    Description:   sine for angles in degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    alpha          double      in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    sin(alpha)
**--------------------------------------------------------------
*/

double deg_sin(double alpha);

/*
**--------------------------------------------------------------
**    Function name: deg_cos
**    Description:   cosine for angles in degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    alpha          double      in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    cos(alpha)
**--------------------------------------------------------------
*/
double deg_cos(double alpha);

/*
**--------------------------------------------------------------
**    Function name: deg_tan
**    Description:   tangent for angles in degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    alpha          double      in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    tan(alpha)
**--------------------------------------------------------------
*/
double deg_tan(double alpha);

/*
**--------------------------------------------------------------
**    Function name: deg_asin
**    Description:   inverse sine for angles in degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    a              double      in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    asin(a)
**--------------------------------------------------------------
*/
double deg_asin(double a);

/*
**--------------------------------------------------------------
**    Function name: deg_atan
**    Description:   inverse tangent for angles in degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    a              double in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    atan(a)
**--------------------------------------------------------------
*/
double deg_atan(double a);

/*
**--------------------------------------------------------------
**    Function name: atanh
**    Description:   inverse hyperbolic tangent
**
**    Parameter      Type        In/Out Req/Opt Default
**    a              double      in     req     none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    atanh(a)
**--------------------------------------------------------------
*/
double atanh(double a);

/*
**--------------------------------------------------------------
**    Function name: deg_min_sec2decimal
**    Description:   converts from degrees, minutes and seconds to decimal degrees
**
**    Parameter      Type        In/Out Req/Opt Default
**    deg            double      in     req     none
**    min            double      in     req     none
**    sec            double      in     req     none
**    dec_deg        double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    All parameters are doubles, so one can also enter decimal minutes or degrees.
**    Note: Nonsense input is accepted too.
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void deg_min_sec2decimal(double deg, double min, double sec, double& dec_deg);
/*
**--------------------------------------------------------------
**    Function name: decimal2deg_min_sec
**    Description:   converts from decimal degrees to degrees, minutes and seconds
**
**    Parameter      Type        In/Out Req/Opt Default
**    dec_deg        double      in     req     none
**    deg            int         out    -       none
**    min            int         out    -       none
**    sec            double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    none
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void decimal2deg_min_sec(double dec_deg, int& deg, int& min, double& sec);
/*
**--------------------------------------------------------------
**    Function name: geographic2cartesian
**    Description:   from geographic coordinates to cartesian coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi            double      in     req     none
**    lambda         double      in     req     none
**    h              double      in     req     none
**    a              double      in     req     none
**    inv_f          double      in     req     none
**    x              double      out    -       none
**    y              double      out    -       none
**    z              double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi      latitude in degrees
**    lambda   longitude in degrees
**    h        ellipsoidal height
**    a        half major axis of the ellisoid
**    inv_f    inverse flattening of the ellipsoid
**    x, y, z  output of cartesian coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void geographic2cartesian(double phi, double lambda, double h,
                          double a, double inv_f,
                          double& x, double& y, double& z );

/*
**--------------------------------------------------------------
**    Function name: cartesian2geographic
**    Description:   from cartesian coordinates to geographic coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    x              double      in     req     none
**    y              double      in     req     none
**    z              double      in     req     none
**    a              double      in     req     none
**    inv_f          double      in     req     none
**    phi            double      out    -       none
**    lambda         double      out    -       none
**    h              double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x, y, z  input of cartesian coordinates
**    a        half major axis of the ellisoid
**    inv_f    inverse flattening of the ellipsoid
**    phi      output latitude in degrees
**    lambda   output longitude in degrees
**    h        output ellipsoidal height
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void cartesian2geographic(double x, double y, double z,
                          double a, double inv_f,
                          double& phi, double& lambda, double& h );

/*
**--------------------------------------------------------------
**    Function name: sim_trans
**    Description:   3 dimensional similarity transformation (7 parameters) around another pivot point "a" than the origin
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_in           double      in     req     none
**    y_in           double      in     req     none
**    z_in           double      in     req     none
**    tx             double      in     req     none
**    ty             double      in     req     none
**    tz             double      in     req     none
**    alpha          double      in     req     none
**    beta           double      in     req     none
**    gamma          double      in     req     none
**    delta          double      in     req     none
**    xa             double      in     req     none
**    ya             double      in     req     none
**    za             double      in     req     none
**    x_out          double      out    -       none
**    y_out          double      out    -       none
**    z_out          double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_in, y_in, z_in     input coordinates
**    tx                   translation in direction of x axis
**    ty                   translation in direction of y axis
**    tz                   translation in direction of z axis
**    alpha                rotation around x axis in radials
**    beta                 rotation around y axis in radials
**    gamma                rotation around z axis in radials
**    delta                scale parameter (scale = 1 + delta)
**    xa, ya, za           coordinates of pivot point a (in case of rotation around the center of the ellipsoid these parameters are zero)
**    x_out, y_out, z_out  output coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void sim_trans(double x_in, double y_in, double z_in,
               double tx, double ty, double tz,
               double alpha, double beta, double gamma,
               double delta,
               double xa, double ya, double za,
               double& x_out, double& y_out, double& z_out);

/*
**--------------------------------------------------------------
**    Function name: rd_projection
**    Description:   stereographic double projection
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi            double      in     req     none
**    lambda         double      in     req     none
**    x_rd           double      out    -       none
**    y_rd           double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi         input Bessel latitude in degrees
**    lambda      input Bessel longitude in degrees
**    x_rd, rd_y  output RD coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void rd_projection(double phi, double lambda,
                   double& x_rd, double& y_rd);

/*
**--------------------------------------------------------------
**    Function name: inv_rd_projection
**    Description:   inverse stereographic double projection
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_rd           double      in     req     none
**    y_rd           double      in     req     none
**    phi            double      out    -       none
**    lambda         double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_rd, rd_y  input RD coordinates
**    phi         output Bessel latitude in degrees
**    lambda      output Bessel longitude in degrees
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
void inv_rd_projection(double x_rd, double y_rd,
                       double& phi, double& lambda);

/*
**--------------------------------------------------------------
**    Function name: read_grd_file_header
**    Description:   reads the header of a grd file
**
**    Parameter      Type        In/Out Req/Opt Default
**    filename       string      in     req     none
**    size_x         short int   out    -       none
**    size_y         short int   out    -       none
**    min_x          double      out    -       none
**    max_x          double      out    -       none
**    min_y          double      out    -       none
**    max_y          double      out    -       none
**    min_value      double      out    -       none
**    max_value      double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    filename   name of the to be read binary file
**    size_x     number of grid values in x direction (row)
**    size_y     number of grid values in y direction (col)
**    min_x      minimum of x
**    max_x      maximum of x
**    min_y      minimum of y
**    max_y      maximum of x
**    min_value  minimum value in grid (besides the error values)
**    max_value  maximum value in grid (besides the error values)
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int read_grd_file_header(string filename,
                         short int& size_x, short int& size_y,
                         double& min_x, double& max_x,
                         double& min_y, double& max_y,
                         double& min_value, double& max_value);

/*
**--------------------------------------------------------------
**    Function name: read_grd_file_body
**    Description:   reads a value from a grd file
**
**    Parameter      Type        In/Out Req/Opt Default
**    filename       string      in     req     none
**    number         long int    in     req     none
**    value          float       out    -       none
**
**    Additional explanation of the meaning of parameters
**    filename       name of the grd file to be read
**    record_number  number defining the position in the file
**    record_value   output of the read value
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int read_grd_file_body(string filename, long int record_number, float& record_value);

/*
**--------------------------------------------------------------
**    Function name: grid_interpolation
**    Description:   grid interpolation using Overhauser splines
**
**    Parameter      Type        In/Out Req/Opt Default
**    x              double      in     req     none
**    y              double      in     req     none
**    grd_file       string      in     req     none
**    value          double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x, y           coordinates of the point for which a interpolated value is desired
**    grd_file       name of the grd file to be read
**    record_value   output of the interpolated value
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int grid_interpolation(double x, double y, string grd_file, double& value);

/*
**--------------------------------------------------------------
**    Function name: rd_correction
**    Description:   apply the modelled distortions in the RD coordinate system
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_pseudo_rd    double      in     req     none
**    y_pseudo_rd    double      in     req     none
**    x_rd           double      out    -       none
**    y_rd           double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_pseudo_rd, y_pseudo_rd  input coordinates in undistorted pseudo RD
**    x_rd, y_rd                output coordinates in real RD
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int rd_correction(double x_pseudo_rd, double y_pseudo_rd,
                  double& x_rd, double& y_rd);

/*
**--------------------------------------------------------------
**    Function name: inv_rd_correction
**    Description:   remove the modelled distortions in the RD coordinate system
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_rd           double      in     req     none
**    y_rd           double      in     req     none
**    x_pseudo_rd    double      out    -       none
**    x_pseudo_rd    double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_rd, y_rd                input coordinates in real RD
**    x_pseudo_rd, y_pseudo_rd  output coordinates in undistorted pseudo RD
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int inv_rd_correction(double x_rd, double y_rd,
                      double& x_pseudo_rd, double& y_pseudo_rd);

/*
**--------------------------------------------------------------
**    Function name: etrs2rd
**    Description:   convert ETRS89 coordinates to RD coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi_etrs       double      in     req     none
**    lambda_etrs    double      in     req     none
**    h_etrs         double      in     req     none
**    x_rd           double      out    -       none
**    y_rd           double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi_etrs, lambda_etrs, h_etrs  input ETRS89 coordinates
**    x_rd, y_rd                     output RD coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int etrs2rd(double phi_etrs, double lambda_etrs, double h_etrs,
            double& x_rd, double& y_rd, double& h_bessel);

/*
**--------------------------------------------------------------
**    Function name: rd2etrs
**    Description:   convert RD coordinates to ETRS89 coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_rd           double      in     req     none
**    y_rd           double      in     req     none
**    nap            double      in     req     none
**    phi_etrs       double      out    -       none
**    lambda_etrs    double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_rd, y_rd, nap        input RD and NAP coordinates
**    phi_etrs, lambda_etrs  output ETRS89 coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int rd2etrs(double x_rd, double y_rd, double nap,
            double& phi_etrs, double& lambda_etrs, double& h_etrs);

/*
**--------------------------------------------------------------
**    Function name: etrs2nap
**    Description:   convert ellipsoidal ETRS89 height to NAP height
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi            double      in     req     none
**    lambda         double      in     req     none
**    h              double      in     req     none
**    nap            double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi, lambda, h  input ETRS89 coordinates
**    nap             output NAP height
**
**    Return value: (besides the standard return values) none
**    on error (outside geoid grid) nap is not compted here
**    instead in etrs2rdnap nap=h_bessel
**--------------------------------------------------------------
*/
int etrs2nap(double phi, double lambda, double h,
             double& nap);

/*
**--------------------------------------------------------------
**    Function name: nap2etrs
**    Description:   convert NAP height to ellipsoidal ETRS89 height
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi            double      in     req     none
**    lambda         double      in     req     none
**    nap            double      in     req     none
**    h              double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi, lambda  input ETRS89 position
**    nap          input NAP height at position phi, lambda
**    h            output ellipsoidal ETRS89 height
**
**    Return value: (besides the standard return values)
**    none
**    on error (outside geoid grid) h is not compted here
**    instead in rdnap2etrs h=h_etrs_sim (from similarity transformation)
**--------------------------------------------------------------
*/
int nap2etrs(double phi, double lambda, double nap,
             double& h);

/*
**--------------------------------------------------------------
**    Function name: etrs2rdnap
**    Description:   convert ETRS89 coordinates to RD and NAP coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    phi            double      in     req     none
**    lambda         double      in     req     none
**    h              double      in     req     none
**    x_rd           double      out    -       none
**    y_rd           double      out    -       none
**    nap            double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    phi, lambda, h   input ETRS89 coordinates
**    x_rd, y_rd, nap  output RD and NAP coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int etrs2rdnap(double phi, double lambda, double h,
               double& x_rd, double& y_rd, double& nap);

/*
**--------------------------------------------------------------
**    Function name: rdnap2etrs
**    Description:   convert RD and NAP coordinates to ETRS89 coordinates
**
**    Parameter      Type        In/Out Req/Opt Default
**    x_rd           double      in     req     none
**    y_rd           double      in     req     none
**    nap            double      in     req     none
**    phi            double      out    -       none
**    lambda         double      out    -       none
**    h              double      out    -       none
**
**    Additional explanation of the meaning of parameters
**    x_rd, y_rd, nap  input RD and NAP coordinates
**    phi, lambda, h   output ETRS89 coordinates
**
**    Return value: (besides the standard return values)
**    none
**--------------------------------------------------------------
*/
int rdnap2etrs(double x_rd, double y_rd, double nap,
                double& phi, double& lambda, double& h);
#endif // RDNAP
