//---------------------------------------------------------------------------
//Archivo: ExclusionAreaList.h
//Contenido: lista de áreas de exclusión
//Última actualización: 06/05/2014
//Autor: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef EXCLUSIONAREALIST_H
#define EXCLUSIONAREALIST_H

#include "RoboticPositionerList3.h"
#include "..\3_Lists\PointersList.h"
#include "..\B_RoboticPositioner\ExclusionArea.h"

//---------------------------------------------------------------------------

using namespace Lists;

//espacio de nombres de modelos
namespace Models {

//###########################################################################
//TExclusionAreaList
//###########################################################################

//clase lista de áreas de exclusión
class TExclusionAreaList : public TItemsList<TExclusionArea*> {
protected:
        //PROPIEDADES DE LOCALIZACIÓN:

        TDoublePoint __O;

        double __rmax;

        double __xmin;
        double __xmax;
        double __ymin;
        double __ymax;

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO
        //###################################################################

        //PROPIEDADES COMPUESTAS EN FORMATO TEXTO:

        //PROPIEDADES DE LOCALIZACIÓN EN FORMATO TEXTO:

        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

public:
        //PROPIEDADES COMPUESTAS:

        //TPointersSlideArray<TExclusionArea> Items

        //PROPIEDADES DE LOCALIZACIÓN:

        //posición del origen del sistema S0
        //valor constante: (0, 0)
        TDoublePoint getO(void) const {return __O;}

        //radio del círculo mínimo que contiene
        //el dominio de todaslasáreas de exlusión
        double getrmax(void) const {return __rmax;}

/*        //límite abcisa inferior del cuadrado que contiene
        //todas las áreas de exclusión
        double getxmin(void) const {return __xmin;}
        //límite abcisa superior del cuadrado que contiene
        //todas las áreas de exclusión
        double getxmax(void) const {return __xmax;}

        //límite ordenada inferior del cuadrado que contiene
        //todas las áreas de exclusión
        double getymin(void) const {return __ymin;}
        //límite ordenada superior del cuadrado que contiene
        //todas las áreas de exclusión
        double getymax(void) const {return __ymax;}
  */
        //PROPIEDADES GRÁFICAS:

        //color del límite del dominio conjunto
        //valor por defecto: clGray
        //#QColor LimitDomainColor;

        //indica si debe dibujar las áreas de exclusión
        //valor por defecto: true
        bool PaintExclusionAreas_;
        //interruptor de dibujo del límite del dominio
        //circular conjunto de los posicionadores
        //valor por defecto: false
        bool PaintLimitDomain_;

        //###################################################################
        //PROPIEDADES EN FORMATO TEXTO
        //###################################################################

        //PROPIEDADES COMPUESTAS EN FORMATO TEXTO:

        //dirección en memoria de la lista de posicionadores
        AnsiString getItemsAddressText(void) const {
                return IntToHex(int(&Items), 8);}

        //PROPIEDADES DE LOCALIZACIÓN EN FORMATO TEXTO:

        AnsiString getOText(void) const;

        AnsiString getrmaxText(void) const;

/*        AnsiString getxminText(void) const;
        AnsiString getxmaxText(void) const;
        AnsiString getyminText(void) const;
        AnsiString getymaxText(void) const;
  */
        //PROPIEDADES GRÁFICAS EN FORMATO TEXTO:

        AnsiString getLimitDomainColorText(void) const;
        void setLimitDomainColorText(const AnsiString&);

        AnsiString getPaintExclusionAreas_Text(void) const;
        void setPaintExclusionAreas_Text(const AnsiString&);
        AnsiString getPaintLimitDomain_Text(void) const;
        void setPaintLimitDomain_Text(const AnsiString&);

        //CONJUNTOS DE PROPIEDADES EN FORMATO TEXTO:

        //tabla de orígenes de coordenadas:
        //      Id      x0      y0      thetaO1
        AnsiString getOriginsTableText(void) const;
        void setOriginsTableText(const AnsiString&);

        //conjunto de propiedades de loaalización
        //en formato asignaciones
        AnsiString getLocationText(void) const;
        //conjunto de propiedades de gráficas
        //en formato asignaciones
        AnsiString getGraphicsText(void) const;

        //conjunto de todas las propiedades
        //en formato de asignaciones
        AnsiString getAllText(void) const;
        //instancia de la lista de posicionadores
        //en formato de asignaciones
        AnsiString getInstanceText(void) const;
        void setInstanceText(const AnsiString&);

        //-------------------------------------------------------------------
        //MÉTODOS ESTÁTICOS:

        //lee una instancia en una cadena
        static void __fastcall ReadInstance(TExclusionAreaList* &RPL,
                const AnsiString& S, int &i);

        //-------------------------------------------------------------------
        //MÉTODOS DE CONTRUCCIÓN, COPIA Y CLONACIÓN

        //construye una lista de áreas de exclusión
        TExclusionAreaList(void);

        //copia las propiedades de localización
        //      (O, rmax)
        void CopyLocation(const TExclusionAreaList*);
        //copia las propiedades gráficas
        //      (LimitDomainColor, PaintEclusionAreas_, PaintLimitDomain_)
        void CopyGraphics(const TExclusionAreaList*);

        //clona una lista de áreas de exclusión
        void Clone(const TExclusionAreaList*);

        //construye un clon de una lista de áreas de exclusión
        TExclusionAreaList(const TExclusionAreaList*);

        //ADVERTENCIA: la destrucción de la lista de áreasde exclusión causará
        //la destrucción de las áreas de exclusión de la lista. Si lo que desea
        //es destruir la lista de áreas de exclusión sin destruirlas,
        //deberá vaciarla antes con el método: ClearWithoutDestroy.

        //AÑADIR Y BORAR ÁREAS DE EXCLUSIÓN:

        //Añadir un área de exclusión a la lista:
        //      Add(RP);
        //Destruye y borra un área de exclusión de la lista
        //      Delete(i);
        //Borra un área de exclusión de la lista sin destruirlo
        //      DeleteWithoutDestroy(i);

        //MÉTODOS DE DESTRUCCION DE ÁREAS DE EXCLUSIÓN:

        //detruye el área de exclusión indicada de la lista
        void Destroy(int i);
        //destruye todas las áreas de exclusión de la lista
        void Destroy(void);
        //destruye las áreas de exclusión seleccionadas de la lista
        int DestroySelected(void);

        //MÉTODOS DE BÚSQUEDA DE ÁREAS DE EXCLUSIÓN:

        //busca un área de exclusión en la lista
        int Search(TExclusionArea *EA) const;
        //busca la primera área de exclusión con el identificador indicado
        int SearchId(int Id) const;
        //devuelve el puntero a la primera área de exclusión
        //con el identificador indicado
        const TExclusionArea *SearchIdPointer(int Id) const;

        //MÉTODOS DE ASIMILACIÓN:

        //calcula el SPM de todoas las áreas de exclusión
        void CalculateSPM(void);

        //El método CalculateSPMComponents() y los sucesivos
        //deberán ser invocados toda vez que los márgenes de seguridad
        //sean modificados.

        //determina los posicionadores que están lo bastante cerca
        //de cada área de seguridad como para invadir su SPM
        void DetermineAdjacents(const TRoboticPositionerList&);
        //ordena las listas de posicionadores adyacentes en
        //sentido levógiro empezando por el más próximo a 0
        void SortAdjacents(void);

        //El método DetermineAdjacents() y los sucesivos
        //deberán ser invocados toda vez que se añadan o borren áreas de exclusión,
        //o se modifique algún parámetro de posicionamiento o dimensionamiento,
        //que pudiera afectar a la adyacencia de algún posicionador.

        //Los métodos de asimilación:
        //      Deben ser aplicados despues de contruir, destruir áreas de exclusión
        //      o modificar la geometría de alguna de ellas.
        //
        //      Son computacionalmente costosos, de modo que no serán invocados
        //      automáticamente al ajustar cada parámetro independiente,
        //      si no que deben ser invocados manualmente una sola vez
        //      despues de ajustar todos los parámetros.

        //calcula las propiedades de localización:
        //      (rmax)//, xmin, xmax, ymin, ymax)
        void CalculateLocationParameters(void);

        //invoca a todos los métodos de asimilación de
        //los parámetros de dimensionamiento y localización:
        //      DetermineAdjacents
        //      SortAdjacents
        void AssimilateSizingAndLocation(const TRoboticPositionerList&);

        //asimila la configurración de posicionadores dada ejecutando:
        //      CalculateSPMComponents();
        //      AssimilateSizingAnsLocation();
        void Assimilate(const TRoboticPositionerList&);

        //MÉTODOS DE DOMINIO CONJUNTO:

/*        //determina el intervalo cartesiano que contiene
        //el conjunto de todas las áreas de exclusión
        //si el número de áreas de exclusión de la lista es menor que uno
        //lanza una excepcion EImproperCall
        void GetDomain(double &xmin, double &xmax,
                double &ymin, double &ymax);
  */
        //determina si un punto se encuentra dentro del círculo
        //que contiene el dominio conjunto de lasáreas de exclusión
        bool IsInCircle(const TDoublePoint&);
/*        //determina si un punto se encuentra dentro del cuadrado
        //que contiene el dominio conjunto de lasáreas de exclusión
        bool IsInSquare(const TDoublePoint&);*/

        //MÉTODOS PARA DETERMINAR SI HAY COLISIONES:

        //levanta las banderas indicadoras de determinación de colisión
        //pendiente de todos los posicionadores de la lista
        void EnablePending(void);
        //determina si algún área de exclusión
        //colisiona con el brazo de algún posicionador adyacente
        bool ThereIsCollision(void);
        //busca las áreas de exclusión de la lista cuya barrera colisiona con
        //el brazo de algún posicionador adyacente
        void SearchCollinding(TVector<int> &indices);
        //obtiene los conjuntos de posicionadores en colisión en la exposición indicada
        void GetCollisionClusterList(TPointersList<TItemsList<TRoboticPositioner*> > &CCL);

        //MÉTODOS GRÁFICOS:

        //establece el estado de la bandera de pintado
        //del cuerpo de las áreas de exlución
        void SetPaintBody(bool);

        //asigna un color a todos los posicionadores de la lista
        //#void SetAllColors(QColor Color);
        //selecciona todos los posicionadores de la lista
        void SelectAll(void);
        //deselecciona todos los posicionadores de la lista
        void DeselectAll(void);

        //imprime la lista de posicionadores en el lienzo de la fotografía
        //con los colores indicados
        //imprime el lienzo de la fotografía en el lienzo de la caja de pintura
        //#void PaintExclusionAreas(TPloterShapes *PS);
        //dibuja el límite circular del dominio conjunto de los posicionadores
        //#void PaintLimitDomain(TPloterShapes *PS);

        //busca el primer área de exclusión que está bajo el punto P
        //y que parte del área de exclusión puede ser agarrado en
        //el punto indicado:
        //      i: índice al área de exclusión bajo el punto P;
        //      dominio i: [0, Count];
        //      n: parte del área de exclusión agarrada;
        //      valores posibles:
        //              0: niguna;
        //              1: alguna;
        //valores de retorno:
        //      false: ninguna parte agarrada
        //      true: alguna parte agarrada
        bool Grab(int &i, int &n, TDoublePoint P);

        //dibuja la lista de áreas de exclusión en un trazador de formas
        //#void Paint(TPloterShapes*);
};
/*
//lista de áreas de exclusión en formato texto
class TExclusionAreaListText {
public:
        //tabla de orígenes de coordenadas
        AnsiString OriginsTableText;

        //lista de pares de cadenas de texto para coneter
        //las siguientes cadenas por cada posicionador:
        //      Barrier->Contour_ColumnText
        //      InstanceText
        TTPointersList<ClusterSS> ItemsText;

        //consturye una lista de áreas de exclusión en formato texto
        TExclusionAreaListText(void);

        //vacia la lista de áreas de exclusión en formato texto
        void Clear(void);

        //indica si el objeto no contiene una instancia
        bool IsEmpty(void);

        //aplica la instancia a una lista de posicionadores
        void GetInstance(TRoboticPositionerList& RPL) const;
        //obtiene la instancia de una lista de posicionadores
        void SetInstance(TRoboticPositionerList& RPL);
};*/

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // EXCLUSIONAREALIST_H
