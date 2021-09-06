#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1

ICommunication* comm;
inputData g_inputs;
int loops = 0;
void setup() {
  #if COMMUNICATION == COMM_SERIAL
    comm = new SerialCommunication();
  #elif COMMUNICATION == COMM_BTSERIAL
    comm = new BTSerialCommunication();
  #endif
  comm->start();

  setupInputs();

  #if USING_FORCE_FEEDBACK
    setupServoHaptics();
  #endif

}

void loop() {
  if (comm->isOpen()){
    #if USING_CALIB_PIN
    bool calibButton = getButton(PIN_CALIB);
    if (calibButton)
      loops = 0;
    #else
    bool calibButton = false;
    #endif

    g_inputs.calib = calibButton;

    bool calibrate = false;
    if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
      calibrate = true;
      loops++;
    }

    int* fingerPos = getFingerPositions(calibrate, calibButton);
    // ugly but fast
    g_inputs.flexion[0] = fingerPos[0];
    g_inputs.flexion[1] = fingerPos[1];
    g_inputs.flexion[2] = fingerPos[2];
    g_inputs.flexion[3] = fingerPos[3];
    g_inputs.flexion[4] = fingerPos[4];
    g_inputs.joyClick = getButton(PIN_JOY_BTN);

    #if TRIGGER_GESTURE
    g_inputs.triggerButton = triggerGesture(fingerPos);
    #else
    g_inputs.triggerButton = getButton(PIN_TRIG_BTN);
    #endif

    g_inputs.aButton = getButton(PIN_A_BTN);
    g_inputs.bButton = getButton(PIN_B_BTN);

    #if GRAB_GESTURE
    g_inputs.grab = grabGesture(fingerPos);
    #else
    g_inputs.grab = getButton(PIN_GRAB_BTN);
    #endif

    #if PINCH_GESTURE
    g_inputs.pinch = grabGesture(fingerPos);
    #else
    g_inputs.pinch = getButton(PIN_PNCH_BTN);
    #endif

    g_inputs.menu = getButton(PIN_MENU_BTN);

    g_inputs.joyX = getJoyX();
    g_inputs.joyY = getJoyY();

    comm->write(&g_inputs);

    #if USING_FORCE_FEEDBACK
      char received[100];
      if (comm->readData(received)){
        int hapticLimits[5];
        decodeData(received, hapticLimits);
        writeServoHaptics(hapticLimits);
      }
    #endif
    delay(LOOP_TIME);
  }
}
