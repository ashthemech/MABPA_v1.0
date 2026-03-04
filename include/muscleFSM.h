#ifndef MUSCLE_FSM_H
#define MUECLE_FSM_H

enum MuscleState {
  RUN_PLACEMENT_PROMPT,
  RUN_TIMER_PROMPT,
  COLLECT_EMG_DATA,
  EMG_DATA_ERROR,
  EMG_DATA_VALID,
  EMG_DONE
};

  MuscleState runMuscleFSM();

#endif