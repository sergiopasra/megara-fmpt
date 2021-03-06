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
//File: FigureList.cpp
//Content: contour figure list
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "ContourFigureList.h"
#include "Function.h"
#include "Vector.h"
#include "Strings.h"
#include "Geometry.h"
#include "Constants.h"

#include <algorithm> //std::min, std::max

//---------------------------------------------------------------------------

using namespace Strings;
using namespace Operators;

//espacio de nombres de modelos
namespace Models {

//---------------------------------------------------------------------------
//TContourFigureList
//--------------------------------------------------------------------------
//MÉTODOS DE CONSTRUCCIÓN,COPIA Y CLONACIÓN:

//construye un contorno
TContourFigureList::TContourFigureList(int Capacity) :
    TItemsList<TContourFigure*>(Capacity, NULL, NULL, NULL,
                                TContourFigure::ContourFigurePrint, TContourFigure::ContourFigureRead)
{
}

//copia una lista de figuras
void TContourFigureList::Copy(const TContourFigureList &CFL)
{
    //libera las figuras de la plantilla previa
    for(int i=0; i<getCount(); i++)
        delete Items[i];

    //iguala la longitud de la lista
    setCount(CFL.getCount());

    //contruye un clon de cada figura
    for(int i=0; i<getCount(); i++) {
        //apunta la figura indicada para facilitar su acceso
        TContourFigure *cf = CFL[i];

        //si la figura es de tipo TSegment
        if(typeid(*cf) == typeid(TSegment))
            Items[i] = new TSegment((TSegment*)cf);

        //si la figura es de tipo TArc
        else if(typeid(*cf) == typeid(TArc))
            Items[i] = new TArc((TArc*)cf);

        //si F apunta a nulo
        else if(cf == NULL)
            Items[i] = cf;

        else {
            //figura geométrica de tipo desconocido
            throw EImpossibleError("contour figure type unknown");
        }
    }
}
TContourFigureList& TContourFigureList::operator=(const TContourFigureList& C)
{
    //libera las figuras de la plantilla previa
    for(int i=0; i<getCount(); i++)
        delete Items[i];

    //iguala la longitud de la lista
    setCount(C.getCount());

    //contruye un clon de cada figura
    for(int i=0; i<getCount(); i++) {
        //apunta la figura indicada para facilitar su acceso
        TContourFigure *F = C[i];

        //si la figura es de tipo TSegment
        if(typeid(*F) == typeid(TSegment))
            Items[i] = new TSegment((TSegment*)F);

        //si la figura es de tipo TArc
        else if(typeid(*F) == typeid(TArc))
            Items[i] = new TArc((TArc*)F);

        //si F apunta a nulo
        else if(F == NULL)
            Items[i] = F;

        else {
            //figura geométrica de tipo desconocido
            throw EImpossibleError("geometrical figure type unknown");
        }
    }

    //devuelve la referencia a esta lista de figurasde contorno
    //para permitir anidar asignaciones
    return *this;
}

//clona una lista de figuras
void TContourFigureList::Clone(const TContourFigureList& CFL)
{
    //destruye las figuras previas
    Destroy(*this);
    //iguala la capacidad
    setCapacity(CFL.getCapacity());
    //copia las figuras
    Copy(CFL);

    //apunta a las mismas funciones externas
    Compare = CFL.Compare;
    Evaluate = CFL.Evaluate;
    Assign = CFL.Assign;
    Print = CFL.Print;
    Read = CFL.Read;
}

//construye un clon de una lista de figuras
TContourFigureList::TContourFigureList(const TContourFigureList &CFL) :
    TItemsList<TContourFigure*>(CFL.getCapacity(), CFL.Compare,
                                CFL.Evaluate, CFL.Assign,
                                CFL.Print, CFL.Read)
{
    //clona el contorno
    Clone(CFL);
}

//determina si una CFL es distinta
bool TContourFigureList::operator!=(const TContourFigureList& CFL) const
{
    if(getCount() != CFL.getCount())
        return true;

    for(int i=0; i<getCount(); i++)
        if(areUnequals(Items[i], CFL[i]))
            return true;

    return false;
}

//--------------------------------------------------------------------------
//MÉTODOS DE CARACTERIZACIÓN:

//determina si todos los punteros del contorno son nulos
bool TContourFigureList::areAllNULL(void) const
{
    for(int i=0; i<getCount(); i++)
        if(Items[i] != NULL)
            return false;

    return true;
}
//determina si todas las figuras de la lista son segmentos y archos
bool TContourFigureList::notAllFiguresAreEitherSegmentsOrArcs(void) const
{
    //for all figures of the list
    for(int i=0; i<getCount(); i++) {
        //point the pointer for facilitate their access
        TContourFigure *F = Items[i];

        //check if the pointer not point neither segment nor arc
        if(typeid(*F) != typeid(TSegment) && typeid(*F) != typeid(TArc))
            return true;
    }

    return false; //indicates that all pointer pont either segment or arc
}

//Determina si la lista de figuras constituye un contorno ordenado
//comprobando que:
//- La lista contiene al menos dos figuras.
//- Cada figura se encuentra engarzada con la siguiente.
//- Ningún vértice siguiente coincide con el vértice inicial
//  de otra figura previa, excepto el de la última figura,
//  que coincide con el inicial de la primera figura.
//- Ninguna figura se interseca con las demás.
//Determina además el estado de inversión de las figuras que
//se encuentren interconectadas desde la primera.
bool TContourFigureList::isAContourSorted(void) const
{
    //si la lista contiene menos de dos figuras
    if(getCount() < 2)
        return false; //indica que no es un contorno ordenado

    //define el estado de la primera figura como no invertido
    TContourFigure *cf_prev = getFirst();
    cf_prev->inverted = false;

    //para cada figura determina el estado de inversión,
    //comprobando que punto (si Pa o Pb) se encuentra
    //conectado al punto final de la figura previa.
    TContourFigure *cf;
    for(int i=1; i<getCount(); i++) {
        cf = Items[i];

        if(cf->getPa() == cf_prev->getPfin())
            cf->inverted = false;
        else if(cf->getPb() == cf_prev->getPfin())
            cf->inverted = true;
        else
            return false;

        cf_prev = cf;
    }

    //comprueba que el punto final de la última figura
    //se encuentra conectado al punto inicial de la primera figura
    cf_prev = cf;
    cf = getFirst();
    if(cf->getPa() != cf_prev->getPfin())
        return false;

/*
    //vértice opuesto al que se encontró conectado en la primera figura
    char vini;
    //vértice opuesto al que se encontró conectado en la siguiente figura (a la actual)
    char vnext;

    //puntero a la figura indicada
    TContourFigure *cf1;
    //puntero a otra figura
    TContourFigure *cf2;

    //índice a figuras de la lista
    int i = 1;

    //apunta la primera figura para facilitar su acceso
    cf1 = getFirst();
    //apunta la segunda figura para facilitar su acceso
    cf2 = Items[i];

    //inicializa vini y vnext según los primeros vértices que
    //se encuentren conectados en la primera y segunda figuras
    if(cf1->getPb() == cf2->getPa()) {
        vini = 'a';
        vnext = 'b';
        cf1->inverted = false;
        cf2->inverted = false;
    } else if(cf1->getPb() == cf2->getPb()) {
        vini = 'a';
        vnext = 'a';
        cf1->inverted = false;
        cf2->inverted = true;
    } else if(cf1->getPa() == cf2->getPa()) {
        vini = 'b';
        vnext = 'b';
        cf1->inverted = true;
        cf2->inverted = false;
    } else if(cf1->getPa() == cf2->getPb()) {
        vini = 'b';
        vnext = 'a';
        cf1->inverted = true;
        cf2->inverted = true;
    } else {
        //indica que no es un contorno ordenado
        return false;
    }

    //por cada figura de la lista
    for(i=2; i<getCount(); i++) {
        //apunta la figura indicada para facilitar su acceso
        cf1 = Items[i-1];
        //apunta la siguiente figura de la lista
        cf2 = Items[i];

        //determina el nuevo vnext
        switch(vnext) {
        case 'a': //en caso de que cf1->getPa(), tenga que estar conectado a algún vértice de cf2
            if(cf1->getPa() == cf2->getPb()) { //si está conectado a Pb
                vnext = 'a'; //indica que el próximo vértice a comprobar es el a
                cf2->inverted = true;
            } else if(cf1->getPa() == cf2->getPa()) { //si no, si está conectado a Pa
                vnext = 'b'; //indica que el próximo vértice a comprobar es el a
                cf2->inverted = false;
            } else //si no
                //indica que la lista no constituye un contorno ordenado
                return false;
            break;
        case 'b': //en caso de que cf1->getPb(), tenga que estar conectado a algún vértice de cf2
            if(cf1->getPb() == cf2->getPb()) { //si está conectado a Pb
                vnext = 'a'; //indica que el próximo vértice a comprobar es el a
                cf2->inverted = true;
            } else if(cf1->getPb() == cf2->getPa()) { //si no, si está conectado a Pa
                vnext = 'b'; //indica que el próximo vértice a comprobar es el a
                cf2->inverted = false;
            } else //si no
                //indica que la lista no constituye un contorno ordenado
                return false;
            break;
        }
    }

    //apunta la última figura indicada para facilitar su acceso
    cf1 = getLast();
    //apunta la primera figura indicada para facilitar su acceso
    cf2 = getFirst();

    //comprueba que el último vnext coincide con vini
    switch(vnext) {
    case 'a': //en caso de que cf1->getPa()...
        switch(vini) {
        case 'a': //...tenga que estar conectado a cf2->getPa()
            if(cf1->getPa() != cf2->getPa()) //si no está conectado
                return false; //indica que no es un contorno ordenado
            break;
        case 'b': //...tenga que estar conectado a cf2->getPb()
            if(cf1->getPa() != cf2->getPb()) //si no está conectado
                return false; //indica que no es un contorno ordenado
            break;
        }
        break;
    case 'b': //en caso de que cf1->getPb()...
        switch(vini) {
        case 'a': //...tenga que estar conectado a cf2->getPa()
            if(cf1->getPb() != cf2->getPa()) //si no está conectado
                return false; //indica que no es un contorno ordenado
            break;
        case 'b': //...tenga que estar conectado a cf2->getPb()
            if(cf1->getPb() != cf2->getPb()) //si no está conectado
                return false; //indica que no es un contorno ordenado
            break;
        }
        break;
    }
*/
    //indica que si es un contorno ordenado
    return true;
}

//determina el estado de inversión de todas las figuras de la lista
//tomando como punto inicial el Pa de la primera figura
void TContourFigureList::determineInverted(void)
{

}

//---------------------------------------------------------------------------
//MÉTODOS DE CÁLCULO DE DISTANCIAS CON EL BRAZO:

//determina la distancia mínima de un punto a este contorno
double TContourFigureList::distanceMin(TDoublePoint P) const
{
    //este contorno debería contener al menos una figura
    if(areAllNULL())
        throw EImproperCall("this contour should contain one figure almost");

    double d; //distancia entre el punto y cada figura
    double dmin = DBL_MAX; //distancia mínima hasta el momento
    TFigure *F; //puntero a una figura indicada de este contorno

    //calcula la distancia entre las figuras de este contorno
    //y las figuras del otro contorno:

    //por cada figura restante de este contorno
    for(int i=0; i<getCount(); i++) {
        //apunta a la figura indicada de este contorno
        //para facilitar su acceso
        F = Items[i];

        //si el puntero no es nulo
        if(F != NULL) {
            //calcula la distancia mínima entre el punto y la figura
            d = F->distancePoint(P);
            //actualiza la distancia mínima
            if(d < dmin)
                dmin = d;
        }
    }

    return dmin; //devuelve la distancia mínima
}
//determina la distancia máxima de un punto a este contorno
double TContourFigureList::distanceMax(TDoublePoint P) const
{
    //este contorno debería contener al menos una figura
    if(areAllNULL())
        throw EImproperCall("this contour should contain one figure almost");

    double d; //distancia entre el punto y cada figura
    double dmax = 0; //distancia máxima hasta el momento
    TFigure *F; //puntero a una figura indicada de este contorno

    //calcula la distancia entre las figuras de este contorno
    //y las figuras del otro contorno:

    //por cada figura de este contorno
    for(int i=0; i<getCount(); i++) {
        //apunta a la figura indicada de este contorno
        //para facilitar su acceso
        F = Items[i];

        //si el puntero no es nulo
        if(F != NULL) {
            //calcula la distancia entre el punto y la figura
            d = F->distancePointMax(P);
            //actualiza la distancia máxima
            if(d > dmax)
                dmax = d;
        }
    }

    return dmax; //devuelve la distancia máxima
}
//determina la distancia mínima de un contorno a este contorno
double TContourFigureList::distanceMin(const TContourFigureList &C) const
{
    //el contorno C debería contener al menos una figura
    if(C.areAllNULL())
        throw EImproperArgument("contour C should contain one figure almost");

    //este contorno debería contener al menos una figura
    if(areAllNULL())
        throw EImproperCall("this contour should contain one figure almost");

    double d; //distancia entre el par de figuras
    double dmin = DBL_MAX; //distancia mínima hasta el momento
    TFigure *F; //puntero a una figura indicada de este contorno
    TFigure *Fo; //puntero a una figura indicada del otro contorno
    int i, j;

    //calcula la distancia entre las figuras de este contorno
    //y las figuras del otro contorno:

    //por cada figura restante de este contorno
    for(i=0; i<getCount(); i++) {
        //apunta a la figura indicada de este contorno
        //para facilitar su acceso
        F = Items[i];
        //por cada figura del otro contorno
        for(j=0; j<C.getCount(); j++) {
            //asigna el puntero indicado del otro contorno
            //para facilitar su acceso
            Fo = C[j];
            //si ambos punteros son no nulos
            if(F!=NULL && Fo!=NULL) {
                //calcula la distancia entre figuras
                d = F->distance(Fo);
                //si la distancia no puedo reducirse más
                if(d <= 0)
                    return 0; //devuelve cero
                else //si no
                    //actualiza la distancia mínima
                    if(d < dmin)
                        dmin = d;
            }
        }
    }

    return dmin; //devuelve la distancia mínima
}

//---------------------------------------------------------------------------
//MÉTODOS DE DETERMINACIÓN DE INTERSECCIÓN CON EL BRAZO:

//determina si la distancia de un contorno a este controno
//es inferior al margen perimetral de seguridad SPM.
bool TContourFigureList::collides(const TContourFigureList &C, double SPM) const
{
    //comprueba las precondiciones
    if(C.areAllNULL())
        throw EImproperArgument("contour C should contain one figure almost");

    double d; //distancia entre el par de figuras
    TFigure *F; //puntero a una figura indicada de este contorno
    TFigure *Fo; //puntero a una figura indicada del otro contorno
    int i, j;

    //calcula la distancia entre las figuras de este contorno
    //y las figuras del otro contorno:

    //por cada figura restante de este contorno
    for(i=0; i<getCount(); i++) {
        //apunta a la figura indicada de este contorno
        //para facilitar su acceso
        F = Items[i];
        //por cada figura del otro contorno
        for(j=0; j<C.getCount(); j++) {
            //asigna el puntero indicado del otro contorno
            //para facilitar su acceso
            Fo = C[j];
            //si ambos punteros son no nulos
            if(F!=NULL && Fo!=NULL) {
                //calcula la distancia entre figuras
                d = F->distance(Fo);
                //si la distancia no puedo reducirse más
                if(d < SPM)
                    return true; //indica que si colisiona
            }
        }
    }

    return false; //indica que no colisiona
}

//determina si un punto está en el interior del contorno
bool TContourFigureList::isInner(TDoublePoint P) const
{
    //ADVERTENCIA: si el contorno no delimita un conjunto convexo,
    //no basta con determinar si el punto está al mismo lado de
    //todas las figuras geométricas que lo componen.

    //si la lista de figuras no es un contorno ordenado
    if(!isAContourSorted())
        //indica que no puede estar dentro por que
        //la lista de figuras no tiene lado interior
        return false;

    //NOTA: para ser un contorno ordenado, debrá haber al menos dos figuras
    //en la lista, ya que un único segmento no tiene lado interior y
    //un único arco tampoco, por que no puede tener una vuelta completa,
    //ya que si sus vértices coinciden se entiende que es un arco de
    //cero radianes.

    //DETERMINA SI EL PUNTO ESTÁ SOBRE LOS VÉRTICES
    //Y SI ES ASÍ DEVUELVE true:

    TContourFigure *F; //puntero a la figura indicada

    //por cada figura del contorno
    for(int i=0; i<getCount(); i++) {
        //apunta la figura indicada para facilitar su acceso
        F = Items[i];

        //si el punto está sobre los vértices
        if(P == F->getPa() || P == F->getPb())
            //indica que el punto está en la figura
            return true;
    }

    //RECORRE LAS FIGURAR CONTABILIZANDO EL DESPLAZAMIENTO ANGULAR:

    //bandera indicadora de si el vértice de la figura indicada
    //conectado a la próxima figura es el b
    bool PbIsNext;
    double acum_theta = 0; //acumulador del ángulo de desplazamiento
    TContourFigure *Fnext; //puntero a la figura próxima a la indicada

    //por cada figura adicional de la lista
    for(int i=0; i<getCount(); i++) {
        //apunta la figura indicada para facilitar su acceso
        F = Items[i];
        //puntero a la figura siguiente a la indicada
        Fnext = Items[(i+1)%getCount()];

        //si el vértice Pb de la figura indicada es igual a
        //alguno de los vértices de la próxima figura
        if(F->getPb()==Fnext->getPa() || F->getPb()==Fnext->getPb())
            //indica que el vértice conectado a la próxima figura es el b
            PbIsNext = true;
        //si no, si el vértice Pa de la figura indicada es igual a
        //alguno de los vértices de la próxima figura
        else if(F->getPa()==Fnext->getPa() || F->getPa()==Fnext->getPb())
            //indica que el vértice conectado a la próxima figura es el a
            PbIsNext = false;
        else {
            //inidica que se ha producido un efecto lateral por que
            //debería haber un vértice de la figura indicada conectado a la próxima figura
            throw EImpossibleError("should be a vertext in the actual figure connect to next figure");
        }

        //contabiliza el ángulo recorrido para desplazarse
        //del vértice actual al próximo vértice de la figura F
        //entorno de P
        acum_theta += rotation(F, PbIsNext, P);
    }

    //si ha dado una vuelta compelta para volver al punto inicial
    if(fabs(acum_theta) >= M_PI)
        return true; //inica que está dentro del contorno

    //Nótese que el umbral para determinar si ha vuelto al mismo sitio
    //o ha dado una vueltacompleta se pone en la mitad.

    //indica que el punto no está en el interior del contorno
    return false;
}

//--------------------------------------------------------------------------
//MÉTODOS DE TRANSFORMACIONES GEOMÉTRICAS:

//obtiene la lista de figuras rotada y trasladada.
//si el número de figuras de la lista no coincide:
//      lanza EImproperArgument
//si alguna figuradelalista no es del mimo tipo
//      lanza EImproperArgument
void TContourFigureList::getRotatedAndTranslated(TContourFigureList &Contour,
                                                 double theta, TDoublePoint V) const
{
    //el número de figuras de la lista debe ser igual
    if(Contour.getCount() != getCount())
        throw EImpossibleError("number figures of thelist should be equal");

    //el tipo de figuras dela lista debe coincidir
    for(int i=0; i<Contour.getCount(); i++)
        if(typeid(Contour[i]) != typeid(Items[i]))
            throw EImpossibleError("types figures in thelist should match");
    //if(isAContourSorted())
    //      int aux = 0;
    //rota y traslada cada figura del contorno
    TFigure *F;
    for(int i=0; i<getCount(); i++) { //por cada figura geométrica
        //apunta la figura geométrica indicada para facilitar su acceso
        F = Contour[i];
        //obtiene la figura geométrica rotada
        Items[i]->getRotated(F, theta);
        //traslada la figura geométrica según el vector V
        F->translate(V);
    }
    //if(Contour.isAContourSorted())
    //      int xxx = 0;
}

//obtiene el contorno del perímetro de seguridad externo
void TContourFigureList::getSecurityContour(TContourFigureList& contour, double SPM) const
{
    if(notAllFiguresAreEitherSegmentsOrArcs())
        throw EImproperCall("contour figure list (securityConotur) should contains only segments and arcs");

    //check if is a contour sorted and determine the inversion status of the figures
    if(!isAContourSorted())
        throw EImproperCall("contour figure list (securityContour) shoule be a contour shorted");

    //initialize the output
    Destroy(contour);

    //build a new contour with similar figures of the original contour
    //displaced segments and semejant arcs
    for(int i=0; i<getCount(); i++) {
        //get the actual figure
        TContourFigure *cf = Items[i];

        //get the corresponding new segment, if any
        TContourFigure *cf_;
        if(typeid(*cf) == typeid(TSegment)) {
            //get the actual segment
            TSegment *s = (TSegment*)cf;
            TDoublePoint pa = s->getPa();
            TDoublePoint pb = s->getPb();

            //determine the normal versor of the segment
            //ponderated by SPM
            double A, B, C;
            canonicalEquation(pa, pb, A, B, C);
            normalize(A, B, C);
            TDoublePoint v;
            v.x = A*SPM;
            v.y = B*SPM;

            //determine the vertex of the new segment
            TDoublePoint pa_, pb_;
            if(cf->inverted) {
                pa_ = pa - v;
                pb_ = pb - v;
            }
            else {
                pa_ = pa + v;
                pb_ = pb + v;
            }

            //build the new figure
            cf_ = (TContourFigure*) new TSegment(pa_, pb_);
        }
        //get the corresponding new arc, if any
        else if(typeid(*cf) == typeid(TArc)) {
            //get the actual arc
            TArc *a = (TArc*)cf;
            TDoublePoint pa = a->getPa();
            TDoublePoint pb = a->getPb();
            TDoublePoint pc = a->getPc();
            double r = a->getR();

            //determine the versor from Pc to each vertex
            //ponderated by SPM
            TDoublePoint va = pa - pc;
            va = va/Mod(va)*SPM;
            TDoublePoint vb = pb - pc;
            vb = vb/Mod(vb)*SPM;

            //determine the vertex and the radious of the new arc
            TDoublePoint pa_, pb_;
            double r_;
            if(cf->inverted) {
                pa_ = pa - va;
                pb_ = pb - vb;
                r_ = r - SPM;
            }
            else {
                pa_ = pa + va;
                pb_ = pb + vb;
                r_ = r + SPM;
            }
            //build the new figure
            cf_ = (TContourFigure*) new TArc(pa_, pb_, pc, r_);
        }
        //check that the figure is either a segment or an arc
        else
            throw EImpossibleError("lateral effect");

        //copy the inversion status in the new figure
        cf_->inverted = cf->inverted;
        //add the displaced figure to the contour
        contour.Add(cf_);
    }

    //complete the contour
    for(int i=getCount()-1; i>=0; i--) {
        //get the consecutive original figures
        TContourFigure *cf1 = Items[i];
        TContourFigure *cf2 = Items[(i + 1)%getCount()];

        //make a rutinary check
        if(cf1->getPfin() != cf2->getPini())
                throw EImpossibleError("lateral effect");

        //get the coincident point
        TDoublePoint p = cf1->getPfin();

        //get the consecutive new figures
        TContourFigure *cf1_ = contour[i];
        TContourFigure *cf2_ = contour[(i + 1)%getCount()];

        //if the points aren't coincidents
        if(Mod(cf2_->getPini() - cf1_->getPfin()) > ERR_NUM) {
            //determine if there is intersections between cf1 and cf2
            bool aux = intersection(cf1_, cf2_);

            //reacts according the instersection status
            if(aux) {
                //set the intersection point to cf1_->Pfin and cf2_->Pini
                //  cf1_->setPfin(p_);
                //  cf2_->setPini(p_);

                //do nothing
            }
            else {
                //insert an arc
                TArc *arc = new TArc(cf1_->getPfin(), cf2_->getPini(), p, SPM);
                contour.Insert(i+1, arc);
            }
        }
    }
}

//--------------------------------------------------------------------------
//FUNCIONES RELACIONADAS:

/*//segrega las figuras de esta lista en listas de figuras engarzadas
//devuelve falso si encuentra más de dos vértices engarzados entre si
bool segregate(TPointersList<TContourFigureList> &LC, TContourFigureList &C)
{
        //puntero a la lista de figuras indicada de LC
        TContourFigureList *C;
        //puntero a la figura a insertar
        TFigure *F;

        //por cada figura de la lista
        for(int i=0; i<Count; i++) {
                //apunta la figura indicada para cafilitar su acceso
                F = Items[i];

                //NOTA: la figura indicada puede insertarse delante o detrás de
                //la primera figura con la que se encuentra engarzada.

                //busca la primera posición de inserción en CL
                int k=0; bool notfound=true;
                while(k<LC.Count && notfound) {
                        //apunta el contorno indicado para facilitar su acceso
                        C = LC.GetPointer(k);
                        //busca la primera posición de inserción en C1
                        int l=0;
                        while(l<C->Count && notfound) {
                                //si la figura a insertar está engarzada con la figura indicada
                                if(AreCrimped(F, C->Get(l))) {
                                        //si la figura indicada está engarzada con la figura previa
                                        if(l>0 && AreCrimped(C->Get(l), C->Get(l-1))) {
                                                //si la figura indicada está engarzada con la figura posterior
                                                if(l+1<C->Count && AreCrimped(C1->Get(l), C->Get(l-1))) {
                                                        //libera las listas de figuras (las figuras no deben ser liberadas por que son las de este contorno)
                                                        LC.Clear();
                                                        //indica que no es un contorno
                                                        return false;
                                                } else { //si no
                                                        //inserta la figura en la posición posterior
                                                        C1->Insert(l+1, F);
                                                        //indica que ha encontrado la posición
                                                        notfound = false;
                                                }
                                        } else { //si no
                                                //inserta la figura en la posición previa
                                                C1->Insert(l, F);
                                                //indica que ha encontrado la posición
                                                notfound = false;
                                        }
                                }
                                //indica a la próxima posición de C1
                                l++;
                        }
                        //indica a la próxima posicion de LC
                        k++;
                }

                //si no ha encontrado una posición de inserción en la que insertar la figura
                if(notfound) {
                        //construye una lista nueva
                        C1 = new TContourFigureList();
                        //añade la figura a la lista
                        C1->Add(F);
                        //añade la lista a la lista de listas
                        LC.Add(C1);
                }
        }

        //indica que no hay más de dos ve´rtices engarzados entre si
        return true;
}
//reduce un conjunto de listas de figuras concatenadas
//concatenando las listas que se puedan entre si
void concatenate(TPointersList<TContourFigureList> &LC)
{
        //NOTA: no es posible saber cual es el sentido dextrógiro de ordenación
        //de cada lista de figuras, hasta que no se tenga el conjunto de listas
        //de figuras más reducido posible.

        //puntero a la lista indicada de LC
        TContourFigureList *C1;
        //puntero a la lista de figuras posterior a la indicada de LC
        TContourFigureList *C2;

        //por cada lista de figuras
        for(int i=0; i<LC.Count; i++) {
                //apunta la lista de figuras indicada para facilitar su acceso
                C1 = LC.GetPointer(i);

                //por cada lista de figuras posterior
                for(int j=i+1; j<LC.Count; j++) {
                        //apunta la lista de figuras indicada para facilitar su acceso
                        C2 = LC.GetPointer(j);

                        //si la última figura de C1 está crimpada con la primera figura de C2
                        if(AreCrimped(C1->Last, C2->First)) {
                                //concatena C2 al final de C1 manteniendo el orden
                                for(int k=0; k<C2->Count; k++)
                                        C1->Add(C2->Get(k));

                                //destruye la lista C2
                                LC.Delete(j);
                        //si no, si la última figura de C1 está crimpada con la última figura de C2
                        } else if(AreCrimped(C1->Last, C2->First)) {
                                //concatena C2 al final de C1 invirtiendo el orden
                                for(int k=C2->Count-1; k>=0; k--)
                                        C1->Add(C2->Get(k));

                                //destruye la lista C2
                                LC.Delete(j);
                        }
                }
        }
}          */

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------

