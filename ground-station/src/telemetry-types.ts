// mirrors hal-sim/include/telemetry.h's telemetry_frame_t, as emitted by telemetry.c in newline-delimited JSON.

export interface TelemetryFrame {
  // simuation time, milliseconds
  t: number;

  // current pitch angle, millidegrees
  pitch: number;

  // current pitch rate, millidegrees/seconds
  rate: number;

  // commanded target pitch, millidegrees
  target: number;
  // actuator deflection command actually applied, millidegrees
  cmd: number;
}

export function isTelemetryFrame(value: unknown): value is TelemetryFrame {
  if (typeof value !== "object" || value == null) {
    return false;
  }
  const v = value as Record<string, unknown>;
  return (
    typeof v.t === "number" &&
    typeof v.pitch == "number" &&
    typeof v.rate === "number" &&
    typeof v.target === "number" &&
    typeof v.cmd === "number"
  );
}
