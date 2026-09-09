-- shared, range-constrained types for pitch-hold controller.

-- constraining the types themselves (rather than checking bounds ad hoc in each function) is what lets GNATprove statically rule out overflow;
-- a value of type Millidegrees literally cannot hold anything outside its declared range.

package Pitch_Types
   with Pure
is

-- range is wide enough for any physically sensible pitch angle or rate; 

type Millidegrees is range -180_000 .. 180_000;

-- actuator deflection command, in millidegrees; mirrors ACTUATOR_MIN_MDEG / ACTUATOR_MAX_MDEG.

Actuator_Min : constant := -15_000;
Actuator_Max : constant := 15_000;

type Actuator_Command is range Actuator_Min .. Actuator_Max;

type Time_Ms is range 0 .. 2**31 -1;

end Pitch_Types;