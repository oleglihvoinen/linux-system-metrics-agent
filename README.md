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
Linux kernel interfaces → C collector → JSONL telemetry → downstream collector → Kafka / observability platform / warehouse.

The transport is intentionally decoupled from collection so different delivery mechanisms can be attached without changing the metrics layer.

## Build and run
```bash
make
./metrics-agent 5
```

## Production evolution
Configuration parsing, TLS-authenticated transport, backpressure, local buffering, richer device metrics, Prometheus/OpenTelemetry export, structured logging and packaging.

**Technologies:** C · Linux · POSIX · /proc · statvfs · systemd · JSON · GCC · GitHub Actions · observability
