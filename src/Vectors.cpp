// Copyright (c) 2012-2017 Isaac Morales Durán. All rights reserved.
// Institute of Astrophysics of Andalusia, IAA-CSIC
//
// This file is part of FMPT (Fiber MOS Positioning Tools)
//
// FMPT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------
//File: Vectors.cpp
//Content: classes and functions for vectors
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "Constants.h" //M_2PI
#include "Scalars.h" //Scalars::RandomUniform
#include "Vectors.h"
#include "StrPR.h" //StrToFloat

//---------------------------------------------------------------------------

using namespace Strings;

//espacio de nombres de clases y funciones matemáticas
namespace Mathematics {

//---------------------------------------------------------------------------
//Clase TDoublePoint:
//---------------------------------------------------------------------------
//INICIALIZA LAS ETIQUETAS:

AnsiString TDoublePoint::xLabel = "x";
AnsiString TDoublePoint::yLabel = "y";

//---------------------------------------------------------------------------
//MÉTODOS PRIVADOS:

AnsiString TDoublePoint::getxText(void) const
{
    return FloatToStr(x);
}
void TDoublePoint::setxText(const AnsiString& S)
{
    try {
        x = StrToFloat(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting x in text format: ");
        throw;
    }
}
AnsiString TDoublePoint::getyText(void) const
{
    return FloatToStr(y);
}
void TDoublePoint::setyText(const AnsiString& S)
{
    try {
        y = StrToFloat(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting y in text format: ");
        throw;
    }
}

AnsiString TDoublePoint::getText(void) const
{
    return DPointToStr(*this);
}
void TDoublePoint::setText(const AnsiString& S)
{
    try {
        *this = StrToDPoint(S);
    } catch(Exception& E) {
        E.Message.Insert(1, "setting double point in text format: ");
        throw;
    }
}

AnsiString TDoublePoint::getRowText(void) const
{
    return getxText()+AnsiString("\t")+getyText();
}

//---------------------------------------------------------------------------
//FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
//DE LAS PROPIEDADES EN FORMATO TEXTO:

//print the properties of an double point in a string
//in row format
void  TDoublePoint::PrintRow(AnsiString& S, const TDoublePoint P)
{
    S += P.getRowText();
}

//obtiene las etiquetas de las propiedades de definición
//en formato texto entre paréntesis
AnsiString TDoublePoint::GetLabels(void)
{
    return AnsiString("(")+xLabel+AnsiString(", ")+yLabel+AnsiString(")");
}
//obtiene las etiquetas de las propiedades de definición
//en formato línea de texto
AnsiString TDoublePoint::GetRowLabels(void)
{
    return xLabel+AnsiString("\t")+yLabel;
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACION:

//copia un punto
void TDoublePoint::Copy(TDoublePoint P)
{
    x = P.x;
    y = P.y;
}
TDoublePoint& TDoublePoint::operator=(const TDoublePoint& P)
{
    x = P.x;
    y = P.y;

    return *this;
}

//MÉTODOS DE CONFIGURACIÓN:

//randomiza las coordenadas
//distribución uniforme en un intervalo
void TDoublePoint::Randomize(double xmin, double xmax, double ymin, double ymax)
{
    try {
        x = RandomUniform(xmin, xmax);
        y = RandomUniform(ymin, ymax);
    } catch(...) {
        throw;
    }
}

//MÉTODOS SIN ARGUMENTOS:

//calcula el módulo del punto:
//      Mathematics::Mod(x, y);
double TDoublePoint::Mod(void) const
{
    return Mathematics::Mod(x, y);
}
//calcula el argumento del punto:
//      Mathematics::Arg(x, y);
double TDoublePoint::Arg(void) const
{
    try {
        return Mathematics::Arg(x, y);
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//Clase TDoubleRect:
//---------------------------------------------------------------------------
//INICIALIZA LAS ETIQUETAS:

AnsiString TDoubleRect::LeftLabel = "Left";
AnsiString TDoubleRect::BottomLabel = "Bottom";
AnsiString TDoubleRect::RightLabel = "Right";
AnsiString TDoubleRect::TopLabel = "Top";

//---------------------------------------------------------------------------
//MÉTODOS PRIVADOS:

TDoublePoint TDoubleRect::getLeftBottom(void) const
{
    return TDoublePoint(Left, Bottom);
}
void TDoubleRect::setLeftBottom(const TDoublePoint& P)
{
    Left = P.x;
    Bottom = P.y;
}
TDoublePoint TDoubleRect::getRightTop(void) const
{
    return TDoublePoint(Right, Top);
}
void TDoubleRect::setRightTop(const TDoublePoint& P)
{
    Right = P.x;
    Top = P.y;
}

AnsiString TDoubleRect::getLeftText(void) const
{
    return FloatToStr(Left);
}
void TDoubleRect::setLeftText(const AnsiString& S)
{
    try {
        Left = StrToFloat(S);
    }catch(...) {
        throw;
    }
}
AnsiString TDoubleRect::getBottomText(void) const
{
    return FloatToStr(Bottom);
}
void TDoubleRect::setBottomText(const AnsiString& S)
{
    try {
        Bottom = StrToFloat(S);
    }catch(...) {
        throw;
    }
}
AnsiString TDoubleRect::getRightText(void) const
{
    return FloatToStr(Right);
}
void TDoubleRect::setRightText(const AnsiString& S)
{
    try {
        Right = StrToFloat(S);
    }catch(...) {
        throw;
    }
}
AnsiString TDoubleRect::getTopText(void) const
{
    return FloatToStr(Top);
}
void TDoubleRect::setTopText(const AnsiString& S)
{
    try {
        Top = StrToFloat(S);
    }catch(...) {
        throw;
    }
}

AnsiString TDoubleRect::getText(void) const
{
    return DRectToStr(*this);
}
void TDoubleRect::setText(const AnsiString& S)
{
    try {
        *this = StrToDRect(S);
    }catch(...) {
        throw;
    }
}
AnsiString TDoubleRect::getRowText(void) const
{
    return getLeftText()+AnsiString("\t")+getBottomText()+AnsiString("\t")+getRightText()+AnsiString("\t")+getTopText();
}

//---------------------------------------------------------------------------
//FUNCIONES ESTÁTICAS PARA OBTENER LAS ETIQUETAS
//DE LAS PROPIEDADES EN FORMATO TEXTO:

//obtiene las etiquetas de las propiedades de definición
//en formato texto entre paréntesis
AnsiString TDoubleRect::GetLabels(void)
{
    return AnsiString("(")+LeftLabel+AnsiString(", ")+BottomLabel+AnsiString(", ")+RightLabel+AnsiString(", ")+TopLabel+AnsiString(")");
}
//obtiene las etiquetas de las propiedades de definición
//en formato línea de texto
AnsiString TDoubleRect::GetRowLabels(void)
{
    return LeftLabel+AnsiString("\t")+BottomLabel+AnsiString("\t")+RightLabel+AnsiString("\t")+TopLabel;
}

//MÉTODOS DE CONSTRUCCIÓN, COPIA Y CLONACION:

//copia un rectángulo
void TDoubleRect::Copy(TDoubleRect R)
{
    Left = R.Left;
    Bottom = R.Bottom;
    Right = R.Right;
    Top = R.Top;
}
TDoubleRect& TDoubleRect::operator=(const TDoubleRect& R)
{
    Left = R.Left;
    Bottom = R.Bottom;
    Right = R.Right;
    Top = R.Top;

    return *this;
}

//MÉTODOS DE CONFIGURACIÓN:

//randomiza las coordenadas
//distribución uniforme en un intervalo
void TDoubleRect::Randomize(double Lmin, double Lmax, double Bmin, double Bmax,
                            double Rmin, double Rmax, double Tmin, double Tmax)
{
    try {
        Left = RandomUniform(Lmin, Lmax);
        Bottom = RandomUniform(Bmin, Bmax);
        Right = RandomUniform(Rmin, Rmax);
        Top = RandomUniform(Tmin, Tmax);
    } catch(...) {
        throw;
    }
}

//---------------------------------------------------------------------------
//Clase TPhasor:
//---------------------------------------------------------------------------

void TPhasor::setr(double r)
{
    //la amplitud debe ser no negativa
    if(r < 0)
        throw EImproperArgument("module r should not be negative");

    p_r = r; //asigna el nuevo valor
}

complex<double> TPhasor::getComplex(void)
{
    complex<double> c(p_r*cos(theta), p_r*sin(theta));
    return c;
}

AnsiString TPhasor::getText(void)
{
    return AnsiString("(")+FloatToStr(p_r)+AnsiString(", ")+FloatToStr(theta)+AnsiString(")");
}

//---------------------------------------------------------------------------
//Métodos públicos:

//construye un un fasor inicializándolo a (r, theta)
TPhasor::TPhasor(double r, double t_theta)
{
    //la amplitud debe ser no negativa
    if(r < 0)
        throw EImproperArgument("module r should not be negative");

    p_r = r; //asigna el nuevo valor
    theta = t_theta; //asigna el nuevo valotheta
}
//construye un fasor a partir de un complejo
TPhasor::TPhasor(const complex<double> &c)
{
    p_r = abs(c);
    if(p_r == 0)
        theta = 0; //la fase del vector nulo es cero
    else
        theta = arg(c);
}

//asigna las propiedades de un fasor a este fasor
TPhasor& TPhasor::operator=(const TPhasor& P)
{
    //copia las propiedades
    p_r = P.p_r;
    theta = P.theta;

    return *this;
}

//traduce y asigna un complejo a este fasor
TPhasor& TPhasor::operator=(const complex<double>& c)
{
    p_r = abs(c);
    if(p_r == 0)
        theta = 0; //la fase del vector nulo es cero
    else
        theta = arg(c);

    return *this;
}

//---------------------------------------------------------------------------
//Clase TVector3D:
//---------------------------------------------------------------------------

//asignación de un vector
TVector3D& TVector3D::operator=(const TVector3D &v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

//suma de componentes
TVector3D TVector3D::operator+(const TVector3D &v) const
{
    TVector3D w;

    w.x = x + v.x;
    w.y = y + v.y;
    w.z = z + v.z;

    return w;
}
//resta de componentes
TVector3D TVector3D::operator-(const TVector3D &v) const
{
    TVector3D w;

    w.x = x - v.x;
    w.y = y - v.y;
    w.z = z - v.z;

    return w;
}
//producto por un escalar
TVector3D TVector3D::operator*(double t) const
{
    TVector3D w;

    w.x = x*t;
    w.y = y*t;
    w.z = z*t;

    return w;
}
//división por un escalar
TVector3D TVector3D::operator/(double t) const
{
    //el escalar t debeería ser distinto de cero
    if(t == 0)
        throw EImproperArgument("escalar t should be unequal zero");

    TVector3D w;

    w.x = x/t;
    w.y = y/t;
    w.z = z/t;

    return w;
}

//producto interno (producto punto) por un vector
double TVector3D::Dot(const TVector3D &v) const
{
    return x*v.x + y*v.y + z*v.z;
}
//producto vectorial por un vector
TVector3D TVector3D::Cross(const TVector3D &v) const
{
    TVector3D w;

    w.x = y*v.z - z*v.y;
    w.y = z*v.x - x*v.z;
    w.z = x*v.y - y*v.x;

    return w;
}

//---------------------------------------------------------------------------
//OPERACIONES CON VECTORES BIDIMENSIONALES:

//determina si los vectores v1 y v2 son semejantes
bool operator== (TDoublePoint v1, TDoublePoint v2)
{
    if(v1.x!=v2.x || v1.y!=v2.y)
        return false; //indica que se ha encontrado alguna diferencia

    return true; //indica que no se ha encontrado ninguna diferecnia
}

//determina si los vectores v1 y v2 son diferentes
bool operator!= (TDoublePoint v1, TDoublePoint v2)
{
    if(v1.x!=v2.x || v1.y!=v2.y)
        return true; //indica que se ha encontrado alguna diferencia

    return false; //indica que no se ha encontrado ninguna diferecnia
}

//invierte el sentido del vector
TDoublePoint operator- (TDoublePoint v)
{
    TDoublePoint w;
    w.x = -v.x;
    w.y = -v.y;
    return w;
}
//suma v1 y v2
TDoublePoint operator+ (TDoublePoint v1, TDoublePoint v2)
{
    TDoublePoint v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    return v;
}
//resta v2 a v1
TDoublePoint operator- (TDoublePoint v1, TDoublePoint v2)
{
    TDoublePoint v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    return v;
}
//producto del escalar x por el vector v
TDoublePoint operator* (double x, TDoublePoint v)
{
    TDoublePoint w;
    w.x = x*v.x;
    w.y = x*v.y;
    return w;
}
//producto del vector v por el escalar x
TDoublePoint operator* (TDoublePoint v, double x)
{
    TDoublePoint w;
    w.x = v.x*x;
    w.y = v.y*x;
    return w;
}
//división escalar de v por x
TDoublePoint operator/ (TDoublePoint v, double x)
{
    if(x == 0)
        throw EImproperArgument("x shoul be unequal zero");

    TDoublePoint w;
    w.x = v.x/x;
    w.y = v.y/x;
    return w;
}

//producto escalar de v1 y v2
double Dot(TDoublePoint v1, TDoublePoint v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}
//gira el vector PI/2 rad en sentido levógiro
TDoublePoint Cross(TDoublePoint v)
{
    TDoublePoint w;
    w.x = -v.y;
    w.y = v.x;
    return w;
}
//determinante de la matriz {{v1.x, v1.y}, {v2.x, v2.y}}
double Det(TDoublePoint v1, TDoublePoint v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

//obtiene el modulo de un punto dado en coordenadas cartesianas rectangulares
double Mod(double x, double y)
{
    return sqrt(x*x + y*y);
}
double Mod(TDoublePoint v)
{
    return Mod(v.x, v.y);
}
//obtiene el argument principal (en el intervalo (-M_PI, M_PI],
//de un punto dado en coordenadas cartesianas rectangulares
double Arg(double x, double y)
{
    //(x, y) no debe ser el vector nulo
    if(x==0 && y==0)
        throw EImproperArgument("(x, y) should not be null");

    //obtiene ún ángulo en el dominio (-PI/2, PI/2)
    //aplicando la función atan(y/x) al cociente por la coordenada mayor
    //si la coordenada mayor es x, el ángulo es obtenido directamente
    //si la coordenada mayor es y, el ángulo es obtenido girando el sistema
    //de coordenadas +PI/2 rad, que deberán ser añadidos después

    double arg;
    if(fabs(x) >= fabs(y)) {
        arg = atan(y/x);
        //ahora arg está en (-PI/2, PI/2)
        if(x < 0) { //si está en los cuadrantes 2 ó 3
            if(y >= 0) { //si está en el cuadrante 2
                arg += M_PI; //avanza media vuelta
            }
            else { //si está en el cuadrante 3
                arg -= M_PI; //retrocede media vuelta
            }
        }
    }
    else { //|y| > |x|
        //calcula la atan con el sistema girado +PI/2 rad
        //o lo que es lo mismo descontando PI/2 radianes
        //y los añade después
        arg = atan(-x/y) + M_PI/2;
        //ahora arg está en (0, PI)
        if(y < 0) { //si está en los cuadrantes 3 ó 4
            arg -= M_PI; //retrocede media vuelta
        }
    }
    return arg; //devuelve el argumento
}
double Arg(TDoublePoint v)
{
    return Arg(v.x, v.y);
}
//obtiene el argument positivo principal (en el intervalo [0, M_2PI),
//de un punto dado en coordenadas cartesianas rectangulares
double ArgPos(double x, double y)
{
    //no debe ser el vector nulo
    if(x==0 && y==0)
        throw EImproperArgument("(x, y) should not be null");

    //obtiene ún ángulo en el dominio (-PI/2, PI/2)
    //aplicando la función atan al cociente por la coordenada mayor
    //si la coordenada mayor es x, el ángulo es obtenido directamente
    //si la coordenada mayor es y, el ángulo es obtenido girando el sistema
    //de coordenadas +PI/2 rad, que deberán ser añadidos despues

    double arg;
    if(fabs(x) >= fabs(y)) {
        arg = atan(y/x);
        //ahora arg está en (-PI/2, PI/2)
        if(x < 0) //si está en los cuadrantes 2 ó 3
            arg += M_PI; //añade media vuelta
        else if(y < 0) //si está en el cuadrante 4
            arg += M_2PI; //añade una vuelta completa
    }
    else { //y > x
        //calcula la atan con el sistema girado +PI/2 rad
        //o lo que es lo mismo descontando PI/2 radianes
        //y los añade despues
        arg = atan(-x/y) + M_PI/2;
        //ahora arg está en (0, PI)
        if(y < 0) //si está en los cuadrantes 4 ó 1
            arg += M_PI; //añade media vuelta
    }
    return arg; //devuelve el argumento

}
double ArgPos(TDoublePoint v)
{
    return ArgPos(v.x, v.y);
}
//traduce de corodenadas polares a cartesianas
void PolarToCartesian(double &x, double &y, double r, double theta)
{
    x = r*cos(theta);
    y = r*sin(theta);
}
//traduce de coordenadas cartesianas a polares
void CartesianToPolar(double &r, double &theta, double x, double y)
{
    r = Mod(x, y);
    theta = Arg(x, y);
}
//calcula el ángulo entre los vectores vini y vfin en sentido levógiro
//en el intervalo (-PI, PI)
double Angle(TDoublePoint vini, TDoublePoint vfin)
{
    //los vectores no deben ser nulos
    if(vini.x==0 && vini.y==0)
        throw EImproperArgument("vini should not be null");
    if(vfin.x==0 && vfin.y==0)
        throw EImproperArgument("vfin should not be null");


    //el dominio de la función Arg es (-PI, PI)

    //determina el ángulo del vector inicial
    double thetaini = Arg(vini.x, vini.y);
    //determina el ángulo del vector final
    double thetafin = Arg(vfin.x, vfin.y);

    //calcula el incremento del ángulo
    double deltatheta = thetafin - thetaini;

    //si la distancia entre thetaini y thetafin es mayor que media vuelta
    //añade o descuenta una vuelta previa a pfin
    if(deltatheta <= -M_PI)
        return deltatheta + M_2PI;
    if(deltatheta > M_PI)
        return deltatheta - M_2PI;

    return deltatheta; //devuelve el ángulo
}
//calcula el ángulo entre los vectores vini y vfin en sentido levógiro
//en el intervalo [0, 2*PI)
double AnglePos(TDoublePoint vini, TDoublePoint vfin)
{
    //los vectores no deben ser nulos
    if(vini.x==0 && vini.y==0)
        throw EImproperArgument("vini should not be null");
    if(vfin.x==0 && vfin.y==0)
        throw EImproperArgument("vfin should not be null");


    //el dominio de la función ArgPos es [0, 2*PI)

    //determina el ángulo del vector inicial
    double thetaini = ArgPos(vini.x, vini.y);
    //determina el ángulo del vector final
    double thetafin = ArgPos(vfin.x, vfin.y);

    //calcula el incremento del ángulo
    double deltatheta = thetafin - thetaini;

    //si el incremento sale negativo
    //s que theta_fin estaba una vuelta atrazado
    if(deltatheta < 0)
        return deltatheta + M_2PI;

    return deltatheta; //devuelve el ángulo
}

//imprime el punto P en la cadena de texto S
AnsiString DPointToStr(TDoublePoint P)
{
    return AnsiString("{")+FloatToStr(P.x)+AnsiString(", ")+FloatToStr(P.y)+AnsiString("}");
}
//imprime el rectángulo R en la cadena de texto S
AnsiString DRectToStr(TDoubleRect R)
{
    return AnsiString("{")+FloatToStr(R.Left)+AnsiString(", ")+FloatToStr(R.Bottom)+AnsiString(", ")+
            FloatToStr(R.Right)+AnsiString(", ")+FloatToStr(R.Top)+AnsiString("}");
}

//calcula el punto al que se desplaza el punto P
//cuando es rotado a rad en torno de Q
TDoublePoint Rotate(TDoublePoint P, TDoublePoint Q, double a)
{
    if(P == Q) //si el punto coincide con el eje de rotación
        return P; //al rotarlo no se desplaza

    //desplaza Q al origen
    TDoublePoint P_ = P - Q;

    //rota el punto P
    TDoublePoint R;
    R.x = P_.x*cos(a) - P_.y*sin(a);
    R.y = P_.x*sin(a) + P_.y*cos(a);

    //desplaza R a Q
    R = R + Q;

    return R;
}

//rotate a bidimensional vector 90 degrees
TDoublePoint rotate90(TDoublePoint v)
{
    TDoublePoint v_(-v.y, v.x);
    return v_;
}

//determine the versor in the middle of two vectors
//when ArgPos(v1) == ArgPos(v2)
//  return TDoublePoint(cos(ArgPos(v1)), sin(ArgPos(v1)))
TDoublePoint middleVersor(TDoublePoint v1, TDoublePoint v2)
{
    double theta1 = ArgPos(v1);
    double theta2 = ArgPos(v2);

    double theta;
    if(theta1 < theta2)
        theta = (theta1 + theta2)/2;
    else if(theta1 > theta2)
        theta = (theta1 + theta2 + M_2PI)/2;
    else //theta1 == theta2
        theta = theta1;

    TDoublePoint v(cos(theta), sin(theta));
    return v;
}

//---------------------------------------------------------------------------
//Funciones con vectores tridimensionales:

//detrmina si un vector es el vector nulo
bool IsNull(const TVector3D &V)
{
    if(V.x!=0 || V.y!=0 || V.z!=0)
        return false;
    return true;
}

//genera un versor tridimensional con distribución uniforme
//en el casquete esférico centrado en el origen que lo contiene
//      x = sen(a)*sen(b)*cos(c) - cos(a)*sen(c)
//      y = sen(a)*sen(b)*sen(c) + cos(a)*cos(c)
//      z = sen(a)*cos(b)
//donde a, b y c son variables aleatorias con distribución uniforme en (-pi, pi]
void RandomVersor(double &x, double &y, double &z)
{
    //genera un ángulo con distribución uniforme en (-pi, pi]
    double a;
    do {
        a = RandomUniform(-M_PI, M_PI);
    } while(a != -M_PI);
    //calcula el seno y el coseno
    double sina = sin(a);
    double cosa = cos(a);

    //genera un ángulo con distribución uniforme en (-pi, pi]
    do {
        a = RandomUniform(-M_PI, M_PI);
    } while(a != -M_PI);
    //calcula el seno y el coseno
    double sinb = sin(a);
    double cosb = cos(a);

    //genera un ángulo con distribución uniforme en (-pi, pi]
    do {
        a = RandomUniform(-M_PI, M_PI);
    } while(a != -M_PI);
    //calcula el seno y el coseno
    double sinc = sin(a);
    double cosc = cos(a);

    //calcula el versor
    x = sina*sinb*cosc - cosa*sinc;
    y = sina*sinb*sinc + cosa*cosc;
    z = sina*cosb;
}

//calcula la colatitud (theta) en el intervalo [-pi/2, pi/2]
//de un vector tridimensional en coordenadas cartesianas
double Colatitud(double x, double y, double z)
{
    return Arg(sqrt(x*x + y*y), z);
}
//calcula el azimut (phi) en el intervalo [-pi, pi]
//de un vector tridimensional en coordenadas cartesianas
double Azimut(double x, double y)
{
    return Arg(x, y);
}

//traduce de coordenadas esféricas con theta en [-pi/2, pi/2] y phi en R
//a coordenadas cartesianas
void SphericToCartesian(double &x, double &y, double &z,
                        double theta, double phi, double r)
{
    //el azimut debe estar en [-pi/2, pi/2]
    if(theta<-M_PI/2 || M_PI/2<theta)
        throw EImproperArgument("azimut theta should be in [-pi/2, pi/2]");

    z = r*sin(theta);
    double aux = r*cos(theta);
    x = aux*cos(phi);
    y = aux*sin(phi);
}
//traduce de coordenadas cartesianas a coordenadas esféricas
//con theta en [-pi/2, pi/2] y phi en [-pi, pi]
void CartesianToSpheric(double &theta, double &phi, double &r,
                        double x, double y, double z)
{
    r = sqrt(x*x + y*y + z*z);
    double aux = sqrt(r*r - z*z);
    theta = Arg(aux, z);
    phi = Arg(x, y);
}

//calcula el ángulo entre los vectores tridimensionales
//en el intervalo [0, 2*PI)
double Angle(const TVector3D &vini, const TVector3D &vfin)
{
    //los vectores no deben ser nulos
    if(IsNull(vini))
        throw EImproperArgument("vini should be not null");
    if(IsNull(vfin))
        throw EImproperArgument("vfin should be not null");

    //calcula el producto escalar
    double t = vini.Dot(vfin);
    //calula el producto de módulos
    double den = vini.getModule()*vfin.getModule();
    //calcula el ángulo entre los dos vectores
    double gamma = acos(t/den);

    return gamma; //devuelve el ángulo
}

//Dados dos vectores no colineales U y V:
//      calcula los versores u y v crrespondientes a normalizar U y V,
//      calcular el versor w normal a u y v,
//      y calcular los versores u' y v' correspondientes a girar u y v
//      gamma radianes en torno de w.
void RotateVectors(TVector3D &u_, TVector3D &v_, TVector3D &w,
                   const TVector3D &U, const TVector3D &V, double gamma)
{
    //el vector U no debería ser nulo
    if(IsNull(U))
        throw EImproperArgument("vector U should not be null");

    //el vector V no debería ser nulo
    if(IsNull(V))
        throw EImproperArgument("vector V should not be null");

    //normaliza los vectores U y V
    TVector3D u = U/U.getModule();
    TVector3D v = V/V.getModule();

    //calcula el versor w ortogonal a los versores u y v
    w = u.Cross(v);

    //los vectores U y V no deberían ser colineales
    if(IsNull(w))
        throw EImproperArgument("vectors U and V should be non-collinear");

    //calcula los coeficientes de las componentes
    double cosg = cos(gamma);
    double sing = sin(gamma);

    //calcula el vector u_
    u_ = u*cosg + v*sing;
    v_ = v*cosg - u*sing;
}

} //namespace Mathematics

//---------------------------------------------------------------------------

