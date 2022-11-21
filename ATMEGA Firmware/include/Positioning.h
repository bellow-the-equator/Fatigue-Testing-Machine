
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




void Movimentar(float Test_Amplitude, float Test_Speed, float Test_Force, int iteration, int direction, boolean check_force){

    stepper.setRPM(mm_min2RPM(Test_Speed));
    double Curr_load = 0;
    float Initial_Position = X;

    while(abs(X - Initial_Position) < (abs(Test_Amplitude) - 0.01)){
        stepper.move(Dist2Step(0.2*direction));
        X = X + 0.2*direction;
        Curr_load = loadCell.get_units(1);
        Upload_State(iteration, (millis() - time_millis), X, Curr_load);
        

        if((abs(Curr_load) > Test_Force) && check_force == true){
            if(abs(loadCell.get_units(1))>Test_Force){
                if(abs(loadCell.get_units(1))>Test_Force){
                    //MOVIMENTO COMPLETO
                    stepper.stop();
                    return;
                }
            }
        }
    }
    //MOVIMENTO COMPLETO
    //stepper.stop();
    return;
}


void Movimentar_Static(float Test_Amplitude, float Test_Speed, float Test_Force, int iteration, int direction){

    stepper.setRPM(mm_min2RPM(Test_Speed));
    double Curr_load = 0;
    float Initial_Position = X;

    while(abs(X - Initial_Position) < (abs(Test_Amplitude) - 0.01)){
        stepper.move(Dist2Step(0.04*direction));
        X = X + 0.04*direction;
        Curr_load = loadCell.get_units(1);
        Upload_State(iteration, (millis() - time_millis), X, Curr_load);
        

        if(abs(Curr_load) > Test_Force){
            if(abs(loadCell.get_units(1))>Test_Force){
                if(abs(loadCell.get_units(1))>Test_Force){
                    //MOVIMENTO COMPLETO
                    stepper.stop();
                    return;
                }
            }
        }
    }
    //MOVIMENTO COMPLETO
    //stepper.stop();
    return;
}


void Movimentar_home(float Test_Amplitude, float Test_Speed, float Test_Force, int iteration, int direction, boolean check_force){

    stepper.setRPM(mm_min2RPM(Test_Speed));
    double Curr_load = 0;
    boolean Loop_stop = false;
    while(Loop_stop == false){
        
        if(digitalRead(Z_END_STOP) == LOW){
                // for (int i = 0; i <= 33; i++){
                //     stepper.move(Dist2Step(0.2*direction));
                //     X = X + 0.2*direction;
                //     Curr_load = loadCell.get_units(1);
                //     Upload_State(iteration, (millis() - time_millis), X, Curr_load);
                // }
                X = 0;
                stepper.stop();          
                Loop_stop = true;
                return;
            }

        if((abs(Curr_load) > Test_Force) && check_force == true){
            if(abs(loadCell.get_units(1))>Test_Force){
                if(abs(loadCell.get_units(1))>Test_Force){
                    //MOVIMENTO COMPLETO
                    stepper.stop();
                    return;
                }
            }
        }

        stepper.move(Dist2Step(0.2*direction));
        X = X + 0.2*direction;
        Curr_load = loadCell.get_units(1);
        Upload_State(iteration, (millis() - time_millis), X, Curr_load);
    }
    //MOVIMENTO COMPLETO
    //stepper.stop();
    return;
}


void Movimentar_home_2(float Test_Amplitude, float Test_Speed, float Test_Force, int direction){

    stepper.setRPM(mm_min2RPM(Test_Speed));
    float Initial_Position = X;

    boolean Loop_stop = false;
    while(Loop_stop == false){
        stepper.move(Dist2Step(0.2*direction));
        X = X + 0.2*direction;
        
        if(digitalRead(Z_END_STOP) == LOW){
                // for (int i = 0; i <= 33; i++){
                //     stepper.move(Dist2Step(0.2*direction));
                //     X = X + 0.2*direction;
                // }
                X = 0;
                stepper.stop();          
                Loop_stop = true;
                return;
            }
    }
    //MOVIMENTO COMPLETO
    //stepper.stop();
    return;
}