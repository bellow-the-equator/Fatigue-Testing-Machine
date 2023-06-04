//Importando Bibliotecas
#include <Preparation.h>
                            
void setup() {

    //Serial
    Serial.begin(115200);

    //LoadCell
    loadCell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    float LOADCELL_DIVIDER;
    EEPROM.get(0,LOADCELL_DIVIDER);
    loadCell.set_scale(LOADCELL_DIVIDER);

    //Stepper
    //stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    stepper.setEnableActiveState(LOW);
    stepper.disable();

    //SD Card
    pinMode(SS, OUTPUT);

    //End_Stop
    pinMode(Y_END_STOP, INPUT_PULLUP);
    pinMode(Z_END_STOP, INPUT_PULLUP);
}

#include <Menu.h>

void loop() {


  Menu();

}

