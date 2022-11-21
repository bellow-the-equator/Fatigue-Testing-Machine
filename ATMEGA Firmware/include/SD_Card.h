void Initiate_SD_Card (){
    Serial.println("\nInicializando SD card...");

    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card falhou ou não está presente");
        delay(1000);
        loop() ;
    }

    Serial.println("SD Card inicializado!.");
}

void Open_datalog (){

    #define FILE_BASE_NAME "Log_"
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[] = FILE_BASE_NAME "00.txt";

    while (SD.exists(fileName)) {
        if (fileName[BASE_NAME_SIZE + 1] != '9') {
            fileName[BASE_NAME_SIZE + 1]++;
        } 
        else if (fileName[BASE_NAME_SIZE] != '9') {
            fileName[BASE_NAME_SIZE + 1] = '0';
            fileName[BASE_NAME_SIZE]++;
        } 
        else {
            Serial.println(F("Can't create file name"));
            loop();
        }
  }

    dataFile = SD.open(fileName, FILE_WRITE);

    if (! dataFile) {
        Serial.println((String) "Erro abrindo " + fileName);
        delay(1000);
        loop() ;
    }
    Serial.println((String) fileName + " aberto com sucesso!");
    dataFile.println("Iteration;Time (ms);Position (mm);Force (N)");
}

void Upload_State (int iteration, double Time, double Position, double Force){
    dataFile.print((String) iteration + ";" + Time + ";" + Position + ";");
    dataFile.println(Force,3);
}

void Close_SD_Card (){
    dataFile.close();
    SD.end();
    Serial.println("SD Card pode ser removido com segurança!");
    delay(1000);
}

void Flush_SD_Card(){
    dataFile.flush();
}