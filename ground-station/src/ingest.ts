import { spawn, type ChildProcessWithoutNullStreams } from "node:child_process";
import { createInterface } from "node:readline";
import { isTelemetryFrame, type TelemetryFrame } from "./telemetry-types";

// spawns the compiled hal-sim driver binary and yields one TelemetryFrame per line NDJSON.
// TODO: swap this for a TCP/WebSocket client later, without changing anything downstream onFrame.

export function ingestFromDriver(
  driverPath: string,
  onFrame: (frame: TelemetryFrame) => void,
  onExit?: (code: number | null) => void,
): ChildProcessWithoutNullStreams {
  const child = spawn(driverPath, [], { stdio: ["ignore", "pipe", "inherit"] });

  const rl = createInterface({ input: child.stdout });
  rl.on("line", (line) => {
    const trimmed = line.trim();
    if (trimmed.length === 0) {
      return;
    }
    try {
      const parsed: unknown = JSON.parse(trimmed);
      if (isTelemetryFrame(parsed)) {
        onFrame(parsed);
      } else {
        console.warn("[ingest] dropped malformed frame:", trimmed);
      }
    } catch (err) {
      console.warn("[ingest] failed to parse line:", trimmed, err);
    }
  });

  child.on("exit", (code) => {
    rl.close();
    onExit?.(code);
  });

  return child;
}
