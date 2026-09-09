-- TODO: flesh out control law and start running ``gnatprove -P flight_core.gpr`

with Pitch_Types; use Pitch_Types;

package body Pitch_Controller
   with SPARK_Mode => On
is

-- wide enough to hold Kp_Num * (max possible error) without overflow, so the proportional term can be computed before clamping without risking a wraparound.
-- error is at most 360_000 (two millidegrees extremes apart), so Kp_Num * Error safely fits in 64 bits. 

type Wide_Value is range -2**62 .. 2**62 -1;

function Clamp_To_Actuator (Raw  : Wide_Value) return Actuator_Command
   with 
      Global => null,
      Post => Clamp_To_Actuator'Result in Actuator_Command;

function Clamp_To_Actuator (Raw  : Wide_Value) return Actuator_Command is
   begin
      if Raw < Wide_Value (Actuator_Command'First) then  
         return Actuator_Command'First;
      elsif Raw > Wide_Value (Actuator_Command'Last) then 
         return Actuator_Command'Last;
      else  
         return Actuator_Command (Raw);
      end if;
end Clamp_To_Actuator;

function Compute_Command
   (Current_Pitch : Millidegrees;
    Target_Pitch  : Millidegrees;
    Pitch_Rate    : Millidegrees)
    return Actuator_Command
   is
      Error    : constant Wide_Value := Wide_Value (Target_Pitch) - Wide_Value (Current_Pitch);
      P_Term   : constant Wide_Value := (Wide_Value (Kp_Num) * Error) / Kp_Den;
      -- rate term acts as simple damping: proportional gain only for now.
      Raw      : constant Wide_Value := P_Term - Wide_Value (Pitch_Rate);
   begin
      return Clamp_To_Actuator (Raw);
end Compute_Command;

function Compute_Command_C
   (Current_Pitch_Mdeg  : Integer;
    Target_Pitch_Mdeg   : Integer;
    Pitch_Rate_Mdeg_S   : Integer)
    return Integer
   is

function Clamp_To_Millidegrees(V : Integer) return Millidegrees is
   (if V < Integer   (Millidegrees'First) then Millidegrees'First
    elsif V > Integer (Millidegrees'Last) then Millidegrees'Last
    else Millidegrees (V));

   Current : constant Millidegrees := Clamp_To_Millidegrees (Current_Pitch_Mdeg);
   Target  : constant Millidegrees := Clamp_To_Millidegrees (Target_Pitch_Mdeg);
   Rate    : constant Millidegrees := Clamp_To_Millidegrees (Pitch_Rate_Mdeg_S);
begin
   return Integer (Compute_Command (Current, Target, Rate));
end Compute_Command_C;

end Pitch_Controller;