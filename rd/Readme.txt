Readme.txt for RDNAPTRANS2008 and the NTv2 implementation

Release of RDNAPTRANS2008

After 24 March 2009 the RD database will contain new values for ETRS89, based on a re-adjustment of the AGRS.NL stations. 
From that moment on the use of RDNAPTRANS2008 for these new ETRS89 coordinates is more precise than the use of the existing RDNAPTRANS2004. 
However the differences do not exceed 1 cm in horizontal and vertical position and there is a transition period till 1-4-2010.

RDNAPTRANS2008 is presented to you in the form of the zip-file RDNAPTRANS2008.zip containing the files:

- rdnaptrans2008.cpp  source code in C++, this code is just an example to show the formulas used.

- rdnaptrans2008.xxx  Windows executable with extension .xxx to let it pass firewalls etc. The program transforms only one point per run. 

- x2c.grd    binary file with the x-RD correction grid (identical in RDNAPTRANS2004)

- y2c.grd    binary file with the y-RD correction grid (identical in RDNAPTRANS2004)

- nlgeo04.grd   binary file with the NLGEO2004 geoid grid (identical in RDNAPTRANS2004)

- Use of RDNAPTRANS2008.doc  Procedure for using the name RDNAPTRANS2008 and test coordinates 

- Transpar_2008.txt  ASCII file with the RDNAPTRANS2008 transformation parameters

- Readme.txt    this explanation

- NTv2_2008.zip   Aprroximate implementation of RDNAPTRANS2008 using the NTv2 and VDatum methods

If coordinates are computed for points outside the region where interpolation in between grid points of x2c.grd, y2c.grd or nlgeo04.grd is possible, the output should be accompanied by a warning. 
Outside these grids RD or NAP are not well defined. On the border of the grids the resulting coordinates show discontinuities up to 25 cm for position or height. 

The minimum changes to update a program from RDNAPTRANS2004 to RDNAPTRANS2008 are the 7 transformation parameters and the height offset of 8.8 mm as given in Transpar_2008.txt.

--------

NTv2_2008.zip

The NTv2_2008.zip contains a NTv2 grid (rdtrans2008.gsb) and a VDatum grid (naptrans2008.gtx).

The NTv2 and VDatum grids are an approximation of RDNAPTRANS2008 with the following limitations:

1) The rdtrans2008 NTv2-grid can only give identical results to RDNAPTRANSTM2008 within 1
millimeter at ground level onshore and at mean seal level offshore. The horizontal deviation is
approximately 1 millimeter per 50 meter height difference from ground level or mean sea level.

2) An exception to 1) is the border of the RDNAPTRANSTM2008 correction grid. Transformation
results within cells of the rdtrans2008 NTv2-grid that are intersected by the border of the
RDNAPTRANSTM2008 correction grid can result in deviations of up to 20 centimeter.

3) The naptrans2008 VDatum-grid cannot be used to determine deflections of the vertical. For
this the NLGEO2004 geoid model has to be used.

4) The naptrans2008 VDatum-grid is referenced to the Bessel-1841 ellipsoid and cannot be used
stand-alone, it has to be used in combination with the rdtrans2008 NTv2-grid.

For information please read the documents in the NTv2_2008.zip file


Lennard Huisman
rd@kadaster.nl
