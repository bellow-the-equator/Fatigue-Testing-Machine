void Controle_Manual_Function(){

    int Input_Num = 3;
    int  Speed = 500; // mm/min
    stepper.begin(Speed/8,MICROSTEPS);
    stepper.enable();    
    Serial.println("\n\n");
    Serial.println("BEM VINDO AO CONTROLE MANUAL!\n");
    Serial.println("Use 7 e 8 para mover -10mm e +10mm respectivamente");
    Serial.println("Use 4 e 5 para mover -05mm e +05mm respectivamente");
    Serial.println("Use 1 e 2 para mover -01mm e +01mm respectivamente");
    Serial.println("\nInsira 9 para finalizar o posicionamento.");


    while (Input_Num != 9){
        Input_Num = Serial.readString().toInt(); 
        switch (Input_Num)
        {
        case 7:
            stepper.rotate(450);
            break;
        case 8:
            stepper.rotate(-450);
            break;
        case 4:
            stepper.rotate(-225);
            break;
        case 5:
            stepper.rotate(225);
            break;
        case 1:
            stepper.rotate(-45);
            break;
        case 2:
            stepper.rotate(45);
            break;
        }
    }

    stepper.disable();

}