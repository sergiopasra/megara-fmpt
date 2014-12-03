#include "..\..\C_FiberMOSModel\FileMethods.h"
#include "..\MotionProgramGenerator.h"
#include "..\..\2_Strings\Strings.h"
#include "..\..\2_Strings\TextFile.h"

#include <QCoreApplication>

#include <iostream> //std
#include <QDebug> //qDebug()

//Dos formas de imprimir en la consola:
//Forma habitual:
//  #include <QDebug> //qDebug()
//  std::cout << "Hello world!" << endl;
//A partir de Qt 5:
//  #include <iostream> //std
//  qDebug() << "Hello world!" << endl;
//En todo caso debe ponerse antes de:
//  return a.exec();

//using namespace Mathematics;
using namespace Strings;
using namespace Models;
using namespace Positioning;

//imprime una linea de texto en la consola
void append(const AnsiString& S)
{
    std::cout << S.c_str() << endl;
}

//programa principal
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //------------------------------------------------------------------
    //CONFIGURA EL SISTEMA:

    //RECUERDE: las excepciones en tiempo de ejecución pueden deberse a
    //que no se ha configurado el sistema.

    //configur el separador decimal
    DecimalSeparator = '.';

    //configura las variables globales para que la función StrToBool
    //pueda funcionar
    if(TrueBoolStrs.getCount() < 1) {
            TrueBoolStrs.setCount(1);
            TrueBoolStrs[0] = "True";
    }
    if(FalseBoolStrs.getCount() < 1) {
            FalseBoolStrs.setCount(1);
            FalseBoolStrs[0] = "False";
    }

    //ADVERTENCIA: BoolToStr no comprueba que:
    //      TrueBoolStrs[0] = "True" y
    //      FalseBoolStrs[0] = "False",
    //aunque dicha condición ha sido configurada en el contructor
    //debe usarse BoolToStr_ que si comprueba la condición,
    //ya que podría cambiar subrepticiamente.

    //------------------------------------------------------------------

    try {
        //la lalmada a MotionProgamGenerator debe contener un argumento
        //if(argc != 2)
          //  throw EImproperArgument("missing argument MEGARA_FiberMOSModel_Instance in call to MotionProgramGenerator");

        //CONTRUYE LOS OBJETOS:

        //Modelo del Fiber MOS
        TFiberMOSModel FMM;
        //generador de programas de movimiento
        TMotionProgramGenerator MPG(&(FMM.RPL));

        //CARGA LAS CONFIGURACIONES DE LOS ARCHIVOS:

        //carga la instancia del FiberMOSModel del directorio indicado
        AnsiString S = GetCurrentDir()+AnsiString("\\Models\\MEGARA_FiberMOSModel_Instance");
        ReadInstanceFromDir(FMM, S);
        append(AnsiString("Fiber MOS Model instance loaded from '")+S+AnsiString("'."));

        //REALIZA LAS OPERACIONES:

        //carga la TPL en una cadena
        AnsiString FileName = GetCurrentDir()+AnsiString(/*argv[1]*/"\\PPLists\\20141124102242.txt");
        StrReadFromFile(S, FileName);
        append(AnsiString("Target point list loaded from '")+FileName+AnsiString("'."));

        //asigna la cadena a la TPL
        MPG.setTargetPointsText(S);
        append("Target points allocated.");

        //mueve los RPs a sus TPs asignados
        MPG.MoveToTargetP3();
        append("RPs moved to target points.");

        //capturas las posiciones iniciales de los RPs
        TPointersList<TPair> IPL;
        FMM.RPL.GetPositions(IPL);
        IPL.Print = TPair::Print;
        S = IPL.getColumnText();
        StrWriteToFile("InitialPositionList.txt", S);
        append("Initial position list saved in 'InitialPositionList.txt'.");

        //segrega los RPs internos y externos
        TRoboticPositionerList Inners, Outsiders;
        FMM.RPL.SegregateInOut(Inners, Outsiders);

        //genera un programa de deposicionamiento
        append("Generating depositioning program...");
        TMotionProgram DP;
        MPG.GenerateDepositioningProgram(DP, Outsiders);
        S = DP.getColumnText();
        StrWriteToFile("DepositioningProgram.txt", S);
        append("Depositioning program generated.");

        //DESTRUYE LOS OBJETOS:

    } catch(Exception &E) {
        append(AnsiString("Exception: ")+E.Message);
    }

    //------------------------------------------------------------------

    return a.exec();
}
