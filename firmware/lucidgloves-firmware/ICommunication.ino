//Interface for communication

struct inputData {
  int flexion[5];
  int joyX;
  int joyY;
  bool joyClick;
  bool triggerButton;
  bool aButton;
  bool bButton;
  bool grab;
  bool pinch;
};



class ICommunication {

  public:
    virtual bool isOpen() = 0;

    virtual void start() = 0;

    // print a null terminated string via the implemented communication interface
    virtual void print(char* data) = 0;
    // write a inputData struct via the implemented communication interface
    virtual void write(inputData* data) = 0;

    virtual bool readData(char* input) = 0;
};
