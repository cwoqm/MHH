#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;

const double nHeatVap = 4.43e-8; //kg vapour/J, page 234/312
const double UBlow = 0.5;
const double UFog = 0.5;
const double UMechCool = 0.5;
const double UThScr = 0.5;
const double PBlow = 0.5;
const double AFlr = 13000; //asm.cpp
const double pAir0 = 1.2; //Air at Sea
const double MWater = 18; //kg/kmol, page 236/312
const double VPOut = 2.1; //kPa, page 35/312, Sicily, DOY 267-272
const double COPMechCool = 1; //page 237/312
const double PMechCool = 1; //page 237/312
const double cHECin = 2.21; //W/m2K2, page 111/312, Almeria, Spain
const double ACov = 1.15e3; //m2, page 111/312, Almeria
const double LAI = 1.5; //asm.cpp
const double delH = 2.45e6; // J/kg water, page 230/312
const double gamma = 65.8; //Pa/K, page 231/312
const double c_pAir = 1e3; // J/kgK, page 231/312
const double R = 8.314e3; //J/kmolK, page 232/312
const double r_smin = 82; //s/m, page 236/312
const double r_b = 275; // s/m, page 236/312

const double phiFog = 0; //check later

const double TAir;
const double TTop;
const double TOut;
const double TCovIn;
const double TThScr;
const double TMechCool;

const double HMechAir;

const double VPMechCool;
const double VPThScr;
const double VPCan;
const double VPCovIn;

const double xPad;
const double xOut;

//need source
const double nPad = 1;
//30
double MVBlowAir()
{
	return nHeatVap * HBlowAir();
}
//30.1
double HBlowAir()
{
	return UBlow * PBlow / AFlr;
}

//fPad coded
double fPad();
//pAir coded
double pAir();
//31
double MVPadAir()
{
	return pAir() * fPad() * (nPad * (xPad - xOut) + xOut);
}

//32
double MVAirOut_Pad(double VPAir)
{
	return fPad() * (MWater / R) * (VPAir / (TAir + 273.15));
}

//33
double MVCanAir(double VPAir)
{
	return VECCanAir()*(VPCan - VPAir);
}

//33.1
double VECCanAir()
{
	return (2 * pAir() * c_pAir * LAI) / (delH * gamma * (r_b + rs()));
}

double rs();

double MVFogAir()
{
	return UFog*phiFog/AFlr;
}

double MVAirThScr(double VPAir)
{
	if (VPAir < VPThScr) return 0;
	return  6.4e-9 * HECAirThScr() * (VPAir - VPThScr);
}
double HECAirThScr()
{
	return 1.7 * UThScr * pow(abs(TAir - TThScr), 1.0/3);
}

double fThScr();
double MVAirTop(double VPAir, double VPTop)
{
	return (MWater / R) * fThScr() * (VPAir / (TAir + 273.15) - VPTop/ (TTop + 273.15));
}

double fVentSide();
double fVentForced();
double fVentRoof();
double MVAirOut(double VPAir)
{
	return (MWater / R) * (fVentForced()+fVentSide())* (VPAir / (TAir + 273.15) - VPOut / (TOut+ 273.15));
}

double MVTopOut(double VPTop)
{
	return (MWater / R) * fVentRoof() * (VPTop/ (TTop + 273.15) - VPOut / (TOut + 273.15));
}

double MVAirMech(double VPAir)
{
	//VPMechCool hay VPMech
	if (VPAir < VPMechCool) return 0;
	return 6.4e-9 * HECMechAir(VPAir) * (VPAir - VPMechCool);
}
double HECMechAir(double VPAir)
{
	return (UMechCool * COPMechCool * PMechCool / AFlr) / (TAir - TMechCool + 6.4e-9 * HMechAir * (VPAir - VPMechCool));
}


//TOP VAPOUR
double MVTopCovIn(double VPTop)
{
	if (VPTop < VPCovIn) return 0;
	return 6.4e-9 * (HECTopCovIn()) * (VPTop - VPCovIn);
}

double HECTopCovIn()
{
	return cHECin * pow(TTop - TCovIn, 1.0 / 3) * (ACov / AFlr);
}

double dVPAir(double t, double VPAir, double VPTop)
{
	double val_MVCanAir = MVCanAir(VPAir);
	double val_MVPadAir = MVPadAir();
	double val_MVFogAir = MVFogAir();
	double val_MVBlowAir = MVBlowAir();
	double val_MVAirThScr = MVAirThScr(VPAir);
	double val_MVAirTop = MVAirTop(VPAir,VPTop);
	double val_MVAirOut = MVAirOut(VPAir);
	double val_MVAirOut_Pad = MVAirOut_Pad(VPAir);
	double val_MVAirMech = MVAirMech(VPAir);
	return val_MVCanAir+val_MVPadAir+val_MVFogAir+val_MVBlowAir-val_MVAirThScr-val_MVAirTop-val_MVAirOut-val_MVAirOut_Pad-val_MVAirMech;
}

double dVPTop(double t, double VPAir, double VPTop)
{
	double val_MVAirTop = MVAirTop(VPAir,VPTop);
	double val_MVTopCovIn = MVTopCovIn(VPTop);
	double val_MVTopOut = MVTopOut(VPTop);
	return val_MVAirTop - val_MVTopCovIn - val_MVTopOut;
}
int main()
{

}