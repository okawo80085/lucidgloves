class SerialCommunication : public ICommunication {
  private:
    bool m_isOpen;

  public:
    SerialCommunication() {
      m_isOpen = false;
    }

    bool isOpen(){
      return m_isOpen;
    }

    void start(){
      //Serial.setTimeout(1000000);
      Serial.begin(SERIAL_BAUD_RATE);
      m_isOpen = true;
    }

    void print(char* data){
      Serial.print(data);
      Serial.flush();
    }

    void write(inputData* data){
      Serial.write((char*)data, sizeof(*data));
    }

    bool readData(char* input){
      byte size = Serial.readBytesUntil('\n', input, 100);
      input[size] = '\0';
      return input != NULL && strlen(input) > 0;
    }
};
