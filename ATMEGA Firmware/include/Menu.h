#include <SD_Card.h>
#include <Positioning.h>
#include <LoadCell_Functions.h>
#include <Static_Test_Functions.h>
#include <Fatigue_Test_Function.h>
#include <Manual_Control.h>


String inputString;
int Function_number;

void Celula_de_Carga(){

    inputString = "";
    Serial.print("\n\n\nCELULA DE CARGA, ESCOLHA UMA OPÇÃO:\n\n1 - Tara\n2 - Calibração\n3 - Ler Força\n\n");
    while (!Serial.available()){}
    Function_number = Serial.readString().toInt(); 

    switch(Function_number){

        case 1:
        Tara();
        break;

        case 2:
        Calibracao();
        break;

        case 3:
        LerForca();
        break;
    }
}


void SD_Card(){

}

void Teste_Estatico(){
    StaticTest();
}

void Teste_Fadiga(){
    FatigueTest();
}

void Controle_Manual(){
    Controle_Manual_Function();
}

void Menu(){
  
    inputString = "";
    Serial.print("\n\n\nBEM VINDO, ESCOLHA UMA OPÇÃO:\n\n1 - Celula de Carga\n2 - SD Card\n3 - Teste Estático\n4 - Teste Fadiga\n5 - Controle Manual\n\n");
    while (!Serial.available()){}
    Function_number = Serial.readString().toInt(); 
    
    switch(Function_number){

        case 1:
        Celula_de_Carga();
        break;

        case 2:
        SD_Card();
        break;

        case 3:
        Teste_Estatico();
        break;

        case 4:
        Teste_Fadiga();
        break;

        case 5:
        Controle_Manual();
        break;
    }
}

