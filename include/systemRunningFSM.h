#ifndef SYSTEM_RUNNING_FSM_H
#define SYSTEM_RUNNING_FSM_H
  //enum for the braking state machine
  enum RunState {
  INIT,
  RELEASE_BRAKE, 
  ENGAGE_BRAKE};

  RunState runSystemRunningFSM();
#endif