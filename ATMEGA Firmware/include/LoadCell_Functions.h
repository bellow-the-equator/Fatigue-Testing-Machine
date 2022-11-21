void Tara(){
    Serial.println("Iniciando processo de tara:");
    loadCell.tare();
    Serial.println("Processo de tara concluído.");
    delay(1000);
}

void Calibracao(){
    stepper.begin(mm_min2RPM(500),MICROSTEPS);
    stepper.enable();
    double valorCalibracao;
    Serial.println("Iniciando processo de calibração:");
    delay(1000);
    Serial.println("Deixe o sistema livre de cargas:");
    delay(2000);
    loadCell.set_scale();
    loadCell.tare();
    delay(2000);
    Serial.println("Insira a massa de calibração no sistema!");
    Serial.println("Valor em Kg: ");
    while (!Serial.available()){}
    delay(3000);
    valorCalibracao = Serial.readString().toDouble();  
    Serial.println((String) "Valor selecionado: " + valorCalibracao + " Kg, ou " + valorCalibracao*9.8066 + " N.");    
    valorCalibracao = loadCell.get_value(32)/(valorCalibracao*9.8066);
    loadCell.set_scale(valorCalibracao);
    EEPROM.put(0,valorCalibracao);
    Serial.println("Processo de Calibração Concluído.");
    delay(1000);
    stepper.disable();
}

void LerForca(){
    address_1:
    loadCell.tare();
    while (!Serial.available()){
    Serial.println((String) "Força (N) = " + loadCell.get_units(30));
    }
    if(Serial.readString() == "t" || Serial.readString() == "T") {
    loadCell.tare();
    Serial.println("Tara realizada.");
    goto address_1;
    }
    Serial.println("Procedimento cancelado.");
    delay(300);
}

