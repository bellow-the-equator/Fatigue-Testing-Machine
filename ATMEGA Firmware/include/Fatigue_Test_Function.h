void FatigueTest(){

    Serial.println("BEM VINDO AO TESTE DE FADIGA!");

    stepper.begin(mm_min2RPM(500),MICROSTEPS);
    stepper.enable();

    Serial.println("Deseja efetuar o homing do eixo X?");
    Serial.println("Y - SIM");
    Serial.println("N - NÃO");
    while (!Serial.available()){}
    boolean Loop_stop = false;
    if(Serial.readString() == "y" || Serial.readString() == "Y") {
        Serial.println("Referenciando Eixo (Homing)");
        stepper.startMove(Dist2Step(-150));
        while(Loop_stop == false){
            stepper.nextAction();
            if(digitalRead(Y_END_STOP) == HIGH){
                stepper.stop();
                Serial.println("Fim de Curso.");
                Loop_stop = true;
                home = true;
            }
        }
        X = -Neutral_Position;
        stepper.move(Dist2Step(Neutral_Position+30));
        Movimentar_home_2(2*Neutral_Position, 300, 200, -1);
        X = 0;
    }
    else{
        X=0;
    }
    
    // Serial.println("Posicionado em ponto neutro.");
    loadCell.tare();
    Serial.println("Posicione o corpe de prova e insira Y para continuar: ");
    Loop_stop = false;
    while(Loop_stop == false){
        if(Serial.available()){
            if(Serial.readString()=="y" || Serial.readString()=="Y"){
                Loop_stop = true;
            }
        }
    }

    //Pto_Din_Neutro();
    Initiate_SD_Card();
    Open_datalog();

    int Test_type;
    int Test_R;
    float Test_Force;
    float Test_Amplitude;
    float Test_Speed;
    int iteration = 1;
    int direction;
    Serial.println("\n\nPor favor, identifique o tipo de teste a ser realizado:");
    Serial.println("1 - Amplitude Constante");
    Serial.println("2 - Força Constante");
    while (!Serial.available()){}
    Test_type = Serial.readString().toInt(); 
    Serial.println("\n\nPor favor, identifique a razão de tensão do teste (R):");
    Serial.println("1 - \"R = -1\"");
    Serial.println("2 - \"R =  0\"");
    while (!Serial.available()){}
    Test_R = Serial.readString().toInt();
    if(Test_R == 1){Test_R = -1;} else{Test_R = 0;}


    switch(Test_type){
        case 1:
            Serial.println("\n\nPor favor, insira a amplitude desejada em mm: ");
            while (!Serial.available()){}
            delay(3000);
            Test_Amplitude = Serial.readString().toFloat();
            Test_Force = 200;
            if(Test_Amplitude > 50 || Test_Amplitude <= 0){
                Serial.println("Valor máximo possível é 50 mm");
                Flush_SD_Card();
                Close_SD_Card();
                stepper.disable();
                loop();
            }
            break;

        case 2:
            Serial.println("\n\nPor favor, insira a força desejada em N: ");
            while (!Serial.available()){}
            delay(3000);
            Test_Force = Serial.readString().toFloat();
            Test_Amplitude = 50;
            if(Test_Force > 200 || Test_Amplitude <= 0){
                Serial.println("Valor máximo possível é 200 N");
                Flush_SD_Card();
                Close_SD_Card();
                stepper.disable();
                loop();
            }
            break;

        default:
            Serial.println("Opção invalida");
            Flush_SD_Card();
            Close_SD_Card();
            stepper.disable();
            loop();
    }

    Serial.println("\n\nPor favor, insira a velocidade do teste em mm/min: ");
    while (!Serial.available()){}
    delay(3000);
    Test_Speed = Serial.readString().toFloat();

    
    Serial.println("\nTESTE INICIADO - NÃO ENCOSTAR NA MAQUINA!\n");
    Serial.println("\nA QUALQUER MOMENTO INSIRA \"0\" PARA CANCELAR O TESTE.\n");

    
    while(true){
        if(Serial.available()){
            if(Serial.readString().toInt() == 0){
                Serial.println("TESTE FINALIZADO!");
                Serial.println("Salvando dados no SD Card");
                Flush_SD_Card();
                Close_SD_Card();
                stepper.disable();
                Serial.println("Salvo com sucesso. SD Card pode ser removido com segurança!\n");
                loop();
            }
        }
        time_millis = millis();
        direction = 1;
        Movimentar(+Test_Amplitude - X, Test_Speed, Test_Force, iteration, direction, true);
        direction = -1;
        Movimentar_home(-X, Test_Speed, Test_Force, iteration, direction, false);

        if(Test_R == -1){
            direction = -1;
            Movimentar(-Test_Amplitude - X, Test_Speed, Test_Force, iteration, direction, true);
            direction = 1;
            Movimentar_home(-X, Test_Speed, Test_Force, iteration, direction, false);
        }
        iteration = iteration + 1;
    }
}er