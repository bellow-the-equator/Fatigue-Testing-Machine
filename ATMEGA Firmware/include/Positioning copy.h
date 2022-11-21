
int Dist2Step (double Dist){
    return (Dist/8)*MOTOR_STEPS*MICROSTEPS;
}

double Step2Dist (double Step){
    return (Step*8)/(MOTOR_STEPS*MICROSTEPS);
}

float mm_min2RPM (float Speed){
    return (Speed/8.);
}

void Zero_position(){
    X = -Neutral_Position;
    Serial.println((String) "Position (mm): " + X);
}

void Pto_Din_Neutro(){
    int step_per_step = Dist2Step(0.1);
    double Mininum_load = 100;
    float Pos_Minimum_load = 0;
    double Curr_load = 100;
    Serial.println("Iniciando procura a ponto de carga mínimo");

    for(int i=1; i<=30; i++){
        Curr_load = abs(loadCell.get_units(1));
        if(Curr_load < Mininum_load){
            Mininum_load = Curr_load;
            Pos_Minimum_load = X;
        }
        stepper.move(step_per_step);
        X = X + 0.1;
    }
    stepper.move(-step_per_step*30);
    X = X - 3;

    for(int i=1; i<=30; i++){
        Curr_load = abs(loadCell.get_units(1));
        if(Curr_load < Mininum_load){
            Mininum_load = Curr_load;
            Pos_Minimum_load = X;
        }
        stepper.move(-step_per_step);
        X = X - 0.1;
    }
    stepper.move(step_per_step*30);
    X = X + 3;

    stepper.move(Dist2Step(Pos_Minimum_load));
    X = 0;
}




void Movimentar(float Test_Amplitude, float Test_Speed, float Test_Force, int iteration, int direction){

    unsigned long wait_time_micros = 1;
    unsigned long start_time_micros = millis();
    unsigned long step_time_micros = start_time_micros;
    stepper.setRPM(mm_min2RPM(Test_Speed));
    stepper.startMove(Dist2Step(Test_Amplitude));
    unsigned int step_period = round(1000*0.2/(Test_Speed/60));
    double Curr_load = 0;

    while(wait_time_micros > 0){

            wait_time_micros = stepper.nextAction();

        if ((millis() - step_time_micros) >= step_period){
            step_time_micros = step_time_micros + step_period;
            Curr_load = loadCell.get_units(1);
            Upload_State(iteration, (step_time_micros - start_time_micros), X + Step2Dist(stepper.getStepsCompleted())*direction, Curr_load);
            

            if(abs(Curr_load) > Test_Force){
                if(abs(loadCell.get_units(1))>Test_Force){
                    if(abs(loadCell.get_units(1))>Test_Force){
                        //MOVIMENTO COMPLETO
                        stepper.stop();
                        X = X + direction*Step2Dist(stepper.getStepsCompleted());
                        return;
                    }
                }
            }

            if(millis()-step_time_micros >= (step_period-1)){
                Serial.println("Deu pau no timing Static_Test_Functions"); 
                Flush_SD_Card();
                Close_SD_Card();
                stepper.disable();
                loop();
            }
        }
    }
    //MOVIMENTO COMPLETO
    stepper.stop();
    X = X + direction*Step2Dist(stepper.getStepsCompleted());
    return;
}