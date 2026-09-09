import { createServer } from "node:http";
import { readFile } from "node:fs/promises";
import { fileURLToPath } from "node:url";
import path from "node:path";
import { WebSocketServer, type WebSocket } from "ws";
import { ingestFromDriver } from "./ingest.js";
import type { TelemetryFrame } from "./telemetry-types.js";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const PUBLIC_DIR = path.join(__dirname, "..", "public");
const HTTP_PORT = 8080;

const DRIVER_PATH =
  process.env.FLIGHT_SIM_DRIVER_PATH ?? "../build/flight_sim_driver";

const httpServer = createServer(async (req, res) => {
  const reqPath = req.url === "/" ? "/index.html" : (req.url ?? "/index.html");
  try {
    const filePath = path.join(PUBLIC_DIR, reqPath);
    const body = await readFile(filePath);
    const contentType = filePath.endsWith(".html")
      ? "text/html"
      : filePath.endsWith(".js")
        ? "text/javascript"
        : "application/octet-stream";
    res.writeHead(200, { "Content-Type": contentType });
    res.end(body);
  } catch {
    res.writeHead(404);
    res.end("not found");
  }
});

const wss = new WebSocketServer({ server: httpServer });
const clients = new Set<WebSocket>();

wss.on("connection", (socket) => {
  clients.add(socket);
  socket.on("close", () => clients.delete(socket));
});

function broadcast(frame: TelemetryFrame): void {
  const payload = JSON.stringify(frame);
  for (const client of clients) {
    if (client.readyState === client.OPEN) {
      client.send(payload);
    }
  }
}

httpServer.listen(HTTP_PORT, () => {
  console.log(`[ground-station] http://localhost:${HTTP_PORT}`);
  console.log(`[ground-station] spawning driver: ${DRIVER_PATH}`);

  ingestFromDriver(
    DRIVER_PATH,
    (frame) => broadcast(frame),
    (code) => console.log(`[ground-station] driver exited with code ${code}`)
  );
});