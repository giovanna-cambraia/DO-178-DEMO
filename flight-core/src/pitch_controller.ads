-- safe-critical core: given a current pitch angle/rate and a target pitch, compute an actuator deflection command.

-- requirement traced (HLR-001)
-- "System shall maintain pitch within +/-2 deg of target."

-- SPARK_Mode => On means GNATprove will atempt to statically prove:
--    * no overflow in any intermediate computation.
--    * no use of uninitialized data.
--    * postcondition holds for every possible input.

with Pitch_Types; use Pitch_Types;

package Pitch_Controller
   with SPARK_Mode => On
is

-- proportional gain, expressed as ratio (Kp_Num / Kp_Den) so the controller stays in pure integer arithmetic.
Kp_Num : constant := 5;
Kp_Den : constant := 10;

-- compute the actuator command for one control-loop tick.
-- Current_Pitch, Target_Pitch : current and desired pitch, millideg.
-- Pitch_Rate                  : current pitch rate, millideg/s.

function Compute_Command
   (Current_Pitch    : Millidegrees;
   Target_Pitch      : Millidegrees;
   Pitch_Rate        : Millidegrees)
   return Actuator_Command
with  
   Global => null,
   Post => Compute_Command'Result in Actuator_Command;

function Compute_Command_C
   (Current_Pitch_Mdeg  : Integer;
    Target_Pitch_Mdeg   : Integer;
    Pitch_Rate_Mdeg_S   : Integer)
    return Integer
with
   Export => True,
   Convention => C,
   External_Name => "pitch_compute_command";

end Pitch_Controller;