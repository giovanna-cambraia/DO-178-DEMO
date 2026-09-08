```
DO-178-DEMO/
├── design/            → Architecture diagrams, state machines
├── docs/           → DO-178C-style plans, requirements, traceability 
├── flight-core/      → Ada/SPARK — the safety-critical pitch-hold controller,
│                        formally proven free of overflow / UB with GNATprove
├── hal-sim/           → C — simulated sensor + actuator HAL, MISRA-C-style,
│                        this is the *unproven* baseline the Ada core is compared against
├── ground-station/   → TypeScript — live telemetry dashboard (Node + WebSocket)
├── reviews/            → Review checklists & logs
├── tests/          → Unit tests & coverage reports
└── .github/workflows → CI: build C, prove + build Ada, lint/build TS
```