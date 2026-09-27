# Linux System Metrics Agent

A lightweight **C/Linux observability agent** that reads host telemetry directly from Linux kernel interfaces and emits newline-delimited JSON for downstream collection.

## What it demonstrates

- C systems programming on Linux
- `/proc/stat`, `/proc/meminfo`, `/proc/net/dev` and `statvfs`
- CPU sampling, memory, disk and network telemetry
- structured JSON output suitable for Kafka, Fluent Bit or another collector
- hardened `systemd` service configuration
- GCC build automation and GitHub Actions validation

## Architecture

Linux kernel interfaces → C collector → JSONL telemetry → downstream log/stream collector → Kafka / observability platform / warehouse.

The current repository intentionally keeps transport decoupled from collection. This makes the agent small and lets different delivery mechanisms be attached without changing the metrics layer.

## Build and run

```bash
make
./metrics-agent 5
```

The optional argument is the collection interval in seconds.

## Production evolution

A production agent would add configuration parsing, TLS-authenticated transport, backpressure, local buffering, richer filesystem/device metrics, Prometheus/OpenTelemetry export, structured logging and packaging.

**Technologies:** C · Linux · POSIX · /proc · statvfs · systemd · JSON · GCC · GitHub Actions · observability
