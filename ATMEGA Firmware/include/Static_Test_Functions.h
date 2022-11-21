void StaticTest(){

    Serial.println("BEM VINDO AO TESTE ESTÁTICO!");

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
        stepper.move(Dist2Step(Neutral_Position));
        X = 0;
    }
    else{
        X=0;
    }

    Serial.println("Posicionado em ponto neutro.");
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

    Pto_Din_Neutro();
    Initiate_SD_Card();
    Open_datalog();
    
    Serial.println("\nTESTE INICIADO - NÃO ENCOSTAR NA MAQUINA!\n");

    // unsigned long wait_time_micros = 1;
    // unsigned long start_time_micros = millis();
    // unsigned long step_time_micros = start_time_micros;
    // stepper.setRPM(1);
    // stepper.startMove(Dist2Step(50));
    // double Curr_load = 0;


    // //funcionando, gargalo 12 ms ler carga, salvar diferntes testes txt
    // while(wait_time_micros > 0){

    //     wait_time_micros = stepper.nextAction();

    //     if ((millis() - step_time_micros) >= 50){
    //         step_time_micros = step_time_micros + 50;
    //         Curr_load = loadCell.get_units(1);
    //         Upload_State(1,(step_time_micros - start_time_micros), Step2Dist(stepper.getStepsCompleted()), Curr_load);
            

    //         if(abs(Curr_load) > 200){
    //             if(abs(loadCell.get_units(1))>200){
    //                 Serial.println("Maquina ultrapassou capacidade máxima! Teste cancelado.");
    //                 Flush_SD_Card();
    //                 Close_SD_Card();
    //                 stepper.disable();
    //                 loop();
    //             }
    //         }

    //         if(millis()-step_time_micros >= 49){Serial.println("Deu pau no timing Static_Test_Functions");}
    //     }
    // }

    time_millis = millis();
    Movimentar_Static(50, 8, 200, 1, 1);

    Flush_SD_Card();
    Close_SD_Card();
    stepper.disable();
    Serial.println("Teste realizado com sucesso! Dados gravados no SD Card.");
    loop();

}